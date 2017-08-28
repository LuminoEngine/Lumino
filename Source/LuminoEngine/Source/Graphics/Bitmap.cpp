
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
	, m_depth(0)
	, m_pitch(0)
	, m_format(PixelFormat::Unknown)
	, m_upFlow(false)
{
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(const SizeI& size, PixelFormat format, bool upFlow)
{
	init();
	m_size = size;
	m_depth = 1;
	m_format = format;
	m_bitmapData = ByteBuffer(getPixelFormatByteCount(m_format, m_size, m_depth));
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(int width, int height, int depth, PixelFormat format, bool upFlow)
{
	init();
	m_size.set(width, height);
	m_depth = depth;
	m_format = format;
	m_bitmapData = ByteBuffer(getPixelFormatByteCount(m_format, m_size, m_depth));
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(Stream* stream, bool flipV)
{
	init();
	LN_THROW(stream != NULL, ArgumentException);

	PngFile pngFile;
	if (!pngFile.load(stream, flipV)) {
		LN_THROW(0, InvalidFormatException);
	}
	m_size = pngFile.m_size;
	m_depth = 1;
	m_format = pngFile.m_format;
	m_bitmapData = pngFile.m_bitmapData;
	m_upFlow = flipV;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(const TCHAR* filePath)
{
	init();
	LN_THROW(filePath != NULL, ArgumentException);

	Ref<FileStream> file = FileStream::create(filePath, FileOpenMode::read);
	PngFile pngFile;
	if (!pngFile.load(file, false)) {
		LN_THROW(0, InvalidFormatException);
	}
	m_size = pngFile.m_size;
	m_depth = 1;
	m_format = pngFile.m_format;
	m_bitmapData = pngFile.m_bitmapData;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(ByteBuffer buffer, const SizeI& size, PixelFormat format)
{
	init();
	m_size = size;
	m_depth = 1;
	m_format = format;
	m_bitmapData = buffer;
}
Bitmap::Bitmap(const ByteBuffer& buffer, const SizeI& size, PixelFormat format, bool upFlow)
{
	init();
	m_size = size;
	m_depth = 1;
	m_format = format;
	m_bitmapData = buffer;
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(void* buffer, const SizeI& size, PixelFormat format, bool upFlow)
{
	init();
	m_size = size;
	m_depth = 1;
	m_format = format;
	m_bitmapData.attach(buffer, getPixelFormatByteCount(m_format, m_size, m_depth));
	m_upFlow = upFlow;
}

//------------------------------------------------------------------------------
Bitmap::Bitmap(void* buffer, int width, int height, int depth, PixelFormat format)
{
	init();
	m_size.set(width, height);
	m_depth = depth;
	m_format = format;
	m_bitmapData.attach(buffer, getPixelFormatByteCount(m_format, m_size, m_depth));
}

//------------------------------------------------------------------------------
Bitmap::~Bitmap()
{
}

//------------------------------------------------------------------------------
void Bitmap::init()
{
	//m_bitmapData;
	m_size.set(0, 0);
	m_pitch = 0;
	m_format = PixelFormat::Unknown;
	m_upFlow = false;
}

//------------------------------------------------------------------------------
void Bitmap::clear(const Color32& color)
{
	// 完全に透明にクリアする場合はバッファクリアでよい。
	if (color.r == 0x00 && color.g == 0x00 && color.b == 0x00 && color.a == 0x00)
	{
		m_bitmapData.clear();
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
				uint32_t* dst = (uint32_t*)m_bitmapData.getData();
				int count = m_bitmapData.getSize() / 4;
				for (int i = 0; i < count; ++i)
				{
					dst[i] = *((uint32_t*)c);
				}
				return;
			}
			case PixelFormat::B8G8R8A8:
			case PixelFormat::B8G8R8X8:
				byte_t c[4] = { color.b, color.g, color.r, color.a };
				uint32_t* dst = (uint32_t*)m_bitmapData.getData();
				int count = m_bitmapData.getSize() / 4;
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
void Bitmap::bitBlt(const RectI& destRect, const Bitmap* srcBitmap, const RectI& srcRect, const Color32& mulColor, bool alphaBlend)
{
	bitBltInternal(this, destRect, srcBitmap, srcRect, RGBA(mulColor.r, mulColor.g, mulColor.b, mulColor.a), alphaBlend);
}

//------------------------------------------------------------------------------
void Bitmap::bitBlt(int x, int y, const Bitmap* srcBitmap, const RectI& srcRect, const Color32& mulColor, bool alphaBlend)
{
	bitBltInternal(this, RectI(x, y, INT_MAX, INT_MAX), srcBitmap, srcRect, RGBA(mulColor.r, mulColor.g, mulColor.b, mulColor.a), alphaBlend);
}

//------------------------------------------------------------------------------
void Bitmap::save(const TCHAR* filePath)
{
	// png に保存するときは RGBA
	Bitmap bitmap(m_size, PixelFormat::R8G8B8A8);
	bitmap.m_upFlow = m_upFlow;
	convertPixelFormat(
		m_bitmapData.getData(), m_bitmapData.getSize(), m_format,
		bitmap.m_bitmapData.getData(), bitmap.m_bitmapData.getSize(), bitmap.m_format);

	// アルファ無しフォーマットであれば、アルファを埋めてから出力する
	if (m_format == PixelFormat::B8G8R8X8) {
		bitmap.fillAlpha(0xFF);
	}

	PngFile pngFile;
	pngFile.save(filePath, bitmap.m_bitmapData, bitmap.m_size, bitmap.m_upFlow);
}

//------------------------------------------------------------------------------
bool Bitmap::equals(const Bitmap* bitmap) const
{
	if (m_size != bitmap->m_size ||
		m_format != bitmap->m_format ||
		m_bitmapData.getSize() != bitmap->m_bitmapData.getSize()) {
		return false;
	}

	return memcmp(m_bitmapData.getData(), bitmap->m_bitmapData.getData(), m_bitmapData.getSize()) == 0;
}

//------------------------------------------------------------------------------
void Bitmap::convertToDownFlow()
{
	int pixelSize = getPixelFormatByteCount(m_format);
	if (pixelSize == 1)
	{
		// XOR で工夫すると演算回数が少なくなるとか最適化の余地はあるけど、
		// とりあえず今は評価目的でしか使わないので愚直に swap。
		byte_t* pixels = m_bitmapData.getData();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_bitmapData.getData();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else {
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
void Bitmap::copyRawData(const void* data, size_t byteCount)
{
	if (LN_CHECK_ARG(m_bitmapData.getSize() <= byteCount)) return;
	m_bitmapData.copy(data, byteCount);
}

//------------------------------------------------------------------------------
size_t Bitmap::getByteCount() const
{
	return m_bitmapData.getSize();
}

//------------------------------------------------------------------------------
void Bitmap::setPixel(int x, int y, int z, const Color32& color)
{
	if (LN_CHECK_RANGE(x, 0, m_size.width)) return;
	if (LN_CHECK_RANGE(y, 0, m_size.height)) return;
	if (LN_CHECK_RANGE(z, 0, m_depth)) return;
	if (LN_CHECK_STATE(
		m_format == PixelFormat::B8G8R8A8 ||
		m_format == PixelFormat::B8G8R8X8 ||
		m_format == PixelFormat::R8G8B8A8 ||
		m_format == PixelFormat::R8G8B8X8)) return;

	struct U32
	{
		byte_t	D[4];
	};

	if (m_upFlow)
	{
		y = m_size.height - 1 - y;
	}

	U32* buf = &((U32*)m_bitmapData.getConstData())[z * (m_size.width * m_size.height) + y * m_size.width + x];
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
Color32 Bitmap::getPixel(int x, int y) const
{
	struct U32
	{
		byte_t	D[4];
	};

	if (m_upFlow) {
		y = m_size.height - 1 - y;
	}

	const U32* buf = &((const U32*)m_bitmapData.getConstData())[y * m_size.width + x];
	if (m_format == PixelFormat::B8G8R8A8 || m_format == PixelFormat::B8G8R8X8)
	{
		return Color32(buf->D[2], buf->D[1], buf->D[0], buf->D[3]);
	}
	else
	{
		return *((const Color32*)buf);
	}
}

//------------------------------------------------------------------------------
size_t Bitmap::getSerializeSize() const
{
	//return
	//	sizeof(size_t) +
	//	m_bitmapData.GetSize() +
	//	sizeof(m_size) +
	//	sizeof(m_pitch) +
	//	sizeof(m_format) +
	//	sizeof(m_upFlow);
	return getSerializeSize(RectI(0, 0, m_size));
}

//------------------------------------------------------------------------------
size_t Bitmap::getSerializeSize(const RectI& rect) const
{
	RectI clipRect = rect;
	clipRect.clip(RectI(0, 0, m_size));
	
	return
		getPropertySerializeSize() +
		sizeof(size_t) +
		getPixelFormatByteCount(m_format, clipRect.getSize(), m_depth);
}

//------------------------------------------------------------------------------
size_t Bitmap::getPropertySerializeSize() const
{
	return
		sizeof(m_size) +
		sizeof(m_depth) +
		sizeof(m_pitch) +
		sizeof(m_format) +
		sizeof(m_upFlow);
}

//------------------------------------------------------------------------------
void Bitmap::serialize(void* buffer)
{
	//byte_t* b = (byte_t*)buffer;

	//*((size_t*)b) = m_bitmapData.GetSize();
	//b += sizeof(size_t);

	//memcpy(b, m_bitmapData.GetConstData(), m_bitmapData.GetSize());
	//b += m_bitmapData.GetSize();

	//*((SizeI*)b) = m_size;
	//b += sizeof(m_size);

	//*((int*)b) = m_pitch;
	//b += sizeof(m_pitch);

	//*((PixelFormat*)b) = m_format;
	//b += sizeof(m_format);

	//*((bool*)b) = m_upFlow;
	////b += sizeof(m_upFlow);
	serialize(buffer, RectI(0, 0, m_size));
}

//------------------------------------------------------------------------------
void Bitmap::serialize(void* buffer, const RectI& rect)
{
	RectI clipRect = rect;
	clipRect.clip(RectI(0, 0, m_size));
	
	byte_t* b = (byte_t*)buffer;

	*((SizeI*)b) = m_size;
	b += sizeof(m_size);

	*((int*)b) = m_depth;
	b += sizeof(m_depth);

	*((int*)b) = m_pitch;
	b += sizeof(m_pitch);

	*((PixelFormat*)b) = m_format;
	b += sizeof(m_format);

	*((bool*)b) = m_upFlow;
	b += sizeof(m_upFlow);
	
	*((size_t*)b) = getPixelFormatByteCount(m_format, clipRect.getSize(), m_depth);
	b += sizeof(size_t);
	
	size_t pixelSize = getPixelFormatByteCount(m_format);
	size_t srcLineSize = pixelSize * m_size.width;
	size_t dstLineSize = pixelSize * clipRect.width;
	for (int y = clipRect.getTop(); y < clipRect.getBottom(); ++y)
	{
		const byte_t* srcLine = &(m_bitmapData.getConstData()[srcLineSize * y]);
		byte_t* dstLine = &(b[dstLineSize * y]);
		memcpy(dstLine, &srcLine[pixelSize * clipRect.x], pixelSize * clipRect.width);
	}
}

//------------------------------------------------------------------------------
void Bitmap::serializeProperty(void* buffer)
{
	byte_t* b = (byte_t*)buffer;

	*((SizeI*)b) = m_size;
	b += sizeof(m_size);

	*((int*)b) = m_depth;
	b += sizeof(m_depth);

	*((int*)b) = m_pitch;
	b += sizeof(m_pitch);

	*((PixelFormat*)b) = m_format;
	b += sizeof(m_format);

	*((bool*)b) = m_upFlow;
	b += sizeof(m_upFlow);
}

//------------------------------------------------------------------------------
void Bitmap::deserialize(void* buffer, bool refMode)
{
	byte_t* b = (byte_t*)buffer;

	m_size = *((SizeI*)b);
	b += sizeof(m_size);

	*((int*)b) = m_depth;
	b += sizeof(m_depth);

	m_pitch = *((int*)b);
	b += sizeof(m_pitch);

	m_format = *((PixelFormat*)b);
	b += sizeof(m_format);

	m_upFlow = *((bool*)b);
	b += sizeof(m_upFlow);

	size_t size = *((size_t*)b);
	b += sizeof(size_t);

	if (refMode)
	{
		m_bitmapData.attach(b, size);
	}
	else
	{
		m_bitmapData.resize(size);
		memcpy(m_bitmapData.getData(), b, size);
	}
	b += m_bitmapData.getSize();
}

//------------------------------------------------------------------------------
void Bitmap::deserializePropertyAndRawData(const void* propData, void* rawData, size_t rawDataSize, bool refMode)
{
	const byte_t* b = (const byte_t*)propData;

	m_size = *((SizeI*)b);
	b += sizeof(m_size);

	m_pitch = *((int*)b);
	b += sizeof(m_pitch);

	m_format = *((PixelFormat*)b);
	b += sizeof(m_format);

	m_upFlow = *((bool*)b);
	b += sizeof(m_upFlow);

	if (refMode)
	{
		m_bitmapData.attach(rawData, rawDataSize);
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
int Bitmap::getPixelFormatByteCount(PixelFormat format)
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
int Bitmap::getPixelFormatByteCount(PixelFormat format, const SizeI& size, int depth)
{
	int c = getPixelFormatByteCount(format);
	return c * size.width * size.height * depth;
}

//------------------------------------------------------------------------------
void Bitmap::convertPixelFormat(
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
void Bitmap::fillAlpha(byte_t alpha)
{
	if (m_format == PixelFormat::R8G8B8A8 ||
		m_format == PixelFormat::B8G8R8A8 ||
		m_format == PixelFormat::B8G8R8X8)
	{
		byte_t* buf = m_bitmapData.getData();
		size_t count = m_bitmapData.getSize() / sizeof(uint32_t);
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
void Bitmap::bitBltInternalTemplate(
	Bitmap* dest, const RectI& destRect,
	const Bitmap* src, const RectI& srcRect,
	ClColor mulColorRGBA, bool alphaBlend) throw()
{
	DestBuffer<TDestConverter> dstBuf(dest, destRect);
	SrcBuffer<TSrcConverter> srcBuf(src, srcRect);

	if (alphaBlend)
	{
		for (int y = 0; y < srcRect.height; ++y)
		{
			dstBuf.setLine(y);
			srcBuf.setLine(y);
			for (int x = 0; x < srcRect.width; ++x)
			{
				ClColor src = srcBuf.getPixel(x);
				ClColor src_alpha = getA(src);
				if (src_alpha == 0) continue;     // フォントならコレでかなり高速化できるはず


				uint32_t dest_color = dstBuf.getPixel(x);
				uint32_t dest_alpha = getA(dest_color);
				uint32_t a, r, g, b;

				a = src_alpha;

				// まず、src と mul をまぜまぜ
				r = (getR(mulColorRGBA) * getR(src)) >> 8;
				g = (getG(mulColorRGBA) * getG(src)) >> 8;
				b = (getB(mulColorRGBA) * getB(src)) >> 8;
				a = (getA(mulColorRGBA) * src_alpha) >> 8;

				// photoshop 等のツール系の計算式ではやや時間がかかるため、
				// DirectX 同様、dest のアルファは無視する方向で実装する。
				// ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
				// 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
				// dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
				// 流石に完全に透明なのに黒ずむのはどうかと…。
				// というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
				if (dest_alpha != 0)
				{
					r = ((getR(dest_color) * (0xff - a)) >> 8) +
						((r * a) >> 8);

					g = ((getG(dest_color) * (0xff - a)) >> 8) +
						((g * a) >> 8);

					b = ((getB(dest_color) * (0xff - a)) >> 8) +
						((b * a) >> 8);
				}

				// 書き込み用に再計算。
				// 乗算だと、半透明を重ねるごとに薄くなってしまう。
				// イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
				// TODO: 本来はブレンドファンクションで表現するべきか…
				a = (dest_alpha + a);
				a = (a > 255) ? 255 : a;

				dstBuf.setPixel(x, RGBA(r, g, b, a));
			}
		}
	}
	else
	{
		for (int y = 0; y < srcRect.height; ++y)
		{
			dstBuf.setLine(y);
			srcBuf.setLine(y);
			for (int x = 0; x < srcRect.width; ++x)
			{
				ClColor src = srcBuf.getPixel(x);
				ClColor c = RGBA(
					(getR(mulColorRGBA) * getR(src)) >> 8,
					(getG(mulColorRGBA) * getG(src)) >> 8,
					(getB(mulColorRGBA) * getB(src)) >> 8,
					(getA(mulColorRGBA) * getA(src)) >> 8);
				dstBuf.setPixel(x, c);
			}
		}
	}
}

//------------------------------------------------------------------------------
template<class TDestConverter>
void Bitmap::bitBltInternalTemplateHelper(
	Bitmap* dest, const RectI& destRect,
	const Bitmap* src, const RectI& srcRect,
	ClColor mulColorRGBA, bool alphaBlend)
{
	switch (src->m_format)
	{
	case PixelFormat::A1:
		bitBltInternalTemplate<TDestConverter, ConverterA1>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::A8:
		bitBltInternalTemplate<TDestConverter, ConverterA8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8A8:
		bitBltInternalTemplate<TDestConverter, ConverterR8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8X8:
		bitBltInternalTemplate<TDestConverter, ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8A8:
		bitBltInternalTemplate<TDestConverter, ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8X8:
		bitBltInternalTemplate<TDestConverter, ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	//case PixelFormat_R32G32B32A32_Float:
	//	return;
	}
	LN_THROW(0, InvalidFormatException);
}

//------------------------------------------------------------------------------
void Bitmap::bitBltInternal(
	Bitmap* dest, const RectI& destRect_,
	const Bitmap* src, const RectI& srcRect_,
	ClColor mulColorRGBA, bool alphaBlend)
{
	// 双方の矩形を Bitmap からはみ出ないようにクリッピングし、範囲の大きさは dest に合わせる。
	// (拡縮はしない。srcRect が小さければ、余分な部分は何もしない)
	RectI destRect = destRect_;
	RectI srcRect = srcRect_;
	destRect.clip(RectI(0, 0, dest->m_size));
	srcRect.clip(RectI(0, 0, src->m_size));
	srcRect.width = std::min(srcRect.width, destRect.width);
	srcRect.height = std::min(srcRect.height, destRect.height);

	switch (dest->m_format)
	{
	case PixelFormat::A1:
		bitBltInternalTemplateHelper<ConverterA1>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::A8:
		bitBltInternalTemplateHelper<ConverterA8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8A8:
		bitBltInternalTemplateHelper<ConverterR8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::R8G8B8X8:
		bitBltInternalTemplateHelper<ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8A8:
		bitBltInternalTemplateHelper<ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	case PixelFormat::B8G8R8X8:
		bitBltInternalTemplateHelper<ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
		return;
	//case PixelFormat_R32G32B32A32_Float:
	//	return;
	}

	LN_THROW(0, InvalidFormatException);

#if 0
	// 双方の矩形を Bitmap からはみ出ないようにクリッピングし、範囲の大きさは dest に合わせる。
	// (拡縮はしない。srcRect が小さければ、余分な部分は何もしない)
	RectI destRect = destRect_;
	RectI srcRect = srcRect_;
	destRect.Clip(RectI(0, 0, dest->m_size));
	srcRect.Clip(RectI(0, 0, src->m_size));
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
