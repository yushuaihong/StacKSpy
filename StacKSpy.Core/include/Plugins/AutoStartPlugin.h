#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IConfigManager.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class AutoStartPlugin : public IPlugin {
    public:
        AutoStartPlugin(std::shared_ptr<Services::IConfigManager> configManager);

        std::string GetName() const override { return "AutoStartPlugin"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        bool RegisterWindowsService();
        bool UnregisterWindowsService();
        std::shared_ptr<Services::IConfigManager> m_configManager;
    };

}}}
