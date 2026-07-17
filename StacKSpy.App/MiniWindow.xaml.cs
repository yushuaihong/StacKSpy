using System.Windows;
using System.Windows.Input;

namespace StacKSpy.App;

public partial class MiniWindow : Window
{
    public MiniWindow()
    {
        InitializeComponent();
    }

    private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
    {
        if (e.ClickCount == 2)
        {
            // 双击切换回主窗口
            var mainWindow = new MainWindow();
            mainWindow.Show();
            this.Close();
        }
        else
        {
            // 拖动窗口
            DragMove();
        }
    }
}
