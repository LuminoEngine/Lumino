
#include "Internal.hpp"
#include <LuminoCore/Base/UnicodeUtils.hpp>
#include "UTF32Encoding.hpp"

namespace ln {

//==============================================================================
// UTF32Encoding

const String UTF32Encoding::NameLE = _T("UTF-32LE");
const String UTF32Encoding::NameBE = _T("UTF-32BE");

UTF32Encoding::UTF32Encoding(bool bigEndian, bool byteOrderMark)
    : m_bigEndian(bigEndian)
    , m_byteOrderMark(byteOrderMark)
{
    if (m_bigEndian) {
        LN_NOTIMPLEMENTED();
    }
}

byte_t* UTF32Encoding::preamble() const
{
	if (m_byteOrderMark) {
		static byte_t bom[] = { 0x00 };
		LN_NOTIMPLEMENTED();
		return bom;
	}
	else {
		return nullptr;
	}
}

int UTF32Encoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
    return static_cast<int>(bufferSize / sizeof(UTF32));
}
#if LN_USTRING32
bool UTF32Encoding::convertToUTF32Stateless(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    UTF32Decoder decoder(this);
    return decoder.convertToUTF32(input, inputByteSize, output, outputElementSize, outResult);
}

//==============================================================================
// UTF32Decoder

bool UTF32Encoding::UTF32Decoder::convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    outResult->usedByteCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    //UTFConversionOptions options;
    //memset(&options, 0, sizeof(options));
    //options.ReplacementChar = encoding()->fallbackReplacementChar();

    const byte_t* iItr = input;
    const byte_t* iEnd = iItr + inputByteSize;
    UTF32* oItr = (UTF32*)output;
    UTF32* oEnd = oItr + outputElementSize;
    while (iItr < iEnd) {
        if (m_lastLeadBytesCount < 4) {
            m_lastLeadBytes[m_lastLeadBytesCount] = *iItr;
            m_lastLeadBytesCount++;
        }
        if (m_lastLeadBytesCount == 4) {
            //UTF16* local = outputItr;
            //UTFConversionResult result = UnicodeUtils::convertCharUTF32toUTF16(
            //    *((UTF32*)m_lastLeadBytes),
            //    &outputItr, // advance itr position
            //    outputEnd,
            //    &options);
            //if (result != UTFConversionResult_Success)
            //    return false;

            *oItr = *((UTF32*)m_lastLeadBytes);
            oItr++;

            m_lastLeadBytesCount = 0;

            outResult->outputByteCount += sizeof(UTF32);
            outResult->outputCharCount += 1;
        }

        iItr++;
        outResult->usedByteCount++;
    }

    return true;
}

//==============================================================================
// UTF32Encoder

bool UTF32Encoding::UTF32Encoder::convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    memcpy(output, input, inputElementSize * sizeof(UTF32));

    outResult->usedElementCount = inputElementSize;
    outResult->outputByteCount = inputElementSize * sizeof(UTF32);
    outResult->outputCharCount = inputElementSize;

    return true;
}
#else
bool UTF32Encoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	UTF32Decoder decoder(this);
	return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}

//==============================================================================
// UTF32Decoder

bool UTF32Encoding::UTF32Decoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    outResult->usedByteCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    UTFConversionOptions options;
    memset(&options, 0, sizeof(options));
    options.ReplacementChar = encoding()->fallbackReplacementChar();

    const byte_t* itr = input;
    const byte_t* end = itr + inputByteSize;
    UTF16* outputItr = (UTF16*)output;
    UTF16* outputEnd = outputItr + outputElementSize;
    while (itr < end) {
        if (m_lastLeadBytesCount < 4) {
            m_lastLeadBytes[m_lastLeadBytesCount] = *itr;
            m_lastLeadBytesCount++;
        }
        if (m_lastLeadBytesCount == 4) {
            UTF16* local = outputItr;
            UTFConversionResult result = UnicodeUtils::convertCharUTF32toUTF16(
                *((UTF32*)m_lastLeadBytes),
                &outputItr, // advance itr position
                outputEnd,
                &options);
            if (result != UTFConversionResult_Success)
                return false;

            m_lastLeadBytesCount = 0;

            outResult->outputByteCount += (outputItr - local) * sizeof(UTF16);
            outResult->outputCharCount += 1;
        }

        itr++;
        outResult->usedByteCount++;
    }

    return true;
}

//==============================================================================
// UTF32Encoder

bool UTF32Encoding::UTF32Encoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    outResult->usedElementCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    // 変換設定
    UTFConversionOptions options;
    memset(&options, 0, sizeof(options));
    options.ReplacementChar = encoding()->fallbackReplacementChar();

    const UTF16* itr = input;
    const UTF16* end = itr + inputElementSize;
    UTF32* outputItr = (UTF32*)output;
    while (itr < end) {
        if (m_hiSurrogate == 0) {
            if (UnicodeUtils::checkUTF16HighSurrogate(*itr)) {
                m_hiSurrogate = *itr;
            } else if (UnicodeUtils::checkUTF16LowSurrogate(*itr)) {
                return false;
            } else {
                const UTF16* s = itr;
                UTFConversionResult result = UnicodeUtils::convertCharUTF16toUTF32(
                    &s, s + 1, &options, outputItr);
                if (result != UTFConversionResult_Success)
                    return false;
                outResult->outputByteCount += 4;
                outResult->outputCharCount += 1;
                outputItr++;
            }
        } else {
            if (UnicodeUtils::checkUTF16LowSurrogate(*itr)) {
                UTF16 buf[2] = {m_hiSurrogate, *itr};
                const UTF16* s = buf;
                UTFConversionResult result = UnicodeUtils::convertCharUTF16toUTF32(
                    &s, s + 2, &options, outputItr);
                if (result != UTFConversionResult_Success)
                    return false;

                m_hiSurrogate = 0;

                outResult->outputByteCount += 4;
                outResult->outputCharCount += 1;
                outputItr++;
            } else {
                return false;
            }
        }

        itr++;
        outResult->usedElementCount++;
    }

    return true;
}
#endif

} // namespace ln
