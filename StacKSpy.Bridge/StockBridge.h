#pragma once
#include "ManagedTypes.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Kernel { class MicroKernel; }}}
namespace StacKSpy { namespace Core { namespace Services {
    class IStockManager;
    class IStockPriceService;
    class IKDJCalculator;
    class IAlertEngine;
}}}

namespace StacKSpy { namespace Bridge {

    public delegate void PriceUpdatedHandler();
    public delegate void KDJUpdatedHandler();
    public delegate void AlertTriggeredHandler();
    public delegate void ConfigChangedHandler();

    public ref class StockBridge {
    public:
        StockBridge();
        ~StockBridge();
        !StockBridge();

        void Initialize();
        void Start();
        void Stop();

        // 桥接接口
        System::String^ GetVersion();
        System::Collections::Generic::List<ManagedStockPrice^>^ FetchAllPrices();
        ManagedKDJIndicator^ CalculateKDJ(System::String^ code, System::String^ period);
        bool AddStock(System::String^ code);
        bool RemoveStock(System::String^ code);
        System::Collections::Generic::List<System::String^>^ GetStockPool();
        bool UpdateConfig(System::String^ key, System::String^ value);
        System::Collections::Generic::List<ManagedAlert^>^ GetAlerts();

        // 事件委托
        event PriceUpdatedHandler^ OnPriceUpdated;
        event KDJUpdatedHandler^ OnKDJUpdated;
        event AlertTriggeredHandler^ OnAlertTriggered;
        event ConfigChangedHandler^ OnConfigChanged;

        // 公开的事件触发方法（供 EventDispatcher 调用）
        void RaisePriceUpdated() { OnPriceUpdated(); }
        void RaiseKDJUpdated() { OnKDJUpdated(); }
        void RaiseAlertTriggered() { OnAlertTriggered(); }

    private:
        std::shared_ptr<Core::Kernel::MicroKernel>* m_kernel;
        std::shared_ptr<Core::Services::IStockManager>* m_stockManager;
        std::shared_ptr<Core::Services::IStockPriceService>* m_priceService;
        std::shared_ptr<Core::Services::IKDJCalculator>* m_kdjCalculator;
        std::shared_ptr<Core::Services::IAlertEngine>* m_alertEngine;
    };

}}
