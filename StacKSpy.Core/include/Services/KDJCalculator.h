#pragma once
#include "Services/IKDJCalculator.h"

namespace StacKSpy { namespace Core { namespace Services {

    class KDJCalculator : public IKDJCalculator {
    public:
        Models::KDJIndicator Calculate(const std::string& stockCode) override;
        double CalculateDailyJ(const std::string& stockCode) override;
        double CalculateWeeklyJ(const std::string& stockCode) override;
        double CalculateMonthlyJ(const std::string& stockCode) override;
    };

}}}
