#pragma once
#include "Models/StockPrice.h"
#include "Models/KLineData.h"
#include <vector>

namespace StacKSpy { namespace Core { namespace Services {

    class IStockPriceService {
    public:
        virtual ~IStockPriceService() = default;
        virtual std::vector<Models::StockPrice> FetchAllPrices(const std::vector<std::string>& stockCodes) = 0;
        virtual std::string QueryStockName(const std::string& code) = 0;
        virtual std::vector<Models::KLineData> FetchKLine(
            const std::string& code,
            const std::string& period,
            int count = 50) = 0;
    };

}}}
