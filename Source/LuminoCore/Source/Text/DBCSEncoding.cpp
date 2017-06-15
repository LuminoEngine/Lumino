
#include "../Internal.h"
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "DBCSEncoding.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// DBCSEncoding
//==============================================================================

// SJIS
extern "C" const unsigned char g_SJISLeadBytePairs[];
extern "C" const unsigned short g_SJISToUTF16Table[];
extern "C" const unsigned short g_UTF16ToSJISTable[];

// GB2312
extern "C" const unsigned char g_GB2312LeadBytePairs[];
extern "C" const unsigned short g_GB2312ToUTF16Table[];
extern "C" const unsigned short g_UTF16ToGB2312Table[];

// EUC-KR
extern "C" const unsigned char g_EUCKRLeadBytePairs[];
extern "C" const unsigned short g_EUCKRToUTF16Table[];
extern "C" const unsigned short g_UTF16ToEUCKRTable[];

// Big5
extern "C" const unsigned char g_Big5LeadBytePairs[];
extern "C" const unsigned short g_Big5ToUTF16Table[];
extern "C" const unsigned short g_UTF16ToBig5Table[];

const DBCSEncoding::TableInfo DBCSEncoding::Tables[(const int)EncodingType::TERMINATOR] =
{
	{ NULL, NULL, NULL, NULL },	// EncodingType_Unknown
	{ NULL, NULL, NULL, NULL },	// EncodingType_ASCII
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF8,
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF8N,
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF16L,
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF16B,
	//{ NULL, NULL, NULL, NULL },	// EncodingType_UTF16LN,
	//{ NULL, NULL, NULL, NULL },	// EncodingType_UTF16BN,
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF32L,
	{ NULL, NULL, NULL, NULL },	// EncodingType_UTF32B,
	//{ NULL, NULL, NULL }	// EncodingType_UTF32LN,
	//{ NULL, NULL, NULL }	// EncodingType_UTF32BN,

	{ _T("Shift_JIS"), g_SJISLeadBytePairs, g_SJISToUTF16Table, g_UTF16ToSJISTable },		// EncodingType_SJIS,
	{ _T("GB_2312-80"), g_GB2312LeadBytePairs, g_GB2312ToUTF16Table, g_UTF16ToGB2312Table },	// EncodingType_GB2312
	{ _T("EUC-KR"), g_EUCKRLeadBytePairs, g_EUCKRToUTF16Table, g_UTF16ToEUCKRTable },		// EncodingType_EUCKR
	{ _T("Big5"), g_Big5LeadBytePairs, g_Big5ToUTF16Table, g_UTF16ToBig5Table },		// EncodingType_BIG5
};

//------------------------------------------------------------------------------
static bool CheckDBCSLeadByte(const DBCSEncoding::TableInfo* info, byte_t byte)
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

//------------------------------------------------------------------------------
DBCSEncoding::DBCSEncoding(EncodingType type)
	: m_encodingType(EncodingType::Unknown)
{
	LN_THROW(Tables[(int)type].leadBytePairs != NULL, ArgumentException);	// DBEncoding としては使えない
	m_encodingType = type;
}

//------------------------------------------------------------------------------
int DBCSEncoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
	int count = 0;
	const byte_t* pos = (const byte_t*)buffer;
	const byte_t* end = pos + bufferSize;
	const TableInfo* tableInfo = &Tables[(int)m_encodingType];
	while (pos < end)
	{
		if (CheckDBCSLeadByte(tableInfo, *pos)) {
			pos++;
		}
		pos++;
		count++;
	}
	return count;
}

//------------------------------------------------------------------------------
int DBCSEncoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
	return (CheckDBCSLeadByte(&Tables[(int)m_encodingType], *((const byte_t*)buffer))) ? 1 : 0;
}

//------------------------------------------------------------------------------
void DBCSEncoding::DBCSDecoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	if (outputElementSize > 0) { output[0] = '\0'; }
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputByteSize == 0) { return; }

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
				LN_THROW(output[outBufPos] != 0x0000, EncodingException);	// 不正文字
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
			LN_THROW(output[outBufPos] != 0x0000, EncodingException);	// 不正文字
			++outBufPos;
			m_lastLeadByte = 0x00;
		}
	}

	// もしバッファ終端が先行バイトであれば、この時点で m_lastLeadByte に先行バイトが入っている

	// DBCSEncoding ではサロゲートを持たないことが決まっているのでそのまま変換した文字数を出力できる。
	*outBytesUsed = outBufPos * sizeof(UTF16);	// 変換後のバイト数
	*outCharsUsed = outBufPos;					// 変換後の文字数
}

//------------------------------------------------------------------------------
void DBCSEncoding::DBCSEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	if (outputByteSize > 0) { output[0] = '\0'; }
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 入力が 0 文字の場合は何もしない (変換の必要なし)
	if (inputElementSize == 0) { return; }

	// 変換
	size_t inBufPos = 0;	// UTF16
	size_t outBufPos = 0;	// MBCS
	for (; inBufPos < inputElementSize; ++inBufPos)
	{
		UTF16 ch = input[inBufPos];

		// サロゲートはエラー
		if (UnicodeUtils::checkUTF16HighSurrogate(ch) || UnicodeUtils::checkUTF16LowSurrogate(ch)) {
			LN_THROW(0, EncodingException);
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

	*outBytesUsed = outBufPos;
	*outCharsUsed = inBufPos;
}

LN_NAMESPACE_END
