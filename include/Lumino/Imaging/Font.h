
#pragma once
#include "../Graphics/Color.h"
#include "../Imaging/Bitmap.h"

namespace Lumino
{
namespace Imaging
{

struct FontGlyphData
{
	Imaging::Bitmap*			GlyphBitmap;
	Imaging::Bitmap*			OutlineBitmap;	///< 縁どり線用 (NULL でなければ先にこれをbltし、その上に GlyphBitmap を blt する)
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

	/// フォント名の設定
	virtual void SetName(const TCHAR* fontName) = 0;

	/// フォント名の取得
	virtual const TCHAR* GetName() const = 0;

	/// フォントサイズの設定
	virtual void SetSize(int size) = 0;

	/// フォントサイズの取得
	virtual int GetSize() const = 0;

	/// フォントカラーの設定
	virtual void SetColor(const Graphics::Color& color) = 0;

	/// フォントカラーの取得
	virtual const Graphics::Color& GetColor() const = 0;

	/// エッジカラーの設定
	virtual void SetEdgeColor(const Graphics::Color& color) = 0;

	/// エッジカラーの取得
	virtual const Graphics::Color& GetEdgeColor() const = 0;

	/// エッジの幅の設定 (0 でエッジ無効)
	virtual void SetEdgeSize(int size) = 0;

	/// エッジの幅の取得
	virtual int GetEdgeSize() const = 0;

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

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual Rect GetTextSize(const char* text, int length) const = 0;

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual Rect GetTextSize(const wchar_t* text, int length) const = 0;

	/// このフォントのコピーを作成する
	virtual Font* Copy() const = 0;

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* MakeGlyphData(UTF32 utf32code, FontGlyphData* prevData) = 0;

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void EndMakeGlyphData(FontGlyphData* glyphData) = 0;
	// ↑メンバに持ってればいいだけだし必要ないかも。スレッドセーフにする必要もないし。

protected:
	Font();
	virtual ~Font();
};

} // namespace Imaging
} // namespace Lumino
