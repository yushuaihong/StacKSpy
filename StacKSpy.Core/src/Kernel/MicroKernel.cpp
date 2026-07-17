#include "Kernel/MicroKernel.h"
#include "Services/ConfigManager.h"

namespace StacKSpy { namespace Core { namespace Kernel {

    MicroKernel::MicroKernel()
        : m_eventBus(std::make_shared<EventBus>())
        , m_pluginManager(std::make_shared<PluginManager>())
        , m_scheduler(std::make_shared<Scheduler::TaskScheduler>())
        , m_configManager(std::make_shared<Services::ConfigManager>()) {}

    MicroKernel::~MicroKernel() { Shutdown(); }

    void MicroKernel::Initialize() {
        m_configManager->Load();
        m_scheduler->Start();
    }

    void MicroKernel::Start() {
        m_pluginManager->InitializeAll();
        m_pluginManager->StartAll();
    }

    void MicroKernel::Stop() {
        m_pluginManager->StopAll();
        m_scheduler->Stop();
    }

    void MicroKernel::Shutdown() {
        Stop();
        m_pluginManager->DestroyAll();
        m_configManager->Save();
    }

    std::shared_ptr<IEventBus> MicroKernel::GetEventBus() const { return m_eventBus; }
    std::shared_ptr<IPluginManager> MicroKernel::GetPluginManager() const { return m_pluginManager; }
    std::shared_ptr<Scheduler::ITaskScheduler> MicroKernel::GetScheduler() const { return m_scheduler; }
    std::shared_ptr<Services::IConfigManager> MicroKernel::GetConfigManager() const { return m_configManager; }

}}}
