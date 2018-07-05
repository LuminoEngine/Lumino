
#pragma once
#include "Common.hpp"

namespace ln {

class BitmapRef
{
public:

};

class Bitmap2D
	: public Object
{
public:
	int width() const { return m_width; }
	int height() const { return m_height; }
	//int depth() const { return m_depth; }

	Color32 getColor32(int x, int y) const;

	/** バッファの先頭へのポインタを取得します。 */
	byte_t* data() LN_NOEXCEPT { return m_buffer.data(); }

	/** バッファの先頭へのポインタを取得します。 */
	const byte_t* data() const LN_NOEXCEPT { return m_buffer.data(); }

	void flipVerticalFlow();

	void save(const StringRef& filePath);

	static int getPixelFormatByteSize(PixelFormat format);
	static int getBitmapByteSize(int width, int height, int depth, PixelFormat format);


LN_INTERNAL_ACCESS:
	Bitmap2D();
	virtual ~Bitmap2D();
	void initialize(int width, int height, PixelFormat format);
	ByteBuffer* rawBuffer() { return &m_buffer; }

private:
	ByteBuffer m_buffer;
	int m_width;
	int m_height;
	//int m_depth;
	PixelFormat m_format;
};


} // namespace ln
