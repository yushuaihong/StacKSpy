#pragma once

namespace StacKSpy { namespace Bridge {

    public ref class ManagedStockPrice {
    public:
        property System::String^ Code;
        property System::String^ Name;
        property double Price;
        property double ChangePercent;
        property double TurnoverRate;
        property System::String^ UpdateTime;
    };

    public ref class ManagedKDJIndicator {
    public:
        property System::String^ Code;
        property System::String^ Name;
        property double DailyJ;
        property double WeeklyJ;
        property double MonthlyJ;
        property System::String^ UpdateDate;
    };

    public ref class ManagedAlert {
    public:
        property System::String^ Code;
        property System::String^ Name;
        property double JValue;
        property int Level;      // 0=None, 1=Low, 2=Medium, 3=High
        property System::String^ Period; // "Weekly" / "Monthly"
        property System::String^ Message;
    };

}}
