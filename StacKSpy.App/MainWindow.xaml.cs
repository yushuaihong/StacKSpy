using System.Windows;
using StacKSpy.App.ViewModels;

namespace StacKSpy.App;

public partial class MainWindow : Window
{
    private readonly MainViewModel _vm = new();

    public MainWindow()
    {
        InitializeComponent();
        DataContext = _vm;
        Title = $"StacKSpy {App.Bridge?.GetVersion()} - 股票监控系统";
        Loaded += (_, _) => _vm.StartRefresh();
        Closed += (_, _) => _vm.StopRefresh();
    }

    private void BtnSwitchMini_Click(object sender, RoutedEventArgs e)
    {
        var miniWindow = new MiniWindow();
        miniWindow.Show();
        this.Hide();
    }

    private void BtnApplyConfig_Click(object sender, RoutedEventArgs e)
    {
        if (int.TryParse(TxtRefreshInterval.Text, out var seconds))
        {
            _vm.SetRefreshInterval(seconds);
            App.Bridge?.UpdateConfig("PriceRefreshInterval", TxtRefreshInterval.Text);
            TxtStatus.Text = $"刷新间隔已更新为 {seconds} 秒";
        }
        else
        {
            MessageBox.Show("请输入有效的数字", "提示", MessageBoxButton.OK, MessageBoxImage.Warning);
        }
    }

    private void BtnAddStock_Click(object sender, RoutedEventArgs e)
    {
        var code = TxtStockCode.Text.Trim();
        if (string.IsNullOrEmpty(code))
        {
            MessageBox.Show("请输入股票代码", "提示", MessageBoxButton.OK, MessageBoxImage.Warning);
            return;
        }
        _vm.AddStock(code);
        TxtStatus.Text = $"已添加股票: {code}";
        TxtStockCode.Clear();
    }

    private void BtnRemoveStock_Click(object sender, RoutedEventArgs e)
    {
        if (DgStockPool.SelectedItem is StockPriceViewModel selected)
        {
            _vm.RemoveStock(selected.Code);
            TxtStatus.Text = $"已删除股票: {selected.Code}";
        }
    }
}
