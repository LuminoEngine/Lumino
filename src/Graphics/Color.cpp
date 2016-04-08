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
	0,5.07705190066176E-06,2.33280046660989E-05,5.69217657121931E-05,0.000107187362341244,0.000175123977503027,0.000261543754548491,0.000367136269815943,0.000492503787191433,
	0.000638182842167022,0.000804658499513058,0.000992374304074325,0.0012017395224384,0.00143313458967186,0.00168691531678928,0.00196341621339647,0.00226295316070643,
	0.00258582559623417,0.00293231832393836,0.00330270303200364,0.00369723957890013,0.00411617709328275,0.00455975492252602,0.00502820345685554,0.00552174485023966,
	0.00604059365484981,0.00658495738258168,0.00715503700457303,0.00775102739766061,0.00837311774514858,0.00902149189801213,0.00969632870165823,0.0103978022925553,
	0.0111260823683832,0.0118813344348137,0.0126637200315821,0.0134733969401426,0.0143105193748841,0.0151752381596252,0.0160677008908869,0.01698805208925,0.0179364333399502,
	0.0189129834237215,0.0199178384387857,0.0209511319147811,0.0220129949193365,0.0231035561579214,0.0242229420675342,0.0253712769047346,0.0265486828284729,0.027755279978126,
	0.0289911865471078,0.0302565188523887,0.0315513914002264,0.0328759169483838,0.034230206565082,0.0356143696849188,0.0370285141619602,0.0384727463201946,0.0399471710015256,
	0.0414518916114625,0.0429870101626571,0.0445526273164214,0.0461488424223509,0.0477757535561706,0.049433457555908,0.0511220500564934,0.052841625522879,0.0545922772817603,
	0.0563740975519798,0.0581871774736854,0.0600316071363132,0.0619074756054558,0.0638148709486772,0.0657538802603301,0.0677245896854243,0.0697270844425988,0.0717614488462391,
	0.0738277663277846,0.0759261194562648,0.0780565899581019,0.080219258736215,0.0824142058884592,0.0846415107254295,0.0869012517876603,0.0891935068622478,0.0915183529989195,
	0.0938758665255778,0.0962661230633397,0.0986891975410945,0.1011451642096,0.103634096655137,0.106156067812744,0.108711149979039,0.11129941482466,0.113920933406333,
	0.116575776178572,0.119264013005047,0.121985713169619,0.124740945387051,0.127529777813422,0.130352278056244,0.1332085131843,0.136098549737202,0.139022453734703,
	0.141980290685736,0.144972125597231,0.147998022982685,0.151058046870511,0.154152260812165,0.157280727890073,0.160443510725344,0.16364067148529,0.166872271890766,
	0.170138373223312,0.173439036332135,0.176774321640903,0.18014428915439,0.183548998464951,0.186988508758844,0.190462878822409,0.193972167048093,0.19751643144034,
	0.201095729621346,0.204710118836677,0.208359655960767,0.212044397502288,0.215764399609395,0.219519718074868,0.223310408341127,0.227136525505149,0.230998124323267,
	0.23489525921588,0.238827984272048,0.242796353254002,0.24680041960155,0.2508402364364,0.254915856566385,0.259027332489606,0.263174716398492,0.267358060183772,
	0.271577415438375,0.275832833461245,0.280124365261085,0.284452061560024,0.288815972797219,0.293216149132375,0.297652640449211,0.302125496358853,0.306634766203158,
	0.311180499057984,0.315762743736397,0.32038154879181,0.325036962521076,0.329729032967515,0.334457807923889,0.339223334935327,0.344025661302187,0.348864834082879,
	0.353740900096629,0.358653905926199,0.363603897920553,0.368590922197487,0.373615024646202,0.37867625092984,0.383774646487975,0.388910256539059,0.394083126082829,
	0.399293299902674,0.404540822567962,0.409825738436323,0.415148091655907,0.420507926167587,0.425905285707146,0.43134021380741,0.436812753800359,0.442322948819202,
	0.44787084180041,0.453456475485731,0.45907989242416,0.46474113497389,0.470440245304218,0.47617726539744,0.481952237050698,0.487765201877811,0.493616201311074,
	0.49950527660303,0.505432468828216,0.511397818884879,0.517401367496673,0.523443155214325,0.529523222417277,0.535641609315311,0.541798355950137,0.547993502196972,
	0.554227087766085,0.560499152204328,0.566809734896638,0.573158875067523,0.579546611782525,0.585972983949661,0.592438030320847,0.598941789493296,0.605484299910907,
	0.612065599865624,0.61868572749878,0.625344720802427,0.632042617620641,0.638779455650817,0.645555272444934,0.652370105410821,0.659223991813387,0.666116968775851,
	0.673049073280942,0.680020342172095,0.687030812154625,0.694080519796882,0.701169501531402,0.708297793656032,0.715465432335048,0.722672453600255,0.729918893352071,
	0.737204787360605,0.744530171266715,0.751895080583051,0.759299550695091,0.766743616862161,0.774227314218442,0.781750677773962,0.789313742415586,0.796916542907978,
	0.804559113894567,0.81224148989849,0.819963705323528,0.827725794455034,0.835527791460841,0.843369730392169,0.851251645184515,0.859173569658532,0.867135537520905,
	0.875137582365205,0.883179737672745,0.891262036813419,0.899384513046529,0.907547199521614,0.915750129279253,0.923993335251873,0.932276850264543,0.940600707035753,
	0.948964938178195,0.957369576199527,0.96581465350313,0.974300202388861,0.982826255053791,0.99139284359294,1
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
