
#pragma once

LN_NAMESPACE_BEGIN
class Vector3;
class Vector4;

LN_NAMESPACE_GRAPHICS_BEGIN
class ColorF;
	
/**
	@brief	各要素を 0～255 の範囲で表す RGBA カラーを定義します。
*/
class Color
{
public:
	static const Color Transparency;			///< 透明	(0, 0, 0, 0)
	static const Color TransparencyWhite;		///< 白透明	(255, 255, 255, 0)
	//static const Color Black;					///< 黒		(0, 0, 0, 255)
	//static const Color White;					///< 白		(255, 255, 255, 255)
	//static const Color Gray;					///< 灰色	(127, 127, 127, 255)
	//static const Color Red;						///< 赤		(255, 0, 0, 255)
	//static const Color Green;					///< 緑		(0, 255, 0, 255)
	//static const Color Blue;					///< 青		(0, 0.0, 255, 255)

	// see https://msdn.microsoft.com/ja-jp/library/system.windows.media.colors(v=vs.110).aspx
	static const Color AliceBlue;             /** (240, 248, 255) */
	static const Color AntiqueWhite;          /** (250, 235, 215) */
	static const Color Aqua;                  /** (0, 255, 255) */
	static const Color Aquamarine;            /** (127, 255, 212) */
	static const Color Azure;                 /** (240, 255, 255) */
	static const Color Beige;                 /** (245, 245, 220) */
	static const Color Bisque;                /** (255, 228, 196) */
	static const Color Black;                 /** (0, 0, 0) */
	static const Color BlanchedAlmond;        /** (255, 235, 205) */
	static const Color Blue;                  /** (0, 0, 255) */
	static const Color BlueViolet;            /** (138, 43, 226) */
	static const Color Brown;                 /** (165, 42, 42) */
	static const Color BurlyWood;             /** (222, 184, 135) */
	static const Color CadetBlue;             /** (95, 158, 160) */
	static const Color Chartreuse;            /** (127, 255, 0) */
	static const Color Chocolate;             /** (210, 105, 30) */
	static const Color Coral;                 /** (255, 127, 80) */
	static const Color CornflowerBlue;        /** (100, 149, 237) */
	static const Color Cornsilk;              /** (255, 248, 220) */
	static const Color Crimson;               /** (220, 20, 60) */
	static const Color Cyan;                  /** (0, 255, 255) */
	static const Color DarkBlue;              /** (0, 0, 139) */
	static const Color DarkCyan;              /** (0, 139, 139) */
	static const Color DarkGoldenrod;         /** (184, 134, 11) */
	static const Color DarkGray;              /** (169, 169, 169) */
	static const Color DarkGreen;             /** (0, 100, 0) */
	static const Color DarkKhaki;             /** (189, 183, 107) */
	static const Color DarkMagenta;           /** (139, 0, 139) */
	static const Color DarkOliveGreen;        /** (85, 107, 47) */
	static const Color DarkOrange;            /** (255, 140, 0) */
	static const Color DarkOrchid;            /** (153, 50, 204) */
	static const Color DarkRed;               /** (139, 0, 0) */
	static const Color DarkSalmon;            /** (233, 150, 122) */
	static const Color DarkSeaGreen;          /** (143, 188, 143) */
	static const Color DarkSlateBlue;         /** (72, 61, 139) */
	static const Color DarkSlateGray;         /** (47, 79, 79) */
	static const Color DarkTurquoise;         /** (0, 206, 209) */
	static const Color DarkViolet;            /** (148, 0, 211) */
	static const Color DeepPink;              /** (255, 20, 147) */
	static const Color DeepSkyBlue;           /** (0, 191, 255) */
	static const Color DimGray;               /** (105, 105, 105) */
	static const Color DodgerBlue;            /** (30, 144, 255) */
	static const Color Firebrick;             /** (178, 34, 34) */
	static const Color FloralWhite;           /** (255, 250, 240) */
	static const Color ForestGreen;           /** (34, 139, 34) */
	static const Color Fuchsia;               /** (255, 0, 255) */
	static const Color Gainsboro;             /** (220, 220, 220) */
	static const Color GhostWhite;            /** (248, 248, 255) */
	static const Color Gold;                  /** (255, 215, 0) */
	static const Color Goldenrod;             /** (218, 165, 32) */
	static const Color Gray;                  /** (128, 128, 128) */
	static const Color Green;                 /** (0, 128, 0) */
	static const Color GreenYellow;           /** (173, 255, 47) */
	static const Color Honeydew;              /** (240, 255, 240) */
	static const Color HotPink;               /** (255, 105, 180) */
	static const Color IndianRed;             /** (205, 92, 92) */
	static const Color Indigo;                /** (75, 0, 130) */
	static const Color Ivory;                 /** (255, 255, 240) */
	static const Color Khaki;                 /** (240, 230, 140) */
	static const Color Lavender;              /** (230, 230, 250) */
	static const Color LavenderBlush;         /** (255, 240, 245) */
	static const Color LawnGreen;             /** (124, 252, 0) */
	static const Color LemonChiffon;          /** (255, 250, 205) */
	static const Color LightBlue;             /** (173, 216, 230) */
	static const Color LightCoral;            /** (240, 128, 128) */
	static const Color LightCyan;             /** (224, 255, 255) */
	static const Color LightGoldenrodYellow;  /** (250, 250, 210) */
	static const Color LightGray;             /** (211, 211, 211) */
	static const Color LightGreen;            /** (144, 238, 144) */
	static const Color LightPink;             /** (255, 182, 193) */
	static const Color LightSalmon;           /** (255, 160, 122) */
	static const Color LightSeaGreen;         /** (32, 178, 170) */
	static const Color LightSkyBlue;          /** (135, 206, 250) */
	static const Color LightSlateGray;        /** (119, 136, 153) */
	static const Color LightSteelBlue;        /** (176, 196, 222) */
	static const Color LightYellow;           /** (255, 255, 224) */
	static const Color Lime;                  /** (0, 255, 0) */
	static const Color LimeGreen;             /** (50, 205, 50) */
	static const Color Linen;                 /** (250, 240, 230) */
	static const Color Magenta;               /** (255, 0, 255) */
	static const Color Maroon;                /** (128, 0, 0) */
	static const Color MediumAquamarine;      /** (102, 205, 170) */
	static const Color MediumBlue;            /** (0, 0, 205) */
	static const Color MediumOrchid;          /** (186, 85, 211) */
	static const Color MediumPurple;          /** (147, 112, 219) */
	static const Color MediumSeaGreen;        /** (60, 179, 113) */
	static const Color MediumSlateBlue;       /** (123, 104, 238) */
	static const Color MediumSpringGreen;     /** (0, 250, 154) */
	static const Color MediumTurquoise;       /** (72, 209, 204) */
	static const Color MediumVioletRed;       /** (199, 21, 133) */
	static const Color MidnightBlue;          /** (25, 25, 112) */
	static const Color MintCream;             /** (245, 255, 250) */
	static const Color MistyRose;             /** (255, 228, 225) */
	static const Color Moccasin;              /** (255, 228, 181) */
	static const Color NavajoWhite;           /** (255, 222, 173) */
	static const Color Navy;                  /** (0, 0, 128) */
	static const Color OldLace;               /** (253, 245, 230) */
	static const Color Olive;                 /** (128, 128, 0) */
	static const Color OliveDrab;             /** (107, 142, 35) */
	static const Color Orange;                /** (255, 165, 0) */
	static const Color OrangeRed;             /** (255, 69, 0) */
	static const Color Orchid;                /** (218, 112, 214) */
	static const Color PaleGoldenrod;         /** (238, 232, 170) */
	static const Color PaleGreen;             /** (152, 251, 152) */
	static const Color PaleTurquoise;         /** (175, 238, 238) */
	static const Color PaleVioletRed;         /** (219, 112, 147) */
	static const Color PapayaWhip;            /** (255, 239, 213) */
	static const Color PeachPuff;             /** (255, 218, 185) */
	static const Color Peru;                  /** (205, 133, 63) */
	static const Color Pink;                  /** (255, 192, 203) */
	static const Color Plum;                  /** (221, 160, 221) */
	static const Color PowderBlue;            /** (176, 224, 230) */
	static const Color Purple;                /** (128, 0, 128) */
	static const Color Red;                   /** (255, 0, 0) */
	static const Color RosyBrown;             /** (188, 143, 143) */
	static const Color RoyalBlue;             /** (65, 105, 225) */
	static const Color SaddleBrown;           /** (139, 69, 19) */
	static const Color Salmon;                /** (250, 128, 114) */
	static const Color SandyBrown;            /** (244, 164, 96) */
	static const Color SeaGreen;              /** (46, 139, 87) */
	static const Color SeaShell;              /** (255, 245, 238) */
	static const Color Sienna;                /** (160, 82, 45) */
	static const Color Silver;                /** (192, 192, 192) */
	static const Color SkyBlue;               /** (135, 206, 235) */
	static const Color SlateBlue;             /** (106, 90, 205) */
	static const Color SlateGray;             /** (112, 128, 144) */
	static const Color Snow;                  /** (255, 250, 250) */
	static const Color SpringGreen;           /** (0, 255, 127) */
	static const Color SteelBlue;             /** (70, 130, 180) */
	static const Color Tan;                   /** (210, 180, 140) */
	static const Color Teal;                  /** (0, 128, 128) */
	static const Color Thistle;               /** (216, 191, 216) */
	static const Color Tomato;                /** (255, 99, 71) */
	static const Color Transparent;           /** (255, 255, 255) */
	static const Color Turquoise;             /** (64, 224, 208) */
	static const Color Violet;                /** (238, 130, 238) */
	static const Color Wheat;                 /** (245, 222, 179) */
	static const Color White;                 /** (255, 255, 255) */
	static const Color WhiteSmoke;            /** (245, 245, 245) */
	static const Color Yellow;                /** (255, 255, 0) */
	static const Color YellowGreen;           /** (154, 205, 50) */


public:
	uint8_t		r;		///< 赤成分 (0～255)
	uint8_t		g;		///< 緑成分 (0～255)
	uint8_t		b;		///< 青成分 (0～255)
	uint8_t		a;		///< アルファ成分 (0～255)

public:

	/** すべての要素を 0 で初期化します。*/
	Color() : r(0), g(0), b(0), a(0) {}

	/** 各要素を指定して初期化します。*/
	Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }
	
	/** ColorF を変換して設定します。*/
	//Color(const ColorF& colorF);

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }

	static Color FromString(const StringRef& str);

public:
	bool operator == (const Color& color) const { return (memcmp(this, &color, sizeof(Color)) == 0); }
	bool operator != (const Color& obj) const { return !operator==(obj); }
};


/**
	@brief	各要素を 0.0～1.0 の範囲で表す RGBA カラーを定義します。
*/
class ColorF
{
public:
	float	r;		///< 赤成分 (0.0～1.0)
	float	g;		///< 緑成分 (0.0～1.0)
	float	b;		///< 青成分 (0.0～1.0)
	float	a;		///< アルファ成分 (0.0～1.0)

public:
	static const ColorF Transparency;			///< 透明 (0.0, 0.0, 0.0, 0.0)
	static const ColorF TransparencyWhite;		///< 白透明 (1.0, 1.0, 1.0, 0.0)
	static const ColorF White;					///< 白 (1.0, 1.0, 1.0, 1.0)
	static const ColorF Black;					///< 黒 (0.0, 0.0, 0.0, 1.0)
	static const ColorF Gray;					///< 灰色 (0.5, 0.5, 0.5, 1.0)
	static const ColorF Red;					///< 赤 (1.0, 0.0, 0.0, 1.0)
	static const ColorF Green;					///< 緑 (0.0, 1.0, 0.0, 1.0)
	static const ColorF Blue;					///< 青 (0.0, 0.0, 1.0, 1.0)
	static const ColorF DimGray;				///< 濃い灰色

public:

	/**
		@brief	すべての要素を 0.0 で初期化します。
	*/
	ColorF() { r = 0.0; g = 0.0; b = 0.0; a = 0.0; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	ColorF(float r_, float g_, float b_, float a_ = 1.0f) { r = r_; g = g_; b = b_; a = a_; }

	/**
		@brief	指定した ColorF をコピーして初期化します。
	*/
	ColorF(const ColorF& color) { r = color.r; g = color.g; b = color.b; a = color.a; }

	/**
		@brief	Color を変換して設定します。
	*/
	ColorF(const Color& color);

	/**
		@brief	指定した Vector3 とアルファ値をコピーして初期化します。
	*/
	ColorF(const Vector3& vec, float a);

	/**
		@brief	指定した Vector4 をコピーして初期化します。
	*/
	ColorF(const Vector4& vec);

public:
	
	/**
		@brief	各要素を設定します。
	*/
	void Set(float r_, float g_, float b_, float a_ = 1.0f) { r = r_; g = g_; b = b_; a = a_; }

	/**
		@brief	この色に指定した色を加算します。0.0～1.0 を超える場合はクランプします。
	*/
	void AddClamp(const ColorF& color);

	/**
		@brief	この色に指定した色を乗算します。0.0～1.0 を超える場合はクランプします。
	*/
	void MultiplyClamp(const ColorF& color);

	static ColorF Lerp(const ColorF& color1, const ColorF& color2, float t);

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

	Color To32BitColor() const;

public:
	//ColorF& operator = (const ColorF& src) { r = src.r; g = src.g; b = src.b; a = src.a; return *this; }

	//bool operator == (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) == 0); }
	//bool operator != (const ColorF& color) { return (memcmp(this, &color, sizeof(ColorF)) != 0); }

	//ColorF& operator = (const LVector4& vec) { r = vec.X; g = vec.Y; b = vec.Z; a = vec.W; return *this; }

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

public:

	/**
		@brief	すべての要素を 0.0 で初期化します。
	*/
	ToneF() { r = 0.0f; g = 0.0f; b = 0.0f; gray = 0.0f; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	ToneF(float r_, float g_, float b_, float gs_) { r = r_; g = g_; b = b_; gray = gs_; }

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


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
	//Color(const ColorF& colorF);

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(uint32_t h, uint32_t s, uint32_t v, uint32_t a_) { H = h; S = s; V = v; a = a_; }

	/**
		@brief	Color 型に変換します。
	*/
	Color ToColor() const;
	
	/**
		@brief	ColorF 型に変換します。
	*/
	ColorF ToColorF() const;

public:
	bool operator == (const Color& color) const { return (memcmp(this, &color, sizeof(Color)) == 0); }
	bool operator != (const Color& obj) const { return !operator==(obj); }
};


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
