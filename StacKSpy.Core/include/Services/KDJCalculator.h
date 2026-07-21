#pragma once
#include "Services/IKDJCalculator.h"
#include "Services/IStockPriceService.h"
#include "Models/KLineData.h"
#include <vector>

namespace StacKSpy { namespace Core { namespace Services {

    class KDJCalculator : public IKDJCalculator {
    public:
        KDJCalculator() = default;
        explicit KDJCalculator(IStockPriceService* priceService);

        Models::KDJIndicator Calculate(const std::string& stockCode) override;
        double CalculateDailyJ(const std::string& stockCode) override;
        double CalculateWeeklyJ(const std::string& stockCode) override;
        double CalculateMonthlyJ(const std::string& stockCode) override;

        void SetPriceService(IStockPriceService* priceService);

    private:
        // 核心 KDJ 算法：从K线数据计算 J 值
        double ComputeJ(const std::vector<Models::KLineData>& klines);
        // 根据period获取K线并计算J
        double FetchAndComputeJ(const std::string& code, const std::string& period);

        IStockPriceService* m_priceService = nullptr;
    };

}}}
