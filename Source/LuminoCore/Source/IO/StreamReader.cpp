#include "../Internal.h"
#include <Lumino/Base/StringBuilder.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/StreamReader.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GenericStreamReader
//==============================================================================
//------------------------------------------------------------------------------
template<typename TChar>
GenericStreamReader<TChar>::GenericStreamReader(Stream* stream, Encoding* encoding)
{
	initReader(stream, encoding);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericStreamReader<TChar>::GenericStreamReader(const TChar* filePath, Encoding* encoding)
{
	auto stream = GenericFileStream<TChar>::create(filePath, FileOpenMode::read);
	initReader(stream, encoding);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericStreamReader<TChar>::~GenericStreamReader()
{
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericStreamReader<TChar>::peek()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (readBuffer() == 0) {
			return -1;
		}
	}
	const TChar* buf = (const TChar*)m_converter.getLastBuffer().getConstData();
	return buf[m_charPos];
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericStreamReader<TChar>::read()
{
	// バッファリングデータを最後まで読んでいた場合は追加読み込み。
	// それでも1つも読み込めなかったら EOF。
	if (m_charPos >= m_charElementLen)
	{
		if (readBuffer() == 0) {
			return -1;
		}
	}
	const TChar* buf = (const TChar*)m_converter.getLastBuffer().getConstData();
	return buf[m_charPos++];
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericStreamReader<TChar>::readLine(GenericString<TChar>* line)
{
	// 変換済みの文字列を全て返していれば (または初回)、次のバッファを読みに行く
	if (m_charPos == m_charElementLen)
	{
		if (readBuffer() == 0) {
			return false;	// EOF
		}
	}

	GenericStringBuilder<TChar> builder;
	do
	{
		int i = m_charPos;
		do
		{
			const TChar* buf = (const TChar*)m_converter.getLastBuffer().getConstData();
			TChar ch = buf[i];
			if (ch == '\r' || ch == '\n')
			{
				builder.append(buf + m_charPos, i - m_charPos);
				m_charPos = i + 1;

				// CR+LF 対応。条件式の中に ReadBuffer() があるが、これは現在のバッファ境界で \r と \n が切れている時の対策。
				if (ch == '\r' && (m_charPos < m_charElementLen || readBuffer() > 0)) {
					if (buf[m_charPos] == '\n') {
						m_charPos++;
					}
				}
				*line = builder.toString();
				return true;
			}
			i++;
		} while (i < m_charElementLen);

		// ここに来るのは、charBuffer の現在位置 ～ 終端までに改行が無かったとき。
		// 現在の残りバッファを str に結合して、次のバッファを ReadBuffer() で読み出す。
		const TChar* buf = (const TChar*)m_converter.getLastBuffer().getConstData();
		builder.append(buf + m_charPos, m_charElementLen - m_charPos);

	} while (readBuffer() > 0);

	*line = builder.toString();
	return true;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericStreamReader<TChar>::readToEnd()
{
	GenericStringBuilder<TChar> builder;
	do
	{
		if (m_charElementLen - m_charPos > 0)
		{
			const TChar* buf = (const TChar*)m_converter.getLastBuffer().getConstData();
			builder.append(buf + m_charPos, m_charElementLen - m_charPos);
			m_charPos = m_charElementLen;
		}

	} while (readBuffer() > 0);

	return builder.toString();
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericStreamReader<TChar>::isEOF()
{
	if (m_charPos < m_charElementLen) {
		return false;	// まだバッファリングされていて読まれていない文字がある
	}
	return (readBuffer() == 0);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStreamReader<TChar>::initReader(Stream* stream, Encoding* encoding)
{
	// encoding 未指定であれば UTF8 とする
	if (encoding == nullptr) {
		encoding = Encoding::getUTF8Encoding();
	}

	m_stream = stream;
	m_converter.getSourceEncoding(encoding);
	m_converter.setDestinationEncoding(Encoding::getEncodingTemplate<TChar>());
	m_byteBuffer.resize(DefaultBufferSize, false);
	m_byteLen = 0;
	m_charElementLen = 0;
	m_charPos = 0;
}

//------------------------------------------------------------------------------
// ストリームからバイト列を読み取って変換し、現在バッファリングされている文字要素数(THCAR)を返す。
template<typename TChar>
int GenericStreamReader<TChar>::readBuffer()
{
	// TODO: BOM チェックするならここで。

	m_charPos = 0;
	m_charElementLen = 0;

	m_byteLen = m_stream->read(m_byteBuffer.getData(), m_byteBuffer.getSize());
	if (m_byteLen == 0) { return m_charElementLen; }

	// 文字コード変換 (ユーザー指定 → TChar)
	m_converter.convert(m_byteBuffer.getData(), m_byteLen);
	m_charElementLen = m_converter.getLastResult().BytesUsed / sizeof(TChar);
	return m_charElementLen;
}

// instantiate template
template class GenericStreamReader<char>;
template class GenericStreamReader<wchar_t>;

LN_NAMESPACE_END
