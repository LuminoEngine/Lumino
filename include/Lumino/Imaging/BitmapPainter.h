
#pragma once

#include <Lumino/Graphics/Color.h>
#include "Bitmap.h"

namespace Lumino
{
namespace Imaging
{

/**
	@brief	 ビットマップへの描画を行うクラスです。
*/
class BitmapPainter
	: public NonCopyable
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
	void Clear(const Graphics::Color& color);

	/**
		@brief		矩形を塗りつぶします。
	*/
	void FillRectangle(const Rect& rect, const Graphics::Color& color);

private:
	uint32_t GetColorByteSec(const Graphics::Color& color, PixelFormat format);

public:
	Bitmap*	m_bitmap;

};

} // namespace Imaging
} // namespace Lumino
