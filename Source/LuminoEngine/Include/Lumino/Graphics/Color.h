
#pragma once

LN_NAMESPACE_BEGIN
class Vector3;
class Vector4;

LN_NAMESPACE_GRAPHICS_BEGIN
class Color;
	
/**
	@brief	各要素を 0～255 の範囲で表す RGBA カラーを定義します。
*/
class Color32
{
public:
	static const Color32 Transparency;			///< 透明	(0, 0, 0, 0)
	static const Color32 TransparencyWhite;		///< 白透明	(255, 255, 255, 0)
	//static const Color32 Black;					///< 黒		(0, 0, 0, 255)
	//static const Color32 White;					///< 白		(255, 255, 255, 255)
	//static const Color32 Gray;					///< 灰色	(127, 127, 127, 255)
	//static const Color32 Red;						///< 赤		(255, 0, 0, 255)
	//static const Color32 Green;					///< 緑		(0, 255, 0, 255)
	//static const Color32 Blue;					///< 青		(0, 0.0, 255, 255)

	// see https://msdn.microsoft.com/ja-jp/library/system.windows.media.colors(v=vs.110).aspx
	static const Color32 AliceBlue;             /** (240, 248, 255) */
	static const Color32 AntiqueWhite;          /** (250, 235, 215) */
	static const Color32 Aqua;                  /** (0, 255, 255) */
	static const Color32 Aquamarine;            /** (127, 255, 212) */
	static const Color32 Azure;                 /** (240, 255, 255) */
	static const Color32 Beige;                 /** (245, 245, 220) */
	static const Color32 Bisque;                /** (255, 228, 196) */
	static const Color32 Black;                 /** (0, 0, 0) */
	static const Color32 BlanchedAlmond;        /** (255, 235, 205) */
	static const Color32 Blue;                  /** (0, 0, 255) */
	static const Color32 BlueViolet;            /** (138, 43, 226) */
	static const Color32 Brown;                 /** (165, 42, 42) */
	static const Color32 BurlyWood;             /** (222, 184, 135) */
	static const Color32 CadetBlue;             /** (95, 158, 160) */
	static const Color32 Chartreuse;            /** (127, 255, 0) */
	static const Color32 Chocolate;             /** (210, 105, 30) */
	static const Color32 Coral;                 /** (255, 127, 80) */
	static const Color32 CornflowerBlue;        /** (100, 149, 237) */
	static const Color32 Cornsilk;              /** (255, 248, 220) */
	static const Color32 Crimson;               /** (220, 20, 60) */
	static const Color32 Cyan;                  /** (0, 255, 255) */
	static const Color32 DarkBlue;              /** (0, 0, 139) */
	static const Color32 DarkCyan;              /** (0, 139, 139) */
	static const Color32 DarkGoldenrod;         /** (184, 134, 11) */
	static const Color32 DarkGray;              /** (169, 169, 169) */
	static const Color32 DarkGreen;             /** (0, 100, 0) */
	static const Color32 DarkKhaki;             /** (189, 183, 107) */
	static const Color32 DarkMagenta;           /** (139, 0, 139) */
	static const Color32 DarkOliveGreen;        /** (85, 107, 47) */
	static const Color32 DarkOrange;            /** (255, 140, 0) */
	static const Color32 DarkOrchid;            /** (153, 50, 204) */
	static const Color32 DarkRed;               /** (139, 0, 0) */
	static const Color32 DarkSalmon;            /** (233, 150, 122) */
	static const Color32 DarkSeaGreen;          /** (143, 188, 143) */
	static const Color32 DarkSlateBlue;         /** (72, 61, 139) */
	static const Color32 DarkSlateGray;         /** (47, 79, 79) */
	static const Color32 DarkTurquoise;         /** (0, 206, 209) */
	static const Color32 DarkViolet;            /** (148, 0, 211) */
	static const Color32 DeepPink;              /** (255, 20, 147) */
	static const Color32 DeepSkyBlue;           /** (0, 191, 255) */
	static const Color32 DimGray;               /** (105, 105, 105) */
	static const Color32 DodgerBlue;            /** (30, 144, 255) */
	static const Color32 Firebrick;             /** (178, 34, 34) */
	static const Color32 FloralWhite;           /** (255, 250, 240) */
	static const Color32 ForestGreen;           /** (34, 139, 34) */
	static const Color32 Fuchsia;               /** (255, 0, 255) */
	static const Color32 Gainsboro;             /** (220, 220, 220) */
	static const Color32 GhostWhite;            /** (248, 248, 255) */
	static const Color32 Gold;                  /** (255, 215, 0) */
	static const Color32 Goldenrod;             /** (218, 165, 32) */
	static const Color32 Gray;                  /** (128, 128, 128) */
	static const Color32 Green;                 /** (0, 128, 0) */
	static const Color32 GreenYellow;           /** (173, 255, 47) */
	static const Color32 Honeydew;              /** (240, 255, 240) */
	static const Color32 HotPink;               /** (255, 105, 180) */
	static const Color32 IndianRed;             /** (205, 92, 92) */
	static const Color32 Indigo;                /** (75, 0, 130) */
	static const Color32 Ivory;                 /** (255, 255, 240) */
	static const Color32 Khaki;                 /** (240, 230, 140) */
	static const Color32 Lavender;              /** (230, 230, 250) */
	static const Color32 LavenderBlush;         /** (255, 240, 245) */
	static const Color32 LawnGreen;             /** (124, 252, 0) */
	static const Color32 LemonChiffon;          /** (255, 250, 205) */
	static const Color32 LightBlue;             /** (173, 216, 230) */
	static const Color32 LightCoral;            /** (240, 128, 128) */
	static const Color32 LightCyan;             /** (224, 255, 255) */
	static const Color32 LightGoldenrodYellow;  /** (250, 250, 210) */
	static const Color32 LightGray;             /** (211, 211, 211) */
	static const Color32 LightGreen;            /** (144, 238, 144) */
	static const Color32 LightPink;             /** (255, 182, 193) */
	static const Color32 LightSalmon;           /** (255, 160, 122) */
	static const Color32 LightSeaGreen;         /** (32, 178, 170) */
	static const Color32 LightSkyBlue;          /** (135, 206, 250) */
	static const Color32 LightSlateGray;        /** (119, 136, 153) */
	static const Color32 LightSteelBlue;        /** (176, 196, 222) */
	static const Color32 LightYellow;           /** (255, 255, 224) */
	static const Color32 Lime;                  /** (0, 255, 0) */
	static const Color32 LimeGreen;             /** (50, 205, 50) */
	static const Color32 Linen;                 /** (250, 240, 230) */
	static const Color32 Magenta;               /** (255, 0, 255) */
	static const Color32 Maroon;                /** (128, 0, 0) */
	static const Color32 MediumAquamarine;      /** (102, 205, 170) */
	static const Color32 MediumBlue;            /** (0, 0, 205) */
	static const Color32 MediumOrchid;          /** (186, 85, 211) */
	static const Color32 MediumPurple;          /** (147, 112, 219) */
	static const Color32 MediumSeaGreen;        /** (60, 179, 113) */
	static const Color32 MediumSlateBlue;       /** (123, 104, 238) */
	static const Color32 MediumSpringGreen;     /** (0, 250, 154) */
	static const Color32 MediumTurquoise;       /** (72, 209, 204) */
	static const Color32 MediumVioletRed;       /** (199, 21, 133) */
	static const Color32 MidnightBlue;          /** (25, 25, 112) */
	static const Color32 MintCream;             /** (245, 255, 250) */
	static const Color32 MistyRose;             /** (255, 228, 225) */
	static const Color32 Moccasin;              /** (255, 228, 181) */
	static const Color32 NavajoWhite;           /** (255, 222, 173) */
	static const Color32 Navy;                  /** (0, 0, 128) */
	static const Color32 OldLace;               /** (253, 245, 230) */
	static const Color32 Olive;                 /** (128, 128, 0) */
	static const Color32 OliveDrab;             /** (107, 142, 35) */
	static const Color32 Orange;                /** (255, 165, 0) */
	static const Color32 OrangeRed;             /** (255, 69, 0) */
	static const Color32 Orchid;                /** (218, 112, 214) */
	static const Color32 PaleGoldenrod;         /** (238, 232, 170) */
	static const Color32 PaleGreen;             /** (152, 251, 152) */
	static const Color32 PaleTurquoise;         /** (175, 238, 238) */
	static const Color32 PaleVioletRed;         /** (219, 112, 147) */
	static const Color32 PapayaWhip;            /** (255, 239, 213) */
	static const Color32 PeachPuff;             /** (255, 218, 185) */
	static const Color32 Peru;                  /** (205, 133, 63) */
	static const Color32 Pink;                  /** (255, 192, 203) */
	static const Color32 Plum;                  /** (221, 160, 221) */
	static const Color32 PowderBlue;            /** (176, 224, 230) */
	static const Color32 Purple;                /** (128, 0, 128) */
	static const Color32 Red;                   /** (255, 0, 0) */
	static const Color32 RosyBrown;             /** (188, 143, 143) */
	static const Color32 RoyalBlue;             /** (65, 105, 225) */
	static const Color32 SaddleBrown;           /** (139, 69, 19) */
	static const Color32 Salmon;                /** (250, 128, 114) */
	static const Color32 SandyBrown;            /** (244, 164, 96) */
	static const Color32 SeaGreen;              /** (46, 139, 87) */
	static const Color32 SeaShell;              /** (255, 245, 238) */
	static const Color32 Sienna;                /** (160, 82, 45) */
	static const Color32 Silver;                /** (192, 192, 192) */
	static const Color32 SkyBlue;               /** (135, 206, 235) */
	static const Color32 SlateBlue;             /** (106, 90, 205) */
	static const Color32 SlateGray;             /** (112, 128, 144) */
	static const Color32 Snow;                  /** (255, 250, 250) */
	static const Color32 SpringGreen;           /** (0, 255, 127) */
	static const Color32 SteelBlue;             /** (70, 130, 180) */
	static const Color32 Tan;                   /** (210, 180, 140) */
	static const Color32 Teal;                  /** (0, 128, 128) */
	static const Color32 Thistle;               /** (216, 191, 216) */
	static const Color32 Tomato;                /** (255, 99, 71) */
	static const Color32 Transparent;           /** (255, 255, 255) */
	static const Color32 Turquoise;             /** (64, 224, 208) */
	static const Color32 Violet;                /** (238, 130, 238) */
	static const Color32 Wheat;                 /** (245, 222, 179) */
	static const Color32 White;                 /** (255, 255, 255) */
	static const Color32 WhiteSmoke;            /** (245, 245, 245) */
	static const Color32 Yellow;                /** (255, 255, 0) */
	static const Color32 YellowGreen;           /** (154, 205, 50) */


public:
	uint8_t		r;		///< 赤成分 (0～255)
	uint8_t		g;		///< 緑成分 (0～255)
	uint8_t		b;		///< 青成分 (0～255)
	uint8_t		a;		///< アルファ成分 (0～255)

public:

	/** すべての要素を 0 で初期化します。*/
	Color32() : r(0), g(0), b(0), a(0) {}

	/** 各要素を指定して初期化します。*/
	Color32(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }
	
	/** ColorF を変換して設定します。*/
	//Color32(const ColorF& colorF);

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }

	static Color32 FromString(const StringRef& str);

public:
	bool operator == (const Color32& color) const { return (memcmp(this, &color, sizeof(Color32)) == 0); }
	bool operator != (const Color32& obj) const { return !operator==(obj); }
};


/**
	@brief	各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
*/
class Color
{
public:
	float	r;		///< 赤成分 (0.0～1.0)
	float	g;		///< 緑成分 (0.0～1.0)
	float	b;		///< 青成分 (0.0～1.0)
	float	a;		///< アルファ成分 (0.0～1.0)

public:
	static const Color Transparency;			///< 透明 (0.0, 0.0, 0.0, 0.0)
	static const Color TransparencyWhite;		///< 白透明 (1.0, 1.0, 1.0, 0.0)

	// see https://msdn.microsoft.com/ja-jp/library/system.windows.media.colors(v=vs.110).aspx
	static const Color AliceBlue;             /** #F0F8FF (0.941176, 0.972549, 1) */
	static const Color AntiqueWhite;          /** #FAEBD7 (0.980392, 0.921569, 0.843137) */
	static const Color Aqua;                  /** #00FFFF (0, 1, 1) */
	static const Color Aquamarine;            /** #7FFFD4 (0.498039, 1, 0.831373) */
	static const Color Azure;                 /** #F0FFFF (0.941176, 1, 1) */
	static const Color Beige;                 /** #F5F5DC (0.960784, 0.960784, 0.862745) */
	static const Color Bisque;                /** #FFE4C4 (1, 0.894118, 0.768627) */
	static const Color Black;                 /** #000000 (0, 0, 0) */
	static const Color BlanchedAlmond;        /** #FFEBCD (1, 0.921569, 0.803922) */
	static const Color Blue;                  /** #0000FF (0, 0, 1) */
	static const Color BlueViolet;            /** #8A2BE2 (0.541176, 0.168627, 0.886275) */
	static const Color Brown;                 /** #A52A2A (0.647059, 0.164706, 0.164706) */
	static const Color BurlyWood;             /** #DEB887 (0.870588, 0.721569, 0.529412) */
	static const Color CadetBlue;             /** #5F9EA0 (0.372549, 0.619608, 0.627451) */
	static const Color Chartreuse;            /** #7FFF00 (0.498039, 1, 0) */
	static const Color Chocolate;             /** #D2691E (0.823529, 0.411765, 0.117647) */
	static const Color Coral;                 /** #FF7F50 (1, 0.498039, 0.313725) */
	static const Color CornflowerBlue;        /** #6495ED (0.392157, 0.584314, 0.929412) */
	static const Color Cornsilk;              /** #FFF8DC (1, 0.972549, 0.862745) */
	static const Color Crimson;               /** #DC143C (0.862745, 0.0784314, 0.235294) */
	static const Color Cyan;                  /** #00FFFF (0, 1, 1) */
	static const Color DarkBlue;              /** #00008B (0, 0, 0.545098) */
	static const Color DarkCyan;              /** #008B8B (0, 0.545098, 0.545098) */
	static const Color DarkGoldenrod;         /** #B8860B (0.721569, 0.52549, 0.0431373) */
	static const Color DarkGray;              /** #A9A9A9 (0.662745, 0.662745, 0.662745) */
	static const Color DarkGreen;             /** #006400 (0, 0.392157, 0) */
	static const Color DarkKhaki;             /** #BDB76B (0.741176, 0.717647, 0.419608) */
	static const Color DarkMagenta;           /** #8B008B (0.545098, 0, 0.545098) */
	static const Color DarkOliveGreen;        /** #556B2F (0.333333, 0.419608, 0.184314) */
	static const Color DarkOrange;            /** #FF8C00 (1, 0.54902, 0) */
	static const Color DarkOrchid;            /** #9932CC (0.6, 0.196078, 0.8) */
	static const Color DarkRed;               /** #8B0000 (0.545098, 0, 0) */
	static const Color DarkSalmon;            /** #E9967A (0.913725, 0.588235, 0.478431) */
	static const Color DarkSeaGreen;          /** #8FBC8F (0.560784, 0.737255, 0.560784) */
	static const Color DarkSlateBlue;         /** #483D8B (0.282353, 0.239216, 0.545098) */
	static const Color DarkSlateGray;         /** #2F4F4F (0.184314, 0.309804, 0.309804) */
	static const Color DarkTurquoise;         /** #00CED1 (0, 0.807843, 0.819608) */
	static const Color DarkViolet;            /** #9400D3 (0.580392, 0, 0.827451) */
	static const Color DeepPink;              /** #FF1493 (1, 0.0784314, 0.576471) */
	static const Color DeepSkyBlue;           /** #00BFFF (0, 0.74902, 1) */
	static const Color DimGray;               /** #696969 (0.411765, 0.411765, 0.411765) */
	static const Color DodgerBlue;            /** #1E90FF (0.117647, 0.564706, 1) */
	static const Color Firebrick;             /** #B22222 (0.698039, 0.133333, 0.133333) */
	static const Color FloralWhite;           /** #FFFAF0 (1, 0.980392, 0.941176) */
	static const Color ForestGreen;           /** #228B22 (0.133333, 0.545098, 0.133333) */
	static const Color Fuchsia;               /** #FF00FF (1, 0, 1) */
	static const Color Gainsboro;             /** #DCDCDC (0.862745, 0.862745, 0.862745) */
	static const Color GhostWhite;            /** #F8F8FF (0.972549, 0.972549, 1) */
	static const Color Gold;                  /** #FFD700 (1, 0.843137, 0) */
	static const Color Goldenrod;             /** #DAA520 (0.854902, 0.647059, 0.12549) */
	static const Color Gray;                  /** #808080 (0.5, 0.5, 0.5) */
	static const Color Green;                 /** #008000 (0, 0.5, 0) */
	static const Color GreenYellow;           /** #ADFF2F (0.678431, 1, 0.184314) */
	static const Color Honeydew;              /** #F0FFF0 (0.941176, 1, 0.941176) */
	static const Color HotPink;               /** #FF69B4 (1, 0.411765, 0.705882) */
	static const Color IndianRed;             /** #CD5C5C (0.803922, 0.360784, 0.360784) */
	static const Color Indigo;                /** #4B0082 (0.294118, 0, 0.509804) */
	static const Color Ivory;                 /** #FFFFF0 (1, 1, 0.941176) */
	static const Color Khaki;                 /** #F0E68C (0.941176, 0.901961, 0.54902) */
	static const Color Lavender;              /** #E6E6FA (0.901961, 0.901961, 0.980392) */
	static const Color LavenderBlush;         /** #FFF0F5 (1, 0.941176, 0.960784) */
	static const Color LawnGreen;             /** #7CFC00 (0.486275, 0.988235, 0) */
	static const Color LemonChiffon;          /** #FFFACD (1, 0.980392, 0.803922) */
	static const Color LightBlue;             /** #ADD8E6 (0.678431, 0.847059, 0.901961) */
	static const Color LightCoral;            /** #F08080 (0.941176, 0.5, 0.5) */
	static const Color LightCyan;             /** #E0FFFF (0.878431, 1, 1) */
	static const Color LightGoldenrodYellow;  /** #FAFAD2 (0.980392, 0.980392, 0.823529) */
	static const Color LightGray;             /** #D3D3D3 (0.827451, 0.827451, 0.827451) */
	static const Color LightGreen;            /** #90EE90 (0.564706, 0.933333, 0.564706) */
	static const Color LightPink;             /** #FFB6C1 (1, 0.713725, 0.756863) */
	static const Color LightSalmon;           /** #FFA07A (1, 0.627451, 0.478431) */
	static const Color LightSeaGreen;         /** #20B2AA (0.12549, 0.698039, 0.666667) */
	static const Color LightSkyBlue;          /** #87CEFA (0.529412, 0.807843, 0.980392) */
	static const Color LightSlateGray;        /** #778899 (0.466667, 0.533333, 0.6) */
	static const Color LightSteelBlue;        /** #B0C4DE (0.690196, 0.768627, 0.870588) */
	static const Color LightYellow;           /** #FFFFE0 (1, 1, 0.878431) */
	static const Color Lime;                  /** #00FF00 (0, 1, 0) */
	static const Color LimeGreen;             /** #32CD32 (0.196078, 0.803922, 0.196078) */
	static const Color Linen;                 /** #FAF0E6 (0.980392, 0.941176, 0.901961) */
	static const Color Magenta;               /** #FF00FF (1, 0, 1) */
	static const Color Maroon;                /** #800000 (0.5, 0, 0) */
	static const Color MediumAquamarine;      /** #66CDAA (0.4, 0.803922, 0.666667) */
	static const Color MediumBlue;            /** #0000CD (0, 0, 0.803922) */
	static const Color MediumOrchid;          /** #BA55D3 (0.729412, 0.333333, 0.827451) */
	static const Color MediumPurple;          /** #9370DB (0.576471, 0.439216, 0.858824) */
	static const Color MediumSeaGreen;        /** #3CB371 (0.235294, 0.701961, 0.443137) */
	static const Color MediumSlateBlue;       /** #7B68EE (0.482353, 0.407843, 0.933333) */
	static const Color MediumSpringGreen;     /** #00FA9A (0, 0.980392, 0.603922) */
	static const Color MediumTurquoise;       /** #48D1CC (0.282353, 0.819608, 0.8) */
	static const Color MediumVioletRed;       /** #C71585 (0.780392, 0.0823529, 0.521569) */
	static const Color MidnightBlue;          /** #191970 (0.0980392, 0.0980392, 0.439216) */
	static const Color MintCream;             /** #F5FFFA (0.960784, 1, 0.980392) */
	static const Color MistyRose;             /** #FFE4E1 (1, 0.894118, 0.882353) */
	static const Color Moccasin;              /** #FFE4B5 (1, 0.894118, 0.709804) */
	static const Color NavajoWhite;           /** #FFDEAD (1, 0.870588, 0.678431) */
	static const Color Navy;                  /** #000080 (0, 0, 0.5) */
	static const Color OldLace;               /** #FDF5E6 (0.992157, 0.960784, 0.901961) */
	static const Color Olive;                 /** #808000 (0.5, 0.5, 0) */
	static const Color OliveDrab;             /** #6B8E23 (0.419608, 0.556863, 0.137255) */
	static const Color Orange;                /** #FFA500 (1, 0.647059, 0) */
	static const Color OrangeRed;             /** #FF4500 (1, 0.270588, 0) */
	static const Color Orchid;                /** #DA70D6 (0.854902, 0.439216, 0.839216) */
	static const Color PaleGoldenrod;         /** #EEE8AA (0.933333, 0.909804, 0.666667) */
	static const Color PaleGreen;             /** #98FB98 (0.596078, 0.984314, 0.596078) */
	static const Color PaleTurquoise;         /** #AFEEEE (0.686275, 0.933333, 0.933333) */
	static const Color PaleVioletRed;         /** #DB7093 (0.858824, 0.439216, 0.576471) */
	static const Color PapayaWhip;            /** #FFEFD5 (1, 0.937255, 0.835294) */
	static const Color PeachPuff;             /** #FFDAB9 (1, 0.854902, 0.72549) */
	static const Color Peru;                  /** #CD853F (0.803922, 0.521569, 0.247059) */
	static const Color Pink;                  /** #FFC0CB (1, 0.752941, 0.796078) */
	static const Color Plum;                  /** #DDA0DD (0.866667, 0.627451, 0.866667) */
	static const Color PowderBlue;            /** #B0E0E6 (0.690196, 0.878431, 0.901961) */
	static const Color Purple;                /** #800080 (0.5, 0, 0.5) */
	static const Color Red;                   /** #FF0000 (1, 0, 0) */
	static const Color RosyBrown;             /** #BC8F8F (0.737255, 0.560784, 0.560784) */
	static const Color RoyalBlue;             /** #4169E1 (0.254902, 0.411765, 0.882353) */
	static const Color SaddleBrown;           /** #8B4513 (0.545098, 0.270588, 0.0745098) */
	static const Color Salmon;                /** #FA8072 (0.980392, 0.5, 0.447059) */
	static const Color SandyBrown;            /** #F4A460 (0.956863, 0.643137, 0.376471) */
	static const Color SeaGreen;              /** #2E8B57 (0.180392, 0.545098, 0.341176) */
	static const Color SeaShell;              /** #FFF5EE (1, 0.960784, 0.933333) */
	static const Color Sienna;                /** #A0522D (0.627451, 0.321569, 0.176471) */
	static const Color Silver;                /** #C0C0C0 (0.752941, 0.752941, 0.752941) */
	static const Color SkyBlue;               /** #87CEEB (0.529412, 0.807843, 0.921569) */
	static const Color SlateBlue;             /** #6A5ACD (0.415686, 0.352941, 0.803922) */
	static const Color SlateGray;             /** #708090 (0.439216, 0.5, 0.564706) */
	static const Color Snow;                  /** #FFFAFA (1, 0.980392, 0.980392) */
	static const Color SpringGreen;           /** #00FF7F (0, 1, 0.498039) */
	static const Color SteelBlue;             /** #4682B4 (0.27451, 0.509804, 0.705882) */
	static const Color Tan;                   /** #D2B48C (0.823529, 0.705882, 0.54902) */
	static const Color Teal;                  /** #008080 (0, 0.5, 0.5) */
	static const Color Thistle;               /** #D8BFD8 (0.847059, 0.74902, 0.847059) */
	static const Color Tomato;                /** #FF6347 (1, 0.388235, 0.278431) */
	static const Color Transparent;           /** #00FFFFFF (1, 1, 1) */
	static const Color Turquoise;             /** #40E0D0 (0.25098, 0.878431, 0.815686) */
	static const Color Violet;                /** #EE82EE (0.933333, 0.509804, 0.933333) */
	static const Color Wheat;                 /** #F5DEB3 (0.960784, 0.870588, 0.701961) */
	static const Color White;                 /** #FFFFFF (1, 1, 1) */
	static const Color WhiteSmoke;            /** #F5F5F5 (0.960784, 0.960784, 0.960784) */
	static const Color Yellow;                /** #FFFF00 (1, 1, 0) */
	static const Color YellowGreen;           /** #9ACD32 (0.603922, 0.803922, 0.196078) */



public:

	/**
		@brief	すべての要素を 0.0 で初期化します。
	*/
	Color() { r = 0.0; g = 0.0; b = 0.0; a = 0.0; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	Color(float r_, float g_, float b_, float a_ = 1.0f) { r = r_; g = g_; b = b_; a = a_; }

	/**
		@brief	指定した ColorF をコピーして初期化します。
	*/
	Color(const Color& color) { r = color.r; g = color.g; b = color.b; a = color.a; }

	/**
		@brief	Color32 を変換して設定します。
	*/
	Color(const Color32& color);

	/**
		@brief	指定した Vector3 とアルファ値をコピーして初期化します。
	*/
	Color(const Vector3& vec, float a);

	/**
		@brief	指定した Vector4 をコピーして初期化します。
	*/
	Color(const Vector4& vec);

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float r_, float g_, float b_, float a_ = 1.0f) { r = r_; g = g_; b = b_; a = a_; }

	/**
		@brief	この色に指定した色を加算します。0.0～1.0 を超える場合はクランプします。
	*/
	void AddClamp(const Color& color);

	/**
		@brief	この色に指定した色を乗算します。0.0～1.0 を超える場合はクランプします。
	*/
	void MultiplyClamp(const Color& color);

	static Color Lerp(const Color& color1, const Color& color2, float t);

	//----------------------------------------------------------------------
	///**
	//  @brief      32 ビットの色コードから色を作成
	//
	//  @note
	//              ARGB の順で各 8 ビット
	//*/
	//----------------------------------------------------------------------
	//void setFrom32Bit(lnU32 code_, LNGraphicsAPI api_);

	//----------------------------------------------------------------------
	///**
	//  @brief      32 ビットの色コードに変換
	//*/
	//----------------------------------------------------------------------
	//lnU32 to32Bit(LNGraphicsAPI api_) const;	// 削除よてい
	//lnU32 to32Bit() const;	// a r g b


	//void dump(const char* str_ = NULL) const;

	Color32 To32BitColor() const;

	static Color FromString(const StringRef& str);

	Color WithAlpha(float alpha) const { return Color(r, g, b, alpha); }

public:
	//ColorF& operator = (const ColorF& src) { r = src.r; g = src.g; b = src.b; a = src.a; return *this; }

	//bool operator == (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) == 0); }
	//bool operator != (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) != 0); }

	//ColorF& operator = (const LVector4& vec) { r = vec.X; g = vec.Y; b = vec.Z; a = vec.W; return *this; }

	bool operator == (const Color& color) const { return r == color.r && g == color.g && b == color.b && a == color.a; }
	bool operator != (const Color& color) const { return !operator==(color); }

	operator const Vector4&() { return *reinterpret_cast<Vector4*>(this); }
	operator const Vector4&() const { return *reinterpret_cast<const Vector4*>(this); }
};

/**
	@brief	色調を定義します。
*/
class ToneF
{
public:
	static const ToneF Zero;	///< ToneF(0, 0, 0, 0);

public:

	float	r;	///< 赤成分のカラーバランス (-1.0～1.0)
	float	g;	///< 緑成分のカラーバランス (-1.0～1.0)
	float	b;	///< 青成分のカラーバランス (-1.0～1.0)
	float	gray;	///< グレースケール化フィルタの強さ (0.0 ～ 1.0)
	// TODO: gray → s (彩度:Saturation)

public:

	/**
		@brief	すべての要素を 0.0 で初期化します。
	*/
	ToneF() { r = 0.0f; g = 0.0f; b = 0.0f; gray = 0.0f; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	ToneF(float r_, float g_, float b_, float gs_) { r = r_; g = g_; b = b_; gray = gs_; }

	ToneF(const Vector4& vec) { r = vec.x; g = vec.y; b = vec.z; gray = vec.w; }

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(float r_, float g_, float b_, float gs_) { r = r_; g = g_; b = b_; gray = gs_; }

	/**
		@brief	この色調に指定した色調を加算します。0.0～1.0 を超える場合はクランプします。
	*/
	void AddClamp(const ToneF& tone);


public:
	//operator Vector4() { return *reinterpret_cast< Vector4* >(this); }
	operator const Vector4&() const { return *reinterpret_cast< const Vector4* >(this); }

	friend Vector3 operator - (const Vector3& v1);
	friend ToneF operator + (const ToneF& v1, const ToneF& v2);
	friend ToneF operator + (const ToneF& v1, float v2);
	friend ToneF operator + (float v1, const ToneF& v2);
	friend ToneF operator - (const ToneF& v1, const ToneF& v2);
	friend ToneF operator - (const ToneF& v1, float v2);
	friend ToneF operator - (float v1, const ToneF& v2);
	friend ToneF operator * (const ToneF& v1, const ToneF& v2);
	friend ToneF operator * (const ToneF& v1, float v2);
	friend ToneF operator * (float v1, const ToneF& v2);
	friend ToneF operator / (const ToneF& v1, const ToneF& v2);
	friend ToneF operator / (const ToneF& v1, float v2);
	friend ToneF operator / (float v1, const ToneF& v2);

	bool operator != (const ToneF& tone) { return r != tone.r || g != tone.g || b != tone.b || gray != tone.gray; }
};

//------------------------------------------------------------------------------
inline ToneF operator - (const ToneF& v1) { return ToneF(-v1.r, -v1.g, -v1.b, -v1.gray); }
inline ToneF operator + (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.gray + v2.gray); }
inline ToneF operator + (const ToneF& v1, float v2) { return ToneF(v1.r + v2, v1.g + v2, v1.b + v2, v1.gray + v2); }
inline ToneF operator + (float v1, const ToneF& v2) { return ToneF(v1 + v2.r, v1 + v2.g, v1 + v2.b, v1 + v2.gray); }
inline ToneF operator - (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r - v2.r, v1.g - v2.g, v1.b - v2.b, v1.gray - v2.gray); }
inline ToneF operator - (const ToneF& v1, float v2) { return ToneF(v1.r - v2, v1.g - v2, v1.b - v2, v1.gray - v2); }
inline ToneF operator - (float v1, const ToneF& v2) { return ToneF(v1 - v2.r, v1 - v2.g, v1 - v2.b, v1 - v2.gray); }
inline ToneF operator * (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.gray * v2.gray); }
inline ToneF operator * (const ToneF& v1, float v2) { return ToneF(v1.r * v2, v1.g * v2, v1.b * v2, v1.gray * v2); }
inline ToneF operator * (float v1, const ToneF& v2) { return ToneF(v1 * v2.r, v1 * v2.g, v1 * v2.b, v1 * v2.gray); }
inline ToneF operator / (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r / v2.r, v1.g / v2.g, v1.b / v2.b, v1.gray / v2.gray); }
inline ToneF operator / (const ToneF& v1, float v2) { return ToneF(v1.r / v2, v1.g / v2, v1.b / v2, v1.gray / v2); }
inline ToneF operator / (float v1, const ToneF& v2) { return ToneF(v1 / v2.r, v1 / v2.g, v1 / v2.b, v1 / v2.gray); }


/**
	@brief	各要素を整数値で表す HSV カラーを定義します。
*/
class HSVColor
{
public:
	uint32_t	H;	///< 色相 0～360
	uint32_t	S;	///< 彩度 0～255
	uint32_t	V;	///< 明度 0～255
	uint32_t	a;	///< 不透明度 0～255

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	HSVColor() { H = 0; S = 0; V = 0; a = 255; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	HSVColor(uint32_t h, uint32_t s, uint32_t v, uint32_t a_) { H = h; S = s; V = v; a = a_; }
	
	/**
		@brief	ColorF を変換して設定します。
	*/
	//Color32(const ColorF& colorF);

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(uint32_t h, uint32_t s, uint32_t v, uint32_t a_) { H = h; S = s; V = v; a = a_; }

	/**
		@brief	Color32 型に変換します。
	*/
	Color32 ToColor() const;
	
	/**
		@brief	ColorF 型に変換します。
	*/
	Color ToColorF() const;

public:
	bool operator == (const Color32& color) const { return (memcmp(this, &color, sizeof(Color32)) == 0); }
	bool operator != (const Color32& obj) const { return !operator==(obj); }
};


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
