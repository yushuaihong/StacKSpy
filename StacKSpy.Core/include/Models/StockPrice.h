#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Models {

    struct StockPrice {
        std::string StockCode;
        std::string StockName;
        double Price = 0.0;
        double ChangePercent = 0.0;
        double TurnoverRate = 0.0;
        std::string UpdateTime;

        StockPrice() = default;
        StockPrice(const std::string& code, const std::string& name,
                   double price, double change, double turnover,
                   const std::string& updateTime);
    };

}}}
