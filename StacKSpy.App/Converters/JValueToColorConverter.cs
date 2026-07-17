using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace StacKSpy.App.Converters;

/// <summary>
/// 根据 J 值转换为告警背景色：
/// J值 < 0   → 黄色背景
/// 0 ≤ J值 < 5 → 红色背景
/// J值 ≤ 5   → 透明（无背景）
/// J值 > 5   → 透明（无背景）
/// </summary>
public class JValueToColorConverter : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        if (value is double jValue)
        {
            if (jValue < 0)
                return Brushes.Yellow;       // 黄色背景 + 白色字体
            if (jValue < 5)
                return Brushes.Red;          // 红色背景 + 白色字体
            if (jValue <= 5)
                return Brushes.Transparent;  // 无背景（普通文本）
        }
        return Brushes.Transparent;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        throw new NotImplementedException();
    }
}
