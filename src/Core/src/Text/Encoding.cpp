
#include <memory>
#include <algorithm>
#include "Internal.hpp"
#include <LuminoCore/Base/Buffer.hpp>
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Text/Encoding.hpp>
#include "UnicodeUtils.hpp"
#include "ASCIIEncoding.hpp"
#include "UTF8Encoding.hpp"
#include "UTF16Encoding.hpp"
#include "UTF32Encoding.hpp"
#include "DBCSEncoding.hpp"
#if defined(LN_OS_WIN32)
#include "Win32CodePageEncoding.hpp"
#endif

namespace ln {

//==============================================================================
// TextEncoding

TextEncoding::TextEncoding()
    : m_fallbackReplacementChar((uint32_t)'?')
{
}

TextEncoding::~TextEncoding()
{
}

const Ref<TextEncoding>& TextEncoding::systemMultiByteEncoding()
{
#ifdef LN_OS_WIN32
    static Win32CodePageEncoding systemEncoding(CP_THREAD_ACP);
#else
    static UTF8Encoding systemEncoding(false);
#endif
    static Ref<TextEncoding> ref(&systemEncoding);
    return ref;
}

const Ref<TextEncoding>& TextEncoding::wideCharEncoding()
{
#if defined(LN_WCHAR_16)
    static UTF16Encoding wideEncoding(false, false);
#elif defined(LN_WCHAR_32)
    static UTF32Encoding wideEncoding(false, false);
#else
#error "Invalid wchar_t size."
#endif
    static Ref<TextEncoding> ref(&wideEncoding);
    return ref;
}

const Ref<TextEncoding>& TextEncoding::tcharEncoding()
{
    return utf16Encoding();
}

const Ref<TextEncoding>& TextEncoding::utf8Encoding()
{
    static UTF8Encoding encoding(false);
    static Ref<TextEncoding> ref(&encoding);
    return ref;
}

const Ref<TextEncoding>& TextEncoding::utf16Encoding()
{
    static UTF16Encoding encoding(false, false);
    static Ref<TextEncoding> ref(&encoding);
    return ref;
}

const Ref<TextEncoding>& TextEncoding::utf32Encoding()
{
    static UTF32Encoding encoding(false, false);
    static Ref<TextEncoding> ref(&encoding);
    return ref;
}

const Ref<TextEncoding>& TextEncoding::getEncoding(EncodingType type)
{
    switch (type) {
        case EncodingType::ASCII: {
            static ASCIIEncoding asciiEncoding;
            static Ref<TextEncoding> ref(&asciiEncoding);
            return ref;
        }
        case EncodingType::SJIS:
        {
        	static DBCSEncoding sjisEncoding(EncodingType::SJIS);
			static Ref<TextEncoding> ref(&sjisEncoding);
			return ref;
        }
        //case EncodingType::EUCJP:
        //{
        //	static EUCJPEncoding eucjpEncoding;
        //	return &eucjpEncoding;
        //}
        //case EncodingType::GB2312:
        //{
        //	static DBCSEncoding gb2312Encoding(EncodingType::GB2312);
        //	return &gb2312Encoding;
        //}
        //case EncodingType::EUCKR:
        //{
        //	static DBCSEncoding euckrEncoding(EncodingType::EUCKR);
        //	return &euckrEncoding;
        //}
        //case EncodingType::BIG5:
        //{
        //	static DBCSEncoding big5Encoding(EncodingType::BIG5);
        //	return &big5Encoding;
        //}
        case EncodingType::UTF8: {
            static UTF8Encoding utf8BOMEncoding(true);
            static Ref<TextEncoding> ref(&utf8BOMEncoding);
            return ref;
        }
        default:
            LN_UNREACHABLE();
            static Ref<TextEncoding> ref;
            return ref;
    }
}

template<>
const Ref<TextEncoding>& TextEncoding::getEncodingTemplate<char>()
{
    return systemMultiByteEncoding();
}
template<>
const Ref<TextEncoding>& TextEncoding::getEncodingTemplate<wchar_t>()
{
    return wideCharEncoding();
}
template<>
const Ref<TextEncoding>& TextEncoding::getEncodingTemplate<char16_t>()
{
    return utf16Encoding();
}
template<>
const Ref<TextEncoding>& TextEncoding::getEncodingTemplate<char32_t>()
{
    return utf32Encoding();
}

size_t TextEncoding::getConversionRequiredByteCount(TextEncoding* from, TextEncoding* to, size_t fromByteCount)
{
    if (LN_REQUIRE(from))
        return 0;
    if (LN_REQUIRE(to))
        return 0;

    // from に入っている最悪パターンの文字数
    size_t srcMaxCharCount = fromByteCount / from->minByteCount();
    srcMaxCharCount += 1; // TextDecoder・TextEncoder の状態保存により前回の余り文字が1つ追加されるかもしれない

    // 出力バッファに必要な最大バイト数
    return srcMaxCharCount * to->maxByteCount();
}

const Ref<TextEncoding>& TextEncoding::detectEncodingSimple(const char* str, int length, bool strict)
{
    if (LN_REQUIRE(str))
        return Ref<TextEncoding>::Null;

    // check UTF8 BOM
    if (length >= 3) {
        const byte_t bom[] = {0xEF, 0xBB, 0xBF};
        if (memcmp(static_cast<const void*>(str), static_cast<const void*>(bom), 3) == 0) {
            return getEncoding(EncodingType::UTF8);
        }
    }

    int utf8pt = 0;
    int localPt = 0;
    const uint8_t* s = (const uint8_t*)str;
    const uint8_t* end = (const uint8_t*)str + length;
    int extra = 0;
    while (s < end) {
        if (*s == 0x00) {
            // 有効範囲内に 0 があった。バイナリかもしれない
            if (strict)
                return Ref<TextEncoding>::Null;
            else
                return getEncoding(EncodingType::ASCII);
        } else if (*s <= 0x7F) {
            // ASCII
            s++;
        } else {
            if (UnicodeUtils::checkUTF8TrailingBytes((const UTF8*)s, (const UTF8*)end, true, &extra) == UTFConversionResult_Success) {
                // UTF8
                utf8pt += 1;
                //s += extra;
            }

            {
                // それ以外 (SJIS など)
                localPt++;
            }
            s++;
        }
    }

    if (utf8pt == 0 && localPt == 0) {
        return getEncoding(EncodingType::ASCII);
    } else if (utf8pt >= localPt) {
        return getEncoding(EncodingType::UTF8N);
    } else {
        return systemMultiByteEncoding();
    }
}

size_t TextEncoding::checkPreamble(const void* buffer, size_t bufferSize) const
{
    if (LN_REQUIRE(buffer == nullptr))
        return 0;

    const char* bom = (const char*)preamble();
    size_t bomLen = strlen(bom);
    if (bufferSize < bomLen) {
        return 0;
    }

    if (memcmp(buffer, static_cast<const void*>(bom), bomLen) == 0) {
        return bomLen;
    }
    return 0;
}

String TextEncoding::decode(const byte_t* bytes, int length, int* outUsedDefaultCharCount)
{
    // TODO: this が UTF16 なら memcpy でよい

    // 入力に入っている最悪パターンの文字数
    size_t srcMaxCharCount = length / minByteCount();
    srcMaxCharCount += 1; // Decoder・Encoder の状態保存により前回の余り文字が1つ追加されるかもしれない
    srcMaxCharCount += 1; // \0 の分

    // 出力バッファに必要な最大バイト数
    size_t utf16MaxByteCount = srcMaxCharCount * 4; // UTF16 は1文字最大4バイト

    // 出力バッファ作成
    std::vector<byte_t> output(utf16MaxByteCount + sizeof(uint16_t));

    // convert
    std::unique_ptr<TextDecoder> decoder(createDecoder());
    TextDecodeResult result;
    decoder->convertToUTF16(
        bytes,
        length,
        (UTF16*)output.data(),
        utf16MaxByteCount / sizeof(UTF16), // \0 強制格納に備え、1文字分余裕のあるサイズを指定する
        &result);

    if (outUsedDefaultCharCount) {
        *outUsedDefaultCharCount = decoder->usedDefaultCharCount();
    }

    // 出力バッファのサイズを、実際に使用したバイト数にする
    output.resize(result.outputByteCount);

    return String((Char*)output.data(), output.size() / sizeof(Char));
}

std::vector<byte_t> TextEncoding::encode(const String& str, int* outUsedDefaultCharCount)
{
    return encode(str.c_str(), str.length(), outUsedDefaultCharCount);
}

std::vector<byte_t> TextEncoding::encode(const Char* str, int length, int* outUsedDefaultCharCount)
{
    // TODO: this が UTF16 なら memcpy でよい

    const byte_t* pr = preamble();
    int preambleLen = (pr) ? strlen(reinterpret_cast<const char*>(pr)) : 0;

    // 入力に入っている最悪パターンの文字数
    size_t srcMaxCharCount = length;

    // 出力バッファに必要な最大バイト数
    size_t outputMaxByteCount = srcMaxCharCount * maxByteCount();

    // 出力バッファ作成
    std::vector<byte_t> output(preambleLen + outputMaxByteCount + maxByteCount()); // \0 強制格納に備え、1文字分余裕のあるサイズを指定する

    // convert
    std::unique_ptr<TextEncoder> encoder(createEncoder());
    TextEncodeResult result;
    encoder->convertFromUTF16(
        (const UTF16*)str,
        length,
        output.data(),
        output.size(),
        &result);

    if (outUsedDefaultCharCount) {
        *outUsedDefaultCharCount = encoder->usedDefaultCharCount();
    }

    output.resize(result.outputByteCount);
    return output;
}

bool TextEncoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    return false;
}

//==============================================================================
// TextDecoder

TextDecoder::TextDecoder(TextEncoding* encoding)
    : /*m_fallbackReplacementChar(0)
	, */
    m_encoding(encoding)
{
    LN_REQUIRE(encoding);
}

TextDecoder::~TextDecoder()
{
}

//==============================================================================
// TextEncoder

TextEncoder::TextEncoder(TextEncoding* encoding)
    : /*m_fallbackReplacementChar(0)
	, */
    m_encoding(encoding)
{
    LN_REQUIRE(encoding);
}

TextEncoder::~TextEncoder()
{
}

} // namespace ln
