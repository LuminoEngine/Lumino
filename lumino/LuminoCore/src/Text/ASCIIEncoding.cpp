
#include "Internal.hpp"
#include "ASCIIEncoding.hpp"

namespace ln {

//==============================================================================
// ASCIIEncoding

const String ASCIIEncoding::Name = _T("US-ASCII");

ASCIIEncoding::ASCIIEncoding()
{
}

bool ASCIIEncoding::ASCIIDecoder::convertToUTF16(const byte_t* input, size_t inBufferByteCount, UTF16* output, size_t outBufferCharCount, TextDecodeResult* outResult)
{
    LN_DCHECK(input);
    LN_DCHECK(output);
    LN_DCHECK(outResult);

    uint32_t fallbackReplacementChar = encoding()->fallbackReplacementChar();

    size_t i = 0;
    for (; i < inBufferByteCount; ++i) {
        byte_t ch = input[i];

        // 普通の ASCII 文字
        if (ch <= 0x7F) {
            output[i] = static_cast<UTF16>(ch);
        }
        // 代替文字が指定されていればそれを使う
        else if (fallbackReplacementChar != 0x00) {
            output[i] = static_cast<UTF16>(fallbackReplacementChar);
        }
        // ASCII 外で代替文字も無ければエラー
        else {
            return false;
        }

        outResult->usedByteCount++;
        outResult->outputByteCount++;
        outResult->outputCharCount++;
    }

    return true;
}

bool ASCIIEncoding::ASCIIEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    LN_DCHECK(input);
    LN_DCHECK(output);
    LN_DCHECK(outResult);

    uint32_t fallbackReplacementChar = encoding()->fallbackReplacementChar();

    for (size_t i = 0; i < inputElementSize; ++i) {
        UTF16 ch = input[i];

        // 普通の ASCII 文字
        if (ch <= 0x7F) {
            output[i] = static_cast<byte_t>(ch);
        }
        // 代替文字が指定されていればそれを使う
        else if (fallbackReplacementChar != 0x00) {
            output[i] = static_cast<byte_t>(fallbackReplacementChar);
        }
        // ASCII 外で代替文字も無ければエラー
        else {
            return false;
        }

        outResult->usedElementCount++;
        outResult->outputByteCount++;
        outResult->outputCharCount++;
    }

    return true;
}

} // namespace ln
