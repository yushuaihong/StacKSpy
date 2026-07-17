#pragma once
#include "Models/KDJIndicator.h"
#include <string>

namespace StacKSpy { namespace Core { namespace Services {

    class IKDJCalculator {
    public:
        virtual ~IKDJCalculator() = default;
        virtual Models::KDJIndicator Calculate(const std::string& stockCode) = 0;
        virtual double CalculateDailyJ(const std::string& stockCode) = 0;
        virtual double CalculateWeeklyJ(const std::string& stockCode) = 0;
        virtual double CalculateMonthlyJ(const std::string& stockCode) = 0;
    };

}}}
