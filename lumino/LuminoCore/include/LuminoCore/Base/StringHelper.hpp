// Copyright (c) 2018+ lriki. Distributed under the MIT license..

#pragma once

#include "Common.hpp"

namespace ln {

/** 数値変換の結果 */
enum class NumberConversionResult
{
	Success = 0,
	ArgsError,
	FormatError,
	Overflow,
};

/** 文字列ユーティリティ */
class StringHelper
{
public:
	template<typename TChar>
	static size_t strlen(const TChar* str)
	{
		LN_DCHECK(str);
		const TChar* s;
		for (s = str; *s; ++s);
		return (s - str);
	}

	template<class TChar>
	static void strcpy(TChar* dst, size_t dstLen, const TChar* src)
	{
		LN_DCHECK(dst);
		LN_DCHECK(src);
		if (dstLen == 0) return;
		dstLen--;
		while (dstLen && *src)
		{
			dstLen--;
			*dst++ = *src++;
		}
		*dst = '\0';
	}

#ifdef va_start
	static int vsprintf(char* out, int charCount, const char* format, va_list args);
	static int vsprintf(wchar_t* out, int charCount, const wchar_t* format, va_list args);
	static int vsprintf2(char* out, int charCount, const char* format, ...);
	static int vsprintf2(wchar_t* out, int charCount, const wchar_t* format, ...);
#endif

	template<typename TChar>
	static int indexOf(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive);

	template<typename TChar>
	static int lastIndexOf(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int startIndex, int count, CaseSensitivity cs);

	template<typename TChar>
	static int compare(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int count, CaseSensitivity cs = CaseSensitivity::CaseSensitive);
	template<typename TChar>
	static int compare(const TChar* str1, const TChar* str2, int count, CaseSensitivity cs = CaseSensitivity::CaseSensitive);
	template<typename TChar>
	static int compare(TChar ch1, TChar ch2, CaseSensitivity cs = CaseSensitivity::CaseSensitive);

	template<typename TChar>
	static void trim(const TChar* begin, int length, const TChar** outBegin, int* outLength);

	template<typename TChar>
	static bool startsWith(const TChar* str1, int len1, const TChar* str2, int len2, CaseSensitivity cs);

	template<typename TChar>
	static bool endsWith(const TChar* str1, int len1, const TChar* str2, int len2, CaseSensitivity cs);

	template<typename TChar>
	static void substr(const TChar* str, int len, int start, int count, const TChar** outBegin, const TChar** outEnd);

	template<typename TChar>
	static void left(const TChar* str, int count, const TChar** outBegin, const TChar** outEnd);

	template<typename TChar>
	static void right(const TChar* str, int count, const TChar** outBegin, const TChar** outEnd);


	// not affected locale
	template<typename TChar>
	static TChar toUpper(TChar ch);

	// not affected locale
	template<typename TChar>
	static TChar toLower(TChar ch);

	// not affected locale
	template<typename TChar>
	static bool isSpace(TChar ch) { return (0 < ch && ch <= 255) ? (isspace(ch) != 0) : false; }

	template<typename TChar>
	static bool isNullOrEmpty(const TChar* str) { return !str || str[0] == '\0'; }

	template<typename TChar, typename TLookuped>
	static void SplitHelper(const TChar* begin, const TChar* end, const TChar* delim, int delimLen, StringSplitOptions option, CaseSensitivity cs, TLookuped callback)
	{
		if (LN_REQUIRE(begin != nullptr)) return;
		if (LN_REQUIRE(end != nullptr)) return;
		if (LN_REQUIRE(begin <= end)) return;
		if (LN_REQUIRE(delim != nullptr)) return;

		delimLen = (delimLen < 0) ? static_cast<int>(strlen(delim)) : delimLen;
		const TChar* cur = begin;
		const TChar* tokenBegin = begin;

		while (cur < end)
		{
			if (*cur == *delim && (end - cur >= delimLen) && compare(cur, delim, delimLen, cs) == 0)
			{
				if (option == StringSplitOptions::RemoveEmptyEntries && tokenBegin == cur)
				{
					// 空文字列を無視する
				}
				else
				{
					callback(tokenBegin, cur);
				}
				cur += delimLen;
				tokenBegin = cur;
			}
			else
			{
				cur++;
			}
		}

		// 最後のトークン or デリミタが1つもないとき の分
		if (option == StringSplitOptions::RemoveEmptyEntries && tokenBegin == cur)
		{
			// 空文字列を無視する
		}
		else
		{
			callback(tokenBegin, cur);
		}
	}

	/**
		@brief		ワイルドカード('*', '?')を考慮して文字列を比較する
		@param[in]	pattern  : ワイルドカードを含むパターン文字列
		@param[in]	str  : 比較対象の文字列
	*/
	template<typename TChar>
	static bool match(const TChar* pattern, const TChar* str);

	/**
		@brief		文字列を整数値に変換します。
		@param[in]	str			: 変換元の文字列
		@param[in]	len			: str の文字数 (-1 を指定すると \0 まで変換する)
		@param[in]	base		: 基数 (0、2、8、10、16 のいずれかであること)
		@param[in]	outEndPtr	: スキャンの終了位置を格納する変数のポインタ
		@param[in]	outResult	: エラーコードを格納する変数のポインタ
		@return		変換結果の数値
		@details	次の書式に従い、文字列を数値に変換します。<br>
					[whitespace] [{+ | – }] [0 [{ x | X }]] [digits | letters]		<br>
					16 進数値のアルファベットは大文字と小文字を区別しません。		<br><br>

					基数に 0 を指定すると、文字列の先頭文字から基数を自動判別します。<br>
					"0x" または "0X" であれば 16 進数、"0" であれば 8 進数、それ以外であれば 10 進数です。
					基数に 8 または 16 が指定されている際、文字列の先頭は "0" または "0x" である必要はありません。<br><br>

					outEndPtr は C言語の strtol() のように使用します。
					この値が str+len と一致しなければ、文字列が全て読み取られなかったことを示します。
	*/
	template<typename TChar>
	static int8_t toInt8(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static uint8_t toUInt8(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static int16_t toInt16(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static uint16_t toUInt16(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static int32_t toInt32(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static uint32_t toUInt32(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static int64_t toInt64(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	/// @copydoc toInt8
	template<typename TChar>
	static uint64_t toUInt64(const TChar* str, int len = -1, int base = 0, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);

	/**
		@brief		文字列を実数値に変換します。
		@details	次の書式に従い、文字列を数値に変換します。	<br>
					[whitespace] [sign] [digits] [.digits] [ {e | E }[sign]digits]	<br><br>

					この変換処理はロケールに依存せず、'.' を小数点とみなします。
					※標準関数の strtod はロケールに依存し、例えばフランス語として設定されている場合、',' を小数点として認識してしまいます。('.' はエラーとなります)
					JSON 等の '.' が小数点として定められている文字列を変換する場合、この関数を使用します。
	*/
	template<typename TChar>
	static double toDouble(const TChar* str, int len = -1, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);
	
	template<typename TChar>
	static float toFloat(const TChar* str, int len = -1, const TChar** outEndPtr = NULL, NumberConversionResult* outResult = NULL);

	static int int64ToString(int64_t value, char format, char* outStr, int bufSize);

	static int uint64ToString(uint64_t value, char format, char* outStr, int bufSize);

	static int doubleToString(double value, char format, int precision, char* outStr, int bufSize);

	template<class TDst, class TSrc>
	static void copySimpleAsciiString(TDst* dst, int dstLen, const TSrc* src, int srcLen)
	{
		while (dstLen && srcLen && *src)
		{
			dstLen--;
			srcLen--;
			*dst++ = static_cast<TDst>(*src++);
		}
	}
};


namespace detail
{

template<int N>
bool getLSB(unsigned char byte)
{
	return byte & (1u << N);
}

template<int N>
void setLSB(unsigned char& byte, bool bit)
{
	if (bit) {
		byte |= 1u << N;
	}
	else {
		byte &= ~(1u << N);
	}
}

enum class UStringRefSource
{
	ByUChar,
	ByChar,
};

class UStringHelper
{
public:
	static size_t strlen(const Char* str);
	static int compare(const Char* str1, const Char* str2);

	template<typename TChar>
	static void toStringInt8(int8_t v, TChar* outStr, int size);
};


class UStringConvert
{
public:
	//static std::basic_string<TCHAR> toStdTString(const Char* str);
	// len: \0 を含まないよう素数
	// size: \0 を含む要素数
	// return: \0 を含まない変換後の要素数
	static int convertNativeString(const char* src, int srcLen, char* dst, int dstSize);
	static int convertNativeString(const char* src, int srcLen, wchar_t* dst, int dstSize);
	static int convertNativeString(const wchar_t* src, int srcLen, char* dst, int dstSize);
	static int convertNativeString(const wchar_t* src, int srcLen, wchar_t* dst, int dstSize);
	static int convertNativeString(const char16_t* src, int srcLen, char* dst, int dstSize);
	static int convertNativeString(const char16_t* src, int srcLen, wchar_t* dst, int dstSize);

	static void convertToStdString(const char* src, int srcLen, std::string* outString);
	static void convertToStdString(const char* src, int srcLen, std::wstring* outString);
	static void convertToStdString(const wchar_t* src, int srcLen, std::string* outString);
	static void convertToStdString(const wchar_t* src, int srcLen, std::wstring* outString);
	static void convertToStdString(const char16_t* src, int srcLen, std::string* outString);
	static void convertToStdString(const char16_t* src, int srcLen, std::wstring* outString);
	/*
	static int getMaxNativeStringConverLength(const char* src, int srcLen, const char* dst);
	static int getMaxNativeStringConverLength(const char* src, int srcLen, const wchar_t* dst);
	static int getMaxNativeStringConverLength(const wchar_t* src, int srcLen, const char* dst);
	static int getMaxNativeStringConverLength(const wchar_t* src, int srcLen, const wchar_t* dst);
	static int getMaxNativeStringConverLength(const char16_t* src, int srcLen, const char* dst);
	static int getMaxNativeStringConverLength(const char16_t* src, int srcLen, const wchar_t* dst);*/
};


} // namespace detail

} // namespace ln

