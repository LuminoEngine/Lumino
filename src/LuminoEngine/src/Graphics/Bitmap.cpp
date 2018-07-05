
#include "Internal.hpp"
#include <png.h>
#include <Lumino/Graphics/Bitmap.hpp>

namespace ln {
namespace detail {

class IImageWriter
{
public:
	virtual ~IImageWriter() = default;
};

class PngImageWriter
	: public IImageWriter
{
public:
	PngImageWriter() {}
	virtual ~PngImageWriter() = default;

	static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
		//std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
		//p->insert(p->end(), data, data + length);

		Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png_ptr));
		stream->write(data, length);
	}

	// データは Bitmap クラスから直接渡されることを想定し、downflow で渡すこと。
	// フォーマットは RGBA
	void save(Stream* stream, const byte_t* data, const SizeI& size)
	{
		//png_struct* p = ;
		//if (LN_ENSURE(p, "png_create_write_struct() failed")) return;


		auto p = std::unique_ptr<png_struct, std::function<void(png_struct*)>>{
			png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL),
			[](png_struct* p) { png_destroy_write_struct(&p, NULL); } };

		


		png_infop info_ptr = png_create_info_struct(p.get());
		if (LN_ENSURE(info_ptr, "png_create_info_struct() failed")) return;

		//ASSERT_EX(info_ptr, "png_create_info_struct() failed");
		//ASSERT_EX(0 == setjmp(png_jmpbuf(p)), "setjmp(png_jmpbuf(p) failed");
		png_set_IHDR(
			p.get(), info_ptr,
			size.width, size.height,
			8,						// 各色 8 bit
			PNG_COLOR_TYPE_RGBA,	// RGBA フォーマット
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
		////png_set_compression_level(p, 1);
		std::vector<png_byte*> rows(size.height);
		for (size_t y = 0; y < size.height; ++y)
			rows[y] = (png_byte*)data + y * size.height * 4;

		png_set_write_fn(p.get(), stream, PngWriteCallback, NULL);

		// write PNG information to file
		png_write_info(p.get(), info_ptr);

		png_set_rows(p.get(), info_ptr, rows.data());
		png_write_png(p.get(), info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

		png_write_end(p.get(), info_ptr);
	}
};




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
}

Bitmap2D::~Bitmap2D()
{
}

void Bitmap2D::initialize(int width, int height, PixelFormat format)
{
	m_width = width;
	m_height = height;
	//m_depth = 0;
	m_format = format;
	m_buffer.resize(getBitmapByteSize(m_width, m_height, 1, m_format));
}

Color32 Bitmap2D::getColor32(int x, int y) const
{
	if (m_format == PixelFormat::RGBA32)
	{
		const uint8_t* pixel = m_buffer.data() + ((y * m_width) + x) * 4;
		return Color32(pixel[0], pixel[1], pixel[2], pixel[3]);
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return Color32();
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
		byte_t* pixels = m_buffer.data();
		for (int y = 0; y < (m_height / 2); ++y) {
			for (int x = 0; x < m_width; ++x) {
				std::swap(pixels[(y * m_width) + x], pixels[((m_height - 1 - y) * m_width) + x]);
			}
		}
	}
	else if (pixelSize == 4)
	{
		uint32_t* pixels = (uint32_t*)m_buffer.data();
		for (int y = 0; y < (m_height / 2); ++y) {
			for (int x = 0; x < m_width; ++x) {
				std::swap(pixels[(y * m_width) + x], pixels[((m_height - 1 - y) * m_width) + x]);
			}
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

void Bitmap2D::save(const StringRef& filePath)
{
	auto file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	detail::PngImageWriter writer;
	writer.save(file, m_buffer.data(), SizeI(m_width, m_height));
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


} // namespace ln
