#include "Plugins/StockManagerPlugin.h"

namespace StacKSpy { namespace Core { namespace Plugins {

    StockManagerPlugin::StockManagerPlugin(std::shared_ptr<Services::IStockManager> stockManager,
                                           std::shared_ptr<Kernel::IEventBus> eventBus)
        : m_stockManager(stockManager), m_eventBus(eventBus) {}

    void StockManagerPlugin::Initialize() {
        m_stockManager->LoadStockPool();
    }

    void StockManagerPlugin::Start() {}
    void StockManagerPlugin::Stop() { m_stockManager->SaveStockPool(); }
    void StockManagerPlugin::Destroy() {}

}}}
