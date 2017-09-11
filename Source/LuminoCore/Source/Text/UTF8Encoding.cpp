
#include "../Internal.h"
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "UTF8Encoding.h"

LN_NAMESPACE_BEGIN
	
//==============================================================================
// UTF8Encoding
//==============================================================================

//------------------------------------------------------------------------------
UTF8Encoding::UTF8Encoding(bool byteOrderMark)
	: m_byteOrderMark(byteOrderMark)
{
}

//------------------------------------------------------------------------------
byte_t* UTF8Encoding::getPreamble() const
{
	static byte_t bom[] = { 0xEF, 0xBB, 0xBF, 0x00 };
	return bom;
}

//------------------------------------------------------------------------------
int UTF8Encoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
	int count;
	UTFConversionResult result = UnicodeUtils::getUTF8CharCount((const byte_t*)buffer, bufferSize, true, &count);
	if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return 0;
	return count;
}

//------------------------------------------------------------------------------
int UTF8Encoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
	int len;
	UTFConversionResult result = UnicodeUtils::checkUTF8TrailingBytes(((const byte_t*)buffer), ((const byte_t*)buffer) + bufferSize, true, &len);
	if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return 0;
	return len;
}

//------------------------------------------------------------------------------
void UTF8Encoding::UTF8Decoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	*outBytesUsed = 0;
	*outCharsUsed = 0;
	int bytesUsed = 0;
	int charsUsed = 0;

	// 変換設定
	UTFConversionOptions options;
	memset(&options, 0, sizeof(options));
	options.ReplacementChar = mFallbackReplacementChar;

	// BOM 付きの場合は取り除く (バッファ縮小)
	if (m_byteOrderMark)
	{
		if (m_bomPhase == 0)
		{
			if (input[0] == 0xEF)
			{
				m_bomPhase = 1;
				++input;
				--inputByteSize;
				if (inputByteSize == 0) { return; }
			}
		}
		if (m_bomPhase == 1)
		{
			if (input[0] == 0xBB)
			{
				m_bomPhase = 2;
				++input;
				--inputByteSize;
				if (inputByteSize == 0) { return; }
			}
			else
			{
				LN_ENSURE_ENCODING(0, _T("bom is required."));
				return;
			}
		}
		if (m_bomPhase == 2)
		{
			if (input[0] == 0xBF)
			{
				m_bomPhase = 0;
				++input;
				--inputByteSize;
				if (inputByteSize == 0) { return; }
			}
			else
			{
				LN_ENSURE_ENCODING(0, _T("bom is required."));
				return;
			}
		}
	}

	const byte_t* srcPos = input;
	const byte_t* srcEnd = input + inputByteSize;
	UTF16* dstPos = output;
	UTF16* dstEnd = dstPos + outputElementSize;
	while (srcPos < srcEnd)
	{
		if (LN_ENSURE_ENCODING(m_lastLeadBytesCount < 5, _T("Illegal UTF-8 char."))) return;

		// 先行バイトの確認
		if (m_requestLeadBytesCount == 0)
		{
			UnicodeUtils::checkUTF8TrailingBytes(srcPos, srcEnd, false, &m_requestLeadBytesCount);
			//m_requestLeadBytesCount++;	// ↓で今回の文字も含めて m_lastLeadBytes に保存したい
		}
		
		if (m_requestLeadBytesCount > 0)
		{
			// 先行バイトの保存
			m_lastLeadBytes[m_lastLeadBytesCount] = srcPos[0];
			++m_lastLeadBytesCount;
			--m_requestLeadBytesCount;
		}
		else
		{
			m_lastLeadBytes[m_lastLeadBytesCount] = srcPos[0];
			++m_lastLeadBytesCount;

			// 一度 UTF32 文字へ
			UTFConversionResult result;
			UnicodeUtils::UTF32 ch;
			const UnicodeUtils::UTF8* start = m_lastLeadBytes;
			const UnicodeUtils::UTF8* end = start + m_lastLeadBytesCount;
			result = UnicodeUtils::convertCharUTF8toUTF32(&start, end, NULL, &ch);
			if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return;

			// UTF32 から UTF16 へ 
			UTF16* prev = dstPos;
			result = UnicodeUtils::convertCharUTF32toUTF16(ch, &dstPos, dstEnd, &options);
			if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return;

			bytesUsed += (dstPos - prev) * sizeof(UTF16);
			++charsUsed;
			m_lastLeadBytesCount = 0;
		}

		++srcPos;
	}

	
	
	
	// 変換
	//UTFConversionResult result = UnicodeUtils::convertUTF8toUTF16(
	//	(UnicodeUtils::UTF8*)input, 
	//	inputByteSize,
	//	(UnicodeUtils::UTF16*)outBuffer, 
	//	outBufferCharCount,
	//	&options);
	//LN_THROW(result == UTFConversionResult_Success, EncodingException);

	// 出力
	*outBytesUsed = bytesUsed;
	*outCharsUsed = charsUsed;
}

//------------------------------------------------------------------------------
//int UTF8Encoding::UTF8Encoder::pollingChar(UTF8 ch, UTF16* outBuffer, size_t outBufferCharCount, size_t* outBytesUsed, size_t* outCharsUsed)
//{
//	
//}

//------------------------------------------------------------------------------
void UTF8Encoding::UTF8Encoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	*outCharsUsed = 0;
	*outBytesUsed = 0;

	// 変換設定
	UTFConversionOptions options;
	memset(&options, 0, sizeof(options));
	options.ReplacementChar = mFallbackReplacementChar;

	const UTF16* srcPos = input;
	const UTF16* srcEnd = input + inputElementSize;
	UTF8* dstPos = (UTF8*)output;
	UTF8* dstEnd = dstPos + outputByteSize;

	while (srcPos < srcEnd)
	{
		// サロゲートの確認
		if (UnicodeUtils::checkUTF16HighSurrogate(*srcPos))
		{
			if (m_lastBufferCount == 0)
			{
				m_lastBuffer[m_lastBufferCount] = *srcPos;
				++m_lastBufferCount;
			}
			else
			{
				// High が連続している
				LN_ENSURE_ENCODING(0);
				return;
			}
		}
		else
		{
			if (UnicodeUtils::checkUTF16LowSurrogate(*srcPos))
			{
				if (m_lastBufferCount == 1) {
				}
				else
				{
					// いきなり Low が来た
					LN_ENSURE_ENCODING(0);
					return;
				}
			}
			else // 普通の文字
			{
				if (m_lastBufferCount == 0) {
				}
				else
				{
					// Low が無かった
					LN_ENSURE_ENCODING(0);
					return;
				}
			}
			m_lastBuffer[m_lastBufferCount] = *srcPos;
			++m_lastBufferCount;


			// 一度 UTF-32 文字へ
			UTFConversionResult result;
			const UTF16* bufStart = m_lastBuffer;
			const UTF16* bufEnd = m_lastBuffer + m_lastBufferCount;
			UnicodeUtils::UTF32 ch;
			result = UnicodeUtils::convertCharUTF16toUTF32(&bufStart, bufEnd, &options, &ch);
			if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return;

			// UTF-8 文字へ
			UTF8* prev = dstPos;
			result = UnicodeUtils::convertCharUTF32toUTF8(ch, &dstPos, dstEnd, &options);
			if (LN_ENSURE_ENCODING(result == UTFConversionResult_Success)) return;

			m_lastBufferCount = 0;
			(*outBytesUsed) += (dstPos - prev);
			(*outCharsUsed) += 1;
		}

		++srcPos;
	}


	// 変換設定
	//UTFConversionOptions options;
	//memset(&options, 0, sizeof(options));
	//options.ReplacementChar = mFallbackReplacementChar;
	//
	//// 変換
	//UTFConversionResult result = UnicodeUtils::convertUTF16toUTF8(
	//	(UnicodeUtils::UTF16*)input, 
	//	inBufferCharCount,
	//	(UnicodeUtils::UTF8*)output, 
	//	outBufferByteCount,
	//	&options);
	//LN_THROW(result == UTFConversionResult_Success, EncodingException);

	// 出力
	//*outBytesUsed = options.ConvertedTargetLength;
	//*outCharsUsed = options.CharCount;
}

LN_NAMESPACE_END
