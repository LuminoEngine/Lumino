
#include "Internal.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Base/UnicodeUtils.hpp>
#include "DBCSEncoding.hpp"

// SJIS
extern "C" const unsigned char g_SJISLeadBytePairs[];
extern "C" const unsigned short g_SJISToUTF16Table[];
extern "C" const unsigned short g_UTF16ToSJISTable[];

namespace ln {

struct TableInfo
{
	const String name;
	const unsigned char* leadBytePairs;		// size = LeadBytePairsSize (NULL の場合、この文字コードは DB ではない)
	const unsigned short* dbcsToUTF16Table;	// size = TableSize
	const unsigned short* utf16ToDBCSTable;	// size = TableSize
};

//==============================================================================
// DBCSEncoding


const TableInfo* DBCSEncoding::getTableInfo(EncodingType type)
{
	static TableInfo sjisInfo = { _LT("Shift_JIS"), g_SJISLeadBytePairs, g_SJISToUTF16Table, g_UTF16ToSJISTable };

	switch (type)
	{
	case ln::EncodingType::SJIS:
		return &sjisInfo;
	default:
		LN_UNREACHABLE();
		return nullptr;
	}
}

static bool CheckDBCSLeadByte(const TableInfo* info, byte_t byte)
{
	for (int i = 0; i < DBCSEncoding::LeadBytePairsSize / 2; ++i)
	{
		int pair = i * 2;
		if (info->leadBytePairs[pair] == 0x00) {
			return false;
		}
		// 先行バイトの範囲内であるか
		if (info->leadBytePairs[pair] <= byte && byte <= info->leadBytePairs[pair + 1]) {
			return true;
		}
	}
	return false;
}

DBCSEncoding::DBCSEncoding(EncodingType type)
	: m_tableInfo(getTableInfo(type))
{
	LN_CHECK(m_tableInfo);
}

const String& DBCSEncoding::name() const
{
	return m_tableInfo->name;
}

int DBCSEncoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
	int count = 0;
	const byte_t* pos = (const byte_t*)buffer;
	const byte_t* end = pos + bufferSize;
	while (pos < end)
	{
		if (CheckDBCSLeadByte(m_tableInfo, *pos)) {
			pos++;
		}
		pos++;
		count++;
	}
	return count;
}

int DBCSEncoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
	return (CheckDBCSLeadByte(m_tableInfo, *((const byte_t*)buffer))) ? 1 : 0;
}

#if LN_USTRING32

bool DBCSEncoding::convertToUTF32Stateless(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	DBCSDecoder decoder(this, m_tableInfo);
	return decoder.convertToUTF32(input, inputByteSize, output, outputElementSize, outResult);
}

bool DBCSEncoding::DBCSDecoder::convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	if (outputElementSize > 0) { output[0] = '\0'; }
	outResult->usedByteCount = 0;
	outResult->outputByteCount = 0;
	outResult->outputCharCount = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputByteSize == 0) { return false; }

	// 変換 (もし前回のバッファ終端が先行バイトだったら、m_lastLeadByte に先行バイトが入っている)
	size_t inBufPos = 0;	// MBCS
	size_t outBufPos = 0;	// UTF32
	for (; inBufPos < inputByteSize; ++inBufPos)
	{
		byte_t b = input[inBufPos];

		// 先行バイト未発見状態の場合
		if (m_lastLeadByte == 0x00)
		{
			if (b == 0x00) {
				break;	// \0 が見つかった。ここで終了
			}
			if (CheckDBCSLeadByte(m_tableInfo, b)) {
				m_lastLeadByte = b;	// 先行バイト発見状態にする
			}
			else {
				// シングルバイト文字。普通に変換する。
				output[outBufPos] = m_tableInfo->dbcsToUTF16Table[b];
				if (output[outBufPos] == 0x0000) return false;	// 不正文字
				++outBufPos;
			}
		}
		// 直前の文字が先行バイトの場合
		else
		{
			// 先行バイトが連続することもある。"＝" は 0x81 0x81 である。
			//if (CheckDBCSLeadByte(m_tableInfo, b)) {	// 先行バイトが連続で来るはず無い
			//	LN_THROW(0, EncodingFallbackException);	// 不正文字
			//}

			// マルチバイト文字。先行バイトを上位バイトにして変換する。
			output[outBufPos] = m_tableInfo->dbcsToUTF16Table[(m_lastLeadByte << 8) | (b & 0xFF)];
			if (output[outBufPos] == 0x0000) return false;	// 不正文字
			++outBufPos;
			m_lastLeadByte = 0x00;
		}
	}

	// もしバッファ終端が先行バイトであれば、この時点で m_lastLeadByte に先行バイトが入っている

	// DBCSEncoding ではサロゲートを持たないことが決まっているのでそのまま変換した文字数を出力できる。
	outResult->outputByteCount = outBufPos * sizeof(UTF32);	// 変換後のバイト数
	outResult->outputCharCount = outBufPos;					// 変換後の文字数
	return true;
}

bool DBCSEncoding::DBCSEncoder::convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
	if (outputByteSize > 0) { output[0] = '\0'; }
	outResult->usedElementCount = 0;
	outResult->outputByteCount = 0;
	outResult->outputCharCount = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputElementSize == 0) { return false; }

	// 変換
	size_t inBufPos = 0;	// UTF32
	size_t outBufPos = 0;	// MBCS
	for (; inBufPos < inputElementSize; ++inBufPos)
	{
		UTF32 ch = input[inBufPos];

		uint16_t dbBytes = m_tableInfo->utf16ToDBCSTable[ch];

		// シングルバイト文字
		if ((dbBytes & 0xFF00) == 0x0000) {
			output[outBufPos] = dbBytes & 0xFF;
			++outBufPos;
		}
		// マルチバイト文字
		else {
			output[outBufPos] = ((dbBytes & 0xFF00) >> 8);
			++outBufPos;
			output[outBufPos] = (dbBytes & 0xFF);
			++outBufPos;
		}
	}

	outResult->outputByteCount = outBufPos;
	outResult->outputCharCount = inBufPos;
	return true;
}
#else

bool DBCSEncoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	DBCSDecoder decoder(this, m_tableInfo);
	return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}

bool DBCSEncoding::DBCSDecoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
	if (outputElementSize > 0) { output[0] = '\0'; }
	outResult->usedByteCount = 0;
	outResult->outputByteCount = 0;
	outResult->outputCharCount = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputByteSize == 0) { return false; }

	// 変換 (もし前回のバッファ終端が先行バイトだったら、m_lastLeadByte に先行バイトが入っている)
	size_t inBufPos = 0;	// MBCS
	size_t outBufPos = 0;	// UTF16
	for (; inBufPos < inputByteSize; ++inBufPos)
	{
		byte_t b = input[inBufPos];

		// 先行バイト未発見状態の場合
		if (m_lastLeadByte == 0x00)
		{
			if (b == 0x00) {
				break;	// \0 が見つかった。ここで終了
			}
			if (CheckDBCSLeadByte(m_tableInfo, b)) {
				m_lastLeadByte = b;	// 先行バイト発見状態にする
			}
			else {
				// シングルバイト文字。普通に変換する。
				output[outBufPos] = m_tableInfo->dbcsToUTF16Table[b];
				if (output[outBufPos] == 0x0000) return false;	// 不正文字
				++outBufPos;
			}
		}
		// 直前の文字が先行バイトの場合
		else
		{
			// 先行バイトが連続することもある。"＝" は 0x81 0x81 である。
			//if (CheckDBCSLeadByte(m_tableInfo, b)) {	// 先行バイトが連続で来るはず無い
			//	LN_THROW(0, EncodingFallbackException);	// 不正文字
			//}

			// マルチバイト文字。先行バイトを上位バイトにして変換する。
			output[outBufPos] = m_tableInfo->dbcsToUTF16Table[(m_lastLeadByte << 8) | (b & 0xFF)];
			if (output[outBufPos] == 0x0000) return false;	// 不正文字
			++outBufPos;
			m_lastLeadByte = 0x00;
		}
	}

	// もしバッファ終端が先行バイトであれば、この時点で m_lastLeadByte に先行バイトが入っている

	// DBCSEncoding ではサロゲートを持たないことが決まっているのでそのまま変換した文字数を出力できる。
	outResult->outputByteCount = outBufPos * sizeof(UTF16);	// 変換後のバイト数
	outResult->outputCharCount = outBufPos;					// 変換後の文字数
	return true;
}

bool DBCSEncoding::DBCSEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
	if (outputByteSize > 0) { output[0] = '\0'; }
	outResult->usedElementCount = 0;
	outResult->outputByteCount = 0;
	outResult->outputCharCount = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputElementSize == 0) { return false; }

	// 変換
	size_t inBufPos = 0;	// UTF16
	size_t outBufPos = 0;	// MBCS
	for (; inBufPos < inputElementSize; ++inBufPos)
	{
		UTF16 ch = input[inBufPos];

		// サロゲートはエラー
		if (UnicodeUtils::checkUTF16HighSurrogate(ch) || UnicodeUtils::checkUTF16LowSurrogate(ch)) {
			return false;
		}

		uint16_t dbBytes = m_tableInfo->utf16ToDBCSTable[ch];

		// シングルバイト文字
		if ((dbBytes & 0xFF00) == 0x0000) {
			output[outBufPos] = dbBytes & 0xFF;
			++outBufPos;
		}
		// マルチバイト文字
		else {
			output[outBufPos] = ((dbBytes & 0xFF00) >> 8);
			++outBufPos;
			output[outBufPos] = (dbBytes & 0xFF);
			++outBufPos;
		}
	}

	outResult->outputByteCount = outBufPos;
	outResult->outputCharCount = inBufPos;
	return true;
}
#endif

} // namespace ln
