using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Threading;
using StacKSpy.Bridge;

namespace StacKSpy.App.ViewModels;

public class MainViewModel : INotifyPropertyChanged
{
    private readonly System.Timers.Timer _timer = new System.Timers.Timer(60000);
    private readonly StockBridge? _bridge;

    public ObservableCollection<StockPriceViewModel> Prices { get; } = new();
    public ObservableCollection<KDJViewModel> KDJIndicators { get; } = new();
    public ObservableCollection<AlertViewModel> Alerts { get; } = new();
    public ObservableCollection<StockPriceViewModel> StockPool { get; } = new();

    public event PropertyChangedEventHandler? PropertyChanged;
    protected void OnPropertyChanged(string propertyName)
        => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

    public MainViewModel()
    {
        _bridge = App.Bridge;
        if (_bridge is not null)
        {
            _bridge.OnPriceUpdated += RefreshPrices;
            _bridge.OnKDJUpdated += RefreshKDJ;
            _bridge.OnAlertTriggered += RefreshAlerts;
        }

        _timer.Elapsed += (_, _) => RefreshAll();
    }

    public void StartRefresh()
    {
        RefreshAll();
        _timer.Start();
    }

    public void StopRefresh()
    {
        _timer.Stop();
    }

    public void SetRefreshInterval(int seconds)
    {
        if (seconds < 5) seconds = 5;
        _timer.Interval = seconds * 1000;
    }

    public void AddStock(string code)
    {
        _bridge?.AddStock(code);
        RefreshStockPool();
    }

    public void RemoveStock(string code)
    {
        _bridge?.RemoveStock(code);
        RefreshStockPool();
    }

    public void RefreshAll()
    {
        RefreshPrices();
        RefreshKDJ();
        RefreshAlerts();
        RefreshStockPool();
    }

    private void RefreshPrices()
    {
        if (_bridge is null) return;
        var data = _bridge.FetchAllPrices();
        Application.Current?.Dispatcher.Invoke(() =>
        {
            Prices.Clear();
            foreach (var item in data)
            {
                Prices.Add(new StockPriceViewModel
                {
                    Code = item.Code ?? "",
                    Name = item.Name ?? "",
                    Price = item.Price,
                    ChangePercent = item.ChangePercent,
                    TurnoverRate = item.TurnoverRate,
                    UpdateTime = item.UpdateTime ?? ""
                });
            }
            OnPropertyChanged(nameof(Prices));
        });
    }

    private void RefreshKDJ()
    {
        if (_bridge is null)
        {
            return;
        }
        var pool = _bridge.GetStockPool();
        if (pool.Count == 0)
        {
            return;
        }

        var datas = new List<KDJViewModel>();
        foreach (var stock in pool)
        {
            var kdj = _bridge.CalculateKDJ(stock.Code, "Daily");
            datas.Add(new KDJViewModel
            {
                Code = stock.Code ?? "",
                Name = stock.Name ?? kdj.Name ?? "",
                DailyJ = kdj.DailyJ,
                WeeklyJ = kdj.WeeklyJ,
                MonthlyJ = kdj.MonthlyJ
            });
        }

        Application.Current?.Dispatcher.Invoke(() =>
        {
            KDJIndicators.Clear();
            foreach (var stockData in datas)
            {
                KDJIndicators.Add(stockData);
            }
            OnPropertyChanged(nameof(KDJIndicators));
        });
    }

    private void RefreshAlerts()
    {
        if (_bridge is null) return;
        var data = _bridge.GetAlerts();
        Application.Current?.Dispatcher.Invoke(() =>
        {
            Alerts.Clear();
            foreach (var item in data)
            {
                Alerts.Add(new AlertViewModel
                {
                    Code = item.Code ?? "",
                    Name = item.Name ?? "",
                    JValue = item.JValue,
                    Level = item.Level,
                    Period = item.Period ?? "",
                    Message = item.Message ?? ""
                });
            }
            OnPropertyChanged(nameof(Alerts));
        });
    }

    private void RefreshStockPool()
    {
        if (_bridge is null) return;
        var pool = _bridge.GetStockPool();

        Application.Current?.Dispatcher.Invoke(() =>
        {
            StockPool.Clear();
            foreach (var stock in pool)
            {
                StockPool.Add(new StockPriceViewModel { Code = stock.Code ?? "", Name = stock.Name ?? "" });
            }
            OnPropertyChanged(nameof(StockPool));
        });
    }
}
