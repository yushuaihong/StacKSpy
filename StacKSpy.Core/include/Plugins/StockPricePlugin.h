#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IStockPriceService.h"
#include "Kernel/IEventBus.h"
#include "Scheduler/ITaskScheduler.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class StockPricePlugin : public IPlugin {
    public:
        StockPricePlugin(std::shared_ptr<Services::IStockPriceService> priceService,
                         std::shared_ptr<Kernel::IEventBus> eventBus,
                         std::shared_ptr<Scheduler::ITaskScheduler> scheduler);

        std::string GetName() const override { return "StockPricePlugin"; }
        std::string GetVersion() const override { return "1.0.0"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        void OnRefreshTimer();
        std::shared_ptr<Services::IStockPriceService> m_priceService;
        std::shared_ptr<Kernel::IEventBus> m_eventBus;
        std::shared_ptr<Scheduler::ITaskScheduler> m_scheduler;
        int m_refreshIntervalSeconds = 60;
    };

}}}
