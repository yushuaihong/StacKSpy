using System.Windows;
using StacKSpy.Bridge;

namespace StacKSpy.App;

public partial class App : Application
{
    private StockBridge? _bridge;

    public static StockBridge? Bridge { get; private set; }

    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);
        _bridge = new StockBridge();
        _bridge.Initialize();
        _bridge.Start();
        Bridge = _bridge;
    }

    protected override void OnExit(ExitEventArgs e)
    {
        _bridge?.Stop();
        _bridge?.Dispose();
        base.OnExit(e);
    }
}
