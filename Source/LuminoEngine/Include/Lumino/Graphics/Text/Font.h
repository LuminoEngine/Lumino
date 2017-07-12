
#pragma once
#include <Lumino/Base/Cache.h>
#include "../Common.h"
#include "../Color.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class FontManager; }
namespace detail { class FontGlyphTextureCache; }
namespace detail { class VectorFontGlyphCache; }
class Bitmap;
class Font;
class RawFont;
using FontPtr = RefPtr<Font>;
using RawFontPtr = RefPtr<RawFont>;

// https://www.freetype.org/freetype2/docs/tutorial/step2.html
struct FontGlobalMetrics
{
	float	ascender;
	float	descender;
	float	lineSpace;		// 1行の最大高さ。次の行までの間隔。
};

struct FontGlyphMetrics
{
	float	bearingX;
	float	bearingY;
	Vector2	advance;		// 次の文字までのベースライン間隔
};

/** ビルトインフォントの大きさ */
enum class BuiltinFontSize
{
	XXSmall,
	//Medium,
	//Large,
};

/**
	@brief		テキストをレンダリングするために使用するフォントを表します。
*/
class Font
	: public Object
{
	LN_OBJECT();
public:

	/**
		@brief	デフォルトのスタイルに基づいてフォントを作成します。
	*/
	static FontPtr create();
	
	/**
		@brief	指定したフォントファミリとデフォルトのサイズに基づいてフォントを作成します。
	*/
	static FontPtr create(const String& family);
	
	/**
		@brief	指定したフォントファミリとサイズに基づいてフォントを作成します。
	*/
	static FontPtr create(const String& family, float size);

	/**
		@brief	デフォルトのフォントを取得します。
	*/
	static FontPtr getDefault();
	
	/**
		@brief	Lumino に内蔵されているビットマップフォントを取得します。
	*/
	static FontPtr getBuiltin(BuiltinFontSize size);

public:

	/** フォントファミリ名の設定 */
	void setFamily(const String& familyName);

	/** フォントファミリ名の取得 */
	const String& getFamily() const;

	/** フォントサイズの有効設定 */
	void setSize(int size);

	/** フォントサイズの取得 */
	int getSize() const;

	/** 太文字の有効設定 */
	void setBold(bool enabled);

	/** 太文字の判定 */
	bool isBold() const;

	/** イタリック体の有効設定 */
	void setItalic(bool enabled);

	/** イタリック体の判定 */
	bool isItalic() const;

	/** アンチエイリアスの有効設定 */
	void setAntiAlias(bool enabled);

	/** アンチエイリアスの有効判定 */
	bool isAntiAlias() const;

	/** 指定した文字列を描画する際のサイズを計算します。*/
	Size measureRenderSize(const StringRef& text);

	/** このフォントのコピーを作成します。*/
	FontPtr clone() const;

LN_INTERNAL_ACCESS:
	Font();
	virtual ~Font();
	void initialize(detail::GraphicsManager* manager, RawFont* builtinRawFont);
	RawFont* resolveRawFont();

private:
	detail::GraphicsManager*	m_manager;
	detail::FontData			m_fontInfo;
	RefPtr<RawFont>				m_rawFont;
	bool						m_builtin;
};





struct FontGlyphLocation
{
	PointI	BitmapTopLeftPosition;			///< ビットマップを転送するべき座標
	PointI	OutlineBitmapTopLeftPosition;	///< アウトライン用ビットマップを転送するべき座標
	PointI	OuterTopLeftPosition;			///< アウトライン有効時は OutlineBitmapTopLeftPosition、無効時は BitmapTopLeftPosition と同じ値になる
	SizeI	BitmapSize;						///< ビットマップグリフのピクセルサイズ
};

struct FontGlyphBitmap
{
	Bitmap*			GlyphBitmap;
	Bitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
	int				OutlineOffset;	///< 縁どり時は、線の分だけサイズが大きくなる。
};


// [Obsolete]
struct FontGlyphData
{
	Bitmap*			GlyphBitmap;
	Bitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
	int				GlyphOffsetX;   ///< ビットマップを転送するべき座標へのXオフセット
	int				GlyphOffsetY;   ///< ビットマップを転送するべき座標へのYオフセット
	int				OutlineOffset;	///< 縁どり時は、線の分だけサイズが大きくなる。

	int				MetricsHeight;	///< 行高さ

	/* 描画先座標は GlyphOffsetX, GlyphOffsetY に格納されている。
	* これは makeGlyphData() を呼ぶたびに送り方向へ加算されていくため、
	* 描画側は座標を加算し続ける必要はない。
	* 常に 描画領域の左上 + GlyphOffsetX(Y) でOK。
	*
	* GlyphOffsetX, GlyphOffsetY は、アウトラインの有無にかかわらず常に同じ位置を指す。
	* アウトラインがある場合は この位置を起点に書いた後、OutlineOffset を加算した場所に 通常グリフを blt する。
	*/

	
};


/**
	@brief		
*/
class RawFont
	: public RefObject
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	static const int DefaultSize = 16;

	// POD
	struct OutlineInfo
	{
		int	startIndex;
		int	vertexCount;
	};

	// POD
	struct FontOutlineVertex
	{
		Vector2	pos;
		float	alpha;
		Vector2	extrusion;	// 押し出し方向
		Vector2	extrusion2;	// 押し出し方向

		FontOutlineVertex(const Vector2& pos_)
			: pos(pos_)
			, alpha(1.0f)
		{}
	};

	struct VectorGlyphInfo
	{
		List<FontOutlineVertex>	vertices;
		List<OutlineInfo>		outlines;
		List<uint16_t>			triangleIndices;	// 要素数は3の倍数となる
	};

	static RawFontPtr getDefaultFont();

	static RawFontPtr create();


	static RefPtr<RawFont> createBuiltInBitmapFontInternal2(int size);


	static void registerFontFile(const StringRef& filePath);


public:

	/// フォント名の設定
	virtual void setName(const String& fontName) = 0;	// TODO: FamiryName

	/// フォント名の取得
	virtual const String& getName() const = 0;

	/// フォントサイズの設定
	/** フォントサイズは相対的な値で、最終的なレンダリングサイズはフォントによって異なります。*/
	virtual void setSize(int size) = 0;

	/// フォントサイズの取得
	virtual int getSize() const = 0;

	///// フォントカラーの設定
	//virtual void setColor(const Graphics::Color& color) = 0;

	///// フォントカラーの取得
	//virtual const Graphics::Color& getColor() const = 0;

	///// エッジカラーの設定
	//virtual void SetEdgeColor(const Graphics::Color& color) = 0;

	///// エッジカラーの取得
	//virtual const Graphics::Color& GetEdgeColor() const = 0;

	///// エッジの幅の設定 (0 でエッジ無効)
	//virtual void SetEdgeSize(int size) = 0;

	///// エッジの幅の取得
	//virtual int GetEdgeSize() const = 0;

	/// 太文字の設定
	virtual void setBold(bool enabled) = 0;

	/// 太文字の判定
	virtual bool isBold() const = 0;

	/// イタリック体の設定
	virtual void setItalic(bool enabled) = 0;

	/// イタリック体の判定
	virtual bool isItalic() const = 0;

	/// アンチエイリアスの有効設定
	virtual void setAntiAlias(bool enabled) = 0;

	/// アンチエイリアスの有効判定
	virtual bool isAntiAlias() const = 0;

	/// このフォントのコピーを作成する
	virtual RawFontPtr copy() const = 0;

	/** 次の行のベースラインまでの長さを返します。*/
	virtual int getLineSpacing() = 0;

	virtual SizeI getGlyphMaxSize() { return SizeI(getLineSpacing(), getLineSpacing()); }

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	//virtual Size getTextSize(const char* text, int length) = 0;
	//virtual Size getTextSize(const wchar_t* text, int length) = 0;
	//virtual Size getTextSize(const UTF32* text, int length) = 0;

	/** (高さは次の行のベースラインまでの長さ) */
	SizeI getTextSize(const StringRef& text);

	virtual FontGlyphLocation* advanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData) = 0;
	virtual FontGlyphBitmap* lookupGlyphBitmap(UTF32 utf32code, int strokeSize) = 0;


	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	/// 削除予定
	//virtual FontGlyphData* LookupGlyphData(UTF32 utf32code, FontGlyphData* prevData) = 0;


	virtual void getGlobalMetrics(FontGlobalMetrics* outMetrix) = 0;
	virtual bool isOutlineSupported() const = 0;
	virtual void decomposeOutline(UTF32 utf32code, RawFont::VectorGlyphInfo* outInfo) = 0;
	virtual Vector2 getKerning(UTF32 prev, UTF32 next) = 0;
	virtual void getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics) = 0;

	virtual detail::FontManager* getManager() const = 0;


LN_CONSTRUCT_ACCESS:
	RawFont();
	virtual ~RawFont();
	void initialize();
	virtual void finalize_();

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void Dispose_();

LN_INTERNAL_ACCESS:
	detail::FontGlyphTextureCache* GetGlyphTextureCache();
	detail::VectorFontGlyphCache* GetVectorGlyphCache();

private:
	detail::FontManager*			m_manager;
	detail::FontGlyphTextureCache*	m_glyphTextureCache;
	detail::VectorFontGlyphCache*	m_vectorGlyphCache;

};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
