#include "Plugins/StockPricePlugin.h"
#include "Services/StockManager.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    StockPricePlugin::StockPricePlugin(std::shared_ptr<Services::IStockPriceService> priceService,
                                       std::shared_ptr<Kernel::IEventBus> eventBus,
                                       std::shared_ptr<Scheduler::ITaskScheduler> scheduler)
        : m_priceService(priceService), m_eventBus(eventBus), m_scheduler(scheduler) {}

    void StockPricePlugin::Initialize() {}

    void StockPricePlugin::Start() {
        m_scheduler->ScheduleInterval("PriceRefresh", [this]() { OnRefreshTimer(); },
                                       std::chrono::seconds(m_refreshIntervalSeconds));
    }

    void StockPricePlugin::Stop() {
        m_scheduler->Cancel("PriceRefresh");
    }

    void StockPricePlugin::Destroy() {}

    void StockPricePlugin::OnRefreshTimer() {
        // TODO: 从 StockManager 获取股票池，调用 FetchAllPrices，通过 EventBus 广播
        m_eventBus->Publish("PRICE_UPDATE", {});
    }

}}}
