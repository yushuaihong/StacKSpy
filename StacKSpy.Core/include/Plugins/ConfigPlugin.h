#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IConfigManager.h"
#include "Kernel/IEventBus.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class ConfigPlugin : public IPlugin {
    public:
        ConfigPlugin(std::shared_ptr<Services::IConfigManager> configManager,
                     std::shared_ptr<Kernel::IEventBus> eventBus);

        std::string GetName() const override { return "ConfigPlugin"; }
        std::string GetVersion() const override { return "1.0.0"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        std::shared_ptr<Services::IConfigManager> m_configManager;
        std::shared_ptr<Kernel::IEventBus> m_eventBus;
    };

}}}
