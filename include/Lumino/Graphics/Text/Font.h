
#pragma once
#include "../Common.h"
#include "../Color.h"
#include "../Bitmap.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

struct FontGlyphLocation
{
	Point	BitmapTopLeftPosition;			///< ビットマップを転送するべき座標
	Point	OutlineBitmapTopLeftPosition;	///< アウトライン用ビットマップを転送するべき座標
	Point	OuterTopLeftPosition;			///< アウトライン有効時は OutlineBitmapTopLeftPosition、無効時は BitmapTopLeftPosition と同じ値になる
	Size	BitmapSize;						///< ビットマップグリフのピクセルサイズ
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
	@brief		フォントのクラスです。
*/
class Font
	: public RefObject
{
public:

	static FontPtr Create();


	static Font* CreateBuiltInBitmapFontInternal(FontManager* manager, int size);			// TODO: manager を何とかしたい

public:

	/// フォント名の設定
	virtual void SetName(const String& fontName) = 0;

	/// フォント名の取得
	virtual const String& GetName() const = 0;

	/// フォントサイズの設定
	virtual void SetSize(int size) = 0;

	/// フォントサイズの取得
	virtual int GetSize() const = 0;

	///// フォントカラーの設定
	//virtual void SetColor(const Graphics::Color& color) = 0;

	///// フォントカラーの取得
	//virtual const Graphics::Color& GetColor() const = 0;

	///// エッジカラーの設定
	//virtual void SetEdgeColor(const Graphics::Color& color) = 0;

	///// エッジカラーの取得
	//virtual const Graphics::Color& GetEdgeColor() const = 0;

	///// エッジの幅の設定 (0 でエッジ無効)
	//virtual void SetEdgeSize(int size) = 0;

	///// エッジの幅の取得
	//virtual int GetEdgeSize() const = 0;

	/// 太文字の設定
	virtual void SetBold(bool enabled) = 0;

	/// 太文字の判定
	virtual bool IsBold() const = 0;

	/// イタリック体の設定
	virtual void SetItalic(bool enabled) = 0;

	/// イタリック体の判定
	virtual bool IsItalic() const = 0;

	/// アンチエイリアスの有効設定
	virtual void SetAntiAlias(bool enabled) = 0;

	/// アンチエイリアスの有効判定
	virtual bool IsAntiAlias() const = 0;

	/// このフォントのコピーを作成する
	virtual Font* Copy() const = 0;

	virtual int GetLineHeight() = 0;

	virtual Size GetGlyphMaxSize() { return Size(GetLineHeight(), GetLineHeight()); }

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	//virtual Size GetTextSize(const char* text, int length) = 0;
	//virtual Size GetTextSize(const wchar_t* text, int length) = 0;
	//virtual Size GetTextSize(const UTF32* text, int length) = 0;


	virtual FontGlyphLocation* AdvanceKerning(UTF32 utf32code, int strokeSize, FontGlyphLocation* prevData) = 0;
	virtual FontGlyphBitmap* LookupGlyphBitmap(UTF32 utf32code, int strokeSize) = 0;


	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	/// 削除予定
	//virtual FontGlyphData* LookupGlyphData(UTF32 utf32code, FontGlyphData* prevData) = 0;


	virtual FontManager* GetManager() const = 0;


protected:
	Font();
	virtual ~Font();
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
