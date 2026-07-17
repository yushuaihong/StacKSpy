#include "Models/KDJIndicator.h"

namespace StacKSpy { namespace Core { namespace Models {

    KDJIndicator::KDJIndicator(const std::string& code, const std::string& name,
                               double dailyJ, double weeklyJ, double monthlyJ,
                               const std::string& updateDate)
        : StockCode(code), StockName(name), DailyJ(dailyJ),
          WeeklyJ(weeklyJ), MonthlyJ(monthlyJ), UpdateDate(updateDate) {}

}}}
