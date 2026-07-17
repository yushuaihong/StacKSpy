#include "Kernel/PluginManager.h"
#include <algorithm>

namespace StacKSpy { namespace Core { namespace Kernel {

    void PluginManager::RegisterPlugin(std::shared_ptr<Plugins::IPlugin> plugin) {
        m_plugins.push_back(plugin);
    }

    void PluginManager::UnregisterPlugin(const std::string& name) {
        m_plugins.erase(
            std::remove_if(m_plugins.begin(), m_plugins.end(),
                [&name](const auto& p) { return p->GetName() == name; }),
            m_plugins.end());
    }

    void PluginManager::InitializeAll() {
        for (auto& plugin : m_plugins) plugin->Initialize();
    }

    void PluginManager::StartAll() {
        for (auto& plugin : m_plugins) plugin->Start();
    }

    void PluginManager::StopAll() {
        for (auto it = m_plugins.rbegin(); it != m_plugins.rend(); ++it)
            (*it)->Stop();
    }

    void PluginManager::DestroyAll() {
        for (auto it = m_plugins.rbegin(); it != m_plugins.rend(); ++it)
            (*it)->Destroy();
        m_plugins.clear();
    }

}}}
