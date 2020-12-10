#pragma once
#include "Common.hpp"

namespace ln {
	
/** カラーインデックス */
LN_ENUM()
enum class UIColorHues
{
    Red = 0,
    Pink = 1,
    Purple = 2,
    DeepPurple = 4,
    Indigo = 5,
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
LN_CLASS(Static)
class UIColors
{
public:
    /** 色の濃さ (0~9) を指定して、 Red カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& red(int shades = 5) { return get(UIColorHues::Red, shades); }

    /** 色の濃さ (0~9) を指定して、 Pink カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& pink(int shades = 5) { return get(UIColorHues::Pink, shades); }

    /** 色の濃さ (0~9) を指定して、 Purple カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& purple(int shades = 5) { return get(UIColorHues::Purple, shades); }

    /** 色の濃さ (0~9) を指定して、 DeepPurple カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& deepPurple(int shades = 5) { return get(UIColorHues::DeepPurple, shades); }

    /** 色の濃さ (0~9) を指定して、 Indigo カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& indigo(int shades = 5) { return get(UIColorHues::Indigo, shades); }

    /** 色の濃さ (0~9) を指定して、 Blue カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& blue(int shades = 5) { return get(UIColorHues::Blue, shades); }

    /** 色の濃さ (0~9) を指定して、 LightBlue カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& lightBlue(int shades = 5) { return get(UIColorHues::LightBlue, shades); }

    /** 色の濃さ (0~9) を指定して、 Cyan カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& cyan(int shades = 5) { return get(UIColorHues::Cyan, shades); }

    /** 色の濃さ (0~9) を指定して、 Teal カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& teal(int shades = 5) { return get(UIColorHues::Teal, shades); }

    /** 色の濃さ (0~9) を指定して、 Green カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& green(int shades = 5) { return get(UIColorHues::Green, shades); }

    /** 色の濃さ (0~9) を指定して、 LightGreen カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& lightGreen(int shades = 5) { return get(UIColorHues::LightGreen, shades); }

    /** 色の濃さ (0~9) を指定して、 Lime カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& lime(int shades = 5) { return get(UIColorHues::Lime, shades); };

    /** 色の濃さ (0~9) を指定して、 Yellow カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& yellow(int shades = 5) { return get(UIColorHues::Yellow, shades); }

    /** 色の濃さ (0~9) を指定して、 Amber カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& amber(int shades = 5) { return get(UIColorHues::Amber, shades); }

    /** 色の濃さ (0~9) を指定して、 Orange カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& orange(int shades = 5) { return get(UIColorHues::Orange, shades); }

    /** 色の濃さ (0~9) を指定して、 DeepOrange カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& deepOrange(int shades = 5) { return get(UIColorHues::DeepOrange, shades); }

    /** 色の濃さ (0~9) を指定して、 Brown カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& brown(int shades = 5) { return get(UIColorHues::Brown, shades); }

    /** 色の濃さ (0~9) を指定して、 Grey カラーパレットの色を取得します。 */
    LN_METHOD()
    static const Color& grey(int shades = 5) { return get(UIColorHues::Grey, shades); }

    static const Color& black(int shades = 5) { return get(UIColorHues::Black, shades); }

    static const Color& white(int shades = 5) { return get(UIColorHues::White, shades); }

    static const int MaxShades = 10;

    /** カラーインデックスと色の濃さ (0~9) を指定して、色を取得します。*/
    LN_METHOD()
    static const Color& get(UIColorHues hue, int shades = 5);
};

} // namespace ln

