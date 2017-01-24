#include "../Internal.h"
#include <Lumino/Base/StringBuilder.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/StreamReader.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// StreamReader
//==============================================================================
//------------------------------------------------------------------------------
StreamReader::StreamReader(Stream* stream, Encoding* encoding)
{
	InitReader(stream, encoding);
}

//------------------------------------------------------------------------------
StreamReader::StreamReader(const TCHAR* filePath, Encoding* encoding)
{
	RefPtr<FileStream> stream = FileStream::Create(filePath, FileOpenMode::Read);
	InitReader(stream, encoding);
}

//------------------------------------------------------------------------------
StreamReader::~StreamReader()
{
}

//------------------------------------------------------------------------------
int StreamReader::Peek()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (ReadBuffer() == 0) {
			return -1;
		}
	}
	const TCHAR* buf = (const TCHAR*)m_converter.GetLastBuffer().GetConstData();
	return buf[m_charPos];
}

//------------------------------------------------------------------------------
int StreamReader::Read()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (ReadBuffer() == 0) {
			return -1;
		}
	}
	const TCHAR* buf = (const TCHAR*)m_converter.GetLastBuffer().GetConstData();
	return buf[m_charPos++];
}

//------------------------------------------------------------------------------
bool StreamReader::ReadLine(String* line)
{
	// 変換済みの文字列を全て返していれば (または初回)、次のバッファを読みに行く
	if (m_charPos == m_charElementLen)
	{
		if (ReadBuffer() == 0) {
			return false;	// EOF
		}
	}

	StringBuilder builder;
	do
	{
		int i = m_charPos;
		do
		{
			const TCHAR* buf = (const TCHAR*)m_converter.GetLastBuffer().GetConstData();
			TCHAR ch = buf[i];
			if (ch == '\r' || ch == '\n')
			{
				builder.Append(buf + m_charPos, i - m_charPos);
				m_charPos = i + 1;

				// CR+LF 対応。条件式の中に ReadBuffer() があるが、これは現在のバッファ境界で \r と \n が切れている時の対策。
				if (ch == '\r' && (m_charPos < m_charElementLen || ReadBuffer() > 0)) {
					if (buf[m_charPos] == '\n') {
						m_charPos++;
					}
				}
				*line = builder.ToString();
				return true;
			}
			i++;
		} while (i < m_charElementLen);

		// ここに来るのは、charBuffer の現在位置 ～ 終端までに改行が無かったとき。
		// 現在の残りバッファを str に結合して、次のバッファを ReadBuffer() で読み出す。
		const TCHAR* buf = (const TCHAR*)m_converter.GetLastBuffer().GetConstData();
		builder.Append(buf + m_charPos, m_charElementLen - m_charPos);

	} while (ReadBuffer() > 0);

	*line = builder.ToString();
	return true;
}

//------------------------------------------------------------------------------
String StreamReader::ReadToEnd()
{
	StringBuilder builder;
	do
	{
		if (m_charElementLen - m_charPos > 0)
		{
			const TCHAR* buf = (const TCHAR*)m_converter.GetLastBuffer().GetConstData();
			builder.Append(buf + m_charPos, m_charElementLen - m_charPos);
			m_charPos = m_charElementLen;
		}

	} while (ReadBuffer() > 0);

	return builder.ToString();
}

//------------------------------------------------------------------------------
bool StreamReader::IsEOF()
{
	if (m_charPos < m_charElementLen) {
		return false;	// まだバッファリングされていて読まれていない文字がある
	}
	return (ReadBuffer() == 0);
}

//------------------------------------------------------------------------------
void StreamReader::InitReader(Stream* stream, Encoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == NULL) {
		encoding = Encoding::GetUTF8Encoding();
	}

	m_stream = stream;
	m_converter.SetSourceEncoding(encoding);
	m_converter.SetDestinationEncoding(Encoding::GetTCharEncoding());
	m_byteBuffer.Resize(DefaultBufferSize, false);
	m_byteLen = 0;
	m_charElementLen = 0;
	m_charPos = 0;
}

//------------------------------------------------------------------------------
// ストリームからバイト列を読み取って変換し、現在バッファリングされている文字要素数(THCAR)を返す。
//------------------------------------------------------------------------------
int StreamReader::ReadBuffer()
{
	// TODO: BOM チェックするならここで。

	m_charPos = 0;
	m_charElementLen = 0;

	m_byteLen = m_stream->Read(m_byteBuffer.GetData(), m_byteBuffer.GetSize());
	if (m_byteLen == 0) { return m_charElementLen; }

	// 文字コード変換 (ユーザー指定 → TCHAR)
	m_converter.Convert(m_byteBuffer.GetData(), m_byteLen);
	m_charElementLen = m_converter.GetLastResult().BytesUsed / sizeof(TCHAR);
	return m_charElementLen;
}

LN_NAMESPACE_END
