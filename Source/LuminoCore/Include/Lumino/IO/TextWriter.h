
#pragma once
#include "../Base/RefObject.h"
#include "../Base/ByteBuffer.h"
#include "../Base/Locale.h"
#include "../Base/String.h"
#include "../Text/Encoding.h"
#include "../Text/EncodingConverter.h"

LN_NAMESPACE_BEGIN

/**
	@brief		各種データを文字列として書き込みを行う機能のベースクラスです。機能を使用するには StreamWriter や StringWriter を使用します。
*/
class TextWriter
	: public RefObject
{
public:
	TextWriter();
	virtual ~TextWriter();
	
public:
	
	/**
		@brief		この TextWriter が出力する文字列のエンコーディングを指定します。
		@details	設定する場合、初回の書き込みの前に設定する必要があります。途中から変更はできません。
	*/
	void setEncoding(Encoding* encoding);

	/**
		@brief		この TextWriter が出力する文字列のエンコーディングを取得します。
	*/
	Encoding* getEncoding() const;

	/**
		@brief		この TextWriter で使用する改行文字列を設定します。
		@details	規定値は String::GetNewLine() で取得できる値です。
	*/
	void setNewLine(const String& newLine);

	/**
		@brief		文字列の書き込み時に使用する書式を表すロケールを指定します。
	*/
	void setFormatLocale(const Locale& locale);

	/**
		@brief		文字を書き込みます。
		@param[in]	ch		: 書き込む文字
	*/
	void write(TCHAR ch);

	/**
		@brief		整数値を文字列に変換して書き込みます。
		@param[in]	value	: 書き込む値
	*/
	void write(int16_t value);
	void write(int32_t value);			/**< @copydoc Write(int16_t) */
	void write(int64_t value);			/**< @copydoc Write(int16_t) */
	void write(uint16_t value);			/**< @copydoc Write(int16_t) */
	void write(uint32_t value);			/**< @copydoc Write(int16_t) */
	void write(uint64_t value);			/**< @copydoc Write(int16_t) */

	/**
		@brief		実数値を文字列に変換して書き込みます。
		@param[in]	value	: 書き込む値
		@attention	小数点文字はロケールに依存します。(例えばフランス語では ',' になります)
					必ず '.' で書き込まれるようにするには、SetFormatLocale() で ANSI ロケールを指定してください。
	*/
	void write(float value);
	void write(double value);			/**< @copydoc Write(float) */


	void write(const StringRef& str);
	void write(const TCHAR* str, int length);

	template<typename... TArgs>
	void writeFormat(const StringRef& str, const TArgs&... args) { String s = String::Format(str, args...); writeInternal(s.c_str(), s.getLength()); }

	/**
		@brief		改行を書き込みます。
	*/
	void writeLine();

	/**
		@brief		文字列を書き込み、続けて改行を書き込みます。
		@param[in]	str		: 書き込む文字列
		@param[in]	len		: 書き込む文字数 (-1 の場合は \0 まで)
	*/
	//void WriteLine(const StringRef& str);
	//void WriteLine(const String& str);
	
	/**
		@brief		文字を書き込みます。
		@param[in]	ch		: 書き込む文字
	*/
	void writeLine(TCHAR ch);

	/**
		@brief		整数値を文字列に変換して書き込み、続けて改行を書き込みます。
		@param[in]	value	: 書き込む値
	*/
	void writeLine(int16_t value);
	void writeLine(int32_t value);		/**< @overload WriteLine(int16_t) */
	void writeLine(int64_t value);		/**< @overload WriteLine(int16_t) */
	void writeLine(uint16_t value);		/**< @overload WriteLine(int16_t) */
	void writeLine(uint32_t value);		/**< @overload WriteLine(int16_t) */
	void writeLine(uint64_t value);		/**< @overload WriteLine(int16_t) */

	/**
		@brief		実数値を文字列に変換して書き込み、続けて改行を書き込みます。
		@param[in]	value		: 書き込む値
		@attention	小数点文字はロケールに依存します。(例えばフランス語では ',' になります)
					必ず '.' で書き込まれるようにするには、SetFormatLocale() で ANSI ロケールを指定してください。
	*/
	void writeLine(float value);
	void writeLine(double value);		/**< @copydoc WriteLine(float) */

	void writeLine(const StringRef& str) { writeInternal(str.getBegin(), str.getLength()); writeLine(); }

	template<typename... TArgs>
	void writeLine(const StringRef& str, const TArgs&... args) { String s = String::Format(str, args...); writeInternal(s.c_str(), s.getLength()); writeLine(); }

	/**
		@brief		バッファリングデータを全てストリームに書き出します。
	*/
	virtual void flash() = 0;

protected:

	/**
		@brief		データの書き込み先を実装します。
	*/
	virtual void writeOverride(const void* data, size_t byteCount) = 0;

private:
	void writeInternal(const TCHAR* str, int len);

private:
	static const int	BufferSize = 2048;
	EncodingConverter	m_converter;
	String				m_newLine;
	Locale				m_locale;
	bool				m_writtenPreamble;
};

LN_NAMESPACE_END
