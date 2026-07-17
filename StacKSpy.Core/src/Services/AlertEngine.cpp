#include "Services/AlertEngine.h"

namespace StacKSpy { namespace Core { namespace Services {

    Models::AlertLevel AlertEngine::DetermineLevel(double jValue) const {
        if (jValue < 0.0) return Models::AlertLevel::High;
        if (jValue < 5.0) return Models::AlertLevel::Medium;
        if (jValue <= 5.0) return Models::AlertLevel::Low;
        return Models::AlertLevel::None;
    }

    std::vector<Models::Alert> AlertEngine::Evaluate(const Models::KDJIndicator& indicator) {
        std::vector<Models::Alert> alerts;
        // 仅针对周线和月线 J 值触发告警
        auto weeklyLevel = DetermineLevel(indicator.WeeklyJ);
        if (weeklyLevel != Models::AlertLevel::None) {
            Models::Alert alert(indicator.StockCode, indicator.StockName,
                                indicator.WeeklyJ, weeklyLevel, "Weekly", "");
            alerts.push_back(alert);
            m_activeAlerts[indicator.StockCode + "_Weekly"] = alert;
        } else {
            m_activeAlerts.erase(indicator.StockCode + "_Weekly");
        }

        auto monthlyLevel = DetermineLevel(indicator.MonthlyJ);
        if (monthlyLevel != Models::AlertLevel::None) {
            Models::Alert alert(indicator.StockCode, indicator.StockName,
                                indicator.MonthlyJ, monthlyLevel, "Monthly", "");
            alerts.push_back(alert);
            m_activeAlerts[indicator.StockCode + "_Monthly"] = alert;
        } else {
            m_activeAlerts.erase(indicator.StockCode + "_Monthly");
        }
        return alerts;
    }

    std::vector<Models::Alert> AlertEngine::GetActiveAlerts() const {
        std::vector<Models::Alert> result;
        for (const auto& [key, alert] : m_activeAlerts)
            result.push_back(alert);
        return result;
    }

    void AlertEngine::ClearAlert(const std::string& stockCode, const std::string& period) {
        m_activeAlerts.erase(stockCode + "_" + period);
    }

}}}
