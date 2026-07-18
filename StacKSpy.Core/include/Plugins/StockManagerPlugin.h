#pragma once
#include "Plugins/IPlugin.h"
#include "Services/IStockManager.h"
#include "Kernel/IEventBus.h"
#include <memory>

namespace StacKSpy { namespace Core { namespace Plugins {

    class StockManagerPlugin : public IPlugin {
    public:
        StockManagerPlugin(std::shared_ptr<Services::IStockManager> stockManager,
                           std::shared_ptr<Kernel::IEventBus> eventBus);

        std::string GetName() const override { return "StockManagerPlugin"; }
        void Initialize() override;
        void Start() override;
        void Stop() override;
        void Destroy() override;

    private:
        std::shared_ptr<Services::IStockManager> m_stockManager;
        std::shared_ptr<Kernel::IEventBus> m_eventBus;
    };

}}}
