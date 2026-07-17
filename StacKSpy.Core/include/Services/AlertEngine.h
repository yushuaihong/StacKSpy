#pragma once
#include "Services/IAlertEngine.h"
#include <unordered_map>

namespace StacKSpy { namespace Core { namespace Services {

    class AlertEngine : public IAlertEngine {
    public:
        std::vector<Models::Alert> Evaluate(const Models::KDJIndicator& indicator) override;
        std::vector<Models::Alert> GetActiveAlerts() const override;
        void ClearAlert(const std::string& stockCode, const std::string& period) override;

    private:
        Models::AlertLevel DetermineLevel(double jValue) const;
        std::unordered_map<std::string, Models::Alert> m_activeAlerts;
    };

}}}
