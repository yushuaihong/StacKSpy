#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IAlertEngine.h"
#include "Kernel/IEventBus.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class AlertPlugin : public IPlugin {
    public:
        AlertPlugin(std::shared_ptr<Services::IAlertEngine> alertEngine,
                    std::shared_ptr<Kernel::IEventBus> eventBus);

        std::string GetName() const override { return "AlertPlugin"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        void OnKDJUpdated(const Kernel::EventData& data);
        std::shared_ptr<Services::IAlertEngine> m_alertEngine;
        std::shared_ptr<Kernel::IEventBus> m_eventBus;
    };

}}}
