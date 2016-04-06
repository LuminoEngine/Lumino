
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
	static const Color Black;					///< 黒		(0, 0, 0, 255)
	static const Color White;					///< 白		(255, 255, 255, 255)
	static const Color Gray;					///< 灰色	(127, 127, 127, 255)
	static const Color Red;						///< 赤		(255, 0, 0, 255)
	static const Color Green;					///< 緑		(0, 255, 0, 255)
	static const Color Blue;					///< 青		(0, 0.0, 255, 255)

public:
	uint8_t		r;		///< 赤成分 (0～255)
	uint8_t		g;		///< 緑成分 (0～255)
	uint8_t		b;		///< 青成分 (0～255)
	uint8_t		a;		///< アルファ成分 (0～255)

public:

	/**
		@brief	すべての要素を 0 で初期化します。
	*/
	Color() { r = 0; g = 0; b = 0; a = 0; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }
	
	/**
		@brief	ColorF を変換して設定します。
	*/
	Color(const ColorF& colorF);

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255) { r = r_; g = g_; b = b_; a = a_; }

	/**
		@brief	RR GG BB AA のバイトシーケンスへ変換します。
	*/
	uint32_t ToR8G8B8A8() const 
	{
		byte_t data[4] = { r, g, b, a };
		return *((uint32_t*)data);
	};

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
	float	GS;	///< グレースケール化フィルタの強さ (0.0 ～ 1.0)

public:

	/**
		@brief	すべての要素を 0.0 で初期化します。
	*/
	ToneF() { r = 0.0f; g = 0.0f; b = 0.0f; GS = 0.0f; }

	/**
		@brief	各要素を指定して初期化します。
	*/
	ToneF(float r_, float g_, float b_, float gs_) { r = r_; g = g_; b = b_; GS = gs_; }

public:

	/**
		@brief	各要素を設定します。
	*/
	void Set(float r_, float g_, float b_, float gs_) { r = r_; g = g_; b = b_; GS = gs_; }

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

	bool operator != (const ToneF& tone) { return r != tone.r || g != tone.g || b != tone.b || GS != tone.GS; }
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
inline ToneF operator - (const ToneF& v1) { return ToneF(-v1.r, -v1.g, -v1.b, -v1.GS); }
inline ToneF operator + (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.GS + v2.GS); }
inline ToneF operator + (const ToneF& v1, float v2) { return ToneF(v1.r + v2, v1.g + v2, v1.b + v2, v1.GS + v2); }
inline ToneF operator + (float v1, const ToneF& v2) { return ToneF(v1 + v2.r, v1 + v2.g, v1 + v2.b, v1 + v2.GS); }
inline ToneF operator - (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r - v2.r, v1.g - v2.g, v1.b - v2.b, v1.GS - v2.GS); }
inline ToneF operator - (const ToneF& v1, float v2) { return ToneF(v1.r - v2, v1.g - v2, v1.b - v2, v1.GS - v2); }
inline ToneF operator - (float v1, const ToneF& v2) { return ToneF(v1 - v2.r, v1 - v2.g, v1 - v2.b, v1 - v2.GS); }
inline ToneF operator * (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.GS * v2.GS); }
inline ToneF operator * (const ToneF& v1, float v2) { return ToneF(v1.r * v2, v1.g * v2, v1.b * v2, v1.GS * v2); }
inline ToneF operator * (float v1, const ToneF& v2) { return ToneF(v1 * v2.r, v1 * v2.g, v1 * v2.b, v1 * v2.GS); }
inline ToneF operator / (const ToneF& v1, const ToneF& v2) { return ToneF(v1.r / v2.r, v1.g / v2.g, v1.b / v2.b, v1.GS / v2.GS); }
inline ToneF operator / (const ToneF& v1, float v2) { return ToneF(v1.r / v2, v1.g / v2, v1.b / v2, v1.GS / v2); }
inline ToneF operator / (float v1, const ToneF& v2) { return ToneF(v1 / v2.r, v1 / v2.g, v1 / v2.b, v1 / v2.GS); }


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
