#pragma once
#include "Plugins/IPlugin.h"
#include <memory>
#include <string>

namespace StacKSpy { namespace Core { namespace Kernel {

    class IPluginManager {
    public:
        virtual ~IPluginManager() = default;
        virtual void RegisterPlugin(std::shared_ptr<Plugins::IPlugin> plugin) = 0;
        virtual void UnregisterPlugin(const std::string& name) = 0;
        virtual void InitializeAll() = 0;
        virtual void StartAll() = 0;
        virtual void StopAll() = 0;
        virtual void DestroyAll() = 0;
    };

}}}
