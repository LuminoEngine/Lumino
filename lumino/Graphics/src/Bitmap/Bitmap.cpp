#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoGraphics/Bitmap/Bitmap.hpp>
#include "BitmapEncoding.hpp"

namespace ln {
namespace detail {



//==============================================================================
// BlitHelper

template<class TDestConverter, class TSrcConverter>
void BlitHelper::bitBltInternalTemplate(
    Bitmap2D* dest, const RectI& destRect,
    const Bitmap2D* src, const RectI& srcRect,
    ClColor mulColorRGBA, bool alphaBlend) throw()
{
    SrcBuffer<TSrcConverter> srcBuf(src->data(), src->width(), false, srcRect, TSrcConverter(mulColorRGBA));
    DestBuffer<TDestConverter> dstBuf(dest->data(), dest->width(), false, destRect, TDestConverter(mulColorRGBA));
    bool mulColor = mulColorRGBA.r != 255 || mulColorRGBA.g != 255 || mulColorRGBA.b != 255 || mulColorRGBA.a != 255;

    if (alphaBlend)
    {
        for (int y = 0; y < srcRect.height; ++y)
        {
            dstBuf.setLine(y);
            srcBuf.setLine(y);
            for (int x = 0; x < srcRect.width; ++x)
            {
                ClColor src = srcBuf.getPixel(x);
                if (src.a == 0) continue;

                ClColor dst = dstBuf.getPixel(x);

                uint8_t a, r, g, b;
                // TODO: 速度調査

                if (mulColor)
                {
                    // mulColor と src のブレンド
                    {
                        int src_a = src.a;
                        int dst_r = mulColorRGBA.r * mulColorRGBA.a / 255;
                        int dst_g = mulColorRGBA.g * mulColorRGBA.a / 255;
                        int dst_b = mulColorRGBA.b * mulColorRGBA.a / 255;
                        r = (dst_r * (255 - src_a) / 255) + (src.r * src_a / 255);
                        g = (dst_g * (255 - src_a) / 255) + (src.g * src_a / 255);
                        b = (dst_b * (255 - src_a) / 255) + (src.b * src_a / 255);
                        a = (mulColorRGBA.a * src.a) / 255;//std::min(mulColorRGBA.a + src.a, 255);
                    }

                    // dst と ↑のブレンド
                    {
                        int dst_r = dst.r * dst.a / 255;
                        int dst_g = dst.g * dst.a / 255;
                        int dst_b = dst.b * dst.a / 255;
                        r = (dst_r * (255 - a) / 255) + (r * a / 255);
                        g = (dst_g * (255 - a) / 255) + (g * a / 255);
                        b = (dst_b * (255 - a) / 255) + (b * a / 255);
                        a = std::min(dst.a + a, 255);
                    }
                }
                else
                {
                    if (dst.a == 0)
                    {
                        r = src.r;
                        g = src.g;
                        b = src.b;
                        a = src.a;
                    }
                    else
                    {
                        int src_a = src.a;
                        int dst_r = dst.r * dst.a / 255;
                        int dst_g = dst.g * dst.a / 255;
                        int dst_b = dst.b * dst.a / 255;

                        r = (dst_r * (255 - src_a) / 255) + (src.r * src_a / 255);
                        g = (dst_g * (255 - src_a) / 255) + (src.g * src_a / 255);
                        b = (dst_b * (255 - src_a) / 255) + (src.b * src_a / 255);
                        a = std::min(dst.a + src.a, 255);
                    }
                }

                dstBuf.setPixel(x, ClColor{ r, g, b, a });

#if 0
                if (src_alpha == 0) continue;     // フォントならコレでかなり高速化できるはず
                uint8_t src_alpha = src.a;
                uint8_t dest_alpha = dest_color.a;
                uint8_t a, r, g, b;
                a = src_alpha;

                // まず、src と mul をまぜまぜ
                r = (mulColorRGBA.r * src.r) >> 8;
                g = (mulColorRGBA.g * src.g) >> 8;
                b = (mulColorRGBA.b * src.b) >> 8;
                a = (mulColorRGBA.a * src_alpha) >> 8;

                // photoshop 等のツール系の計算式ではやや時間がかかるため、
                // DirectX 同様、dest のアルファは無視する方向で実装する。
                // ただし、このままだと dest(0, 0, 0, 0) に半透明の色を合成する場合、
                // 黒ずみが発生してしまう。テクスチャのデフォルトはこの状態。
                // dest(1, 0, 0, 0) とかなら、ユーザーが黒と合成されることを意図していると考えられるが、
                // 流石に完全に透明なのに黒ずむのはどうかと…。
                // というわけで、dest_alpha == 0 なら src が 100% になるように細工している。
                if (dest_alpha != 0)
                {
                    r = ((dest_color.r * (0xff - a)) >> 8) +
                        ((r * a) >> 8);

                    g = ((dest_color.g * (0xff - a)) >> 8) +
                        ((g * a) >> 8);

                    b = ((dest_color.b * (0xff - a)) >> 8) +
                        ((b * a) >> 8);
                }

                // 書き込み用に再計算。
                // 乗算だと、半透明を重ねるごとに薄くなってしまう。
                // イメージとしては、重ねるごとに濃くなる加算が適切だと思う。
                // TODO: 本来はブレンドファンクションで表現するべきか…
                a = (dest_alpha + a);
                a = (a > 255) ? 255 : a;

                dstBuf.setPixel(x, ClColor{ r, g, b, a });
#endif

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
                // TODO: 結果が 1減る
                ClColor src = srcBuf.getPixel(x);
                ClColor c = {
                    static_cast<uint8_t>((mulColorRGBA.r * src.r) >> 8),
                    static_cast<uint8_t>((mulColorRGBA.g * src.g) >> 8),
                    static_cast<uint8_t>((mulColorRGBA.b * src.b) >> 8),
                    static_cast<uint8_t>((mulColorRGBA.a * src.a) >> 8) };
                dstBuf.setPixel(x, c);
            }
        }
    }
}

template<class TDestConverter>
void BlitHelper::bitBltInternalTemplateHelper(
    Bitmap2D* dest, const RectI& destRect,
    const Bitmap2D* src, const RectI& srcRect,
    ClColor mulColorRGBA, bool alphaBlend)
{
    switch (src->m_format)
    {
    case PixelFormat::A8:
        bitBltInternalTemplate<TDestConverter, PixelAccessor_A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
        break;
    case PixelFormat::RGBA8:
        bitBltInternalTemplate<TDestConverter, PixelAccessor_R8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
        break;
    //case PixelFormat::R8G8B8X8:
    //    bitBltInternalTemplate<TDestConverter, ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
    //case PixelFormat::B8G8R8A8:
    //    bitBltInternalTemplate<TDestConverter, ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
    //case PixelFormat::B8G8R8X8:
    //    bitBltInternalTemplate<TDestConverter, ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
        //case PixelFormat_R32G32B32A32_Float:
        //	break;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }
}

void BlitHelper::bitBltInternal(
    Bitmap2D* dest, const RectI& destRect_,
    const Bitmap2D* src, const RectI& srcRect_,
    ClColor mulColorRGBA, bool alphaBlend)
{
    // 双方の矩形を RawBitmap からはみ出ないようにクリッピングし、範囲の大きさは dest に合わせる。
    // (拡縮はしない。srcRect が小さければ、余分な部分は何もしない)
    RectI destRect = destRect_;
    RectI srcRect = srcRect_;
    destRect.clip(RectI(0, 0, dest->m_size));
    srcRect.clip(RectI(0, 0, src->m_size));
    srcRect.width = std::min(srcRect.width, destRect.width);
    srcRect.height = std::min(srcRect.height, destRect.height);

    switch (dest->m_format)
    {
    case PixelFormat::A8:
        bitBltInternalTemplateHelper<PixelAccessor_A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
        break;
    case PixelFormat::RGBA8:
        bitBltInternalTemplateHelper<PixelAccessor_R8G8B8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
        break;
    //case PixelFormat::R8G8B8X8:
    //    bitBltInternalTemplateHelper<ConverterR8G8B8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
    //case PixelFormat::B8G8R8A8:
    //    bitBltInternalTemplateHelper<ConverterB8G8R8A8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
    //case PixelFormat::B8G8R8X8:
    //    bitBltInternalTemplateHelper<ConverterB8G8R8X8>(dest, destRect, src, srcRect, mulColorRGBA, alphaBlend);
    //    break;
        //case PixelFormat_R32G32B32A32_Float:
        //	break;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }
}

size_t BlitHelper::getPixelSize(PixelFormat format)
{
    switch (format)
    {
    case PixelFormat::Unknown:
        return 0;
    case PixelFormat::A8:
        return 1;
    case PixelFormat::RGBA8:
        return 4;
    case PixelFormat::RGB8:
        return 3;
    case PixelFormat::RGBA32F:
        return 16;
    case PixelFormat::R32S:
        return 4;
    default:
        LN_UNREACHABLE();
        return 0;
    }
}



} // namespace detail


//==============================================================================
// Bitmap2D

  /*
  TextureFormat で処理を分岐するのは switch と関数ポインタどちらがいい？
  100000 ループした結果

  switch(20個) : FuncPtr
  323019 : 623231 [ns]
  331858 : 647465
  320168 : 606980
  490659 : 947391
  386312 : 628933
  317032 : 609831
  399997 : 705340
  318173 : 606695
  551101 : 608406
  320168 : 610972

  switch の方が数が増えるほど遅くなるが20個あれば十分。
  */

Bitmap2D::Bitmap2D()
{
	m_buffer = makeRef<ByteBuffer>();
}

Bitmap2D::~Bitmap2D()
{
}

void Bitmap2D::init()
{
	m_size.width = 0;
	m_size.height = 0;
	m_format = PixelFormat::Unknown;
}

void Bitmap2D::init(int width, int height, PixelFormat format, const void* data)
{
	m_format = format;
	resize(width, height);
    if (data) {
        memcpy(m_buffer->data(), data, m_buffer->size());
    }
}

ColorI Bitmap2D::getPixel32(int x, int y) const
{
	if (m_format == PixelFormat::RGBA8)
	{
		const uint8_t* pixel = m_buffer->data() + ((y * m_size.width) + x) * 4;
		return ColorI(pixel[0], pixel[1], pixel[2], pixel[3]);
	}
    else if (m_format == PixelFormat::RGB8)
    {
        const uint8_t* pixel = m_buffer->data() + ((y * m_size.width) + x) * 3;
        return ColorI(pixel[0], pixel[1], pixel[2], 0xFF);
    }
	else
	{
		LN_NOTIMPLEMENTED();
		return ColorI();
	}
}

void Bitmap2D::setPixel32(int x, int y, const ColorI& color)
{
	if (m_format == PixelFormat::RGBA8)
	{
		ColorI* pixel = reinterpret_cast<ColorI*>(m_buffer->data() + ((y * m_size.width) + x) * 4);
		*pixel = color;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void Bitmap2D::clear(const ColorI& color)
{
    // Clear the buffer if completely transparent.
    if (color.r == 0x00 && color.g == 0x00 && color.b == 0x00 && color.a == 0x00)
    {
        m_buffer->clear();
    }
    else
    {
        switch (m_format)
        {
        //case PixelFormat::A1:
        //    return;
        case PixelFormat::A8:
            return;
        case PixelFormat::RGBA8:
        //case PixelFormat::R8G8B8X8:
        {
            byte_t c[4] = { color.r, color.g, color.b, color.a };
            uint32_t* dst = (uint32_t*)m_buffer->data();
            int count = m_buffer->size() / 4;
            for (int i = 0; i < count; ++i)
            {
                dst[i] = *((uint32_t*)c);
            }
            return;
        }
        //case PixelFormat::B8G8R8A8:
        //case PixelFormat::B8G8R8X8:
        //    byte_t c[4] = { color.b, color.g, color.r, color.a };
        //    uint32_t* dst = (uint32_t*)m_bitmapData.getData();
        //    int count = m_bitmapData.getSize() / 4;
        //    for (int i = 0; i < count; ++i)
        //    {
        //        dst[i] = *((uint32_t*)c);
        //    }
        //    return;
        default:
            LN_NOTIMPLEMENTED();
            return;
        }

    }
}

void Bitmap2D::resize(int width, int height)
{
	m_size.width = width;
	m_size.height = height;
	m_buffer->resize(getBitmapByteSize(m_size.width, m_size.height, 1, m_format));
}

void Bitmap2D::flipVerticalFlow()
{
	if (LN_REQUIRE(m_format != PixelFormat::Unknown)) return;
	//if (LN_REQUIRE(m_format != PixelFormat::A1)) return;

	size_t pixelSize = detail::BlitHelper::getPixelSize(m_format);
	if (pixelSize == 1)
	{
		// XOR で工夫すると演算回数が少なくなるとか最適化の余地はあるけど、
		// とりあえず今は評価目的でしか使わないので愚直に swap。
		byte_t* pixels = m_buffer->data();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else if (pixelSize == 3)
	{
        byte_t* pixels = (byte_t*)m_buffer->data();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
                byte_t* h0 = pixels + ((y * m_size.width) + x) * 3;
                byte_t* h1 = pixels + (((m_size.height - 1 - y) * m_size.width) + x) * 3;
				std::swap(h0[0], h1[0]);
                std::swap(h0[1], h1[1]);
                std::swap(h0[2], h1[2]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_buffer->data();
		for (int y = 0; y < (m_size.height / 2); ++y) {
            int ry = (m_size.height - 1 - y);
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[(ry * m_size.width) + x]);
			}
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void Bitmap2D::load(const StringView& filePath)
{
	auto file = FileStream::create(filePath);
    load(file);
}

void Bitmap2D::load(Stream* stream)
{
    auto diag = makeObject_deprecated<DiagnosticsManager>();

    auto decoder = detail::IBitmapDecoder::load(stream, diag);

    diag->dumpToLog();
    if (diag->succeeded())
    {
        detail::BitmapFrame* frame = decoder->getBitmapFrame();
        m_buffer = frame->data;
        m_size = frame->size;
        m_format = frame->format;
    }
}

void Bitmap2D::save(const StringView& filePath)
{
	auto file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
    detail::IBitmapEncoder::save(file, m_buffer->data(), m_size, m_format);
}

Ref<Bitmap2D> Bitmap2D::clone() const
{
	return makeObject_deprecated<Bitmap2D>(m_size.width, m_size.height, m_format, m_buffer->data());
}

Ref<Bitmap2D> Bitmap2D::transcodeTo(PixelFormat format, const ColorI& color) const
{
	auto dstBitmap = makeObject_deprecated<Bitmap2D>(m_size.width, m_size.height, format);

	RectI rect(0, 0, m_size.width, m_size.height);
    detail::ClColor c{ color.r, color.g, color.b, color.a };
	detail::SrcBuffer<detail::PixelAccessor_A8> src(data(), width(), false, rect, detail::PixelAccessor_A8(c));
	detail::DestBuffer<detail::PixelAccessor_R8G8B8A8> dst(dstBitmap->data(), dstBitmap->width(), false, rect, detail::PixelAccessor_R8G8B8A8(c));

	for (int y = 0; y < rect.height; y++)
	{
		src.setLine(y);
		dst.setLine(y);
		for (int x = 0; x < rect.width; x++)
		{
			dst.setPixel(x, src.getPixel(x));
		}
	}

	//struct U32
	//{
	//	byte_t D[4];
	//};

	//if (m_format == PixelFormat::A8 && format == PixelFormat::RGBA32)
	//{

	//}
	//else
	//{
	//	LN_NOTIMPLEMENTED();
	//}

	return dstBitmap;
}

void Bitmap2D::blit(const RectI& destRect, const Bitmap2D* srcBitmap, const RectI& srcRect, const ColorI& color, BitmapBlitOptions options)
{
    detail::BlitHelper::bitBltInternal(this, destRect, srcBitmap, srcRect, detail::ClColor{ color.r, color.g, color.b, color.a }, testFlag(options, BitmapBlitOptions::AlphaBlend));
}

int Bitmap2D::getBitmapByteSize(int width, int height, int depth, PixelFormat format)
{
	return detail::BlitHelper::getPixelSize(format) * width * height * depth;
}

//==============================================================================
// Bitmap3D

Bitmap3D::Bitmap3D()
	: m_buffer(makeRef<ByteBuffer>())
	, m_width(0)
	, m_height(0)
	, m_depth(0)
	, m_format(PixelFormat::Unknown)
{
}

Bitmap3D::~Bitmap3D()
{
}

void Bitmap3D::init(int width, int height, int depth, PixelFormat format)
{
	if (LN_REQUIRE(width > 0)) return;
	if (LN_REQUIRE(height > 0)) return;
	if (LN_REQUIRE(depth > 0)) return;
	Object::init();
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_format = format;
	m_buffer->resize(Bitmap2D::getBitmapByteSize(m_width, m_height, m_depth, m_format));
}

void Bitmap3D::setPixel32(int x, int y, int z, const ColorI& color)
{
	if (m_format == PixelFormat::RGBA8)
	{
		size_t faceSize = m_width * m_height;
		ColorI* pixel = reinterpret_cast<ColorI*>(m_buffer->data() + ((z * faceSize) + ((y * m_width) + x)) * 4);
		*pixel = color;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

//==============================================================================
// BitmapHelper

namespace detail {

void BitmapHelper::blitRawSimple(void* dst, const void* src, size_t width, size_t height, size_t pixelBytes, bool flipVertical)
{
	if (LN_REQUIRE(dst)) return;
	if (LN_REQUIRE(src)) return;
	if (LN_REQUIRE(dst != src)) return;
	size_t lineBytes = width * pixelBytes;

	if (!flipVertical)
	{
		for (size_t y = 0; y < height; y++)
		{
			byte_t* d = static_cast<byte_t*>(dst) + (lineBytes * y);
			const byte_t* s = static_cast<const byte_t*>(src) + (lineBytes * y);
			memcpy(d, s, lineBytes);
		}
	}
	else
	{
		for (size_t y = 0; y < height; y++)
		{
			byte_t* d = static_cast<byte_t*>(dst) + (lineBytes * y);
			const byte_t* s = static_cast<const byte_t*>(src) + (lineBytes * (height - y - 1));
			memcpy(d, s, lineBytes);
		}
	}
}

void BitmapHelper::blitRawSimple3D(void* dst, const void* src, size_t width, size_t height, size_t depth, size_t pixelBytes, bool flipVertical)
{
	if (LN_REQUIRE(dst)) return;
	if (LN_REQUIRE(src)) return;
	if (LN_REQUIRE(dst != src)) return;
	size_t faceBytes = (width * pixelBytes) * height;

	for (size_t z = 0; z < depth; z++)
	{
		blitRawSimple(
			static_cast<byte_t*>(dst) + (faceBytes * z),
			static_cast<const byte_t*>(src) + (faceBytes * z),
			width, height, pixelBytes, flipVertical);
	}
}

//void BitmapHelper::drawText(Bitmap2D* bitmap, const StringView& text, const RectI& rect, Font* font, const Color& color)
//{
//
//}

} // namespace detail

} // namespace ln
