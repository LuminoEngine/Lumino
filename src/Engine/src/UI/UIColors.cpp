
#include "Internal.hpp"
#include <LuminoEngine/UI/UIColors.hpp>

namespace ln {
	
//==============================================================================
// UIColors

static bool g_colorsInit = false;
static std::array<std::array<Color, UIColors::MaxShades>, 21> g_colors;

static const void InitColors()
{
    if (!g_colorsInit)
    {
#define LN_COLOR_DEF(name, depth, r, g, b) g_colors[(int)UIColorHues::name][depth] = Color(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f, 1.0f);
#define LN_COLOR_DEF_A(name, depth, r, g, b)
#include "UIColorsDefine.inl"

        for (int i = 0; i < UIColors::MaxShades; i++) g_colors[(int)UIColorHues::Black][i] = Color(0, 0, 0, 1);
        for (int i = 0; i < UIColors::MaxShades; i++) g_colors[(int)UIColorHues::White][i] = Color(1, 1, 1, 1);

        g_colorsInit = true;
    }
}

const Color& UIColors::get(UIColorHues index, int depth)
{
    if (LN_REQUIRE_RANGE(depth, 0, UIColors::MaxShades)) return Color::Black;
    InitColors();
    return g_colors[(int)index][depth];
}

} // namespace ln

