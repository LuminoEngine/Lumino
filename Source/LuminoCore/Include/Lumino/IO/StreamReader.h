
#pragma once
#include "../Text/Encoding.h"
#include "../Text/EncodingConverter.h"
#include "Stream.h"
#include "TextReader.h"

LN_NAMESPACE_BEGIN

/**
	@brief	特定のエンコーディングのストリームを文字列として読み込む TextReader です。
*/
template<typename TChar>
class GenericStreamReader
	: public GenericTextReader<TChar>
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
	GenericStreamReader(Stream* stream, Encoding* encoding = nullptr);
	// TODO: UTF BOM 自動判別

	/**
		@brief		指定したパスのファイル用の StreamReader を初期化します。
		@param[in]	filePath	: 読み込み元ファイルのパス
		@param[in]	encoding	: 読み込むテキストのエンコーディング
		@details	encoding が NULL の場合は UTF8Encoding を使用します。
	*/
	GenericStreamReader(const TChar* filePath, Encoding* encoding = nullptr);

	~GenericStreamReader();

public:
	Stream* getStream() const { return m_stream; }
	
public:
	// GenericTextReader interface
	virtual int peek();
	virtual int read();
	virtual bool readLine(GenericString<TChar>* line);
	virtual GenericString<TChar> readToEnd();
	virtual bool isEOF();

private:
	void initReader(Stream* stream, Encoding* encoding);
	int readBuffer();

	RefPtr<Stream>			m_stream;
	EncodingConverter		m_converter;
	ByteBuffer				m_byteBuffer;
	int						m_byteLen;
	int						m_charElementLen;
	int						m_charPos;			///< 次に読むべき文字がある文字列バッファの位置。有効最大は m_charElementLen - 1
};

using StreamReader = GenericStreamReader<TCHAR>;

LN_NAMESPACE_END
