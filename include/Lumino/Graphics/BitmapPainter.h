
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
	void Clear(const Color& color);

	/**
		@brief		矩形を塗りつぶします。
	*/
	void FillRectangle(const Rect& rect, const Color& color);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(BitmapPainter);

	uint32_t GetColorByteSec(const Color& color, PixelFormat format);

public:
	Bitmap*	m_bitmap;

};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
