
#pragma once
#include <Lumino/Graphics/Color.h>
#include "Bitmap.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief	 ビットマップへの描画を行うクラスです。
*/
class BitmapPainter
{
public:

	/**
		@brief		指定した Bitmap へ描画を行う BitmapPainter を初期化します。
	*/
	BitmapPainter(Bitmap* targetBitmap);

	~BitmapPainter();

public:

	/**
		@brief		指定した色でビットマップ全体を塗りつぶします。
	*/
	void clear(const Color32& color);

	/**
		@brief		矩形を塗りつぶします。
	*/
	void fillRectangle(const RectI& rect, const Color32& color);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(BitmapPainter);

	uint32_t getColorByteSec(const Color32& color, PixelFormat format);

public:
	Bitmap*	m_bitmap;

};

class BitmapFilter
{
public:
	void flipVertical(Bitmap* dst, const Bitmap* src);
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
