
#include "../Internal.h"
#include "ASCIIEncoding.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// ASCIIEncoding
//==============================================================================

//------------------------------------------------------------------------------
ASCIIEncoding::ASCIIEncoding()
{
}

//------------------------------------------------------------------------------
void ASCIIEncoding::ASCIIDecoder::convertToUTF16(const byte_t* inBuffer, size_t inBufferByteCount, UTF16* outBuffer, size_t outBufferCharCount, size_t* outBytesUsed, size_t* outCharsUsed)
{
	for (size_t i = 0; i < inBufferByteCount; ++i)
	{
		byte_t ch = inBuffer[i];

		// 普通の ASCII 文字
		if (ch <= 0x7F) {
			outBuffer[i] = static_cast<UTF16>(ch);
		}
		// 代替文字が指定されていればそれを使う
		else if (mFallbackReplacementChar != 0x00) {
			outBuffer[i] = static_cast<UTF16>(mFallbackReplacementChar);
		}
		// ASCII 外で代替文字も無ければエラー
		else {
			LN_THROW(0, EncodingException);
		}
	}

	(*outBytesUsed) = inBufferByteCount * sizeof(UTF16);
	(*outCharsUsed) = inBufferByteCount;
}

//------------------------------------------------------------------------------
void ASCIIEncoding::ASCIIEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	for (size_t i = 0; i < inputElementSize; ++i)
	{
		UTF16 ch = input[i];

		// 普通の ASCII 文字
		if (ch <= 0x7F) {
			output[i] = static_cast<byte_t>(ch);
		}
		// 代替文字が指定されていればそれを使う
		else if (mFallbackReplacementChar != 0x00) {
			output[i] = static_cast<byte_t>(mFallbackReplacementChar);
		}
		// ASCII 外で代替文字も無ければエラー
		else {
			LN_THROW(0, EncodingException);
		}
	}

	(*outBytesUsed) = inputElementSize;	// 文字数がそのままバイト数
	(*outCharsUsed) = inputElementSize;
}

LN_NAMESPACE_END
