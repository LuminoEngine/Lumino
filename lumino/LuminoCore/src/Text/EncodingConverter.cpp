
#include "Internal.hpp"
#include <algorithm>
#include <LuminoCore/Text/EncodingConverter.hpp>

namespace ln {

//==============================================================================
// EncodingConverter

EncodingConverter::EncodingConverter()
    : m_dstEncoding(nullptr)
    , m_srcEncoding(nullptr)
    , m_dstEncoder(nullptr)
    , m_srcDecoder(nullptr)
    , m_outputBuffer()
    , m_options()
    , m_lastResult()
    , m_tmpBuffer()
    , m_encodingModified(false)
{
    detail::GenericBufferHelper::setAutoClear(&m_outputBuffer, false);
    m_options.NullTerminated = false;
}

EncodingConverter::~EncodingConverter()
{
    if (m_dstEncoder) {
        delete m_dstEncoder;
    }
    if (m_srcDecoder) {
        delete m_srcDecoder;
    }
}

void EncodingConverter::setDestinationEncoding(TextEncoding* encoding)
{
    m_dstEncoding = encoding;
    m_encodingModified = true;
}

TextEncoding* EncodingConverter::getDestinationEncoding() const
{
    return m_dstEncoding;
}

void EncodingConverter::getSourceEncoding(TextEncoding* encoding)
{
    m_srcEncoding = encoding;
    m_encodingModified = true;
}

TextEncoding* EncodingConverter::getSourceEncoding() const
{
    return m_srcEncoding;
}

void EncodingConverter::setConversionOptions(const EncodingConversionOptions& options)
{
    m_options = options;
}

const ByteBuffer& EncodingConverter::convert(const void* data, size_t byteCount, EncodingConversionResult* outResult)
{
    checkUpdateEncoderDecoder();

    // 変換するのに必要なバイト数で領域確保
    size_t size = TextEncoding::getConversionRequiredByteCount(m_srcEncoding, m_dstEncoding, byteCount);
    if (m_options.NullTerminated) {
        size += m_dstEncoding->minByteCount();
    }
    m_outputBuffer.resize(size);

    if (m_options.NullTerminated) {
        m_outputBuffer.fill(0);
    }

    if (m_srcDecoder->canRemain()) {
        convertDecoderRemain(data, byteCount, m_srcDecoder, m_outputBuffer.data(), size, m_dstEncoder, &m_lastResult);

        // 余分に確保されているので、見かけ上のサイズを実際に文字のあるサイズに減らす
        m_outputBuffer.resize(m_lastResult.BytesUsed);

        if (outResult != nullptr) {
            *outResult = m_lastResult;
        }
    }
    // デコーダが変換状態を保持できない場合はやむを得ないので一時メモリを確保し、ソースバッファ全体を一度に変換する。
    else {
        // Char を UTF16 へ全部変換するのに必要なバイト数で一時メモリ確保
        size_t totalByteCount = TextEncoding::getConversionRequiredByteCount(m_srcEncoding, m_dstEncoding, byteCount);
        m_tmpBuffer.resize(totalByteCount);

#if LN_USTRING32
        // 後のコードがキャストだらけにならないように
        UTF32* utf32Buf = (UTF32*)m_tmpBuffer.data();
        int utf32ElementCount = m_tmpBuffer.size() / sizeof(UTF32);

        // Char を中間コード(UTF16) へ
        TextDecodeResult decodeResult;
        m_srcDecoder->convertToUTF32((const byte_t*)data, byteCount, utf32Buf, utf32ElementCount, &decodeResult);

        // 中間コード(UTF16)を出力コードへ
        TextEncodeResult encodeResult;
        m_dstEncoder->convertFromUTF32(utf32Buf, decodeResult.outputByteCount / sizeof(UTF16), m_outputBuffer.data(), m_outputBuffer.size(), &encodeResult);

#else
        // 後のコードがキャストだらけにならないように
        UTF16* utf16Buf = (UTF16*)m_tmpBuffer.data();
        int utf16ElementCount = m_tmpBuffer.size() / sizeof(UTF16);

        // Char を中間コード(UTF16) へ
        TextDecodeResult decodeResult;
        m_srcDecoder->convertToUTF16((const byte_t*)data, byteCount, utf16Buf, utf16ElementCount, &decodeResult);

        // 中間コード(UTF16)を出力コードへ
        TextEncodeResult encodeResult;
        m_dstEncoder->convertFromUTF16(utf16Buf, decodeResult.outputByteCount / sizeof(UTF16), m_outputBuffer.data(), m_outputBuffer.size(), &encodeResult);
#endif

        // 余分に確保されているので、見かけ上のサイズを実際に文字のあるサイズに減らす
        m_outputBuffer.resize(encodeResult.outputByteCount);

        m_lastResult.BytesUsed = encodeResult.outputByteCount;
        m_lastResult.CharsUsed = encodeResult.outputCharCount;
        m_lastResult.UsedDefaultChar = (m_srcDecoder->usedDefaultCharCount() > 0 || m_dstEncoder->usedDefaultCharCount() > 0);
        if (outResult != nullptr) {
            outResult->BytesUsed = m_lastResult.BytesUsed;
            outResult->CharsUsed = m_lastResult.CharsUsed;
            outResult->UsedDefaultChar = m_lastResult.UsedDefaultChar;
        }
    }
    return m_outputBuffer;
}

const ByteBuffer& EncodingConverter::getLastBuffer() const
{
    return m_outputBuffer;
}

const EncodingConversionResult& EncodingConverter::getLastResult() const
{
    return m_lastResult;
}

void EncodingConverter::checkUpdateEncoderDecoder()
{
    if (m_encodingModified) {
        if (LN_REQUIRE(m_dstEncoding)) return;
        if (LN_REQUIRE(m_srcEncoding)) return;

        if (m_dstEncoder) delete m_dstEncoder;
        if (m_srcDecoder) delete m_srcDecoder;

        m_dstEncoder = m_dstEncoding->createEncoder();
        m_srcDecoder = m_srcEncoding->createDecoder();
        m_encodingModified = false;
    }
}

void EncodingConverter::convertDecoderRemain(
    const void* src_,
    size_t srcByteCount,
    TextDecoder* srcDecoder,
    void* dest_,
    size_t destByteCount,
    TextEncoder* destEncoder,
    EncodingConversionResult* outResult)
{
    if (LN_REQUIRE(srcDecoder != nullptr)) return;
    if (LN_REQUIRE(srcDecoder->canRemain())) return;
    if (LN_REQUIRE(destEncoder != nullptr)) return;

    const size_t BufferingElements = 512;
#if LN_USTRING32
    UTF32 utf32[BufferingElements];
#else
    UTF16 utf16[BufferingElements];
#endif
    size_t totalBytesUsed = 0;
    size_t totalCharsUsed = 0;
    const byte_t* src = (const byte_t*)src_;
    byte_t* dest = (byte_t*)dest_;
    size_t srcPos = 0;
    size_t destPos = 0;

    for (;;) {
        if (srcPos >= srcByteCount || destPos >= destByteCount) {
            break;
        }
#if LN_USTRING32
        // UTF32 へ
        size_t srcBytes = LN_MIN(srcByteCount - srcPos, BufferingElements);
        TextDecodeResult decodeResult;
        srcDecoder->convertToUTF32(
            &src[srcPos],
            srcBytes,
            utf32,
            BufferingElements,
            &decodeResult);
        srcPos += srcBytes;

        // UTF16 文字をターゲットへ
        TextEncodeResult encodeResult;
        destEncoder->convertFromUTF32(
            utf32,
            decodeResult.outputByteCount / sizeof(UTF32),
            &dest[destPos],
            destByteCount - destPos,
            &encodeResult);
        destPos += encodeResult.outputByteCount;
#else
        // UTF16 へ
        size_t srcBytes = LN_MIN(srcByteCount - srcPos, BufferingElements);
        TextDecodeResult decodeResult;
        srcDecoder->convertToUTF16(
            &src[srcPos],
            srcBytes,
            utf16,
            BufferingElements,
            &decodeResult);
        srcPos += srcBytes;

        // UTF16 文字をターゲットへ
        TextEncodeResult encodeResult;
        destEncoder->convertFromUTF16(
            utf16,
            decodeResult.outputByteCount / sizeof(UTF16),
            &dest[destPos],
            destByteCount - destPos,
            &encodeResult);
        destPos += encodeResult.outputByteCount;
#endif

        totalBytesUsed += encodeResult.outputByteCount;
        totalCharsUsed += encodeResult.outputCharCount;
    }

    if (outResult) {
        outResult->BytesUsed = totalBytesUsed;
        outResult->CharsUsed = totalCharsUsed;
        outResult->UsedDefaultChar = (srcDecoder->usedDefaultCharCount() > 0 || destEncoder->usedDefaultCharCount() > 0);
    }
}

} // namespace ln
