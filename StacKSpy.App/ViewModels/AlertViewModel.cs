using System.ComponentModel;

namespace StacKSpy.App.ViewModels;

public class AlertViewModel : INotifyPropertyChanged
{
    private string _code = "";
    private string _name = "";
    private double _jValue;
    private int _level;
    private string _period = "";
    private string _message = "";

    public string Code { get => _code; set { _code = value; OnPropertyChanged(nameof(Code)); } }
    public string Name { get => _name; set { _name = value; OnPropertyChanged(nameof(Name)); } }
    public double JValue { get => _jValue; set { _jValue = value; OnPropertyChanged(nameof(JValue)); } }
    public int Level { get => _level; set { _level = value; OnPropertyChanged(nameof(Level)); } }
    public string Period { get => _period; set { _period = value; OnPropertyChanged(nameof(Period)); } }
    public string Message { get => _message; set { _message = value; OnPropertyChanged(nameof(Message)); } }

    public event PropertyChangedEventHandler? PropertyChanged;
    protected void OnPropertyChanged(string name) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
}
