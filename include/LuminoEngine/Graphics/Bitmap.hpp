
#pragma once
#include "Common.hpp"
#include "ColorStructs.hpp"

namespace ln {

class Bitmap2D
	: public Object
{
public:
	int width() const { return m_size.width; }
	int height() const { return m_size.height; }
	PixelFormat format() const { return m_format; }

	Color32 getPixel32(int x, int y) const;
	void setPixel32(int x, int y, const Color32& color);

	/** バッファの先頭へのポインタを取得します。 */
	byte_t* data() LN_NOEXCEPT { return m_buffer->data(); }

	/** バッファの先頭へのポインタを取得します。 */
	const byte_t* data() const LN_NOEXCEPT { return m_buffer->data(); }

	void flipVerticalFlow();

	void load(const StringRef& filePath);
	void save(const StringRef& filePath);

	static int getPixelFormatByteSize(PixelFormat format);
	static int getBitmapByteSize(int width, int height, int depth, PixelFormat format);

LN_CONSTRUCT_ACCESS:
	Bitmap2D();
	virtual ~Bitmap2D();
	void initialize();
	void initialize(int width, int height, PixelFormat format);

LN_INTERNAL_ACCESS:
	ByteBuffer* rawBuffer() { return m_buffer; }

private:
	Ref<ByteBuffer> m_buffer;
	SizeI m_size;
	PixelFormat m_format;
};

class Bitmap3D
	: public Object
{
public:
	int width() const { return m_width; }
	int height() const { return m_height; }
	int depth() const { return m_height; }
	PixelFormat format() const { return m_format; }

	void setPixel32(int x, int y, int z, const Color32& color);

	/** バッファの先頭へのポインタを取得します。 */
	byte_t* data() LN_NOEXCEPT { return m_buffer->data(); }

	/** バッファの先頭へのポインタを取得します。 */
	const byte_t* data() const LN_NOEXCEPT { return m_buffer->data(); }

LN_CONSTRUCT_ACCESS:
	Bitmap3D();
	virtual ~Bitmap3D();
	void initialize(int width, int height, int depth, PixelFormat format);

LN_INTERNAL_ACCESS:
	ByteBuffer* rawBuffer() { return m_buffer; }

private:
	Ref<ByteBuffer> m_buffer;
	int m_width;
	int m_height;
	int m_depth;
	PixelFormat m_format;
};

namespace detail {

class BitmapHelper
{
public:
	static void blitRawSimple(void* dst, const void* src, size_t width, size_t height, size_t pixelBytes, bool flipVertical);
	static void blitRawSimple3D(void* dst, const void* src, size_t width, size_t height, size_t depth, size_t pixelBytes, bool flipVertical);
};
} // namespace detail
} // namespace ln
