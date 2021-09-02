
#include "Internal.hpp"
#include <LuminoCore/Base/UnicodeUtils.hpp>
#include "UTF16Encoding.hpp"

namespace ln {

struct TableInfo;

//==============================================================================
// UTF16Encoding

// .NET に合わせてみる
const String UTF16Encoding::NameLE = _T("UTF-16");
const String UTF16Encoding::NameBE = _T("UTF-16BE");

UTF16Encoding::UTF16Encoding(bool bigEndian, bool byteOrderMark)
    : m_bigEndian(bigEndian)
    , m_byteOrderMark(byteOrderMark)
{
    if (m_bigEndian) {
        LN_NOTIMPLEMENTED();
    }
}

byte_t* UTF16Encoding::preamble() const
{
	if (m_byteOrderMark) {
		static byte_t bom[] = { 0xFF, 0xFE, 0x00 };	// little
		return bom;
	}
	else {
		return nullptr;
	}
};

int UTF16Encoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
    bool s;
    UTFConversionResult result = UnicodeUtils::checkUTF16Surrogate((const UTF16*)buffer, ((const UTF16*)buffer) + bufferSize / sizeof(UTF16), true, &s);
    if (LN_ENSURE(result == UTFConversionResult_Success))
        return 0;
    return (s) ? 1 : 0;
}

int UTF16Encoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
    int count;
    UTFConversionResult result = UnicodeUtils::getUTF16CharCount((const UTF16*)buffer, bufferSize / sizeof(UTF16), true, &count);
    if (LN_ENSURE(result == UTFConversionResult_Success))
        return 0;
    return count;
}

bool UTF16Encoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	UTF16Decoder decoder(this);
	return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}

//==============================================================================
// UTF16Decoder

bool UTF16Encoding::UTF16Decoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    outResult->usedByteCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    /* バイトストリームの UTF-16 から、内部文字コードの UTF-16 への変換となる。
	 * バイトストリームは、UTF-16 文字がバッファ終端で途切れる場合も考慮しなければならない。
	 */

    // 前回途中で途切れたバイトがあれば先に処理する (inBuffer の先頭バイトとつなげて curLead に格納)
    uint16_t curLead = 0x0000;
    if (m_lastLeadByte != 0x00) {
        byte_t* word = (byte_t*)&curLead;
        word[0] = m_lastLeadByte;
        word[1] = input[0];
        input++;         // 消費した分だけバッファを縮める
        inputByteSize--; // 消費した分だけバッファを縮める
        outResult->usedByteCount++;
        m_lastLeadByte = 0x00;
    }

    // 入力が奇数バイト。最後のバイトを取っておく
    if (inputByteSize % 2 != 0) {
        m_lastLeadByte = input[inputByteSize - 1];
        inputByteSize--; // 消費した分だけバッファを縮める
        outResult->usedByteCount++;
    }

        // 変換 (もし前回のバッファ終端が上位サロゲートだったら、m_lastLeadWord に先行バイトが入っている)
#ifndef SIZE_T_MAX
    const size_t SIZE_T_MAX = (size_t)-1;
#endif
    size_t inWordPos = (curLead != 0x0000) ? SIZE_T_MAX : 0; // MBCS
    size_t outWordPos = 0;                                   // UTF16
    size_t inWordCount = inputByteSize / 2;
    uint16_t* inWords = (uint16_t*)input;
    for (; inWordPos == SIZE_T_MAX || (inWordPos < inWordCount && outWordPos < outputElementSize);) {
        uint16_t ch = (inWordPos == SIZE_T_MAX) ? curLead : inWords[inWordPos]; // 途切れたバイトと結合した最初の1文字を考慮

        // 上位サロゲート未発見状態の場合
        if (m_lastLeadWord == 0x0000) {
            if (UnicodeUtils::checkUTF16HighSurrogate(ch)) {
                m_lastLeadWord = ch; // 上位サロゲート発見状態にする
            } else {
                // 普通のUTF16文字。普通に格納。
                output[outWordPos++] = ch;
                outResult->outputCharCount++;
                outResult->outputByteCount += 2;
            }
        }
        // 直前の文字が先行バイトの場合
        else {
            // 下位サロゲートが見つかれば格納
            if (UnicodeUtils::checkUTF16LowSurrogate(ch)) {
                output[outWordPos++] = m_lastLeadWord;
                output[outWordPos++] = ch;
                outResult->outputCharCount++;
                outResult->outputByteCount += 4;
                m_lastLeadWord = 0x0000;
            } else {
                // 下位サロゲート以外の文字はNG
                return false;
            }
        }

        // inWordPos を進める
        if (inWordPos == SIZE_T_MAX) {
            inWordPos = 0; // 一番最初の文字の処理が終わった。inWords を見始める
        } else {
            ++inWordPos;
            outResult->usedByteCount += 2;
        }
    }

    return true;
}

//==============================================================================
// UTF16Encoder

bool UTF16Encoding::UTF16Encoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    outResult->usedElementCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    const UTF16* itr = input;
    const UTF16* end = itr + inputElementSize;
    UTF16* outputUTF16 = (UTF16*)output;
    while (itr < end) {
        if (m_hiSurrogate == 0) {
            if (UnicodeUtils::checkUTF16HighSurrogate(*itr)) {
                m_hiSurrogate = *itr;
            } else if (UnicodeUtils::checkUTF16LowSurrogate(*itr)) {
                return false;
            } else {
                outputUTF16[outResult->outputCharCount] = *itr;
                outResult->outputCharCount += 1;
                outResult->outputByteCount += 2;
            }
        } else {
            if (UnicodeUtils::checkUTF16LowSurrogate(*itr)) {
                outputUTF16[outResult->outputCharCount] = m_hiSurrogate;
                outputUTF16[outResult->outputCharCount + 1] = *itr;
                outResult->outputCharCount += 2;
                outResult->outputByteCount += 4;
            } else {
                return false;
            }
        }

        itr++;
        outResult->usedElementCount++;
    }

    return true;
}

} // namespace ln
