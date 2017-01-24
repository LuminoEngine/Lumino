
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
	void SetEncoding(Encoding* encoding);

	/**
		@brief		この TextWriter が出力する文字列のエンコーディングを取得します。
	*/
	Encoding* GetEncoding() const;

	/**
		@brief		この TextWriter で使用する改行文字列を設定します。
		@details	規定値は String::GetNewLine() で取得できる値です。
	*/
	void SetNewLine(const String& newLine);

	/**
		@brief		文字列の書き込み時に使用する書式を表すロケールを指定します。
	*/
	void SetFormatLocale(const Locale& locale);

	/**
		@brief		文字を書き込みます。
		@param[in]	ch		: 書き込む文字
	*/
	void Write(TCHAR ch);

	/**
		@brief		整数値を文字列に変換して書き込みます。
		@param[in]	value	: 書き込む値
	*/
	void Write(int16_t value);
	void Write(int32_t value);			/**< @copydoc Write(int16_t) */
	void Write(int64_t value);			/**< @copydoc Write(int16_t) */
	void Write(uint16_t value);			/**< @copydoc Write(int16_t) */
	void Write(uint32_t value);			/**< @copydoc Write(int16_t) */
	void Write(uint64_t value);			/**< @copydoc Write(int16_t) */

	/**
		@brief		実数値を文字列に変換して書き込みます。
		@param[in]	value	: 書き込む値
		@attention	小数点文字はロケールに依存します。(例えばフランス語では ',' になります)
					必ず '.' で書き込まれるようにするには、SetFormatLocale() で ANSI ロケールを指定してください。
	*/
	void Write(float value);
	void Write(double value);			/**< @copydoc Write(float) */


	void Write(const StringRef& str);
	void Write(const TCHAR* str, int length);

	template<typename... TArgs>
	void WriteFormat(const StringRef& str, const TArgs&... args) { String s = String::Format(str, args...); WriteInternal(s.c_str(), s.GetLength()); }

	/**
		@brief		改行を書き込みます。
	*/
	void WriteLine();

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
	void WriteLine(TCHAR ch);

	/**
		@brief		整数値を文字列に変換して書き込み、続けて改行を書き込みます。
		@param[in]	value	: 書き込む値
	*/
	void WriteLine(int16_t value);
	void WriteLine(int32_t value);		/**< @overload WriteLine(int16_t) */
	void WriteLine(int64_t value);		/**< @overload WriteLine(int16_t) */
	void WriteLine(uint16_t value);		/**< @overload WriteLine(int16_t) */
	void WriteLine(uint32_t value);		/**< @overload WriteLine(int16_t) */
	void WriteLine(uint64_t value);		/**< @overload WriteLine(int16_t) */

	/**
		@brief		実数値を文字列に変換して書き込み、続けて改行を書き込みます。
		@param[in]	value		: 書き込む値
		@attention	小数点文字はロケールに依存します。(例えばフランス語では ',' になります)
					必ず '.' で書き込まれるようにするには、SetFormatLocale() で ANSI ロケールを指定してください。
	*/
	void WriteLine(float value);
	void WriteLine(double value);		/**< @copydoc WriteLine(float) */

	void WriteLine(const StringRef& str) { WriteInternal(str.GetBegin(), str.GetLength()); WriteLine(); }

	template<typename... TArgs>
	void WriteLine(const StringRef& str, const TArgs&... args) { String s = String::Format(str, args...); WriteInternal(s.c_str(), s.GetLength()); WriteLine(); }

	/**
		@brief		バッファリングデータを全てストリームに書き出します。
	*/
	virtual void Flash() = 0;

protected:

	/**
		@brief		データの書き込み先を実装します。
	*/
	virtual void WriteOverride(const void* data, size_t byteCount) = 0;

private:
	void WriteInternal(const TCHAR* str, int len);

private:
	static const int	BufferSize = 2048;
	EncodingConverter	m_converter;
	String				m_newLine;
	Locale				m_locale;
	bool				m_writtenPreamble;
};

LN_NAMESPACE_END
