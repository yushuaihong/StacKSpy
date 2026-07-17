#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Models {

    struct KDJIndicator {
        std::string StockCode;
        std::string StockName;
        double DailyJ = 0.0;
        double WeeklyJ = 0.0;
        double MonthlyJ = 0.0;
        std::string UpdateDate;

        KDJIndicator() = default;
        KDJIndicator(const std::string& code, const std::string& name,
                     double dailyJ, double weeklyJ, double monthlyJ,
                     const std::string& updateDate);
    };

}}}
