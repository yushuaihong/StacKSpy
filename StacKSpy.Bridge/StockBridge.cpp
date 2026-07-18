// StockBridge.cpp - C++/CLI 桥接层实现
// 关键约束：
//   1. 托管成员函数中禁止 lambda (E2093)
//   2. Lambda 不能捕获托管类型 (C3498)
//   3. #pragma unmanaged 中禁止调用托管代码 (C3821)
//   4. event 的 raise 方法默认是 private (E0265)
// 解决方案：原生 EventDispatcher 实现 IEventHandler + gcroot 持有托管引用
//           通过虚函数指针调用，完全避开 lambda 和 std::function

#include "StockBridge.h"
#include "Version.h"
#include "Kernel/MicroKernel.h"
#include "Kernel/IEventBus.h"
#include "Services/StockPriceService.h"
#include "Services/KDJCalculator.h"
#include "Services/AlertEngine.h"
#include "Services/StockManager.h"
#include "Plugins/StockPricePlugin.h"
#include "Plugins/KDJCalculatorPlugin.h"
#include "Plugins/AlertPlugin.h"
#include "Plugins/StockManagerPlugin.h"
#include "Plugins/ConfigPlugin.h"
#include "Plugins/AutoStartPlugin.h"
#include <msclr/gcroot.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace StacKSpy { namespace Bridge {

    // ===== 字符串转换（不使用 marshal.h，避免 E2020）=====
    static std::string ToNativeString(System::String^ str) {
        if (!str) return "";
        IntPtr ptr = Marshal::StringToHGlobalAnsi(str);
        std::string result(static_cast<const char*>(ptr.ToPointer()));
        Marshal::FreeHGlobal(ptr);
        return result;
    }

    static System::String^ ToManagedString(const std::string& str) {
        return gcnew System::String(str.c_str());
    }

    // ===== 原生事件分发器 =====
    class EventDispatcher : public Core::Kernel::IEventHandler {
    public:
        msclr::gcroot<StockBridge^> m_bridge;

        EventDispatcher(StockBridge^ bridge) : m_bridge(bridge) {}

        void OnEvent(const std::string& eventName,
                     const Core::Kernel::EventData&) override
        {
            if (eventName == "PRICE_UPDATE")
                m_bridge->RaisePriceUpdated();
            else if (eventName == "KDJ_UPDATE")
                m_bridge->RaiseKDJUpdated();
            else if (eventName == "ALERT_NOTIFY")
                m_bridge->RaiseAlertTriggered();
        }
    };

    static EventDispatcher* s_dispatchers[3] = { 0, 0, 0 };

    static void SubscribeBridgeEvents(
        std::shared_ptr<Core::Kernel::IEventBus> eventBus,
        StockBridge^ bridge)
    {
        s_dispatchers[0] = new EventDispatcher(bridge);
        eventBus->SubscribeHandler("PRICE_UPDATE", s_dispatchers[0]);

        s_dispatchers[1] = new EventDispatcher(bridge);
        eventBus->SubscribeHandler("KDJ_UPDATE", s_dispatchers[1]);

        s_dispatchers[2] = new EventDispatcher(bridge);
        eventBus->SubscribeHandler("ALERT_NOTIFY", s_dispatchers[2]);
    }

    // ===== StockBridge 实现 =====

    StockBridge::StockBridge() {
        m_kernel = new std::shared_ptr<Core::Kernel::MicroKernel>(
            std::make_shared<Core::Kernel::MicroKernel>());
        m_stockManager = nullptr;
        m_priceService = nullptr;
        m_kdjCalculator = nullptr;
        m_alertEngine = nullptr;
    }

    StockBridge::~StockBridge() { this->!StockBridge(); }

    StockBridge::!StockBridge() {
        if (m_kernel && *m_kernel) {
            auto eventBus = (*m_kernel)->GetEventBus();
            if (eventBus) {
                eventBus->Unsubscribe("PRICE_UPDATE");
                eventBus->Unsubscribe("KDJ_UPDATE");
                eventBus->Unsubscribe("ALERT_NOTIFY");
            }
            auto scheduler = (*m_kernel)->GetScheduler();
            if (scheduler) {
                scheduler->Stop();
            }
            (*m_kernel)->Shutdown();
            delete m_kernel;
            m_kernel = nullptr;
        }

        // 保存股票池后释放
        if (m_stockManager && *m_stockManager)
            (*m_stockManager)->SaveStockPool();

        delete m_stockManager; m_stockManager = nullptr;
        delete m_priceService; m_priceService = nullptr;
        delete m_kdjCalculator; m_kdjCalculator = nullptr;
        delete m_alertEngine; m_alertEngine = nullptr;

        for (int i = 0; i < 3; ++i) {
            if (s_dispatchers[i]) {
                delete s_dispatchers[i];
                s_dispatchers[i] = 0;
            }
        }
    }

    // 获取当前模块所在目录（使用 .NET API，避免引入 windows.h）
    static std::string GetModuleDir() {
        System::String^ asmPath = System::Reflection::Assembly::GetExecutingAssembly()->Location;
        System::String^ dir = System::IO::Path::GetDirectoryName(asmPath);
        return ToNativeString(dir + "\\");
    }

    void StockBridge::Initialize() {
        auto& kernel = *m_kernel;
        kernel->Initialize();

        auto eventBus = kernel->GetEventBus();
        auto scheduler = kernel->GetScheduler();
        auto configManager = kernel->GetConfigManager();

        // 创建服务实例并保存到 Bridge 成员
        auto priceService = std::make_shared<Core::Services::StockPriceService>();
        auto kdjCalculator = std::make_shared<Core::Services::KDJCalculator>();
        auto alertEngine = std::make_shared<Core::Services::AlertEngine>();
        auto stockManager = std::make_shared<Core::Services::StockManager>(priceService.get());

        // 设置股票池文件路径为 exe 目录
        stockManager->SetConfigPath(GetModuleDir() + "stockpool.json");
        stockManager->LoadStockPool();

        m_priceService = new std::shared_ptr<Core::Services::IStockPriceService>(priceService);
        m_kdjCalculator = new std::shared_ptr<Core::Services::IKDJCalculator>(kdjCalculator);
        m_alertEngine = new std::shared_ptr<Core::Services::IAlertEngine>(alertEngine);
        m_stockManager = new std::shared_ptr<Core::Services::IStockManager>(stockManager);

        // 注册插件
        auto pluginManager = kernel->GetPluginManager();
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::StockPricePlugin>(
            priceService, eventBus, scheduler));
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::KDJCalculatorPlugin>(
            kdjCalculator, stockManager, eventBus, scheduler));
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::AlertPlugin>(
            alertEngine, eventBus));
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::StockManagerPlugin>(
            stockManager, eventBus));
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::ConfigPlugin>(
            configManager, eventBus));
        pluginManager->RegisterPlugin(std::make_shared<Core::Plugins::AutoStartPlugin>(
            configManager));

        SubscribeBridgeEvents(eventBus, this);
    }

    void StockBridge::Start() { (*m_kernel)->Start(); }
    void StockBridge::Stop() { (*m_kernel)->Stop(); }

    // ===== 桥接方法实现 =====

    System::String^ StockBridge::GetVersion() {
        return gcnew System::String(STACKSPY_VERSION_FULL);
    }

    System::Collections::Generic::List<ManagedStockPrice^>^ StockBridge::FetchAllPrices() {
        auto results = gcnew System::Collections::Generic::List<ManagedStockPrice^>();
        if (!m_stockManager || !*m_stockManager || !m_priceService || !*m_priceService) return results;

        auto pool = (*m_stockManager)->GetStockPool();
        std::vector<std::string> codes;
        codes.reserve(pool.size());
        for (const auto& info : pool) {
            codes.push_back(info.Code);
        }
        auto prices = (*m_priceService)->FetchAllPrices(codes);

        for (const auto& p : prices) {
            auto mp = gcnew ManagedStockPrice();
            mp->Code = ToManagedString(p.StockCode);
            mp->Name = ToManagedString(p.StockName);
            mp->Price = p.Price;
            mp->ChangePercent = p.ChangePercent;
            mp->TurnoverRate = p.TurnoverRate;
            mp->UpdateTime = ToManagedString(p.UpdateTime);
            results->Add(mp);
        }
        return results;
    }

    ManagedKDJIndicator^ StockBridge::CalculateKDJ(System::String^ code, System::String^ period) {
        auto result = gcnew ManagedKDJIndicator();
        result->Code = code;
        if (!m_kdjCalculator || !*m_kdjCalculator) return result;

        std::string nativeCode = ToNativeString(code);
        auto kdj = (*m_kdjCalculator)->Calculate(nativeCode);
        result->Name = ToManagedString(kdj.StockName);
        result->DailyJ = kdj.DailyJ;
        result->WeeklyJ = kdj.WeeklyJ;
        result->MonthlyJ = kdj.MonthlyJ;
        result->UpdateDate = ToManagedString(kdj.UpdateDate);
        return result;
    }

    bool StockBridge::AddStock(System::String^ code) {
        if (!m_stockManager || !*m_stockManager) return false;
        std::string nativeCode = ToNativeString(code);
        bool ok = (*m_stockManager)->AddStock(nativeCode);
        if (ok) (*m_stockManager)->SaveStockPool();
        return ok;
    }

    bool StockBridge::RemoveStock(System::String^ code) {
        if (!m_stockManager || !*m_stockManager) return false;
        std::string nativeCode = ToNativeString(code);
        bool ok = (*m_stockManager)->RemoveStock(nativeCode);
        if (ok) (*m_stockManager)->SaveStockPool();
        return ok;
    }

    // TODO:
    System::Collections::Generic::List<System::String^>^ StockBridge::GetStockPool() {
        auto results = gcnew System::Collections::Generic::List<System::String^>();
        if (!m_stockManager || !*m_stockManager) return results;
        auto pool = (*m_stockManager)->GetStockPool();
        for (const auto& info : pool) {
            results->Add(ToManagedString(info.Code));
        }
        return results;
    }

    bool StockBridge::UpdateConfig(System::String^ key, System::String^ value) {
        std::string nativeKey = ToNativeString(key);
        std::string nativeValue = ToNativeString(value);
        auto configMgr = (*m_kernel)->GetConfigManager();
        configMgr->SetString(nativeKey, nativeValue);
        configMgr->Save();
        OnConfigChanged();
        return true;
    }

    System::Collections::Generic::List<ManagedAlert^>^ StockBridge::GetAlerts() {
        auto results = gcnew System::Collections::Generic::List<ManagedAlert^>();
        if (!m_alertEngine || !*m_alertEngine) return results;

        auto alerts = (*m_alertEngine)->GetActiveAlerts();
        for (const auto& a : alerts) {
            auto ma = gcnew ManagedAlert();
            ma->Code = ToManagedString(a.StockCode);
            ma->Name = ToManagedString(a.StockName);
            ma->JValue = a.JValue;
            ma->Level = static_cast<int>(a.Level);
            ma->Period = ToManagedString(a.Period);
            ma->Message = ToManagedString(a.Message);
            results->Add(ma);
        }
        return results;
    }

}}
