
#pragma once

#include "../Graphics/Color.h"

namespace Lumino
{
namespace Imaging
{
class FileManager;

/**
	@brief		フォントのクラス
*/
class Font
	: public RefObject
{
public:
	static Font* CreateBitmapFont(FileManager* fileManager, Bitmap* bitmap);

public:
#if 0
	/// フォント名の設定
	virtual void setName(const TCHAR* fontName) = 0;

	/// フォント名の取得
	virtual const TCHAR* getName() const = 0;

	/// フォントサイズの設定
	virtual void setSize(int size) = 0;

	/// フォントサイズの取得
	virtual int getSize() const = 0;
/*
	/// フォントカラーの設定
	virtual void setColor(const Color& color) = 0;

	/// フォントカラーの取得
	virtual const Color& getColor() const = 0;

	/// エッジカラーの設定
	virtual void setEdgeColor(const Color& color) = 0;

	/// エッジカラーの取得
	virtual const Color& getEdgeColor() const = 0;*/

	///// エッジの幅の設定 (0 でエッジ無効)
	//virtual void setEdgeSize(int size) = 0;

	///// エッジの幅の取得
	//virtual int getEdgeSize() const = 0;

	/// 太文字の設定
	virtual void setBold(bool flag) = 0;

	/// 太文字の判定
	virtual bool isBold() const = 0;

	/// イタリック体の設定
	virtual void setItalic(bool flag) = 0;

	/// イタリック体の判定
	virtual bool isItalic() const = 0;

	/// アンチエイリアスの有効設定
	virtual void setAntiAlias(bool flag) = 0;

	/// アンチエイリアスの有効判定
	virtual bool isAntiAlias() const = 0;

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual void getTextSize(const char* text, int length, Geometry::Rect* outRect) = 0;

	/// 文字列を描画したときのサイズ (ピクセル単位) の取得 (length = -1 で \0 まで)
	virtual void getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect) = 0;

	/// このフォントのコピーを作成する
	virtual Font* copy() = 0;

	/// グリフデータの取得 (最初の文字の場合、prevData に NULL を渡す。以降は戻り値を渡し続ける。非スレッドセーフ)
	virtual FontGlyphData* makeGlyphData(int utf32code, FontGlyphData* prevData) = 0;

	/// グリフデータの取得を終了する (メモリ解放。一連の makeGlyphData() を呼び終わった後、最後に呼ぶ)
	virtual void postGlyphData(FontGlyphData* glyphData) = 0;
#endif
protected:
	Font();
	~Font();
};

} // namespace Imaging
} // namespace Lumino
