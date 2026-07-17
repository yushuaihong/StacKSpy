using System.ComponentModel;

namespace StacKSpy.App.ViewModels;

public class KDJViewModel : INotifyPropertyChanged
{
    private string _code = "";
    private string _name = "";
    private double _dailyJ;
    private double _weeklyJ;
    private double _monthlyJ;

    public string Code { get => _code; set { _code = value; OnPropertyChanged(nameof(Code)); } }
    public string Name { get => _name; set { _name = value; OnPropertyChanged(nameof(Name)); } }
    public double DailyJ { get => _dailyJ; set { _dailyJ = value; OnPropertyChanged(nameof(DailyJ)); } }
    public double WeeklyJ { get => _weeklyJ; set { _weeklyJ = value; OnPropertyChanged(nameof(WeeklyJ)); } }
    public double MonthlyJ { get => _monthlyJ; set { _monthlyJ = value; OnPropertyChanged(nameof(MonthlyJ)); } }

    public event PropertyChangedEventHandler? PropertyChanged;
    protected void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
}
