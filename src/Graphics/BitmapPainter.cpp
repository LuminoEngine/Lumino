
#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Bitmap
//==============================================================================

//------------------------------------------------------------------------------
BitmapPainter::BitmapPainter(Bitmap* targetBitmap)
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
void BitmapPainter::Clear(const Color& color)
{
	FillRectangle(Rect(Point(0, 0), m_bitmap->GetSize()), color);
}

//------------------------------------------------------------------------------
void BitmapPainter::FillRectangle(const Rect& rect, const Color& color)
{
	Rect destRect = rect;
	destRect.Clip(Rect(Point(0, 0), m_bitmap->GetSize()));

	PixelFormat format = m_bitmap->GetPixelFormat();

	// 32bit 系
	if (format == PixelFormat_BYTE_R8G8B8A8 ||
		format == PixelFormat_BYTE_B8G8R8A8 ||
		format == PixelFormat_BYTE_B8G8R8X8)
	{
		uint32_t* data = (uint32_t*)m_bitmap->GetBitmapBuffer()->GetData();

		uint32_t c = GetColorByteSec(color, format);
		for (int y = 0; y < destRect.height; y++)
		{
			for (int x = 0; x < destRect.width; x++)
			{
				data[(destRect.y + y) * m_bitmap->GetSize().width + (destRect.x + x)] = c;
			}
		}
		return;
	}

	LN_THROW(0, InvalidFormatException);
}

//------------------------------------------------------------------------------
uint32_t BitmapPainter::GetColorByteSec(const Color& color, PixelFormat format)
{
	switch (format)
	{
	case PixelFormat_BYTE_R8G8B8A8:
	{
		byte_t b[4] = { color.r, color.g, color.b, color.a };
		return *((uint32_t*)b);
	}
	case PixelFormat_BYTE_B8G8R8A8:
	{
		byte_t b[4] = { color.b, color.g, color.r, color.a };
		return *((uint32_t*)b);
	}
	case PixelFormat_BYTE_B8G8R8X8:
	{
		byte_t b[4] = { color.b, color.g, color.r, 0xFF };
		return *((uint32_t*)b);
	}
	default:
		break;
	}
	LN_THROW(0, InvalidFormatException);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
