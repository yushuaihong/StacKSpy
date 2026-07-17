#include "Plugins/AutoStartPlugin.h"
#include <windows.h>

namespace StacKSpy { namespace Core { namespace Plugins {

    AutoStartPlugin::AutoStartPlugin(std::shared_ptr<Services::IConfigManager> configManager)
        : m_configManager(configManager) {}

    void AutoStartPlugin::Initialize() {}

    void AutoStartPlugin::Start() {
        if (m_configManager->GetBool("AutoStart", true)) {
            RegisterWindowsService();
        }
    }

    void AutoStartPlugin::Stop() {}

    void AutoStartPlugin::Destroy() {}

    bool AutoStartPlugin::RegisterWindowsService() {
        // TODO: 通过 sc create 注册 Windows 服务实现开机自启
        // 使用 ShellExecute 或 CreateProcess 调用:
        // sc create StacKSpy binPath= "应用路径" start= auto
        return true;
    }

    bool AutoStartPlugin::UnregisterWindowsService() {
        // TODO: sc delete StacKSpy
        return true;
    }

}}}
