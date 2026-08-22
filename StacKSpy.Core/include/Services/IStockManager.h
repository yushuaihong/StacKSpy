#pragma once
#include <vector>
#include <string>
#include "Models/StockInfo.h"

namespace StacKSpy { namespace Core { namespace Services {

    class IStockManager {
    public:
        virtual ~IStockManager() = default;
        virtual bool AddStock(const std::string& code) = 0;
        virtual bool RemoveStock(const std::string& code) = 0;
        virtual std::vector<Models::StockInfo> GetStockPool() const = 0;
        virtual std::string GetStockName(const std::string& code) const = 0;
        virtual bool ContainsStock(const std::string& code) const = 0;
        virtual void SaveStockPool() = 0;
        virtual void LoadStockPool() = 0;
    };

}}}
