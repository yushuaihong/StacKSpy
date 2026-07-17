#pragma once
#include "Kernel/IPluginManager.h"
#include <vector>

namespace StacKSpy { namespace Core { namespace Kernel {

    class PluginManager : public IPluginManager {
    public:
        void RegisterPlugin(std::shared_ptr<Plugins::IPlugin> plugin) override;
        void UnregisterPlugin(const std::string& name) override;
        void InitializeAll() override;
        void StartAll() override;
        void StopAll() override;
        void DestroyAll() override;

    private:
        std::vector<std::shared_ptr<Plugins::IPlugin>> m_plugins;
    };

}}}
