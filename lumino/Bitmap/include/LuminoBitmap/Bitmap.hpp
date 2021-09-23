
#pragma once
#include <LuminoEngine/Reflection/Object.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "Common.hpp"

namespace ln {
class Bitmap2D;

namespace detail {

struct ClColor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct U32
{
	byte_t D[4];
};

class PixelAccessor_A8
{
public:
    ClColor color;

    PixelAccessor_A8(ClColor c)
        : color(c)
    {}
    inline size_t pitch(int width) const
    {
        return width;
    }
    inline ClColor get(const byte_t* line, int x) const
    {
        byte_t c = line[x];
        return ClColor{ color.r, color.g, color.b, c };
    }
    inline void set(byte_t* line, int x, ClColor color) const
    {
        line[x] = color.a;
    }
};

class PixelAccessor_R8G8B8A8
{
public:
    PixelAccessor_R8G8B8A8(ClColor c)
    {}
	inline size_t pitch(int width) const
	{
		return width * 4;
	}
	inline ClColor get(const byte_t* line, int x) const
	{
		U32* c = &((U32*)line)[x];
		return ClColor{ c->D[0], c->D[1], c->D[2], c->D[3] };
	}
	inline void set(byte_t* line, int x, ClColor color) const
	{
		U32* w = &((U32*)line)[x];
		w->D[0] = color.r; w->D[1] = color.g; w->D[2] = color.b; w->D[3] = color.a;
	}
};

template<class TPixelAccessor>
class SrcBuffer
{
public:
	/// bitmap	: 転送元 RawBitmap
	/// srcRect	: 転送元領域 (RawBitmap のサイズに収まるようにクリッピングされていること)
	SrcBuffer(const byte_t* data, size_t width, bool upFlow, const RectI& srcRect, TPixelAccessor accessor)
		: m_data(data)
		, m_widthPitch(accessor.pitch(width))
		, m_srcRect(srcRect)
		, m_bottomLine(srcRect.getBottom() - 1)	// 転送範囲の最後の行 (0スタート)
		, m_currentLine(nullptr)
		, m_upFlow(upFlow)
		, m_accessor(accessor)
	{}

public:
	inline void setLine(int y)
	{
		if (!m_upFlow)
			m_currentLine = &m_data[(m_widthPitch * (m_srcRect.y + y))];
		else
			m_currentLine = &m_data[(m_widthPitch * (m_bottomLine - y))];
	}

	inline ClColor getPixel(int x) const
	{
		return m_accessor.get(m_currentLine, m_srcRect.x + x);
	}

private:
	const byte_t* m_data;
	size_t m_widthPitch;
	const RectI&	m_srcRect;
	int				m_bottomLine;
	const byte_t*	m_currentLine;
	bool			m_upFlow;
	TPixelAccessor m_accessor;
};

template<class TPixelAccessor>
class DestBuffer
{
public:
	/// bitmap	: 転送先 RawBitmap
	/// rect	: 転送先領域 (RawBitmap のサイズに収まるようにクリッピングされていること)
	DestBuffer(byte_t* data, size_t width, bool upFlow, const RectI& dstRect, TPixelAccessor accessor)
		: m_data(data)
		, m_widthPitch(accessor.pitch(width))
		, m_dstRect(dstRect)
		, m_bottomLine(dstRect.getBottom() - 1)	// 転送範囲の最後の行 (0スタート)
		, m_currentLine(nullptr)
		, m_upFlow(upFlow)
		, m_accessor(accessor)
	{}

public:
	inline void setLine(int y)
	{
		if (!m_upFlow)
			m_currentLine = &m_data[(m_widthPitch * (m_dstRect.y + y))];
		else
			m_currentLine = &m_data[(m_widthPitch * (m_bottomLine - y))];
	}

	inline ClColor getPixel(int x)
	{
		return m_accessor.get(m_currentLine, m_dstRect.x + x);
	}

	inline void setPixel(int x, ClColor color)
	{
		m_accessor.set(m_currentLine, m_dstRect.x + x, color);
	}

private:
	byte_t*			m_data;
	int				m_widthPitch;
	const RectI&	m_dstRect;
	int				m_bottomLine;
	byte_t*			m_currentLine;
	bool			m_upFlow;
	TPixelAccessor m_accessor;
};



class BlitHelper
{
public:
    template<class TDestConverter, class TSrcConverter>
    static void bitBltInternalTemplate(
        Bitmap2D* dest, const RectI& destRect,
        const Bitmap2D* src, const RectI& srcRect,
        ClColor mulColorRGBA, bool alphaBlend) throw();

    template<class TDestConverter>
    static void bitBltInternalTemplateHelper(
        Bitmap2D* dest, const RectI& destRect,
        const Bitmap2D* src, const RectI& srcRect,
        ClColor mulColorRGBA, bool alphaBlend);

    static void bitBltInternal(
        Bitmap2D* dest, const RectI& destRect,
        const Bitmap2D* src, const RectI& srcRect,
        ClColor mulColorRGBA, bool alphaBlend);

	static size_t getPixelSize(PixelFormat format);
};



} // namespace detail

enum class BitmapBlitOptions
{
    None = 0x0000,
    AlphaBlend = 0x0001,
};

class Bitmap2D
	: public Object
{
public:
	int width() const { return m_size.width; }
	int height() const { return m_size.height; }
	PixelFormat format() const { return m_format; }

	ColorI getPixel32(int x, int y) const;
	void setPixel32(int x, int y, const ColorI& color);
    void clear(const ColorI& color);
	void resize(int width, int height);

	/** バッファの先頭へのポインタを取得します。 */
	byte_t* data() LN_NOEXCEPT { return m_buffer->data(); }

	/** バッファの先頭へのポインタを取得します。 */
	const byte_t* data() const LN_NOEXCEPT { return m_buffer->data(); }

	void flipVerticalFlow();

	void load(const StringRef& filePath);  // TODO: 種類の指定
    void load(Stream* stream);   // TODO: 種類の指定
	void save(const StringRef& filePath);

	Ref<Bitmap2D> clone() const;
	Ref<Bitmap2D> transcodeTo(PixelFormat format, const ColorI& color = ColorI::White) const;

    void blit(const RectI& destRect, const Bitmap2D* srcBitmap, const RectI& srcRect, const ColorI& color, BitmapBlitOptions options);

	//static int getPixelFormatByteSize(PixelFormat format);
	static int getBitmapByteSize(int width, int height, int depth, PixelFormat format);

LN_CONSTRUCT_ACCESS:
	Bitmap2D();
	virtual ~Bitmap2D();
	void init();
	void init(int width, int height, PixelFormat format, const void* data = nullptr);

LN_INTERNAL_ACCESS:
	ByteBuffer* rawBuffer() { return m_buffer; }

public:	// TODO:
	//Ref<Bitmap2D> transcodeA8ToRGBA32() const;

	Ref<ByteBuffer> m_buffer;	// Font などからの reserve 動作のため、m_size * m_format とバイトサイズは一致しないことがあるので注意。
	SizeI m_size;
	PixelFormat m_format;

    friend class detail::BlitHelper;
};

class Bitmap3D
	: public Object
{
public:
	int width() const { return m_width; }
	int height() const { return m_height; }
	int depth() const { return m_height; }
	PixelFormat format() const { return m_format; }

	void setPixel32(int x, int y, int z, const ColorI& color);

	/** バッファの先頭へのポインタを取得します。 */
	byte_t* data() LN_NOEXCEPT { return m_buffer->data(); }

	/** バッファの先頭へのポインタを取得します。 */
	const byte_t* data() const LN_NOEXCEPT { return m_buffer->data(); }

LN_CONSTRUCT_ACCESS:
	Bitmap3D();
	virtual ~Bitmap3D();
	void init(int width, int height, int depth, PixelFormat format);

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
    //static void drawText(Bitmap2D* bitmap, const StringRef& text, const RectI& rect, Font* font, const Color& color);
};
} // namespace detail
} // namespace ln
