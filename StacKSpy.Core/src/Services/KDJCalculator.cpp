#include "Services/KDJCalculator.h"
#include <algorithm>
#include <chrono>
#include <ctime>

namespace StacKSpy { namespace Core { namespace Services {

    KDJCalculator::KDJCalculator(IStockPriceService* priceService)
        : m_priceService(priceService) {}

    void KDJCalculator::SetPriceService(IStockPriceService* priceService) {
        m_priceService = priceService;
    }

    // ===== 核心 KDJ 算法 =====
    // RSV = (Close - Low_9) / (High_9 - Low_9) * 100
    // K = 2/3 * 前K + 1/3 * RSV  (初始值50)
    // D = 2/3 * 前D + 1/3 * K    (初始值50)
    // J = 3K - 2D
    double KDJCalculator::ComputeJ(const std::vector<Models::KLineData>& klines) {
        if (klines.empty())
        {
            return 50.0;
        }

        const int period = 9;
        double k = 50.0;
        double d = 50.0;

        for (size_t i = 0; i < klines.size(); ++i) {
            // 计算9周期内的最高价和最低价
            size_t start = (i >= static_cast<size_t>(period - 1)) ? (i - period + 1) : 0;
            double highest = klines[start].High;
            double lowest = klines[start].Low;
            for (size_t j = start + 1; j <= i; ++j) {
                highest = std::max(highest, klines[j].High);
                lowest = std::min(lowest, klines[j].Low);
            }

            // 计算 RSV
            double rsv = 50.0;  // 高低点相同时默认50
            if (highest > lowest) {
                rsv = (klines[i].Close - lowest) / (highest - lowest) * 100.0;
            }

            // 平滑计算 K 和 D
            k = 2.0 / 3.0 * k + 1.0 / 3.0 * rsv;
            d = 2.0 / 3.0 * d + 1.0 / 3.0 * k;
        }

        return 3.0 * k - 2.0 * d;
    }

    double KDJCalculator::FetchAndComputeJ(const std::string& code, const std::string& period) {
        if (!m_priceService) return 50.0;

        auto klines = m_priceService->FetchKLine(code, period, 50);
        if (klines.empty()) return 50.0;

        return ComputeJ(klines);
    }

    Models::KDJIndicator KDJCalculator::Calculate(const std::string& stockCode) {
        Models::KDJIndicator result;
        result.StockCode = stockCode;
        result.DailyJ = CalculateDailyJ(stockCode);
        result.WeeklyJ = CalculateWeeklyJ(stockCode);
        result.MonthlyJ = CalculateMonthlyJ(stockCode);

        // 获取当前日期作为更新日期
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        struct tm tmBuf = {};
        localtime_s(&tmBuf, &timeT);
        char dateBuf[16];
        std::strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", &tmBuf);
        result.UpdateDate = dateBuf;

        return result;
    }

    double KDJCalculator::CalculateDailyJ(const std::string& stockCode) {
        return FetchAndComputeJ(stockCode, "day");
    }

    double KDJCalculator::CalculateWeeklyJ(const std::string& stockCode) {
        return FetchAndComputeJ(stockCode, "week");
    }

    double KDJCalculator::CalculateMonthlyJ(const std::string& stockCode) {
        return FetchAndComputeJ(stockCode, "month");
    }

}}}
