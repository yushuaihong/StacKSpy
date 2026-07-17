#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Models {

    enum class AlertLevel {
        None = 0,       // J值 > 5，无告警
        Low = 1,        // J值 <= 5，普通提示（无背景）
        Medium = 2,     // 0 <= J值 < 5，红色背景 + 白色字体
        High = 3        // J值 < 0，黄色背景 + 白色字体
    };

    struct Alert {
        std::string StockCode;
        std::string StockName;
        double JValue = 0.0;
        AlertLevel Level = AlertLevel::None;
        std::string Period;       // "Weekly" 或 "Monthly"
        std::string Message;

        Alert() = default;
        Alert(const std::string& code, const std::string& name,
              double jValue, AlertLevel level,
              const std::string& period, const std::string& message);
    };

}}}
