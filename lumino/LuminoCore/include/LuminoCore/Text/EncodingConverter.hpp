// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include "../Base/Buffer.hpp"
#include "Encoding.hpp"

namespace ln {

/** テキスト間のエンコーディングの変換を行うクラスです。同じエンコーディングで何回も変換する際、効率的に変換を行うことができます。 */
class EncodingConverter
{
public:
	EncodingConverter();
	~EncodingConverter();

	/** 変換先テキストのエンコーディングを設定します。 */
	void setDestinationEncoding(TextEncoding* encoding);

	/** 変換先テキストのエンコーディングを取得します。 */
	TextEncoding* getDestinationEncoding() const;

	/** 変換元テキストのエンコーディングを設定します。 */
	void getSourceEncoding(TextEncoding* encoding);

	/** 変換元テキストのエンコーディングを取得します。 */
	TextEncoding* getSourceEncoding() const;

	/** 変換のオプションを設定します。 */
	void setConversionOptions(const EncodingConversionOptions& options);

	/** テキストを変換します。 */
	const ByteBuffer& convert(const void* data, size_t byteCount, EncodingConversionResult* outResult = nullptr);

	/** 最後に呼び出した convert() で変換されたバッファを取得します。これは convert() の戻り値と同一です。 */
	const ByteBuffer& getLastBuffer() const;

	/** 最後に呼び出した convert() の EncodingConversionResult を取得します。 */
	const EncodingConversionResult& getLastResult() const;

private:
	void checkUpdateEncoderDecoder();

	void convertDecoderRemain(
		const void* src_, size_t srcByteCount, TextDecoder* srcDecoder,
		void* dest_, size_t destByteCount, TextEncoder* destEncoder,
		EncodingConversionResult* outResult);

	LN_DISALLOW_COPY_AND_ASSIGN(EncodingConverter);
	TextEncoding*					m_dstEncoding;
	TextEncoding*					m_srcEncoding;
	TextEncoder*					m_dstEncoder;
	TextDecoder*					m_srcDecoder;
	ByteBuffer					m_outputBuffer;
	EncodingConversionOptions	m_options;
	EncodingConversionResult	m_lastResult;
	ByteBuffer					m_tmpBuffer;			///< 状態を保持できないデコーダを使っている場合に使用する一時バッファ
	bool						m_encodingModified;
};

} // namespace ln
