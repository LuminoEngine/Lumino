
#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// RawBitmap
//==============================================================================

//------------------------------------------------------------------------------
BitmapPainter::BitmapPainter(RawBitmap* targetBitmap)
	: m_bitmap(NULL)
{
	LN_REFOBJ_SET(m_bitmap, targetBitmap);
}

//------------------------------------------------------------------------------
BitmapPainter::~BitmapPainter()
{
	LN_SAFE_RELEASE(m_bitmap);
}

//------------------------------------------------------------------------------
void BitmapPainter::clear(const Color32& color)
{
	fillRectangle(RectI(0, 0, m_bitmap->getSize()), color);
}

//------------------------------------------------------------------------------
void BitmapPainter::fillRectangle(const RectI& rect, const Color32& color)
{
	RectI destRect = rect;
	destRect.clip(RectI(0, 0, m_bitmap->getSize()));

	PixelFormat format = m_bitmap->getPixelFormat();

	// 32bit 系
	if (format == PixelFormat::R8G8B8A8 ||
		format == PixelFormat::B8G8R8A8 ||
		format == PixelFormat::B8G8R8X8)
	{
		uint32_t* data = (uint32_t*)m_bitmap->getBitmapBuffer()->getData();

		uint32_t c = getColorByteSec(color, format);
		for (int y = 0; y < destRect.height; y++)
		{
			for (int x = 0; x < destRect.width; x++)
			{
				data[(destRect.y + y) * m_bitmap->getSize().width + (destRect.x + x)] = c;
			}
		}
		return;
	}

	LN_THROW(0, InvalidFormatException);
}

//------------------------------------------------------------------------------
uint32_t BitmapPainter::getColorByteSec(const Color32& color, PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::R8G8B8A8:
	{
		byte_t b[4] = { color.r, color.g, color.b, color.a };
		return *((uint32_t*)b);
	}
	case PixelFormat::B8G8R8A8:
	{
		byte_t b[4] = { color.b, color.g, color.r, color.a };
		return *((uint32_t*)b);
	}
	case PixelFormat::B8G8R8X8:
	{
		byte_t b[4] = { color.b, color.g, color.r, 0xFF };
		return *((uint32_t*)b);
	}
	default:
		break;
	}
	LN_THROW(0, InvalidFormatException);
	return 0;
}


//==============================================================================
// BitmapFilter
//==============================================================================

//------------------------------------------------------------------------------
void BitmapFilter::flipVertical(RawBitmap* dst, const RawBitmap* src)
{
	for (int y = 0; y < dst->getSize().height; ++y)
	{
		for (int x = 0; x < dst->getSize().width; ++x)
		{
			dst->setPixel(x, y, src->getPixel(x, src->getSize().height - y - 1));
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
