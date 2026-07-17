#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IKDJCalculator.h"
#include "Services/IStockManager.h"
#include "Kernel/IEventBus.h"
#include "Scheduler/ITaskScheduler.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class KDJCalculatorPlugin : public IPlugin {
    public:
        KDJCalculatorPlugin(std::shared_ptr<Services::IKDJCalculator> calculator,
                            std::shared_ptr<Services::IStockManager> stockManager,
                            std::shared_ptr<Kernel::IEventBus> eventBus,
                            std::shared_ptr<Scheduler::ITaskScheduler> scheduler);

        std::string GetName() const override { return "KDJCalculatorPlugin"; }
        std::string GetVersion() const override { return "1.0.0"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        void OnDailyRefresh();
        std::shared_ptr<Services::IKDJCalculator> m_calculator;
        std::shared_ptr<Services::IStockManager> m_stockManager;
        std::shared_ptr<Kernel::IEventBus> m_eventBus;
        std::shared_ptr<Scheduler::ITaskScheduler> m_scheduler;
    };

}}}
