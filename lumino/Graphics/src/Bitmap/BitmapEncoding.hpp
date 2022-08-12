#pragma once
#include <LuminoCore/Base/Result.hpp>
#include <LuminoGraphics/Bitmap/Common.hpp>

namespace ln {
class DiagnosticsManager;
namespace detail {

class BitmapFrame
{
public:
    Ref<ByteBuffer> data;
    SizeI size;
    PixelFormat format;
};

class IBitmapEncoder
    : public RefObject
{
public:
    virtual ~IBitmapEncoder() = default;

    static void save(Stream* stream, const byte_t* data, const SizeI& size, PixelFormat format);
};

class IBitmapDecoder
    : public RefObject
{
public:
    virtual ~IBitmapDecoder() = default;

    virtual BitmapFrame* getBitmapFrame() = 0;

    static Ref<IBitmapDecoder> load(Stream* stream, DiagnosticsManager* diag);
};

} // namespace detail
} // namespace ln
