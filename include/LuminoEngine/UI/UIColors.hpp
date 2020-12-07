#pragma once
#include "Common.hpp"

namespace ln {
	
enum class UIColorHues
{
    Red,
    Pink,
    Purple,
    DeepPurple,
    Indigo,
    Blue,
    LightBlue,
    Cyan,
    Teal,
    Green,
    LightGreen,
    Lime,
    Yellow,
    Amber,
    Orange,
    DeepOrange,
    Brown,
    Grey,
    Black,
    White,
};

/**
 * 定義済みの色を取得するためのクラスです。
 * 
 * カラーパレットは MaterialDesign の色で定義されています。
 * https://material.io/design/color/the-color-system.html
 * 
 * MaterialDesign をベースとしていますが、連続的なアクセスを簡単にするため
 * カラーインデックス (引数 `shades`) は 0~9 で指定する点に注意してください。
 */
class UIColors
{
public:
    static const Color& red(int shades = 5) { return get(UIColorHues::Red, shades); }
    static const Color& pink(int shades = 5) { return get(UIColorHues::Pink, shades); }
    static const Color& purple(int shades = 5) { return get(UIColorHues::Purple, shades); }
    static const Color& deepPurple(int shades = 5) { return get(UIColorHues::DeepPurple, shades); }
    static const Color& indigo(int shades = 5) { return get(UIColorHues::Indigo, shades); }
    static const Color& blue(int shades = 5) { return get(UIColorHues::Blue, shades); }
    static const Color& lightBlue(int shades = 5) { return get(UIColorHues::LightBlue, shades); }
    static const Color& cyan(int shades = 5) { return get(UIColorHues::Cyan, shades); }
    static const Color& teal(int shades = 5) { return get(UIColorHues::Teal, shades); }
    static const Color& green(int shades = 5) { return get(UIColorHues::Green, shades); }
    static const Color& lightGreen(int shades = 5) { return get(UIColorHues::LightGreen, shades); }
    static const Color& lime(int shades = 5) { return get(UIColorHues::Lime, shades); };
    static const Color& yellow(int shades = 5) { return get(UIColorHues::Yellow, shades); }
    static const Color& amber(int shades = 5) { return get(UIColorHues::Amber, shades); }
    static const Color& orange(int shades = 5) { return get(UIColorHues::Orange, shades); }
    static const Color& deepOrange(int shades = 5) { return get(UIColorHues::DeepOrange, shades); }
    static const Color& brown(int shades = 5) { return get(UIColorHues::Brown, shades); }
    static const Color& grey(int shades = 5) { return get(UIColorHues::Grey, shades); }
    static const Color& black(int shades = 5) { return get(UIColorHues::Black, shades); }
    static const Color& white(int shades = 5) { return get(UIColorHues::White, shades); }

    static const int MaxShades = 10;

    /** 色を取得します。*/
    static const Color& get(UIColorHues hue, int shades = 5);
};

} // namespace ln

