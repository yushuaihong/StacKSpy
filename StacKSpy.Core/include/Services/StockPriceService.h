#pragma once
#include "Services/IStockPriceService.h"

namespace StacKSpy { namespace Core { namespace Services {

    class StockPriceService : public IStockPriceService {
    public:
        std::vector<Models::StockPrice> FetchAllPrices(const std::vector<std::string>& stockCodes) override;
        std::string QueryStockName(const std::string& code) override;
        std::vector<Models::KLineData> FetchKLine(
            const std::string& code,
            const std::string& period,
            int count = 50) override;
    };

}}}
