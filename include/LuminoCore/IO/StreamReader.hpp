// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "../Text/EncodingConverter.hpp"
#include "Stream.hpp"
#include "TextReader.hpp"

namespace ln {

/** 
 * 特定のエンコーディングのストリームを文字列として読み込む TextReader です。 
 *
 * エンコーディングを明示しない場合、ファイルは UTF-8 テキストとして読み込まれます。
 * ただし明示した場合でも、UTF 系の BOM を検出した場合は BOM が示すエンコーディングが優先されます。
 */
class StreamReader
    : public TextReader
{
public:
    /**
	 * 指定したストリーム用の StreamReader を構築します。
	 *
	 * @param[in]	stream		: 読み込み元ストリーム
	 * @param[in]	encoding	: 読み込むテキストのエンコーディング (null の場合は UTF8Encoding を使用します)
	 */
    StreamReader(Stream* stream, TextEncoding* encoding = nullptr);

    /**
	 * 指定したパスのファイルから読み込むの StreamReader を構築します。
	 *
	 * @param[in]	filePath	: 読み込み元ファイルのパス
	 * @param[in]	encoding	: 読み込むテキストのエンコーディング (null の場合は UTF8Encoding を使用します)
	 */
    StreamReader(const StringRef& filePath, TextEncoding* encoding = nullptr);

    virtual ~StreamReader();

    Stream* stream() const { return m_stream; }

    virtual int peek() override;
    virtual int read() override;
    virtual bool readLine(String* line) override;
    virtual String readToEnd() override;
    virtual bool isEOF() override;

private:
    void initReader(Stream* stream, TextEncoding* encoding);
    int readBuffer();

    static const int DefaultBufferSize = 1024;

    Ref<Stream> m_stream;
    EncodingConverter m_converter;
    ByteBuffer m_byteBuffer;
    std::basic_string<Char> m_readLineCache;
    int m_byteLen;
    int m_charElementLen;
    int m_charPos; // The position of the string buffer where the next character to read is located. max is m_charElementLen - 1
    bool m_initial;
};

} // namespace ln
