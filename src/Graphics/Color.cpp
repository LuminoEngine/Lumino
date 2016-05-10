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

	- F を使いたいのは頂点バッファを直接操作するときか、シェーダに値をセットするとき。

	あと、型については色系と座標系を一緒に考えないほうがいい。
	つまり、色系(Color, Tone) 座標系(Size, Rect)
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
//const const Color Color::Black = Color(0, 0, 0, 255);
//const const Color Color::White = Color(255, 255, 255, 255);
//const const Color Color::Gray = Color(127, 127, 127, 255);
//const const Color Color::Red = Color(255, 0, 0, 255);
//const const Color Color::Green = Color(0, 255, 0, 255);
//const const Color Color::Blue = Color(0, 0, 255, 255);
const Color Color::AliceBlue(240, 248, 255);
const Color Color::AntiqueWhite(250, 235, 215);
const Color Color::Aqua(0, 255, 255);
const Color Color::Aquamarine(127, 255, 212);
const Color Color::Azure(240, 255, 255);
const Color Color::Beige(245, 245, 220);
const Color Color::Bisque(255, 228, 196);
const Color Color::Black(0, 0, 0);
const Color Color::BlanchedAlmond(255, 235, 205);
const Color Color::Blue(0, 0, 255);
const Color Color::BlueViolet(138, 43, 226);
const Color Color::Brown(165, 42, 42);
const Color Color::BurlyWood(222, 184, 135);
const Color Color::CadetBlue(95, 158, 160);
const Color Color::Chartreuse(127, 255, 0);
const Color Color::Chocolate(210, 105, 30);
const Color Color::Coral(255, 127, 80);
const Color Color::CornflowerBlue(100, 149, 237);
const Color Color::Cornsilk(255, 248, 220);
const Color Color::Crimson(220, 20, 60);
const Color Color::Cyan(0, 255, 255);
const Color Color::DarkBlue(0, 0, 139);
const Color Color::DarkCyan(0, 139, 139);
const Color Color::DarkGoldenrod(184, 134, 11);
const Color Color::DarkGray(169, 169, 169);
const Color Color::DarkGreen(0, 100, 0);
const Color Color::DarkKhaki(189, 183, 107);
const Color Color::DarkMagenta(139, 0, 139);
const Color Color::DarkOliveGreen(85, 107, 47);
const Color Color::DarkOrange(255, 140, 0);
const Color Color::DarkOrchid(153, 50, 204);
const Color Color::DarkRed(139, 0, 0);
const Color Color::DarkSalmon(233, 150, 122);
const Color Color::DarkSeaGreen(143, 188, 143);
const Color Color::DarkSlateBlue(72, 61, 139);
const Color Color::DarkSlateGray(47, 79, 79);
const Color Color::DarkTurquoise(0, 206, 209);
const Color Color::DarkViolet(148, 0, 211);
const Color Color::DeepPink(255, 20, 147);
const Color Color::DeepSkyBlue(0, 191, 255);
const Color Color::DimGray(105, 105, 105);
const Color Color::DodgerBlue(30, 144, 255);
const Color Color::Firebrick(178, 34, 34);
const Color Color::FloralWhite(255, 250, 240);
const Color Color::ForestGreen(34, 139, 34);
const Color Color::Fuchsia(255, 0, 255);
const Color Color::Gainsboro(220, 220, 220);
const Color Color::GhostWhite(248, 248, 255);
const Color Color::Gold(255, 215, 0);
const Color Color::Goldenrod(218, 165, 32);
const Color Color::Gray(128, 128, 128);
const Color Color::Green(0, 128, 0);
const Color Color::GreenYellow(173, 255, 47);
const Color Color::Honeydew(240, 255, 240);
const Color Color::HotPink(255, 105, 180);
const Color Color::IndianRed(205, 92, 92);
const Color Color::Indigo(75, 0, 130);
const Color Color::Ivory(255, 255, 240);
const Color Color::Khaki(240, 230, 140);
const Color Color::Lavender(230, 230, 250);
const Color Color::LavenderBlush(255, 240, 245);
const Color Color::LawnGreen(124, 252, 0);
const Color Color::LemonChiffon(255, 250, 205);
const Color Color::LightBlue(173, 216, 230);
const Color Color::LightCoral(240, 128, 128);
const Color Color::LightCyan(224, 255, 255);
const Color Color::LightGoldenrodYellow(250, 250, 210);
const Color Color::LightGray(211, 211, 211);
const Color Color::LightGreen(144, 238, 144);
const Color Color::LightPink(255, 182, 193);
const Color Color::LightSalmon(255, 160, 122);
const Color Color::LightSeaGreen(32, 178, 170);
const Color Color::LightSkyBlue(135, 206, 250);
const Color Color::LightSlateGray(119, 136, 153);
const Color Color::LightSteelBlue(176, 196, 222);
const Color Color::LightYellow(255, 255, 224);
const Color Color::Lime(0, 255, 0);
const Color Color::LimeGreen(50, 205, 50);
const Color Color::Linen(250, 240, 230);
const Color Color::Magenta(255, 0, 255);
const Color Color::Maroon(128, 0, 0);
const Color Color::MediumAquamarine(102, 205, 170);
const Color Color::MediumBlue(0, 0, 205);
const Color Color::MediumOrchid(186, 85, 211);
const Color Color::MediumPurple(147, 112, 219);
const Color Color::MediumSeaGreen(60, 179, 113);
const Color Color::MediumSlateBlue(123, 104, 238);
const Color Color::MediumSpringGreen(0, 250, 154);
const Color Color::MediumTurquoise(72, 209, 204);
const Color Color::MediumVioletRed(199, 21, 133);
const Color Color::MidnightBlue(25, 25, 112);
const Color Color::MintCream(245, 255, 250);
const Color Color::MistyRose(255, 228, 225);
const Color Color::Moccasin(255, 228, 181);
const Color Color::NavajoWhite(255, 222, 173);
const Color Color::Navy(0, 0, 128);
const Color Color::OldLace(253, 245, 230);
const Color Color::Olive(128, 128, 0);
const Color Color::OliveDrab(107, 142, 35);
const Color Color::Orange(255, 165, 0);
const Color Color::OrangeRed(255, 69, 0);
const Color Color::Orchid(218, 112, 214);
const Color Color::PaleGoldenrod(238, 232, 170);
const Color Color::PaleGreen(152, 251, 152);
const Color Color::PaleTurquoise(175, 238, 238);
const Color Color::PaleVioletRed(219, 112, 147);
const Color Color::PapayaWhip(255, 239, 213);
const Color Color::PeachPuff(255, 218, 185);
const Color Color::Peru(205, 133, 63);
const Color Color::Pink(255, 192, 203);
const Color Color::Plum(221, 160, 221);
const Color Color::PowderBlue(176, 224, 230);
const Color Color::Purple(128, 0, 128);
const Color Color::Red(255, 0, 0);
const Color Color::RosyBrown(188, 143, 143);
const Color Color::RoyalBlue(65, 105, 225);
const Color Color::SaddleBrown(139, 69, 19);
const Color Color::Salmon(250, 128, 114);
const Color Color::SandyBrown(244, 164, 96);
const Color Color::SeaGreen(46, 139, 87);
const Color Color::SeaShell(255, 245, 238);
const Color Color::Sienna(160, 82, 45);
const Color Color::Silver(192, 192, 192);
const Color Color::SkyBlue(135, 206, 235);
const Color Color::SlateBlue(106, 90, 205);
const Color Color::SlateGray(112, 128, 144);
const Color Color::Snow(255, 250, 250);
const Color Color::SpringGreen(0, 255, 127);
const Color Color::SteelBlue(70, 130, 180);
const Color Color::Tan(210, 180, 140);
const Color Color::Teal(0, 128, 128);
const Color Color::Thistle(216, 191, 216);
const Color Color::Tomato(255, 99, 71);
const Color Color::Transparent(255, 255, 255);
const Color Color::Turquoise(64, 224, 208);
const Color Color::Violet(238, 130, 238);
const Color Color::Wheat(245, 222, 179);
const Color Color::White(255, 255, 255);
const Color Color::WhiteSmoke(245, 245, 245);
const Color Color::Yellow(255, 255, 0);
const Color Color::YellowGreen(154, 205, 50);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Color::Color(const ColorF& colorF)
//{
//	r = static_cast<uint8_t>(colorF.r * 255);
//	g = static_cast<uint8_t>(colorF.g * 255);
//	b = static_cast<uint8_t>(colorF.b * 255);
//	a = static_cast<uint8_t>(colorF.a * 255);
//}

static bool TryParseHexChar(int c, int* outValue)
{
	const int zeroChar = (int)'0';
	const int aLower = (int)'a';
	const int aUpper = (int)'A';

	int intChar = (int)c;

	if ((intChar >= zeroChar) && (intChar <= (zeroChar + 9)))
	{
		*outValue = (intChar - zeroChar);
		return true;
	}

	if ((intChar >= aLower) && (intChar <= (aLower + 5)))
	{
		*outValue = (intChar - aLower + 10);
		return true;
	}

	if ((intChar >= aUpper) && (intChar <= (aUpper + 5)))
	{
		*outValue = (intChar - aUpper + 10);
		return true;
	}
	return false;
}
static int ParseHexChar(int c)
{
	int value;
	if (TryParseHexChar(c, &value))
	{
		return value;
	}
	LN_THROW(0, InvalidFormatException);
}

static bool ParseHexColor(const StringRef& str, Color* outColor)
{
	uint8_t r, g, b, a;
	if (str.GetLength() >= 3 && str[0] == _T('#'))
	{
		// #FFFFFFFF
		if (str.GetLength() >= 9)
		{
			a = ParseHexChar(str[1]) * 16 + ParseHexChar(str[2]);
			r = ParseHexChar(str[3]) * 16 + ParseHexChar(str[4]);
			g = ParseHexChar(str[5]) * 16 + ParseHexChar(str[6]);
			b = ParseHexChar(str[7]) * 16 + ParseHexChar(str[8]);
		}
		// #FFFFFF
		else if (str.GetLength() >= 7)
		{
			a = 255;
			r = ParseHexChar(str[1]) * 16 + ParseHexChar(str[2]);
			g = ParseHexChar(str[3]) * 16 + ParseHexChar(str[4]);
			b = ParseHexChar(str[5]) * 16 + ParseHexChar(str[6]);
		}
		// #FFFF
		else if (str.GetLength() >= 5)
		{
			a = ParseHexChar(str[1]);
			a = a + a * 16;
			r = ParseHexChar(str[2]);
			r = r + r * 16;
			g = ParseHexChar(str[3]);
			g = g + g * 16;
			b = ParseHexChar(str[4]);
			b = b + b * 16;
		}
		else if (str.GetLength() >= 4)
		{
			r = ParseHexChar(str[1]);
			r = r + r * 16;
			g = ParseHexChar(str[2]);
			g = g + g * 16;
			b = ParseHexChar(str[3]);
			b = b + b * 16;
		}
		else
		{
			return false;
		}

		outColor->Set(r, g, b, a);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Color Color::FromString(const StringRef& str)
{
	Color c;
	if (!ParseHexColor(str, &c))
	{
		LN_THROW(0, InvalidFormatException);
	}
	return c;
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

static const float g_color255Table[256] =
{
	0, 0.00392157, 0.00784314, 0.0117647, 0.0156863, 0.0196078, 0.0235294, 0.027451, 0.0313725, 0.0352941, 0.0392157, 0.0431373, 0.0470588, 0.0509804, 0.054902, 0.0588235,
	0.0627451, 0.0666667, 0.0705882, 0.0745098, 0.0784314, 0.0823529, 0.0862745, 0.0901961, 0.0941176, 0.0980392, 0.101961, 0.105882, 0.109804, 0.113725, 0.117647, 0.121569,
	0.12549, 0.129412, 0.133333, 0.137255, 0.141176, 0.145098, 0.14902, 0.152941, 0.156863, 0.160784, 0.164706, 0.168627, 0.172549, 0.176471, 0.180392, 0.184314,
	0.188235, 0.192157, 0.196078, 0.2, 0.203922, 0.207843, 0.211765, 0.215686, 0.219608, 0.223529, 0.227451, 0.231373, 0.235294, 0.239216, 0.243137, 0.247059,
	0.25098, 0.254902, 0.258824, 0.262745, 0.266667, 0.270588, 0.27451, 0.278431, 0.282353, 0.286275, 0.290196, 0.294118, 0.298039, 0.301961, 0.305882, 0.309804,
	0.313725, 0.317647, 0.321569, 0.32549, 0.329412, 0.333333, 0.337255, 0.341176, 0.345098, 0.34902, 0.352941, 0.356863, 0.360784, 0.364706, 0.368627, 0.372549,
	0.376471, 0.380392, 0.384314, 0.388235, 0.392157, 0.396078, 0.4, 0.403922, 0.407843, 0.411765, 0.415686, 0.419608, 0.423529, 0.427451, 0.431373, 0.435294,
	0.439216, 0.443137, 0.447059, 0.45098, 0.454902, 0.458824, 0.462745, 0.466667, 0.470588, 0.47451, 0.478431, 0.482353, 0.486275, 0.490196, 0.494118, 0.498039,
	0.501961, 0.505882, 0.509804, 0.513725, 0.517647, 0.521569, 0.52549, 0.529412, 0.533333, 0.537255, 0.541176, 0.545098, 0.54902, 0.552941, 0.556863, 0.560784,
	0.564706, 0.568627, 0.572549, 0.576471, 0.580392, 0.584314, 0.588235, 0.592157, 0.596078, 0.6, 0.603922, 0.607843, 0.611765, 0.615686, 0.619608, 0.623529,
	0.627451, 0.631373, 0.635294, 0.639216, 0.643137, 0.647059, 0.65098, 0.654902, 0.658824, 0.662745, 0.666667, 0.670588, 0.67451, 0.678431, 0.682353, 0.686275,
	0.690196, 0.694118, 0.698039, 0.701961, 0.705882, 0.709804, 0.713725, 0.717647, 0.721569, 0.72549, 0.729412, 0.733333, 0.737255, 0.741176, 0.745098, 0.74902,
	0.752941, 0.756863, 0.760784, 0.764706, 0.768627, 0.772549, 0.776471, 0.780392, 0.784314, 0.788235, 0.792157, 0.796078, 0.8, 0.803922, 0.807843, 0.811765,
	0.815686, 0.819608, 0.823529, 0.827451, 0.831373, 0.835294, 0.839216, 0.843137, 0.847059, 0.85098, 0.854902, 0.858824, 0.862745, 0.866667, 0.870588, 0.87451,
	0.878431, 0.882353, 0.886275, 0.890196, 0.894118, 0.898039, 0.901961, 0.905882, 0.909804, 0.913725, 0.917647, 0.921569, 0.92549, 0.929412, 0.933333, 0.937255,
	0.941176, 0.945098, 0.94902, 0.952941, 0.956863, 0.960784, 0.964706, 0.968627, 0.972549, 0.976471, 0.980392, 0.984314, 0.988235, 0.992157, 0.996078, 1,
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColorF::ColorF(const Color& color)
{
	r = g_color255Table[color.r];
	g = g_color255Table[color.g];
	b = g_color255Table[color.b];
	a = g_color255Table[color.a];
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
	gray = Math::Clamp(gray + tone.gray, 0.0f, 1.0f);
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
