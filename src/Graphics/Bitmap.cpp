
#include "../Internal.h"
#include <algorithm>
#include <Lumino/IO/Stream.h>
#include "PngFile.h"
#include <Lumino/Graphics/Bitmap.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Bitmap
//==============================================================================

//------------------------------------------------------------------------------
Bitmap::Bitmap()
	: m_bitmapData()
	, m_size()
	, m_pitch(0)
	, m_format(PixelFormat::Unknown)
	, m_upFlow(false)
{
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(const Size& size, PixelFormat format, bool upFlow)
{
	Init();
	m_size = size;
	m_format = format;
	m_bitmapData = ByteBuffer(GetPixelFormatByteCount(format, size));
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
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
	m_bitmapData = pngFile.m_bitmapData;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(const TCHAR* filePath)
{
	Init();
	LN_THROW(filePath != NULL, ArgumentException);

	FileStreamPtr file = FileStream::Create(filePath, FileOpenMode::Read);
	PngFile pngFile;
	if (!pngFile.load(file, false)) {
		LN_THROW(0, InvalidFormatException);
	}
	m_size = pngFile.m_size;
	m_format = pngFile.m_format;
	m_bitmapData = pngFile.m_bitmapData;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(ByteBuffer buffer, const Size& size, PixelFormat format)
{
	Init();
	m_size = size;
	m_format = format;
	m_bitmapData = buffer;
}
Bitmap::Bitmap(const ByteBuffer& buffer, const Size& size, PixelFormat format, bool upFlow)
{
	Init();
	m_size = size;
	m_format = format;
	m_bitmapData = buffer;
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(void* buffer, const Size& size, PixelFormat format)
{
	Init();
	m_size = size;
	m_format = format;
	m_bitmapData.Attach(buffer, GetPixelFormatByteCount(m_format, m_size));
}

//------------------------------------------------------------------------------
Bitmap::~Bitmap()
{
}

//------------------------------------------------------------------------------
void Bitmap::Init()
{
	//m_bitmapData;
	m_size.Set(0, 0);
	m_pitch = 0;
	m_format = PixelFormat::Unknown;
	m_upFlow = false;
}

//------------------------------------------------------------------------------
void Bitmap::Clear(const Color& color)
{
	// 完全に透明にクリアする場合はバッファクリアでよい。
	if (color.r == 0x00 && color.g == 0x00 && color.b == 0x00 && color.a == 0x00)
	{
		m_bitmapData.Clear();
	}
	else
	{
		switch (m_format)
		{
			case PixelFormat::A1:
				return;
			case PixelFormat::A8:
				return;
			case PixelFormat::R8G8B8A8:
			case PixelFormat::R8G8B8X8:
			{
				byte_t c[4] = { color.r, color.g, color.b, color.a };
				uint32_t* dst = (uint32_t*)m_bitmapData.GetData();
				int count = m_bitmapData.GetSize() / 4;
				for (int i = 0; i < count; ++i)
				{
					dst[i] = *((uint32_t*)c);
				}
				return;
			}
			case PixelFormat::B8G8R8A8:
			case PixelFormat::B8G8R8X8:
				byte_t c[4] = { color.b, color.g, color.r, color.a };
				uint32_t* dst = (uint32_t*)m_bitmapData.GetData();
				int count = m_bitmapData.GetSize() / 4;
				for (int i = 0; i < count; ++i)
				{
					dst[i] = *((uint32_t*)c);
				}
				return;
		}

		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
void Bitmap::BitBlt(const Rect& destRect, const Bitmap* srcBitmap, const Rect& srcRect, const Color& mulColor, bool alphaBlend)
{
	BitBltInternal(this, destRect, srcBitmap, srcRect, RGBA(mulColor.r, mulColor.g, mulColor.b, mulColor.a), alphaBlend);
}

//------------------------------------------------------------------------------
void Bitmap::BitBlt(int x, int y, const Bitmap* srcBitmap, const Rect& srcRect, const Color& mulColor, bool alphaBlend)
{
	BitBltInternal(this, Rect(x, y, INT_MAX, INT_MAX), srcBitmap, srcRect, RGBA(mulColor.r, mulColor.g, mulColor.b, mulColor.a), alphaBlend);
}

//------------------------------------------------------------------------------
void Bitmap::Save(const TCHAR* filePath)
{
	// png に保存するときは RGBA
	Bitmap bitmap(m_size, PixelFormat::R8G8B8A8);
	bitmap.m_upFlow = m_upFlow;
	ConvertPixelFormat(
		m_bitmapData.GetData(), m_bitmapData.GetSize(), m_format,
		bitmap.m_bitmapData.GetData(), bitmap.m_bitmapData.GetSize(), bitmap.m_format);

	// アルファ無しフォーマットであれば、アルファを埋めてから出力する
	if (m_format == PixelFormat::B8G8R8X8) {
		bitmap.FillAlpha(0xFF);
	}

	PngFile pngFile;
	pngFile.Save(filePath, bitmap.m_bitmapData, bitmap.m_size, bitmap.m_upFlow);
}

//------------------------------------------------------------------------------
bool Bitmap::Equals(const Bitmap* bitmap) const
{
	if (m_size != bitmap->m_size ||
		m_format != bitmap->m_format ||
		m_bitmapData.GetSize() != bitmap->m_bitmapData.GetSize()) {
		return false;
	}

	return memcmp(m_bitmapData.GetData(), bitmap->m_bitmapData.GetData(), m_bitmapData.GetSize()) == 0;
}

//------------------------------------------------------------------------------
void Bitmap::ConvertToDownFlow()
{
	int pixelSize = GetPixelFormatByteCount(m_format);
	if (pixelSize == 1)
	{
		// XOR で工夫すると演算回数が少なくなるとか最適化の余地はあるけど、
		// とりあえず今は評価目的でしか使わないので愚直に swap。
		byte_t* pixels = m_bitmapData.GetData();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_bitmapData.GetData();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//------------------------------------------------------------------------------
void Bitmap::CopyRawData(const void* data, size_t byteCount)
{
	LN_CHECK_ARG(m_bitmapData.GetSize() <= byteCount);
	m_bitmapData.Copy(data, byteCount);
}

//------------------------------------------------------------------------------
size_t Bitmap::GetByteCount() const
{
	return m_bitmapData.GetSize();
}

//------------------------------------------------------------------------------
void Bitmap::SetPixel(int x, int y, const Color& color)
{
	LN_CHECK_ARG(0 <= x && x < m_size.width);
	LN_CHECK_ARG(0 <= y && y < m_size.height);
	LN_CHECK_STATE(
		m_format == PixelFormat::B8G8R8A8 ||
		m_format == PixelFormat::B8G8R8X8 ||
		m_format == PixelFormat::R8G8B8A8 ||
		m_format == PixelFormat::R8G8B8X8);

	struct U32
	{
		byte_t	D[4];
	};

	if (m_upFlow)
	{
		y = m_size.height - 1 - y;
	}

	U32* buf = &((U32*)m_bitmapData.GetConstData())[y * m_size.width + x];
	if (m_format == PixelFormat::B8G8R8A8 || m_format == PixelFormat::B8G8R8X8)
	{
		buf->D[2] = color.r;
		buf->D[1] = color.g;
		buf->D[0] = color.b;
		buf->D[3] = color.a;
	}
	else if (m_format == PixelFormat::R8G8B8A8 || m_format == PixelFormat::R8G8B8X8)
	{
		buf->D[0] = color.r;
		buf->D[1] = color.g;
		buf->D[2] = color.b;
		buf->D[3] = color.a;
	}
}

//------------------------------------------------------------------------------
size_t Bitmap::GetSerializeSize() const
{
	return
		sizeof(size_t) + // m_bitmapData.GetSize()
		m_bitmapData.GetSize() +
		sizeof(m_size) +
		sizeof(m_pitch) +
		sizeof(m_format) +
		sizeof(m_upFlow);
}

//------------------------------------------------------------------------------
void Bitmap::Serialize(void* buffer)
{
	byte_t* b = (byte_t*)buffer;

	*((size_t*)b) = m_bitmapData.GetSize();
	b += sizeof(size_t);

	memcpy(b, m_bitmapData.GetConstData(), m_bitmapData.GetSize());
	b += m_bitmapData.GetSize();

	*((Size*)b) = m_size;
	b += sizeof(m_size);

	*((int*)b) = m_pitch;
	b += sizeof(m_pitch);

	*((PixelFormat*)b) = m_format;
	b += sizeof(m_format);

	*((bool*)b) = m_upFlow;
	//b += sizeof(m_upFlow);
}

//------------------------------------------------------------------------------
void Bitmap::Deserialize(const void* buffer)
{
	const byte_t* b = (const byte_t*)buffer;

	size_t size = *((size_t*)b);
	b += sizeof(size_t);

	m_bitmapData.Resize(size);
	memcpy(m_bitmapData.GetData(), b, size);
	b += m_bitmapData.GetSize();

	m_size = *((Size*)b);
	b += sizeof(m_size);

	m_pitch = *((int*)b);
	b += sizeof(m_pitch);

	m_format = *((PixelFormat*)b);
	b += sizeof(m_format);

	m_upFlow = *((bool*)b);
	//b += sizeof(m_upFlow);
}

//------------------------------------------------------------------------------
int Bitmap::GetPixelFormatByteCount(PixelFormat format)
{
	const int table[] =
	{
		0,	// PixelFormat::Unknown = 0,
		1,	// PixelFormat::A1,
		1,	// PixelFormat::A8,
		4,	// PixelFormat::R8G8B8A8,
		4,	// PixelFormat::R8G8B8X8,
		4,	// PixelFormat::B8G8R8A8,
		4,	// PixelFormat::B8G8R8X8,
		16,	// PixelFormat::FloatR32G32B32A32,
	};
	assert(LN_ARRAY_SIZE_OF(table) == (int)PixelFormat::_Count);
	return table[(int)format];
}

//------------------------------------------------------------------------------
int Bitmap::GetPixelFormatByteCount(PixelFormat format, const Size& size)
{
	int c = GetPixelFormatByteCount(format);
	return c * size.width * size.height;
}

//------------------------------------------------------------------------------
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
		(inputFormat == PixelFormat::R8G8B8A8 && outputFormat == PixelFormat::B8G8R8A8) ||
		(inputFormat == PixelFormat::B8G8R8A8 && outputFormat == PixelFormat::R8G8B8A8) ||
		(inputFormat == PixelFormat::B8G8R8X8 && outputFormat == PixelFormat::R8G8B8A8))
	{
		const uint32_t* in = (const uint32_t*)input;
		uint32_t* out = (uint32_t*)output;
		size_t count = outputSize / sizeof(uint32_t);	// ピクセル数は outputSize を使用しなければ危ない。FontGlyphTextureCache 等ではメモリ確保を抑えるために input 側を多めに確保している。
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

//------------------------------------------------------------------------------
void Bitmap::FillAlpha(byte_t alpha)
{
	if (m_format == PixelFormat::R8G8B8A8 ||
		m_format == PixelFormat::B8G8R8A8 ||
		m_format == PixelFormat::B8G8R8X8)
	{
		byte_t* buf = m_bitmapData.GetData();
		size_t count = m_bitmapData.GetSize() / sizeof(uint32_t);
		for (size_t i = 0; i < count; ++i) {
			buf[i * 4 + 3] = alpha;
		}
		return;
	}
	LN_THROW(0, InvalidFormatException);
}

//------------------------------------------------------------------------------

//void Bitmap::ConvertPixelU32(U32* p, PixelFormat inputFormat, PixelFormat outputFormat)
//{
//	// 同じフォーマットなら何もしない
//	if (inputFormat == outputFormat) {
//		return;
//	}
//}

//------------------------------------------------------------------------------
template<class TDestConverter, class TSrcConverter>
void Bitmap::BitBltInternalTemplate(
	Bitmap* dest, const Rect& destRect,
	const Bitmap* src, const Rect& srcRect,
	ClColor mulColorRGBA, bool alphaBlend) throw()
{
	DestBuffer<TDestConverter> dstBuf(dest, destRect);
	SrcBuffer<TSrcConverter> srcBuf(src, srcRect);

	if (alphaBlend)
	{
		for (int y = 0; y < srcRect.height; ++y)
		{
			dstBuf.SetLine(y);
			srcBuf.SetLine(y);
			for (int x = 0; x < srcRect.width; ++x)
			{
				ClColor src = srcBuf.GetPixel(x);
				ClColor src_alpha = GetA(src);
				if (src_alpha == 0) continue;     // フォントならコレでかなり高速化できるはず

				uint32_t dest_color = dstBuf.GetPixel(x);
				uint32_t dest_alpha = GetA(dest_color);
				uint32_t a, r, g, b;

				// photoshop 等のツール系の計算式ではやや時間がかかるため、
				// DirectX 同様、dest のアルファは無視する方向で実装する。
				// ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
				// 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
				// dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
				// 流石に完全に透明なのに黒ずむのはどうかと…。
				// というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
				if (dest_alpha == 0) a = 0xff;
				else a = src_alpha;

				r = (GetR(mulColorRGBA) * GetR(src)) >> 8;
				g = (GetG(mulColorRGBA) * GetG(src)) >> 8;
				b = (GetB(mulColorRGBA) * GetB(src)) >> 8;
				a = (GetA(mulColorRGBA) * a) >> 8;

				r = ((GetR(dest_color) * (0xff - a)) >> 8) +
					((r * a) >> 8);

				g = ((GetG(dest_color) * (0xff - a)) >> 8) +
					((g * a) >> 8);

				b = ((GetB(dest_color) * (0xff - a)) >> 8) +
					((b * a) >> 8);

				// 書き込み用に再計算。
				// 乗算だと、半透明を重ねるごとに薄くなってしまう。
				// イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
				// TODO: 本来はブレンドファンクションで表現するべきか…
				a = (dest_alpha + a);
				a = (a > 255) ? 255 : a;

				dstBuf.SetPixel(x, RGBA(r, g, b, a));
			}
		}
	}
	else
	{
		for (int y = 0; y < srcRect.height; ++y)
		{
			dstBuf.SetLine(y);
			srcBuf.SetLine(y);
			for (int x = 0; x < srcRect.width; ++x)
			{
				ClColor src = srcBuf.GetPixel(x);
				ClColor c = RGBA(
					(GetR(mulColorRGBA) * GetR(src)) >> 8,
					(GetG(mulColorRGBA) * GetG(src)) >> 8,
					(GetB(mulColorRGBA) * GetB(src)) >> 8,
					(GetA(mulColorRGBA) * GetA(src)) >> 8);
				dstBuf.SetPixel(x, c);
			}
		}
	}
}

//------------------------------------------------------------------------------
template<class TDestConverter>
void Bitmap::BitBltInternalTemplateHelper(
	Bitmap* dest, const Rect& destRect,
	const Bitmap* src, const Rect& srcRect,
	ClColor mulColorRGBA, bool alphaBlend)
{
	switch (src->m_format)
	{
	case PixelFormat::A1:
		BitBltInternalTemplate<TDestConverter, ConverterA1>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::A8:
		BitBltInternalTemplate<TDestConverter, ConverterA8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8A8:
		BitBltInternalTemplate<TDestConverter, ConverterR8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8X8:
		BitBltInternalTemplate<TDestConverter, ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8A8:
		BitBltInternalTemplate<TDestConverter, ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8X8:
		BitBltInternalTemplate<TDestConverter, ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	//case PixelFormat_R32G32B32A32_Float:
	//	return;
	}
	LN_THROW(0, InvalidFormatException);
}

//------------------------------------------------------------------------------
void Bitmap::BitBltInternal(
	Bitmap* dest, const Rect& destRect_,
	const Bitmap* src, const Rect& srcRect_,
	ClColor mulColorRGBA, bool alphaBlend)
{
	// 双方の矩形を Bitmap からはみ出ないようにクリッピングし、範囲の大きさは dest に合わせる。
	// (拡縮はしない。srcRect が小さければ、余分な部分は何もしない)
	Rect destRect = destRect_;
	Rect srcRect = srcRect_;
	destRect.Clip(Rect(0, 0, dest->m_size));
	srcRect.Clip(Rect(0, 0, src->m_size));
	srcRect.width = std::min(srcRect.width, destRect.width);
	srcRect.height = std::min(srcRect.height, destRect.height);

	switch (dest->m_format)
	{
	case PixelFormat::A1:
		BitBltInternalTemplateHelper<ConverterA1>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::A8:
		BitBltInternalTemplateHelper<ConverterA8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8A8:
		BitBltInternalTemplateHelper<ConverterR8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8X8:
		BitBltInternalTemplateHelper<ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8A8:
		BitBltInternalTemplateHelper<ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8X8:
		BitBltInternalTemplateHelper<ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	//case PixelFormat_R32G32B32A32_Float:
	//	return;
	}

	LN_THROW(0, InvalidFormatException);

#if 0
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
	SrcBitmapWrapper srcWrapper(src, srcRect);

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
	else if (PixelFormat::R8G8B8A8 <= dest->m_format && dest->m_format <= PixelFormat::B8G8R8X8 &&
		PixelFormat::R8G8B8A8 <= src->m_format && src->m_format <= PixelFormat::B8G8R8X8)
	{
		bool destIsRGBALike = (dest->m_format == PixelFormat::R8G8B8A8 || dest->m_format == PixelFormat::B8G8R8X8);
		bool srcIsRGBALike = (src->m_format == PixelFormat::R8G8B8A8 || src->m_format == PixelFormat::B8G8R8X8);

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
#endif
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
