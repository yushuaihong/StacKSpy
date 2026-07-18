#pragma once
#include "Services/IStockManager.h"
#include "Models/StockInfo.h"
#include <mutex>

namespace StacKSpy { namespace Core { namespace Services {

    class IStockPriceService;

    class StockManager : public IStockManager {
    public:
        StockManager() = default;
        explicit StockManager(IStockPriceService* priceService);

        bool AddStock(const std::string& code) override;
        bool RemoveStock(const std::string& code) override;
        std::vector<Models::StockInfo> GetStockPool() const override;
        bool ContainsStock(const std::string& code) const override;
        void SaveStockPool() override;
        void LoadStockPool() override;

        void SetConfigPath(const std::string& path);
        void SetPriceService(IStockPriceService* priceService);

    private:
        bool ContainsLocked(const std::string& code) const;

        std::vector<Models::StockInfo> m_stockPool;
        mutable std::mutex m_mutex;
        std::string m_configPath = "stockpool.json";
        IStockPriceService* m_priceService = nullptr;
    };

}}}
