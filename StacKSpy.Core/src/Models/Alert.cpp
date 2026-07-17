#include "Models/Alert.h"

namespace StacKSpy { namespace Core { namespace Models {

    Alert::Alert(const std::string& code, const std::string& name,
                 double jValue, AlertLevel level,
                 const std::string& period, const std::string& message)
        : StockCode(code), StockName(name), JValue(jValue),
          Level(level), Period(period), Message(message) {}

}}}
