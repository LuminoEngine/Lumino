/*
	- UE4
		FColor		32bit
		FLinerColor	float
	- Unity
		Color		float
		Color32		32bit
	- cocos2d-x
		Color4B		32bit
		Color4F		float
	- WPF
		Color		32bit
	- XNA
		Color		32bit
	- Qt
		QColor		32bit
	- Ogre
		ColourValue	float
	- siv3d
		Color		32bit
		ColorF		float(double)
	- altseed
		Color		32bit
	- dxruby
		Color		32bit
	
	- Color32 → ColorF への変換はテーブルで高速化できる。
	  外部から受け取るときは 32、内部では F でいいかも。
	- GUI の ColorPicker とかは 0～255 で指定できたほうが操作しやすい。
*/
#include "../Internal.h"
#include <Lumino/Math/Vector3.h>
#include <Lumino/Math/Vector4.h>
#include <Lumino/Graphics/Color.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

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
	r = static_cast<uint8_t>(colorF.r * 255);
	g = static_cast<uint8_t>(colorF.g * 255);
	b = static_cast<uint8_t>(colorF.b * 255);
	a = static_cast<uint8_t>(colorF.a * 255);
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
const ColorF ColorF::DimGray = ColorF(0.25, 0.25, 0.25, 1.0);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF::ColorF(const Color& color)
{
	r = static_cast<float>(color.r) / 255;
	g = static_cast<float>(color.g) / 255;
	b = static_cast<float>(color.b) / 255;
	a = static_cast<float>(color.a) / 255;
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF::ColorF(const Vector3& vec, float a_)
{
	r = vec.x; g = vec.y; b = vec.z; a = a_;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF::ColorF(const Vector4& vec)
{
	r = vec.x; g = vec.y; b = vec.z; a = vec.w;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ColorF::AddClamp(const ColorF& color)
{
	r = Math::Clamp(r + color.r, 0.0f, 1.0f);
	g = Math::Clamp(g + color.g, 0.0f, 1.0f);
	b = Math::Clamp(b + color.b, 0.0f, 1.0f);
	a = Math::Clamp(a + color.a, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ColorF::MultiplyClamp(const ColorF& color)
{
	r = Math::Clamp(r * color.r, 0.0f, 1.0f);
	g = Math::Clamp(g * color.g, 0.0f, 1.0f);
	b = Math::Clamp(b * color.b, 0.0f, 1.0f);
	a = Math::Clamp(a * color.a, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
ColorF ColorF::Lerp(const ColorF& color1, const ColorF& color2, float t)
{
	return ColorF(
		Math::Lerp(color1.r, color2.r, t),
		Math::Lerp(color1.g, color2.g, t),
		Math::Lerp(color1.b, color2.b, t),
		Math::Lerp(color1.a, color2.a, t));
}

//=============================================================================
// ToneF
//=============================================================================

const ToneF ToneF::Zero(0, 0, 0, 0);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ToneF::AddClamp(const ToneF& tone)
{
	r = Math::Clamp(r + tone.r, 0.0f, 1.0f);
	g = Math::Clamp(g + tone.g, 0.0f, 1.0f);
	b = Math::Clamp(b + tone.b, 0.0f, 1.0f);
	GS = Math::Clamp(GS + tone.GS, 0.0f, 1.0f);
}


//=============================================================================
// HSVColor
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Color HSVColor::ToColor() const
{
	unsigned char c1, c2, c3;
	if (S == 0)
	{
		return Color(V, V, V, a);
	}
	else
	{
		int t = (H * 6) % 360;
		c1 = (V*(255 - S)) / 255;
		c2 = (V*(255 - (S*t) / 360)) / 255;
		c3 = (V*(255 - (S*(360 - t)) / 360)) / 255;
		switch (H / 60)
		{
		case 0: return Color(V, c3, c1, a);
		case 1: return Color(c2, V, c1, a);
		case 2: return Color(c1, V, c3, a);
		case 3: return Color(c1, c2, V, a);
		case 4: return Color(c3, c1, V, a);
		case 5: return Color(V, c1, c2, a);
		}
	}
	return Color(0, 0, 0, a);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF HSVColor::ToColorF() const
{
	Color c = ToColor();
	return ColorF(c);
}


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
