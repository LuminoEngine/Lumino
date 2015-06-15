
#include "../Internal.h"
#include <algorithm>
#include "PngFile.h"
#include "../../include/Lumino/Imaging/Bitmap.h"

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
Bitmap::Bitmap(const Size& size, PixelFormat format, bool upFlow)
{
	Init();
	m_size = size;
	m_format = format;
	m_bitmapData = LN_NEW ByteBuffer(GetPixelFormatByteCount(format, size));
	m_upFlow = upFlow;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap::Bitmap(Stream* stream)
{
	Init();
	LN_THROW(stream != NULL, ArgumentException);

	PngFile pngFile;
	if (!pngFile.load(stream, false)) {
		LN_THROW(0, InvalidFormatException);
	}
	m_size = pngFile.m_size;
	m_format = pngFile.m_format;
	LN_REFOBJ_SET(m_bitmapData, pngFile.m_bitmapData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap::Bitmap(const TCHAR* filePath)
{
	Init();
	LN_THROW(filePath != NULL, ArgumentException);

	FileStream file(filePath, FileOpenMode::Read);
	PngFile pngFile;
	if (!pngFile.load(&file, false)) {
		LN_THROW(0, InvalidFormatException);
	}
	m_size = pngFile.m_size;
	m_format = pngFile.m_format;
	LN_REFOBJ_SET(m_bitmapData, pngFile.m_bitmapData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap::Bitmap(ByteBuffer* buffer, const Size& size, PixelFormat format)
{
	Init();
	m_size = size;
	m_format = format;
	LN_REFOBJ_SET(m_bitmapData, buffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap::~Bitmap()
{
	LN_SAFE_RELEASE(m_bitmapData);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::Init()
{
	m_bitmapData = NULL;
	m_size.Set(0, 0);
	m_pitch = 0;
	m_format = PixelFormat_Unknown;
	m_upFlow = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::BitBlt(const Rect& destRect, const Bitmap* srcBitmap, const Rect& srcRect, bool alphaBlend)
{
	BitBltInternal(this, destRect, srcBitmap, srcRect, alphaBlend);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::Save(const TCHAR* filePath)
{
	// png に保存するときは RGBA
	Bitmap bitmap(m_size, PixelFormat_BYTE_R8G8B8A8);
	ConvertPixelFormat(
		m_bitmapData->GetData(), m_bitmapData->GetSize(), m_format,
		bitmap.m_bitmapData->GetData(), bitmap.m_bitmapData->GetSize(), bitmap.m_format);

	// アルファ無しフォーマットであれば、アルファを埋めてから出力する
	if (m_format == PixelFormat_BYTE_B8G8R8X8) {
		bitmap.FillAlpha(0xFF);
	}

	PngFile pngFile;
	pngFile.Save(filePath, bitmap.m_bitmapData, bitmap.m_size, bitmap.m_upFlow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Bitmap::Equals(const Bitmap* bitmap) const
{
	if (m_size != bitmap->m_size ||
		m_format != bitmap->m_format ||
		m_bitmapData->GetSize() != bitmap->m_bitmapData->GetSize()) {
		return false;
	}

	return memcmp(m_bitmapData->GetData(), bitmap->m_bitmapData->GetData(), m_bitmapData->GetSize()) == 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::ConvertToDownFlow()
{
	int pixelSize = GetPixelFormatByteCount(m_format);
	if (pixelSize == 1)
	{
		// XOR で工夫すると演算回数が少なくなるとか最適化の余地はあるけど、
		// とりあえず今は評価目的でしか使わないので愚直に swap。
		byte_t* pixels = m_bitmapData->GetData();
		for (int y = 0; y < (m_size.Height / 2); ++y) {
			for (int x = 0; x < m_size.Width; ++x) {
				std::swap(pixels[(y * m_size.Width) + x], pixels[((m_size.Height - 1 - y) * m_size.Width) + x]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_bitmapData->GetData();
		for (int y = 0; y < (m_size.Height / 2); ++y) {
			for (int x = 0; x < m_size.Width; ++x) {
				std::swap(pixels[(y * m_size.Width) + x], pixels[((m_size.Height - 1 - y) * m_size.Width) + x]);
			}
		}
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Bitmap::GetPixelFormatByteCount(PixelFormat format)
{
	const int table[] =
	{
		0,	// PixelFormat_Unknown = 0,
		1,	// PixelFormat_A1,
		1,	// PixelFormat_A8,
		4,	// PixelFormat_BYTE_R8G8B8A8,
		4,	// PixelFormat_BYTE_R8G8B8X8,
		4,	// PixelFormat_BYTE_B8G8R8A8,
		4,	// PixelFormat_BYTE_B8G8R8X8,
		16,	// PixelFormat_R32G32B32A32_Float,
	};
	assert(LN_ARRAY_SIZE_OF(table) == PixelFormat_Max);
	return table[format];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Bitmap::GetPixelFormatByteCount(PixelFormat format, const Size& size)
{
	int c = GetPixelFormatByteCount(format);
	return c * size.Width * size.Height;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::ConvertPixelFormat(
	const byte_t* input, size_t inputSize, PixelFormat inputFormat,
	byte_t* output, size_t outputSize, PixelFormat outputFormat)
{
	// 同じフォーマットならコピーするだけ
	if (inputFormat == outputFormat)
	{
		memcpy(output, input, outputSize);
	}
	// RGBA ⇔ BGRA (R と B を入れ替えるだけなので共用できる)
	else if (
		(inputFormat == PixelFormat_BYTE_R8G8B8A8 && outputFormat == PixelFormat_BYTE_B8G8R8A8) ||
		(inputFormat == PixelFormat_BYTE_B8G8R8A8 && outputFormat == PixelFormat_BYTE_R8G8B8A8) ||
		(inputFormat == PixelFormat_BYTE_B8G8R8X8 && outputFormat == PixelFormat_BYTE_R8G8B8A8))
	{
		const uint32_t* in = (const uint32_t*)input;
		uint32_t* out = (uint32_t*)output;
		size_t count = inputSize / sizeof(uint32_t);
		for (size_t i = 0; i < count; ++i) {
			uint32_t c = in[i];
			out[i] =
				c & 0xff000000 |
				((c & 0x000000ff) << 16) |
				((c & 0x0000ff00)) |
				((c & 0x00ff0000) >> 16);
		}
	}
	else {
		LN_THROW(0, InvalidFormatException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Bitmap::FillAlpha(byte_t alpha)
{
	if (m_format == PixelFormat_BYTE_R8G8B8A8 ||
		m_format == PixelFormat_BYTE_B8G8R8A8 ||
		m_format == PixelFormat_BYTE_B8G8R8X8)
	{
		byte_t* buf = m_bitmapData->GetData();
		size_t count = m_bitmapData->GetSize() / sizeof(uint32_t);
		for (size_t i = 0; i < count; ++i) {
			buf[i * 4 + 3] = alpha;
		}
		return;
	}
	LN_THROW(0, InvalidFormatException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

//void Bitmap::ConvertPixelU32(U32* p, PixelFormat inputFormat, PixelFormat outputFormat)
//{
//	// 同じフォーマットなら何もしない
//	if (inputFormat == outputFormat) {
//		return;
//	}
//}

void Bitmap::BitBltInternal(Bitmap* dest, const Rect& destRect_, const Bitmap* src, const Rect& srcRect_, bool alphaBlend)
{
	// 双方の矩形を Bitmap からはみ出ないようにクリッピングし、範囲の大きさは dest に合わせる。
	// (拡縮はしない。srcRect が小さければ、余分な部分は何もしない)
	Rect destRect = destRect_;
	Rect srcRect = srcRect_;
	destRect.Clip(Rect(0, 0, dest->m_size));
	srcRect.Clip(Rect(0, 0, src->m_size));
	srcRect.Width = std::min(srcRect.Width, destRect.Width);
	srcRect.Height = std::min(srcRect.Height, destRect.Height);

	// ラッピングインターフェイス
	DestBitmapWrapper destWrapper(dest, destRect);
	SecBitmapWrapper srcWrapper(src, srcRect);

	const byte_t* input = src->m_bitmapData->GetData();
	byte_t* output = dest->m_bitmapData->GetData();
	size_t outputSize = dest->m_bitmapData->GetSize();



	// 同じフォーマットならコピーするだけ
	if (dest->m_format == src->m_format)
	{
		// TODO: ダメ。ちゃんと1pxずつブレンドする。
		memcpy(output, input, outputSize);
		return;
	}
	// 双方が 32bit 系フォーマットである
	else if (PixelFormat_BYTE_R8G8B8A8 <= dest->m_format && dest->m_format <= PixelFormat_BYTE_B8G8R8X8 &&
		PixelFormat_BYTE_R8G8B8A8 <= src->m_format && src->m_format <= PixelFormat_BYTE_B8G8R8X8)
	{
		bool destIsRGBALike = (dest->m_format == PixelFormat_BYTE_R8G8B8A8 || dest->m_format == PixelFormat_BYTE_B8G8R8X8);
		bool srcIsRGBALike = (src->m_format == PixelFormat_BYTE_R8G8B8A8 || src->m_format == PixelFormat_BYTE_B8G8R8X8);

		// 同じバイトシーケンスであればコピーするだけでよい
		if (destIsRGBALike == srcIsRGBALike)
		{
			// TODO: ダメ。ちゃんと1pxずつブレンドする。
			memcpy(output, input, outputSize);
		}
		// 違うバイトシーケンスであれば R と B を入れ替える
		else
		{
			if (alphaBlend)
			{
				LN_THROW(0, NotImplementedException);
			}
			else
			{
				for (int y = 0; y < destRect.Height; ++y)
				{
					destWrapper.SetLineU32(y);
					srcWrapper.SetLineU32(y);
					for (int x = 0; x < destRect.Width; ++x)
					{
						U32 c = srcWrapper.GetPixelU32(x);
						SwapPixelRBU32(&c);
						destWrapper.SetPixelU32(x, c);
					}
				}
			}
		}
		return;
	}

	LN_THROW(0, InvalidFormatException);
}

} // namespace Imaging
} // namespace Lumino
