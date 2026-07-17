using System.ComponentModel;

namespace StacKSpy.App.ViewModels;

public class StockPriceViewModel : INotifyPropertyChanged
{
    private string _code = "";
    private string _name = "";
    private double _price;
    private double _changePercent;
    private double _turnoverRate;
    private string _updateTime = "";

    public string Code { get => _code; set { _code = value; OnPropertyChanged(nameof(Code)); } }
    public string Name { get => _name; set { _name = value; OnPropertyChanged(nameof(Name)); } }
    public double Price { get => _price; set { _price = value; OnPropertyChanged(nameof(Price)); } }
    public double ChangePercent { get => _changePercent; set { _changePercent = value; OnPropertyChanged(nameof(ChangePercent)); } }
    public double TurnoverRate { get => _turnoverRate; set { _turnoverRate = value; OnPropertyChanged(nameof(TurnoverRate)); } }
    public string UpdateTime { get => _updateTime; set { _updateTime = value; OnPropertyChanged(nameof(UpdateTime)); } }

    public event PropertyChangedEventHandler? PropertyChanged;
    protected void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
}
