
#include "../Internal.h"
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "UTF32Encoding.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UTF32Encoding
//==============================================================================

//------------------------------------------------------------------------------
UTF32Encoding::UTF32Encoding(bool bigEndian, bool byteOrderMark)
	: m_bigEndian(bigEndian)
	, m_byteOrderMark(byteOrderMark)
{
	if (m_bigEndian) {
		LN_NOTIMPLEMENTED();
	}
}

//------------------------------------------------------------------------------
byte_t* UTF32Encoding::getPreamble() const
{
	static byte_t bom[] = { 0x00 };
	LN_NOTIMPLEMENTED();
	return bom;
}

//------------------------------------------------------------------------------
int UTF32Encoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
	return bufferSize / sizeof(UTF32);
}

//------------------------------------------------------------------------------
void UTF32Encoding::UTF32Decoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 変換設定
	UTFConversionOptions options;
	memset(&options, 0, sizeof(options));
	options.ReplacementChar = mFallbackReplacementChar;

	// 前回途中で途切れたバイトがあれば先に変換する
	if (m_lastLeadBytesCount > 0) 
	{
		size_t req = 4 - m_lastLeadBytesCount;	// 追加要求バイト数
		byte_t buf[4];
		memcpy_s(buf, 4, m_lastLeadBytes, m_lastLeadBytesCount);
		memcpy_s(buf + m_lastLeadBytesCount, req, input, req);

		// 変換 (1文字だけ)
		UTFConversionResult result = UnicodeUtils::convertUTF32toUTF16((UnicodeUtils::UTF32*)buf, 1, (UnicodeUtils::UTF16*)output, outputElementSize, &options);
		LN_THROW(result == UTFConversionResult_Success, EncodingException);

		// バッファ先頭は消費した分だけ進め、バッファサイズは消費した分だけ縮める
		input += req;
		inputByteSize -= req;
		output += options.ConvertedTargetLength / sizeof(UnicodeUtils::UTF16);
		outputElementSize -= options.ConvertedTargetLength / sizeof(UnicodeUtils::UTF16);
		(*outBytesUsed) += options.ConvertedTargetLength * sizeof(UnicodeUtils::UTF16);
		(*outCharsUsed) += 1;
	}

	// バッファ終端で文字が途切れている場合は次回の変換に回すため記憶しておく
	m_lastLeadBytesCount = inputByteSize % 4;
	if (m_lastLeadBytesCount > 0)
	{
		memcpy_s(m_lastLeadBytes, 4, &input[inputByteSize - m_lastLeadBytesCount], m_lastLeadBytesCount);
		inputByteSize -= m_lastLeadBytesCount;
	}
	
	// 変換
	UTFConversionResult result = UnicodeUtils::convertUTF32toUTF16(
		(UnicodeUtils::UTF32*)input,
		inputByteSize / sizeof(UnicodeUtils::UTF32),
		(UnicodeUtils::UTF16*)output,
		outputElementSize,
		&options);
	LN_THROW(result == UTFConversionResult_Success, EncodingException);

	// 出力
	(*outBytesUsed) += options.ConvertedTargetLength * sizeof(UnicodeUtils::UTF16);
	(*outCharsUsed) += options.CharCount;
}

//------------------------------------------------------------------------------
void UTF32Encoding::UTF32Encoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 変換設定
	UTFConversionOptions options;
	memset(&options, 0, sizeof(options));
	options.ReplacementChar = mFallbackReplacementChar;

	// 前回途中で途切れたワードがあれば先に変換する
	if (m_lastLeadWord != 0x0000)
	{
		uint16_t buf[2] = { m_lastLeadWord, input[0] };

		// 変換 (サロゲートペアで1文字だけ)
		UTFConversionResult result = UnicodeUtils::convertUTF16toUTF32((UnicodeUtils::UTF16*)buf, 2, (UnicodeUtils::UTF32*)output, outputByteSize, &options);
		LN_THROW(result == UTFConversionResult_Success, EncodingException);

		// バッファ先頭は消費した分だけ進め、バッファサイズは消費した分だけ縮める
		size_t usedTargetBytes = options.ConvertedTargetLength * sizeof(UnicodeUtils::UTF32);
		input += 1;
		inputElementSize -= 1;
		output += usedTargetBytes;
		outputByteSize -= usedTargetBytes;
		(*outBytesUsed) += usedTargetBytes;
		(*outCharsUsed) += options.CharCount;
	}

	// 前回途中で途切れたワードがあれば先に変換する (とりあえずエンディアン考慮せず、サロゲートなら取っておく)
	if (UnicodeUtils::checkUTF16HighSurrogate(input[inputElementSize - 1])/* || UnicodeUtils::checkUTF16LowSurrogate(inBuffer[inputElementSize])*/)
	{
		m_lastLeadWord = input[inputElementSize];
		inputElementSize -= 1;
	}
	
	// 変換
	UTFConversionResult result = UnicodeUtils::convertUTF16toUTF32(
		(UnicodeUtils::UTF16*)input,
		inputElementSize,
		(UnicodeUtils::UTF32*)output,
		outputByteSize / sizeof(UnicodeUtils::UTF32),
		&options);
	LN_THROW(result == UTFConversionResult_Success, EncodingException);

	// 出力
	(*outBytesUsed) += options.ConvertedTargetLength * sizeof(UnicodeUtils::UTF32);
	(*outCharsUsed) += options.CharCount;
}

LN_NAMESPACE_END
