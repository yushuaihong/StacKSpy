#include "Plugins/ConfigPlugin.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    ConfigPlugin::ConfigPlugin(std::shared_ptr<Services::IConfigManager> configManager,
                               std::shared_ptr<Kernel::IEventBus> eventBus)
        : m_configManager(configManager), m_eventBus(eventBus) {}

    void ConfigPlugin::Initialize() {
        m_configManager->Load();
    }

    void ConfigPlugin::Start() {}
    void ConfigPlugin::Stop() { m_configManager->Save(); }
    void ConfigPlugin::Destroy() {}

}}}
