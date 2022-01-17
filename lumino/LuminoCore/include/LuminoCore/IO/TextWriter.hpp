// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once
#include "../Base/RefObject.hpp"
#include "../Base/Locale.hpp"
#include "../Base/String.hpp"
#include "../Text/Encoding.hpp"
#include "../Text/EncodingConverter.hpp"

namespace ln {

/** 各種データを文字列として書き込みを行う機能のベースクラスです。機能を使用するには StreamWriter や StringWriter を使用します。 */
class TextWriter
    : public RefObject
{
public:
    /** 文字を書き込みます。 */
    void write(char ch);
    void write(wchar_t ch);
    void write(char16_t ch);
    void write(char32_t ch);

    /** 整数値を文字列に変換して書き込みます。 */
    void write(int16_t value);
    void write(int32_t value);  /**< @copydoc write(int16_t) */
    void write(int64_t value);  /**< @copydoc write(int16_t) */
    void write(uint16_t value); /**< @copydoc write(int16_t) */
    void write(uint32_t value); /**< @copydoc write(int16_t) */
    void write(uint64_t value); /**< @copydoc write(int16_t) */

    /** 実数値を文字列に変換して書き込みます。 */
    void write(float value);
    void write(double value); /**< @copydoc write(float) */

    /** 文字列を書き込みます。 */
    void write(const StringRef& str);

    /** 書式指定された文字列を書き込みます。 */
    template<typename... TArgs>
    void writeFormat(const StringRef& format, const TArgs&... args);

    /** 改行を書き込みます。 */
    void writeLine();

    /**　文字を書き込み、続けて改行を書き込みます。 */
    void writeLine(char value);
    void writeLine(wchar_t value);
    void writeLine(char16_t value);
    void writeLine(char32_t value);

    /**　整数値を文字列に変換して書き込み、続けて改行を書き込みます。　*/
    void writeLine(int16_t value);
    void writeLine(int32_t value);  /**< @overload writeLine(int16_t) */
    void writeLine(int64_t value);  /**< @overload writeLine(int16_t) */
    void writeLine(uint16_t value); /**< @overload writeLine(int16_t) */
    void writeLine(uint32_t value); /**< @overload writeLine(int16_t) */
    void writeLine(uint64_t value); /**< @overload writeLine(int16_t) */

    /**　実数値を文字列に変換して書き込み、続けて改行を書き込みます。 */
    void writeLine(float value);
    void writeLine(double value); /**< @copydoc writeLine(float) */

    /** 文字列を書き込み、続けて改行を書き込みます。 */
    void writeLine(const StringRef& str);

    /** 書式指定された文字列を書き込み、続けて改行を書き込みます。 */
    template<typename... TArgs>
    void writeLineFormat(const StringRef& str, const TArgs&... args);

    /** バッファリングデータを全てストリームに書き出します。 */
    void flush();

    /** この TextWriter が出力する文字列のエンコーディングを取得します。 */
    TextEncoding* encoding() const;

    /** この TextWriter で使用する改行文字列を設定します。（既定値は環境に依存します） */
    void setNewLine(const String& newLine);

    /** この TextWriter で使用する改行文字列を取得します。 */
    const String& newLine() const;

    /** 文字列の書き込み時に使用する書式を表すロケールを指定します。 */
    void setFormatLocale(const Locale& locale);

    /** 文字列の書き込み時に使用する書式を表すロケールを取得します。 */
    const Locale& formatLocale() const;

protected:
    TextWriter();
    virtual ~TextWriter();

    /**
	 * この TextWriter が出力する文字列のエンコーディングを指定します。
	 * 設定する場合、初回の書き込みの前に設定する必要があります。途中から変更はできません。
	 */
    void setEncoding(TextEncoding* encoding);

    /** データの書き込み先を実装します。 */
    virtual void onWriteOverride(const void* data, size_t byteCount) = 0;

    /** バッファリングデータを全てストリームに書き出します。 */
    virtual void onFlush() = 0;

private:
    void writeInternal(const Char* str, int len);

    static const int BufferSize = 2048;
    EncodingConverter m_converter;
    String m_newLine;
    Locale m_locale;
    bool m_writtenPreamble;
};

template<typename... TArgs>
inline void TextWriter::writeFormat(const StringRef& format, const TArgs&... args)
{
    String s = ln::format(format, args...);
    writeInternal(s.c_str(), s.length());
}

template<typename... TArgs>
inline void TextWriter::writeLineFormat(const StringRef& str, const TArgs&... args)
{
    String s = ln::format(str, args...);
    writeInternal(s.c_str(), s.length());
    writeLine();
}

} // namespace ln
