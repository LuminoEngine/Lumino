
#pragma once
#include "../Text/EncodingConverter.hpp"
#include "TextReader.hpp"

namespace ln {
class Stream;

/**
	@brief	特定のエンコーディングのストリームを文字列として読み込む TextReader です。
*/
class StreamReader
	: public TextReader
{
public:
	static const int DefaultBufferSize = 1024;

public:

	/**
		@brief		指定したストリーム用の StreamReader を初期化します。
		@param[in]	stream		: 読み込み元ストリーム
		@param[in]	encoding	: 読み込むテキストのエンコーディング
		@details	encoding が nullptr の場合は UTF8Encoding を使用します。
	*/
	StreamReader(Stream* stream, TextEncoding* encoding = nullptr);
	// TODO: UTF BOM 自動判別

	/**
		@brief		指定したパスのファイル用の StreamReader を初期化します。
		@param[in]	filePath	: 読み込み元ファイルのパス
		@param[in]	encoding	: 読み込むテキストのエンコーディング
		@details	encoding が NULL の場合は UTF8Encoding を使用します。
	*/
	StreamReader(const Char* filePath, TextEncoding* encoding = nullptr);

	~StreamReader();

public:
	Stream* getStream() const { return m_stream; }
	
public:
	// GenericTextReader interface
	virtual int peek();
	virtual int read();
	virtual bool readLine(String* line);
	virtual String readToEnd();
	virtual bool isEOF();

private:
	void initReader(Stream* stream, TextEncoding* encoding);
	int readBuffer();

	Ref<Stream>			m_stream;
	EncodingConverter		m_converter;
	ByteBuffer				m_byteBuffer;
	std::basic_string<Char>	m_readLineCache;
	int						m_byteLen;
	int						m_charElementLen;
	int						m_charPos;			///< 次に読むべき文字がある文字列バッファの位置。有効最大は m_charElementLen - 1
};

} // namespace ln
