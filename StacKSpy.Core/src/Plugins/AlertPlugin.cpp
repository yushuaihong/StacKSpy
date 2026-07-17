#include "Plugins/AlertPlugin.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    AlertPlugin::AlertPlugin(std::shared_ptr<Services::IAlertEngine> alertEngine,
                             std::shared_ptr<Kernel::IEventBus> eventBus)
        : m_alertEngine(alertEngine), m_eventBus(eventBus) {}

    void AlertPlugin::Initialize() {
        m_eventBus->Subscribe("KDJ_UPDATE", [this](const Kernel::EventData& data) {
            OnKDJUpdated(data);
        });
    }

    void AlertPlugin::Start() {}
    void AlertPlugin::Stop() { m_eventBus->Unsubscribe("KDJ_UPDATE"); }
    void AlertPlugin::Destroy() {}

    void AlertPlugin::OnKDJUpdated(const Kernel::EventData& data) {
        // TODO: 从 data 提取 KDJIndicator，调用 Evaluate，发布 ALERT_NOTIFY
        m_eventBus->Publish("ALERT_NOTIFY", {});
    }

}}}
