
#include "../Internal.h"
#include <Lumino/Graphics/Color.h>

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// Color
//=============================================================================
const Color Color::Transparency = Color(0, 0, 0, 0);
const Color Color::TransparencyWhite = Color(255, 255, 255, 0);
const Color Color::Black = Color(0, 0, 0, 255);
const Color Color::White = Color(255, 255, 255, 255);
const Color Color::Gray = Color(127, 127, 127, 255);
const Color Color::Red = Color(255, 0, 0, 255);
const Color Color::Green = Color(0, 255, 0, 255);
const Color Color::Blue = Color(0, 0, 255, 255);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Color::Color(const ColorF& colorF)
{
	R = static_cast<uint8_t>(colorF.R * 255);
	G = static_cast<uint8_t>(colorF.G * 255);
	B = static_cast<uint8_t>(colorF.B * 255);
	A = static_cast<uint8_t>(colorF.A * 255);
}

//=============================================================================
// ColorF
//=============================================================================
const ColorF ColorF::Transparency = ColorF(0.0, 0.0, 0.0, 0.0);
const ColorF ColorF::TransparencyWhite = ColorF(1.0, 1.0, 1.0, 0.0);
const ColorF ColorF::Black = ColorF(0.0, 0.0, 0.0, 1.0);
const ColorF ColorF::White = ColorF(1.0, 1.0, 1.0, 1.0);
const ColorF ColorF::Gray = ColorF(0.5, 0.5, 0.5, 1.0);
const ColorF ColorF::Red = ColorF(1.0, 0.0, 0.0, 1.0);
const ColorF ColorF::Green = ColorF(0.0, 1.0, 0.0, 1.0);
const ColorF ColorF::Blue = ColorF(0.0, 0.0, 1.0, 1.0);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF::ColorF(const Color& color)
{
	R = static_cast<float>(color.R) / 255;
	G = static_cast<float>(color.G) / 255;
	B = static_cast<float>(color.B) / 255;
	A = static_cast<float>(color.A) / 255;
}

//=============================================================================
// HSVColor
//=============================================================================
Color HSVColor::ToColor() const
{
	unsigned char c1, c2, c3;
	if (S == 0)
	{
		return Color(V, V, V, A);
	}
	else
	{
		int t = (H * 6) % 360;
		c1 = (V*(255 - S)) / 255;
		c2 = (V*(255 - (S*t) / 360)) / 255;
		c3 = (V*(255 - (S*(360 - t)) / 360)) / 255;
		switch (H / 60)
		{
		case 0: return Color(V, c3, c1, A);
		case 1: return Color(c2, V, c1, A);
		case 2: return Color(c1, V, c3, A);
		case 3: return Color(c1, c2, V, A);
		case 4: return Color(c3, c1, V, A);
		case 5: return Color(V, c1, c2, A);
		}
	}
	return Color(0, 0, 0, A);
}


} // namespace Graphics
} // namespace Lumino
