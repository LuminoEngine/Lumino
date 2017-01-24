
#include <stdio.h>
#include <string.h>
#include "../Internal.h"
#include "../../include/Lumino/Base/String.h"
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "../../include/Lumino/Text/EncodingDetector.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Encoding
//==============================================================================

//------------------------------------------------------------------------------
EncodingType EncodingDetector::Detect(const void* bytes, size_t bytesSize)
{
	if (bytes == NULL || bytesSize == 0) {
		m_type = EncodingType::Unknown;
		return m_type;
	}
	m_buffer = (const Byte*)bytes;
	m_bufferSize = bytesSize;

	// UTF 系の BOM チェック
	EncodingType type = CheckUTFBom();
	if (type != EncodingType::Unknown) {
		m_type = type;
		return m_type;
	}

	// ASCII チェック
	if (CheckASCII()) {
		m_type = EncodingType::ASCII;
		return m_type;
	}

	// マルチバイト文字コードのチェック
	SJISDetector sjisDetector(m_buffer, m_bufferSize);
	UTF8NDetector utf8NDetector(m_buffer, m_bufferSize);
	IMBSCodeDetector* mbsDetectors[] = {
		&utf8NDetector,
		&sjisDetector,
	};
	const int mbsDetectorsCount = LN_ARRAY_SIZE_OF(mbsDetectors);
	for (int i = 0; i < mbsDetectorsCount; ++i)
	{
		mbsDetectors[i]->Detect(true);
	}

	// 有効 Detector の中で一番スコアの大きいものを選択する
	int maxScore = 0;
	IMBSCodeDetector* maxScoreDetector = NULL;
	for (int i = 0; i < mbsDetectorsCount; ++i)
	{
		if (mbsDetectors[i]->GetUnMatchCount() == 0)
		{
			// まだひとつも見つかっていなければ、とりあえずマッチしたものを保持する
			if (maxScoreDetector == NULL)
			{
				maxScoreDetector = mbsDetectors[i];
			}
			// 現在保持しているものよりスコアが大きければそれを保持する
			else if (mbsDetectors[i]->GetScore() > maxScoreDetector->GetScore())
			{
				maxScoreDetector = mbsDetectors[i];
			}
		}
	}
	if (maxScoreDetector != NULL) {
		m_type = maxScoreDetector->GetEncodingType();
		return m_type;
	}

	m_type = EncodingType::Unknown;	// 判別失敗
	return m_type;
}

//------------------------------------------------------------------------------
bool EncodingDetector::CheckASCII() const
{
	const Byte* bufferEnd = m_buffer + m_bufferSize;
	for (size_t pos = 0; pos < m_bufferSize; ++pos)
	{
		byte_t b1 = m_buffer[pos];
		if (b1 <= 0x7F)	// ASCII (0x00-0x7F)
		{
			int n = StringTraits::CheckNewLineSequence(&m_buffer[pos], bufferEnd);
			if (n > 0) {
				pos += n - 1;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
EncodingType EncodingDetector::CheckUTFBom()
{
	/* UTF8		: 0xEF, 0xBB, 0xBF
	 * UTF16	: 0xFF, 0xFE
	 * UTF16Big	: 0xFE, 0xFF
	 * UTF32	: 0xFF, 0xFE, 0x00, 0x00
	 * UTF32Big	: 0x00, 0x00, 0xFE, 0xFF
	 */

	struct BomData
	{
		EncodingType	Type;
		Byte			Bom[4];
		size_t			Length;
	};
	BomData data[] = 
	{
		{ EncodingType::UTF8,	{ 0xEF, 0xBB, 0xBF, 0x00 }, 3 },
		{ EncodingType::UTF32L,	{ 0xFF, 0xFE, 0x00, 0x00 }, 4 },	// UTF16Little と同じなので先に判別しないとダメ
		{ EncodingType::UTF32B,	{ 0x00, 0x00, 0xFE, 0xFF }, 4 },
		{ EncodingType::UTF16L,	{ 0xFF, 0xFE, 0x00, 0x00 }, 2 },
		{ EncodingType::UTF16B,	{ 0xFE, 0xFF, 0x00, 0x00 }, 2 },
	};
	for (int i = 0; i < 5; i++)
	{
		if (data[i].Length <= m_bufferSize &&
			memcmp(m_buffer, data[i].Bom, data[i].Length) == 0)
		{
			return data[i].Type;
		}
	}

	return EncodingType::Unknown;	// BOM が無かった
}


//==============================================================================
// UTF8NDetector
//==============================================================================

//------------------------------------------------------------------------------
UTF8NDetector::UTF8NDetector(const void* bytes, size_t bytesSize)
	: m_buffer((byte_t*)bytes)
	, m_bufferSize(bytesSize)
	, m_pos(0)
	, m_lineNum(0)
	, m_score(0)
	, m_unmatch(0)
{
}

//------------------------------------------------------------------------------
void UTF8NDetector::Detect(bool untilUnmatch)
{
	byte_t* bufferEnd = m_buffer + m_bufferSize;
	for (; m_pos < m_bufferSize; ++m_pos)
	{
		byte_t b1 = m_buffer[m_pos];
		if (b1 <= 0x7F)	// ASCII (0x00-0x7F)
		{
			int n = StringTraits::CheckNewLineSequence(&m_buffer[m_pos], bufferEnd);
			if (n > 0) {
				m_pos += n - 1;
				++m_lineNum;
			}
		}
		else
		{
			int extra;	// 追加で読むべきバイト数。pos の分は含まない
			if (UnicodeUtils::CheckUTF8TrailingBytes(&m_buffer[m_pos], bufferEnd, true, &extra) == UTFConversionResult_Success)
			{
				m_pos += extra;
				m_score += 1 + extra;
			}
			else {
				m_unmatch++;
			}
		}

		// 不正文字が見つかったので終了
		if (untilUnmatch && m_unmatch > 0) { break; }
	}
}

//==============================================================================
// SJISDetector
//==============================================================================

//------------------------------------------------------------------------------
SJISDetector::SJISDetector(const void* bytes, size_t bytesSize)
	: m_buffer((byte_t*)bytes)
	, m_bufferSize(bytesSize)
	, m_pos(0)
	, m_lineNum(0)
	, m_score(0)
	, m_unmatch(0)
{
}

//------------------------------------------------------------------------------
void SJISDetector::Detect(bool untilUnmatch)
{
	for (; m_pos < m_bufferSize; ++m_pos)
	{
		byte_t b1 = m_buffer[m_pos];
		if (b1 <= 0x7F ||	// ASCII (0x00-0x7F)
			b1 == 0x80 ||	// cp932 では 0x80 のマッピングが存在する。グリフがあるわけではないが…。。
			b1 == 0xA0 ||	// cp932
			b1 == 0xFD ||	// cp932
			b1 == 0xFE ||	// cp932
			b1 == 0xFF)		// cp932
		{
			int n = StringTraits::CheckNewLineSequence(&m_buffer[m_pos], m_buffer + m_bufferSize);
			if (n > 0) {
				m_pos += n - 1;
				++m_lineNum;
			}
		}
		else if (b1 >= 0xA1 && b1 <= 0xDF) // 1バイト半角カナ
		{
			++m_score;
		}
		else if (m_pos + 1 < m_bufferSize)
		{
			byte_t b2 = m_buffer[m_pos];
			if ((0x81 <= b1 && b1 <= 0x9F) ||		// 先行バイト
				(0xE0 <= b1 && b1 <= 0xFC))
			{
				if ((0x40 <= b2 && b2 <= 0x7E) ||	// 2byte目
					(0x80 <= b2 && b2 <= 0xFC))
				{
					++m_pos;
					m_score += 2;
				}
				else {
					m_unmatch++;
				}
			}
			else {
				m_unmatch++;
			}
		}
		else { // バッファ末尾でマルチバイト文字が途切れている
			m_unmatch++;
		}

		// 不正文字が見つかったので終了
		if (untilUnmatch && m_unmatch > 0) { break; }
	}
}

LN_NAMESPACE_END

