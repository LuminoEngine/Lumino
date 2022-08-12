#include <png.h>
// vcpkg で nanovg を使う場合、その中で定義されている。
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "BitmapEncoding.hpp"

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
	void save(Stream* stream, const byte_t* data, const SizeI& size, PixelFormat format);
};

void PngBitmapEncoder::save(Stream* stream, const byte_t* data, const SizeI& size, PixelFormat format)
{
    int colorType = 0;
    if (format == PixelFormat::RGBA8) {
        colorType = PNG_COLOR_TYPE_RGBA;
    }
    else if (format == PixelFormat::RGB8) {
        colorType = PNG_COLOR_TYPE_RGB;
    }
    else {
        LN_ERROR();
        return;
    }

	png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (LN_ENSURE(png, "png_create_write_struct() failed")) return;

	png_infop info_ptr = png_create_info_struct(png);
	auto se = makeScopedCall([&]() {png_destroy_write_struct(&png, &info_ptr); }); // finalizer
	if (LN_ENSURE(info_ptr, "png_create_info_struct() failed")) return;



	png_set_IHDR(
		png, info_ptr,
		size.width, size.height,
		8,						// 各色 8 bit
        colorType,	// RGBA フォーマット
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	//png_set_compression_level(png, 1);
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
			diag->reportError(_TT("invalid data size."));
			return false;
		}

		// データが png かどうか調べる
		stream->read(sig, 4);
		stream->seek(0, SeekOrigin::Begin);
		res = png_check_sig(sig, PNG_BYTES_TO_CHECK);
		if (!res) {
			diag->reportError(_TT("invalid png signature."));
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
			m_frame.format = PixelFormat::RGBA8;
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
			m_frame.format = PixelFormat::RGBA8;
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


class StbBitmapDecoder
    : public IBitmapDecoder
{
public:
    Result load(Stream* stream)
    {
        ByteBuffer buffer(stream->length());
        stream->read(buffer.data(), buffer.size());

        int width;
        int height;
        int bpp;
        unsigned char* pixels = stbi_load_from_memory(buffer.data(), buffer.size(), &width, &height, &bpp, 4);

        m_frame.size.width = width;
        m_frame.size.height = height;
        m_frame.format = PixelFormat::RGBA8;
        m_frame.data = makeRef<ByteBuffer>(pixels, 4 * m_frame.size.width * m_frame.size.height);


        stbi_image_free(pixels);
        return ok();
    }

    virtual BitmapFrame* getBitmapFrame() override { return &m_frame; }

private:
    BitmapFrame m_frame;
};

//==============================================================================
// IBitmapEncoder

void IBitmapEncoder::save(Stream* stream, const byte_t* data, const SizeI& size, PixelFormat format)
{
    PngBitmapEncoder encoder;
    encoder.save(stream, data, size, format);
}

//==============================================================================
// IBitmapDecoder

Ref<IBitmapDecoder> IBitmapDecoder::load(Stream* stream, DiagnosticsManager* diag)
{
    {
        auto decoder = makeRef<StbBitmapDecoder>();
        if (decoder->load(stream)) {
            return decoder;
        }
    }

    {
        auto decoder = makeRef<PngBitmapDecoder>();
        decoder->load(stream, diag);
        return decoder;
    }
}

} // namespace detail
} // namespace ln
