#pragma once
#include "Kernel/EventBus.h"
#include "Kernel/PluginManager.h"
#include "Scheduler/TaskScheduler.h"
#include "Services/IConfigManager.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Kernel {

    class MicroKernel {
    public:
        MicroKernel();
        ~MicroKernel();

        void Initialize();
        void Start();
        void Stop();
        void Shutdown();

        std::shared_ptr<IEventBus> GetEventBus() const;
        std::shared_ptr<IPluginManager> GetPluginManager() const;
        std::shared_ptr<Scheduler::ITaskScheduler> GetScheduler() const;
        std::shared_ptr<Services::IConfigManager> GetConfigManager() const;

    private:
        std::shared_ptr<EventBus> m_eventBus;
        std::shared_ptr<PluginManager> m_pluginManager;
        std::shared_ptr<Scheduler::TaskScheduler> m_scheduler;
        std::shared_ptr<Services::IConfigManager> m_configManager;
    };

}}}
