
#include "Internal.hpp"
#include <png.h>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>

namespace ln {
namespace detail {

template<class TFunc>
class ScopedCall
{
public:
	ScopedCall(TFunc finalizer)
		: m_finalizer(finalizer)
	{}

	~ScopedCall()
	{
		m_finalizer();
	}

private:
	TFunc m_finalizer;
};

template<class TFunc>
ScopedCall<TFunc> makeScopedCall(TFunc finalizer)
{
	return ScopedCall<TFunc>(finalizer);
}

class BitmapFrame
{
public:
	Ref<ByteBuffer> data;
	SizeI size;
	PixelFormat format;
};

class IBitmapEncoder
{
public:
	virtual ~IBitmapEncoder() = default;
};

class PngBitmapEncoder
	: public IBitmapEncoder
{
public:
	PngBitmapEncoder() {}
	virtual ~PngBitmapEncoder() = default;

	static void pngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
		Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png_ptr));
		stream->write(data, length);
	}

	// データは Bitmap クラスから直接渡されることを想定し、downflow で渡すこと。
	// フォーマットは RGBA
	void save(Stream* stream, const byte_t* data, const SizeI& size);
};

void PngBitmapEncoder::save(Stream* stream, const byte_t* data, const SizeI& size)
{
	png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (LN_ENSURE(png, "png_create_write_struct() failed")) return;

	png_infop info_ptr = png_create_info_struct(png);
	auto se = makeScopedCall([&]() {png_destroy_write_struct(&png, &info_ptr); }); // finalizer
	if (LN_ENSURE(info_ptr, "png_create_info_struct() failed")) return;

	png_set_IHDR(
		png, info_ptr,
		size.width, size.height,
		8,						// 各色 8 bit
		PNG_COLOR_TYPE_RGBA,	// RGBA フォーマット
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(png, 1);
	png_set_write_fn(png, stream, pngWriteCallback, NULL);

	std::vector<png_byte*> rows(size.height);
	int rowBytes = png_get_rowbytes(png, info_ptr);
	for (size_t y = 0; y < size.height; ++y)
		rows[y] = (png_byte*)data + (rowBytes * y);

	// write PNG information to file
	//png_write_info(png, info_ptr);

	png_set_rows(png, info_ptr, rows.data());
	png_write_png(png, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	//png_write_end(png, info_ptr);
}



class IBitmapDecoder
{
public:
	virtual ~IBitmapDecoder() = default;

	virtual BitmapFrame* getBitmapFrame() = 0;
};

class PngBitmapDecoder
	: public IBitmapDecoder
{
public:
	PngBitmapDecoder() {}
	virtual ~PngBitmapDecoder() = default;

	static const int PNG_BYTES_TO_CHECK = 4;	// png 識別用の、ファイルの先頭バイト数
	png_struct* m_png;
	png_info* m_info;

	bool load(Stream* stream, DiagnosticsManager* diag)
	{
		const bool swapHeight = false;
		png_byte	sig[PNG_BYTES_TO_CHECK];
		int			res;

		size_t dataSize = (size_t)stream->length();

		// サイズチェック
		if (dataSize < PNG_BYTES_TO_CHECK) {
			diag->reportError("invalid data size.");
			return false;
		}

		// データが png かどうか調べる
		stream->read(sig, 4);
		stream->seek(0, SeekOrigin::Begin);
		res = png_check_sig(sig, PNG_BYTES_TO_CHECK);
		if (!res) {
			diag->reportError("invalid png signature.");
			return false;
		}

		// png_struct構造体を確保・初期化する
		m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (LN_ENSURE(m_png)) return false;

		// finalizer
		auto se = makeScopedCall([&]() {png_destroy_read_struct(&m_png, &m_info, NULL); });

		// png_info構造体を確保・初期化する
		m_info = png_create_info_struct(m_png);
		if (LN_ENSURE(m_info)) return false;

		if (setjmp(png_jmpbuf(m_png))) return false;

		// png データ、読み込みコールバック設定
		png_set_read_fn(m_png, (void*)stream, pngReadCallback);

		// シグネチャの確認で読み飛ばしたバイト数を知らせる
		//png_set_sig_bytes( m_png, PNG_BYTES_TO_CHECK );

		// PNGファイルのヘッダ情報を読み込む
		png_read_png(m_png, m_info, PNG_TRANSFORM_EXPAND, NULL);

		// IHDRチャンク情報を取得する
		png_uint_32 width, height;
		int bitDepth, colorType, interlaceType;
		png_get_IHDR(
			m_png, m_info, &width, &height,
			&bitDepth, &colorType, &interlaceType, NULL, NULL);
		m_frame.size.width = width;
		m_frame.size.height = height;

		int pixelDepth = png_get_bit_depth(m_png, m_info) * png_get_channels(m_png, m_info);

		// 必ず1色 8 ビットで
		if (bitDepth != 8) return false;

		// パレットモードは非対応
		if (colorType & PNG_COLOR_MASK_PALETTE) return false;

		//-----------------------------------------------------
		// ビットマップ格納

		unsigned int	row_bytes = png_get_rowbytes(m_png, m_info);	// 横幅のバイト数 (例えば ABGR の時は 横幅 * 4)
		png_bytepp		row_pointers = png_get_rows(m_png, m_info);	// ビットマップデータ

		int sign = (swapHeight) ? -1 : 1;			// 反転するか？
		int unit = (swapHeight) ? m_frame.size.height - 1 : 0;	// イテレート開始行 (一番上か、一番下か)

															// ABGR
															// (R155, G128, B0, A78) のとき、U32(Little) で 4e0080ff となる。
															// byte[4] の並びは AA RR GG BB
		if (colorType == PNG_COLOR_TYPE_RGB_ALPHA && pixelDepth == 32)
		{
			m_frame.format = PixelFormat::RGBA32;
			m_frame.data = makeRef<ByteBuffer>(m_frame.size.width * m_frame.size.height * 4);
			byte_t* bitmap = m_frame.data->data();

			// 1行ずつコピー
			for (int h = 0; h < m_frame.size.height; ++h) {
				memcpy(&bitmap[row_bytes * (unit + (sign * h))], row_pointers[h], row_bytes);
			}
		}
		// BGR
		// ABGR に拡張して読み込む
		else if (colorType == PNG_COLOR_TYPE_RGB && pixelDepth == 24)
		{
			m_frame.format = PixelFormat::RGBA32;
			m_frame.data = makeRef<ByteBuffer>(m_frame.size.width * m_frame.size.height * 4);
			byte_t* bitmap = m_frame.data->data();

			byte_t* row;
			for (int y = 0; y < m_frame.size.height; ++y)
			{
				row = row_pointers[unit + (sign * y)];
				for (int x = 0; x < m_frame.size.width; ++x)
				{
					byte_t* src = &row[x * 3];
					byte_t* dest = &bitmap[(x + m_frame.size.width * y) * 4];
					dest[0] = src[0];	// R
					dest[1] = src[1];	// G
					dest[2] = src[2];	// B
					dest[3] = 0xFF;		// A
				}
			}
		}
		// Gray
		else if (colorType == PNG_COLOR_TYPE_GRAY && pixelDepth == 8)
		{
			m_frame.format = PixelFormat::A8;
			m_frame.data = makeRef<ByteBuffer>(m_frame.size.width * m_frame.size.height * 1);
			byte_t* bitmap = m_frame.data->data();

			for (int h = 0; h < m_frame.size.height; ++h) {
				memcpy(&bitmap[row_bytes * (unit + (sign * h))], row_pointers[h], row_bytes);
			}
		}
		else {
			return false;
		}

		//PNG_COLOR_TYPE_GRAY// (ビット深度 1, 2, 4, 8, 16)
		//PNG_COLOR_TYPE_GRAY_ALPHA// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_PALETTE// (ビット深度 1, 2, 4, 8)
		//PNG_COLOR_TYPE_RGB// (ビット深度 8, 16)
		//PNG_COLOR_TYPE_RGB_ALPHA// (ビット深度 8, 16)

		//PNG_COLOR_MASK_PALETTE
		//PNG_COLOR_MASK_COLOR
		//PNG_COLOR_MASK_ALPHA

		//if(png_get_valid(pPng, pInfo, PNG_INFO_tRNS))
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_PALETTE)
		//	png_set_expand(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY && bpp < 8)
		//	png_set_expand(pPng);
		//if(bpp > 8)
		//	png_set_strip_16(pPng);
		//if(ColorType == PNG_COLOR_TYPE_GRAY)
		//	png_set_gray_to_rgb(pPng);
		return true;
	}

	static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		Stream* stream = (Stream*)png_get_io_ptr(png_ptr);
		int validSize = stream->read(data, length);
		if (validSize != length) {
			png_error(png_ptr, "_readPngData failed");
		}
	}

	virtual BitmapFrame* getBitmapFrame() override { return &m_frame; }

private:

	BitmapFrame m_frame;
};




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

    if (alphaBlend)
    {
        for (int y = 0; y < srcRect.height; ++y)
        {
            dstBuf.setLine(y);
            srcBuf.setLine(y);
            for (int x = 0; x < srcRect.width; ++x)
            {
                ClColor src = srcBuf.getPixel(x);
                uint8_t src_alpha = src.a;
                if (src_alpha == 0) continue;     // フォントならコレでかなり高速化できるはず


                ClColor dest_color = dstBuf.getPixel(x);
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
    case PixelFormat::RGBA32:
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
    case PixelFormat::RGBA32:
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

void Bitmap2D::initialize()
{
	m_size.width = 0;
	m_size.height = 0;
	m_format = PixelFormat::Unknown;
}

void Bitmap2D::initialize(int width, int height, PixelFormat format)
{
	m_size.width = width;
	m_size.height = height;
	m_format = format;
	m_buffer->resize(getBitmapByteSize(m_size.width, m_size.height, 1, m_format));
}

Color32 Bitmap2D::getPixel32(int x, int y) const
{
	if (m_format == PixelFormat::RGBA32)
	{
		const uint8_t* pixel = m_buffer->data() + ((y * m_size.width) + x) * 4;
		return Color32(pixel[0], pixel[1], pixel[2], pixel[3]);
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return Color32();
	}
}

void Bitmap2D::setPixel32(int x, int y, const Color32& color)
{
	if (m_format == PixelFormat::RGBA32)
	{
		Color32* pixel = reinterpret_cast<Color32*>(m_buffer->data() + ((y * m_size.width) + x) * 4);
		*pixel = color;
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void Bitmap2D::flipVerticalFlow()
{
	if (LN_REQUIRE(m_format != PixelFormat::Unknown)) return;
	if (LN_REQUIRE(m_format != PixelFormat::A1)) return;

	int pixelSize = getPixelFormatByteSize(m_format);
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
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_buffer->data();
		for (int y = 0; y < (m_size.height / 2); ++y) {
			for (int x = 0; x < m_size.width; ++x) {
				std::swap(pixels[(y * m_size.width) + x], pixels[((m_size.height - 1 - y) * m_size.width) + x]);
			}
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void Bitmap2D::load(const StringRef& filePath)
{
	auto file = FileStream::create(filePath);
	detail::PngBitmapDecoder decoder;

	auto diag = newObject<DiagnosticsManager>();
	decoder.load(file, diag);
	diag->dumpToLog();
	if (diag->succeeded())
	{
		detail::BitmapFrame* frame = decoder.getBitmapFrame();
		m_buffer = frame->data;
		m_size = frame->size;
		m_format = frame->format;
	}
}

void Bitmap2D::save(const StringRef& filePath)
{
	auto file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	detail::PngBitmapEncoder encoder;
	encoder.save(file, m_buffer->data(), m_size);
}

Ref<Bitmap2D> Bitmap2D::transcodeTo(PixelFormat format, const Color32& color) const
{
	auto dstBitmap = newObject<Bitmap2D>(m_size.width, m_size.height, format);

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

void Bitmap2D::blit(const RectI& destRect, const Bitmap2D* srcBitmap, const RectI& srcRect, const Color32& color, BitmapBlitOptions options)
{
    detail::BlitHelper::bitBltInternal(this, destRect, srcBitmap, srcRect, detail::ClColor{ color.r, color.g, color.b, color.a }, testFlag(options, BitmapBlitOptions::AlphaBlend));
}

int Bitmap2D::getPixelFormatByteSize(PixelFormat format)
{
	const int table[] =
	{
		0,	// Unknown,
		1,	// A1,
		1,	// A8,
		4,	// RGBA32,
		16,	// R32G32B32A32Float,
	};
	return table[(int)format];
}

int Bitmap2D::getBitmapByteSize(int width, int height, int depth, PixelFormat format)
{
	return getPixelFormatByteSize(format) * width * height * depth;
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

void Bitmap3D::initialize(int width, int height, int depth, PixelFormat format)
{
	if (LN_REQUIRE(width > 0)) return;
	if (LN_REQUIRE(height > 0)) return;
	if (LN_REQUIRE(depth > 0)) return;
	Object::initialize();
	m_width = width;
	m_height = height;
	m_depth = depth;
	m_format = format;
	m_buffer->resize(Bitmap2D::getBitmapByteSize(m_width, m_height, m_depth, m_format));
}

void Bitmap3D::setPixel32(int x, int y, int z, const Color32& color)
{
	if (m_format == PixelFormat::RGBA32)
	{
		size_t faceSize = m_width * m_height;
		Color32* pixel = reinterpret_cast<Color32*>(m_buffer->data() + ((z * faceSize) + ((y * m_width) + x)) * 4);
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

//void BitmapHelper::drawText(Bitmap2D* bitmap, const StringRef& text, const RectI& rect, Font* font, const Color& color)
//{
//
//}

} // namespace detail

} // namespace ln
