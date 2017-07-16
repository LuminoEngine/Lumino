
#pragma once
#include "Stream.h"
#include "PathName.h"
#include "TextWriter.h"

LN_NAMESPACE_BEGIN

/**
	@brief		文字列を特定のエンコーディングでストリームに書き込むための TextWriter の実装です。
	@section	ファイルに1行ずつ書き込み、UTF-8 として保存する
	~~~~~~~~~~~~~~~{.cpp}
	StreamWriter writer(_T("file"), Encoding::getUTF8Encoding());
	writer.WriteLine(_T("text"));
	writer.WriteLine(10);
	writer.WriteLine(3.14);
	~~~~~~~~~~~~~~~
*/
class StreamWriter
	: public TextWriter
{
public:

	/**
		@brief		指定したストリーム用の StreamWriter を初期化します。
		@param[in]	filePath	: 書き込み先ファイルのパス
		@param[in]	encoding	: 書き込み時に使用するエンコーディング
		@details	encoding が NULL の場合は文字列の書き込み時に文字コードのデコードを行いません。
	*/
	StreamWriter(Stream* stream, Encoding* encoding = NULL);

	/**
		@brief		パス名で指定したファイル用の StreamWriter を初期化します。
		@param[in]	filePath	: 書き込み先ファイルのパス
		@param[in]	encoding	: 書き込み時に使用するエンコーディング
		@param[in]	mode		: ファイルのオープンモード
		@details	encoding が NULL の場合は文字列の書き込み時に文字コードのデコードを行いません。
	*/
	StreamWriter(const PathName& filePath, Encoding* encoding = NULL, FileWriteMode mode = FileWriteMode_Truncate);

	virtual ~StreamWriter();
	
private:
	void init(Stream* stream, Encoding* encoding);
	virtual void flash();
	virtual void writeOverride(const void* data, size_t byteCount);

private:
	Ref<Stream>	m_stream;
};

LN_NAMESPACE_END
