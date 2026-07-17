#include "Models/StockPrice.h"

namespace StacKSpy { namespace Core { namespace Models {

    StockPrice::StockPrice(const std::string& code, const std::string& name,
                           double price, double change, double turnover,
                           const std::string& updateTime)
        : StockCode(code), StockName(name), Price(price),
          ChangePercent(change), TurnoverRate(turnover), UpdateTime(updateTime) {}

}}}
