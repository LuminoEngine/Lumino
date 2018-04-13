#include "Internal.hpp"
#include <sstream>
#include <Lumino/IO/FileStream.hpp>
#include <Lumino/IO/StreamReader.hpp>

namespace ln {

//==============================================================================
// StreamReader
//==============================================================================
//------------------------------------------------------------------------------
StreamReader::StreamReader(Stream* stream, TextEncoding* encoding)
{
	initReader(stream, encoding);
}

//------------------------------------------------------------------------------
StreamReader::StreamReader(const Char* filePath, TextEncoding* encoding)
{
	auto stream = FileStream::create(filePath, FileOpenMode::read);
	initReader(stream, encoding);
}

//------------------------------------------------------------------------------
StreamReader::~StreamReader()
{
}

//------------------------------------------------------------------------------
int StreamReader::peek()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (readBuffer() == 0) {
			return -1;
		}
	}
	const Char* buf = (const Char*)m_converter.getLastBuffer().getConstData();
	return buf[m_charPos];
}

//------------------------------------------------------------------------------
int StreamReader::read()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (readBuffer() == 0) {
			return -1;
		}
	}
	const Char* buf = (const Char*)m_converter.getLastBuffer().getConstData();
	return buf[m_charPos++];
}

//------------------------------------------------------------------------------
bool StreamReader::readLine(String* line)
{
	// 変換済みの文字列を全て返していれば (または初回)、次のバッファを読みに行く
	if (m_charPos == m_charElementLen)
	{
		if (readBuffer() == 0) {
			return false;	// EOF
		}
	}

	//StringBuilder builder;
	//std::basic_stringstream<Char> builder;
	m_readLineCache.clear();
	do
	{
		int i = m_charPos;
		do
		{
			const Char* buf = (const Char*)m_converter.getLastBuffer().getConstData();
			Char ch = buf[i];
			if (ch == '\r' || ch == '\n')
			{
				//builder.append(buf + m_charPos, i - m_charPos);
				m_readLineCache.append(buf + m_charPos, i - m_charPos);
				m_charPos = i + 1;

				// CR+LF 対応。条件式の中に ReadBuffer() があるが、これは現在のバッファ境界で \r と \n が切れている時の対策。
				if (ch == '\r' && (m_charPos < m_charElementLen || readBuffer() > 0)) {
					if (buf[m_charPos] == '\n') {
						m_charPos++;
					}
				}
				*line = m_readLineCache.c_str();// builder.toString();
				return true;
			}
			i++;
		} while (i < m_charElementLen);

		// ここに来るのは、charBuffer の現在位置 ～ 終端までに改行が無かったとき。
		// 現在の残りバッファを str に結合して、次のバッファを ReadBuffer() で読み出す。
		const Char* buf = (const Char*)m_converter.getLastBuffer().getConstData();
		//builder.append(buf + m_charPos, m_charElementLen - m_charPos);
		m_readLineCache.append(buf + m_charPos, m_charElementLen - m_charPos);

	} while (readBuffer() > 0);

	*line = m_readLineCache.c_str();//builder.toString();
	return true;
}

//------------------------------------------------------------------------------
String StreamReader::readToEnd()
{
	//GenericStringBuilder<Char> builder;
	m_readLineCache.clear();
	do
	{
		if (m_charElementLen - m_charPos > 0)
		{
			const Char* buf = (const Char*)m_converter.getLastBuffer().getConstData();
			//builder.append(buf + m_charPos, m_charElementLen - m_charPos);
			m_readLineCache.append(buf + m_charPos, m_charElementLen - m_charPos);
			m_charPos = m_charElementLen;
		}

	} while (readBuffer() > 0);

	return String(m_readLineCache.c_str());//builder.toString();
}

//------------------------------------------------------------------------------
bool StreamReader::isEOF()
{
	if (m_charPos < m_charElementLen) {
		return false;	// まだバッファリングされていて読まれていない文字がある
	}
	return (readBuffer() == 0);
}

//------------------------------------------------------------------------------
void StreamReader::initReader(Stream* stream, TextEncoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == nullptr) {
		encoding = TextEncoding::utf8Encoding();
	}

	m_stream = stream;
	m_converter.getSourceEncoding(encoding);
	m_converter.setDestinationEncoding(TextEncoding::getEncodingTemplate<Char>());
	m_byteBuffer.resize(DefaultBufferSize, false);
	m_byteLen = 0;
	m_charElementLen = 0;
	m_charPos = 0;
}

//------------------------------------------------------------------------------
// ストリームからバイト列を読み取って変換し、現在バッファリングされている文字要素数(THCAR)を返す。
int StreamReader::readBuffer()
{
	// TODO: BOM チェックするならここで。

	m_charPos = 0;
	m_charElementLen = 0;

	m_byteLen = m_stream->read(m_byteBuffer.getData(), m_byteBuffer.getSize());
	if (m_byteLen == 0) { return m_charElementLen; }

	// 文字コード変換 (ユーザー指定 → TChar)
	m_converter.convert(m_byteBuffer.getData(), m_byteLen);
	m_charElementLen = m_converter.getLastResult().BytesUsed / sizeof(Char);
	return m_charElementLen;
}

} // namespace ln
