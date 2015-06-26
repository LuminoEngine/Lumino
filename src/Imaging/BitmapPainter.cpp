
#include "../Internal.h"
#include <Lumino/Imaging/BitmapPainter.h>

namespace Lumino
{
namespace Imaging
{


//=============================================================================
// Bitmap
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapPainter::BitmapPainter(Bitmap* targetBitmap)
	: m_bitmap(NULL)
{
	LN_REFOBJ_SET(m_bitmap, targetBitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BitmapPainter::~BitmapPainter()
{
	LN_SAFE_RELEASE(m_bitmap);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapPainter::Clear(const Graphics::Color& color)
{
	FillRectangle(Rect(Point(0, 0), m_bitmap->GetSize()), color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void BitmapPainter::FillRectangle(const Rect& rect, const Graphics::Color& color)
{
	Rect destRect = rect;
	destRect.Clip(Rect(Point(0, 0), m_bitmap->GetSize()));

	PixelFormat format = m_bitmap->GetPixelFormat();

	// 32bit Œn
	if (format == PixelFormat_BYTE_R8G8B8A8 ||
		format == PixelFormat_BYTE_B8G8R8A8 ||
		format == PixelFormat_BYTE_B8G8R8X8)
	{
		uint32_t* data = (uint32_t*)m_bitmap->GetBitmapBuffer().GetData();

		uint32_t c = GetColorByteSec(color, format);
		for (int y = 0; y < destRect.Height; y++)
		{
			for (int x = 0; x < destRect.Width; x++)
			{
				data[(destRect.Y + y) * m_bitmap->GetSize().Width + (destRect.X + x)] = c;
			}
		}
		return;
	}

	LN_THROW(0, InvalidFormatException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t BitmapPainter::GetColorByteSec(const Graphics::Color& color, PixelFormat format)
{
	switch (format)
	{
	case Lumino::Imaging::PixelFormat_BYTE_R8G8B8A8:
	{
		byte_t b[4] = { color.R, color.G, color.B, color.A };
		return *((uint32_t*)b);
	}
	case Lumino::Imaging::PixelFormat_BYTE_B8G8R8A8:
	{
		byte_t b[4] = { color.B, color.G, color.R, color.A };
		return *((uint32_t*)b);
	}
	case Lumino::Imaging::PixelFormat_BYTE_B8G8R8X8:
	{
		byte_t b[4] = { color.B, color.G, color.R, 0xFF };
		return *((uint32_t*)b);
	}
	default:
		break;
	}
	LN_THROW(0, InvalidFormatException);
}

} // namespace Imaging
} // namespace Lumino
