#pragma once
#include <string>

namespace StacKSpy { namespace Core { namespace Models {

    struct KLineData {
        std::string Date;
        double Open = 0.0;
        double Close = 0.0;
        double High = 0.0;
        double Low = 0.0;
        double Volume = 0.0;

        KLineData() = default;
        KLineData(const std::string& date, double open, double close,
                  double high, double low, double volume)
            : Date(date), Open(open), Close(close),
              High(high), Low(low), Volume(volume) {}
    };

}}}
