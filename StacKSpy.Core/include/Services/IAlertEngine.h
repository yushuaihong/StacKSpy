#pragma once
#include "Models/Alert.h"
#include "Models/KDJIndicator.h"
#include <vector>

namespace StacKSpy { namespace Core { namespace Services {

    class IAlertEngine {
    public:
        virtual ~IAlertEngine() = default;
        virtual std::vector<Models::Alert> Evaluate(const Models::KDJIndicator& indicator) = 0;
        virtual std::vector<Models::Alert> GetActiveAlerts() const = 0;
        virtual void ClearAlert(const std::string& stockCode, const std::string& period) = 0;
    };

}}}
