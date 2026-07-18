#pragma once
#include "Services/IStockManager.h"
#include "Models/StockInfo.h"
#include <mutex>

namespace StacKSpy { namespace Core { namespace Services {

    class StockManager : public IStockManager {
    public:
        bool AddStock(const std::string& code) override;
        bool RemoveStock(const std::string& code) override;
        std::vector<std::string> GetStockPool() const override;
        bool ContainsStock(const std::string& code) const override;
        void SaveStockPool() override;
        void LoadStockPool() override;

        void SetConfigPath(const std::string& path);

    private:
        bool ContainsLocked(const std::string& code) const; // 调用者已持锁

        std::vector <Models::StockInfo> m_stockPool;
        mutable std::mutex m_mutex;
        std::string m_configPath = "stockpool.json";
    };

}}}
