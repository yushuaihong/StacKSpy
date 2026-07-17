#pragma once
#include <vector>
#include <string>

namespace StacKSpy { namespace Core { namespace Services {

    class IStockManager {
    public:
        virtual ~IStockManager() = default;
        virtual bool AddStock(const std::string& code) = 0;
        virtual bool RemoveStock(const std::string& code) = 0;
        virtual std::vector<std::string> GetStockPool() const = 0;
        virtual bool ContainsStock(const std::string& code) const = 0;
        virtual void SaveStockPool() = 0;
        virtual void LoadStockPool() = 0;
    };

}}}
