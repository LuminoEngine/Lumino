
#pragma once
#include "../Base/ByteBuffer.h"
#include "Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		テキスト間のエンコーディングの変換を行うクラスです。
	@details	同じエンコーディングで何回も変換する際、効率的に変換を行うことができます。
*/
class EncodingConverter
{
public:
	EncodingConverter();
	~EncodingConverter();

public:

	/**
		@brief		変換先テキストのエンコーディングを設定します。
	*/
	void SetDestinationEncoding(Encoding* encoding);

	/**
		@brief		変換先テキストのエンコーディングを取得します。
	*/
	Encoding* GetDestinationEncoding() const;

	/**
		@brief		変換元テキストのエンコーディングを設定します。
	*/
	void SetSourceEncoding(Encoding* encoding);

	/**
		@brief		変換元テキストのエンコーディングを取得します。
	*/
	Encoding* GetSourceEncoding() const;

	/**
		@brief		変換のオプションを設定します。
	*/
	void SetConversionOptions(const EncodingConversionOptions& options);

	/**
		@brief		テキストを変換します。
	*/
	const ByteBuffer& Convert(const void* data, size_t byteCount, EncodingConversionResult* outResult = NULL);

	/**
		@brief		最後に呼び出した Convert() で変換されたバッファを取得します。これは Convert() の戻り値と同一です。
	*/
	const ByteBuffer& GetLastBuffer() const;

	/**
		@brief		最後に呼び出した Convert() の EncodingConversionResult を取得します。
	*/
	const EncodingConversionResult& GetLastResult() const;

private:
	void CheckUpdateEncoderDecoder();

	void ConvertDecoderRemain(
		const void* src_, size_t srcByteCount, Decoder* srcDecoder,
		void* dest_, size_t destByteCount, Encoder* destEncoder,
		EncodingConversionResult* outResult);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(EncodingConverter);
	Encoding*					m_dstEncoding;
	Encoding*					m_srcEncoding;
	Encoder*					m_dstEncoder;
	Decoder*					m_srcDecoder;
	ByteBuffer					m_outputBuffer;
	EncodingConversionOptions	m_options;
	EncodingConversionResult	m_lastResult;
	ByteBuffer					m_tmpBuffer;			///< 状態を保持できないデコーダを使っている場合に使用する一時バッファ
	bool						m_encodingModified;
};

LN_NAMESPACE_END
