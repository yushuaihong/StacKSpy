#include "Plugins/KDJCalculatorPlugin.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    KDJCalculatorPlugin::KDJCalculatorPlugin(std::shared_ptr<Services::IKDJCalculator> calculator,
                                             std::shared_ptr<Services::IStockManager> stockManager,
                                             std::shared_ptr<Kernel::IEventBus> eventBus,
                                             std::shared_ptr<Scheduler::ITaskScheduler> scheduler)
        : m_calculator(calculator), m_stockManager(stockManager),
          m_eventBus(eventBus), m_scheduler(scheduler) {}

    void KDJCalculatorPlugin::Initialize() {}

    void KDJCalculatorPlugin::Start() {
        m_scheduler->ScheduleDaily("KDJRefresh", [this]() { OnDailyRefresh(); }, 17, 0);
    }

    void KDJCalculatorPlugin::Stop() {
        m_scheduler->Cancel("KDJRefresh");
    }

    void KDJCalculatorPlugin::Destroy() {}

    void KDJCalculatorPlugin::OnDailyRefresh() {
        // TODO: 遍历股票池，计算KDJ，通过 EventBus 广播
        m_eventBus->Publish("KDJ_UPDATE", {});
    }

}}}
