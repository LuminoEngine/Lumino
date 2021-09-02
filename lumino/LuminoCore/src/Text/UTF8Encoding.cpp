
#include "Internal.hpp"
#include <LuminoCore/Base/UnicodeUtils.hpp>
#include "UTF8Encoding.hpp"

namespace ln {

//==============================================================================
// UTF8Encoding

const byte_t UTF8Encoding::BOM[] = { 0xEF, 0xBB, 0xBF };
const String UTF8Encoding::Name = _T("UTF-8");

UTF8Encoding::UTF8Encoding(bool byteOrderMark)
    : m_byteOrderMark(byteOrderMark)
{
}

byte_t* UTF8Encoding::preamble() const
{
    if (m_byteOrderMark) {
        static byte_t bom[] = { 0xEF, 0xBB, 0xBF, 0x00 };
        return bom;
    }
    else {
        return nullptr;
    }
}

int UTF8Encoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
    int count;
    UTFConversionResult result = UnicodeUtils::getUTF8CharCount((const byte_t*)buffer, bufferSize, true, &count);
    if (LN_ENSURE(result == UTFConversionResult_Success))
        return 0;
    return count;
}

int UTF8Encoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
    int len;
    UTFConversionResult result = UnicodeUtils::checkUTF8TrailingBytes(((const byte_t*)buffer), ((const byte_t*)buffer) + bufferSize, true, &len);
    if (LN_ENSURE(result == UTFConversionResult_Success))
        return 0;
    return len;
}

bool UTF8Encoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	UTF8Decoder decoder(this, m_byteOrderMark);
	return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}

//==============================================================================
// UTF8Decoder

bool UTF8Encoding::UTF8Decoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    outResult->usedByteCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;
    int charsUsed = 0;

    // 変換設定
    UTFConversionOptions options;
    memset(&options, 0, sizeof(options));
    options.ReplacementChar = encoding()->fallbackReplacementChar();

    // BOM 付きの場合は取り除く (バッファ縮小)
    if (m_byteOrderMark) {
        if (m_bomPhase == 0) {
            if (input[0] == 0xEF) {
                m_bomPhase = 1;
                ++input;
                --inputByteSize;
                if (inputByteSize == 0) {
                    return false;
                }
            }
        }
        if (m_bomPhase == 1) {
            if (input[0] == 0xBB) {
                m_bomPhase = 2;
                ++input;
                --inputByteSize;
                if (inputByteSize == 0) {
                    return false;
                }
            } else {
                return false;
            }
        }
        if (m_bomPhase == 2) {
            if (input[0] == 0xBF) {
                m_bomPhase = 0;
                ++input;
                --inputByteSize;
                if (inputByteSize == 0) {
                    return false;
                }
            } else {
                return false;
            }
        }

        outResult->usedByteCount += 3;
    }

    const byte_t* srcPos = input;
    const byte_t* srcEnd = input + inputByteSize;
    UTF16* dstPos = reinterpret_cast<UTF16*>(output);
    UTF16* dstEnd = dstPos + outputElementSize;
    while (srcPos < srcEnd) {
        if (m_lastLeadBytesCount >= 5)
            return false;

        // 先行バイトの確認
        if (m_requestLeadBytesCount == 0) {
            UnicodeUtils::checkUTF8TrailingBytes(srcPos, srcEnd, false, &m_requestLeadBytesCount);
            //m_requestLeadBytesCount++;	// ↓で今回の文字も含めて m_lastLeadBytes に保存したい
        }

        if (m_requestLeadBytesCount > 0) {
            // 先行バイトの保存
            m_lastLeadBytes[m_lastLeadBytesCount] = srcPos[0];
            ++m_lastLeadBytesCount;
            --m_requestLeadBytesCount;
        } else {
            m_lastLeadBytes[m_lastLeadBytesCount] = srcPos[0];
            ++m_lastLeadBytesCount;

            // 一度 UTF32 文字へ
            UTFConversionResult result;
            UTF32 ch;
            const UTF8* start = m_lastLeadBytes;
            const UTF8* end = start + m_lastLeadBytesCount;
            result = UnicodeUtils::convertCharUTF8toUTF32(&start, end, NULL, &ch);
            if (result != UTFConversionResult_Success)
                return false;

            // UTF32 から UTF16 へ
            UTF16* prev = dstPos;
            result = UnicodeUtils::convertCharUTF32toUTF16(ch, &dstPos, dstEnd, &options);
            if (result != UTFConversionResult_Success)
                return false;

            outResult->outputByteCount += (dstPos - prev) * sizeof(UTF16);
            ++outResult->outputCharCount;
            m_lastLeadBytesCount = 0;
        }

        ++srcPos;
        ++outResult->usedByteCount;
    }

    return true;
}

//==============================================================================
// UTF8Encoder

bool UTF8Encoding::UTF8Encoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    outResult->usedElementCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    // 変換設定
    UTFConversionOptions options;
    memset(&options, 0, sizeof(options));
    options.ReplacementChar = encoding()->fallbackReplacementChar();

    const UTF16* srcPos = input;
    const UTF16* srcEnd = input + inputElementSize;
    UTF8* dstPos = (UTF8*)output;
    UTF8* dstEnd = dstPos + outputByteSize;

    if (m_byteOrderMark) {
        memcpy(dstPos, UTF8Encoding::BOM, 3);
        dstPos += 3;
        outResult->outputByteCount += 3;
        m_bomWrited = true;
    }

    while (srcPos < srcEnd) {
        // サロゲートの確認
        if (UnicodeUtils::checkUTF16HighSurrogate(*srcPos)) {
            if (m_lastBufferCount == 0) {
                m_lastBuffer[m_lastBufferCount] = *srcPos;
                ++m_lastBufferCount;
            } else {
                // High が連続している
                return false;
            }
        } else {
            if (UnicodeUtils::checkUTF16LowSurrogate(*srcPos)) {
                if (m_lastBufferCount == 1) {
                } else {
                    // いきなり Low が来た
                    return false;
                }
            } else // 普通の文字
            {
                if (m_lastBufferCount == 0) {
                } else {
                    // Low が無かった
                    return false;
                }
            }
            m_lastBuffer[m_lastBufferCount] = *srcPos;
            ++m_lastBufferCount;

            // 一度 UTF-32 文字へ
            UTFConversionResult result;
            const UTF16* bufStart = reinterpret_cast<const UTF16*>(m_lastBuffer);
            const UTF16* bufEnd = bufStart + m_lastBufferCount;
            UTF32 ch;
            result = UnicodeUtils::convertCharUTF16toUTF32(&bufStart, bufEnd, &options, &ch);
            if (result != UTFConversionResult_Success)
                return false;

            // UTF-8 文字へ
            UTF8* prev = dstPos;
            result = UnicodeUtils::convertCharUTF32toUTF8(ch, &dstPos, dstEnd, &options);
            if (result != UTFConversionResult_Success)
                return false;

            m_lastBufferCount = 0;
            outResult->outputByteCount += (dstPos - prev);
            outResult->outputCharCount += 1;
        }

        ++srcPos;
        ++outResult->usedElementCount;
    }

    return true;
}

} // namespace ln
