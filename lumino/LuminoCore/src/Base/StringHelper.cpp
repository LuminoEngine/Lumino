
#include <errno.h>
#include <stdarg.h>
#include <wchar.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <LuminoCore/Base/Assertion.hpp>
#include <LuminoCore/Base/StringHelper.hpp>
#include <LuminoCore/Base/StlHelper.hpp>

#ifdef LN_MSVC
#else
#include <xlocale.h>
#endif

namespace ln {

//==============================================================================
// StringHelper

#ifdef _WIN32
int StringHelper::vsprintf(char* out, int charCount, const char* format, va_list args) { return _vsnprintf_s(out, charCount, _TRUNCATE, format, args); }
int StringHelper::vsprintf(wchar_t* out, int charCount, const wchar_t* format, va_list args) { return vswprintf(out, charCount, format, args); }
#else
int StringHelper::vsprintf(char* out, int charCount, const char* format, va_list args) { return vsnprintf(out, charCount, format, args); }
int StringHelper::vsprintf(wchar_t* out, int charCount, const wchar_t* format, va_list args)
{
	LN_NOTIMPLEMENTED();	// vswprintf は動作保障無し
	return vswprintf(out, charCount, format, args);
}
#endif

int StringHelper::vsprintf2(char* out, int charCount, const char* format, ...)
{
	va_list list;
	va_start(list, format);
	int r = vsprintf(out, charCount, format, list);
	va_end(list);
	return r;
}

int StringHelper::vsprintf2(wchar_t* out, int charCount, const wchar_t* format, ...)
{
	va_list list;
	va_start(list, format);
	int r = vsprintf(out, charCount, format, list);
	va_end(list);
	return r;
}


//template<typename TChar>
//bool StringHelper::isSpace(TChar ch)
//{
//	return isspace((uint8_t)ch) != 0;
//}
//template bool StringHelper::isSpace(UTF8 ch);
//template bool StringHelper::isSpace(UTF16 ch);
//template bool StringHelper::isSpace(UTF32 ch);




template<typename TChar>
void StringHelper::trim(const TChar* begin, int length, const TChar** outBegin, int* outLength)
{
	if (LN_ENSURE(begin != nullptr)) return;
	if (LN_ENSURE(length >= 0)) return;
	if (LN_ENSURE(outBegin != nullptr)) return;
	if (LN_ENSURE(outLength != nullptr)) return;

	if (length == 0) {
		*outBegin = begin;
		*outLength = 0;
		return;
	}

	const TChar* end = begin + length;

	// Left 部分
	while (*begin)
	{
		if (!isSpace(*begin)) {
			break;
		}
		++begin;
	}

	// Right 部分
	while (begin < end)
	{
		if (!isSpace(*(end - 1))) {
			break;
		}
		--end;
	}

	*outBegin = begin;
	*outLength = (int)(end - begin);
}
template void StringHelper::trim<char>(const char* begin, int length, const char** outBegin, int* outLength);
template void StringHelper::trim<wchar_t>(const wchar_t* begin, int length, const wchar_t** outBegin, int* outLength);
template void StringHelper::trim<Char>(const Char* begin, int length, const Char** outBegin, int* outLength);

template<typename TChar>
bool StringHelper::startsWith(const TChar* str1, int len1, const TChar* str2, int len2, CaseSensitivity cs)
{
	return compare(str1, str2, len2, cs) == 0;
}
template bool StringHelper::startsWith<char>(const char* str1, int len1, const char* str2, int len2, CaseSensitivity cs);
template bool StringHelper::startsWith<wchar_t>(const wchar_t* str1, int len1, const wchar_t* str2, int len2, CaseSensitivity cs);
template bool StringHelper::startsWith<Char>(const Char* str1, int len1, const Char* str2, int len2, CaseSensitivity cs);

template<typename TChar>
bool StringHelper::endsWith(const TChar* str1, int len1, const TChar* str2, int len2, CaseSensitivity cs)
{
	// 長さが -1 の場合は \0 までカウント
	len1 = static_cast<int>((len1 < 0) ? strlen(str1) : len1);
	len2 = static_cast<int>((len2 < 0) ? strlen(str2) : len2);

	const TChar* p1 = str1 + len1;
	const TChar* p2 = str2 + len2;

	// 大文字小文字を区別しない場合
	if (cs == CaseSensitivity::CaseInsensitive)
	{
		while (str2 <= p2)
		{
			if (p1 < str1 ||
				toupper(*p1) != toupper(*p2)) {
				return false;
			}
			p1--;
			p2--;
		}
		return true;
	}
	// 大文字小文字を区別する場合
	else
	{
		while (str2 <= p2)
		{
			if (p1 < str1 ||
				*p1 != *p2) {
				return false;
			}
			p1--;
			p2--;
		}
		return true;
	}
}
template bool StringHelper::endsWith<char>(const char* str1, int len1, const char* str2, int len2, CaseSensitivity cs);
template bool StringHelper::endsWith<wchar_t>(const wchar_t* str1, int len1, const wchar_t* str2, int len2, CaseSensitivity cs);
template bool StringHelper::endsWith<Char>(const Char* str1, int len1, const Char* str2, int len2, CaseSensitivity cs);

template<typename TChar>
void StringHelper::substr(const TChar* str, int len, int start, int count, const TChar** outBegin, const TChar** outEnd)
{
	if (start < 0)
	{
		start = 0;
	}
	if (count < 0)
	{
		count = len - start;
	}
	if (start + count > len)
	{
		count = len - start;
	}
	if (start > len)
	{
		count = 0;
	}

	if (start == 0 && count == len)
	{
		*outBegin = str;
		*outEnd = str + len;
	}
	else
	{
		*outBegin = str + start;
		*outEnd = str + start + count;
	}
}
template void StringHelper::substr<char>(const char* str, int len, int start, int count, const char** outBegin, const char** outEnd);
template void StringHelper::substr<wchar_t>(const wchar_t* str, int len, int start, int count, const wchar_t** outBegin, const wchar_t** outEnd);
template void StringHelper::substr<Char>(const Char* str, int len, int start, int count, const Char** outBegin, const Char** outEnd);

template<typename TChar>
void StringHelper::left(const TChar* str, int count, const TChar** outBegin, const TChar** outEnd)
{
	if (count < 0)
	{
		count = 0;
	}

	int len = (int)strlen(str);
	len = LN_MIN(len, count);
	*outBegin = str;
	*outEnd = str + len;
}
template void StringHelper::left<char>(const char* str, int count, const char** outBegin, const char** outEnd);
template void StringHelper::left<wchar_t>(const wchar_t* str, int count, const wchar_t** outBegin, const wchar_t** outEnd);
template void StringHelper::left<Char>(const Char* str, int count, const Char** outBegin, const Char** outEnd);

template<typename TChar>
void StringHelper::right(const TChar* str, int count, const TChar** outBegin, const TChar** outEnd)
{
	if (count < 0)
	{
		count = 0;
	}

	int len = (int)strlen(str);
	*outBegin = str + len - count;
	*outEnd = str + len;
}
template void StringHelper::right<char>(const char* str, int count, const char** outBegin, const char** outEnd);
template void StringHelper::right<wchar_t>(const wchar_t* str, int count, const wchar_t** outBegin, const wchar_t** outEnd);
template void StringHelper::right<Char>(const Char* str, int count, const Char** outBegin, const Char** outEnd);

//------------------------------------------------------------------------------
// Note:C言語標準では uint64_t 用の 変換関数が無いため自作した。
//		ちなみに C++ の istream も uint64_t 用の変換は標準ではない。(VC++にはあるが)
//------------------------------------------------------------------------------
template<typename TChar, typename TSigned, typename TUnigned>
static NumberConversionResult StrToNumInternal(
	const TChar* str,
	int len,			// -1 可
	int base,
	bool reqUnsigned,	// unsigned として読み取りたいかどうか
	TSigned signedMin,
	TSigned signedMax,
	TUnigned unsignedMax,
	const TChar** outEndPtr,	// NULL 可。処理完了後、str+len と一致しなかったら後ろに数字ではない文字がある。
	TUnigned* outNumber)		// NULL 可 (文字数カウント用)
{
	if (outNumber != NULL) { *outNumber = 0; }

	if (str == NULL) { return NumberConversionResult::ArgsError; }
	if (!(base == 0 || base == 2 || base == 8 || base == 10 || base == 16)) { return NumberConversionResult::ArgsError; }

	const TChar* p = str;
	const TChar* end = str + (len < 0 ? StringHelper::strlen(str) : len);

	// 空白をスキップ
	while (StringHelper::isSpace(*p)) { ++p; }

	// 符号チェック
	bool isNeg = false;
	if (*p == '-') {
		isNeg = true;	// マイナス符号有り
		++p;
	}
	else if (*p == '+') {
		++p;			// プラス符号はスキップするだけ
	}
	if (p >= end) { return NumberConversionResult::FormatError; }	// 符号しかなかった

																	// 基数が 0 の場合は自動判別する
	if (base == 0)
	{
		if (p[0] != '0') {
			base = 10;
		}
		else if (p[1] == 'x' || p[1] == 'X') {
			base = 16;
		}
		else {
			base = 8;
		}
	}

	// 基数によってプレフィックスをスキップする
	if (base == 8)
	{
		if (p[0] == '0')
		{
			++p;
			if (p >= end)
			{
				if (outEndPtr != NULL) { *outEndPtr = p; }
				*outNumber = 0;
				return NumberConversionResult::Success;
			}

		}
	}
	else if (base == 16)
	{
		if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
			++p;
			++p;
			if (p >= end) { return NumberConversionResult::FormatError; }	// 0x しかなかった
		}
	}

	TUnigned num = 0;
	TUnigned overflowMax = unsignedMax / base;	// 乗算しようとするとき、この値以上であれば次の乗算でオーバフローする
	TUnigned baseMax = unsignedMax % base;
	int count = 0;	// 読み取った文字数
	bool isOverflow = false;
	for (;;)
	{
		// 1文字読み取る
		TUnigned d;
		if (isdigit(*p)) {
			d = *p - '0';
		}
		else if ('A' <= *p && *p <= 'F') {
			d = *p - 'A' + 10;
		}
		else if ('a' <= *p && *p <= 'f') {
			d = *p - 'a' + 10;
		}
		else {
			break;
		}
		if (d >= (TUnigned)base) {
			// 基数より大きい桁が見つかった (10進数なのに A があったなど)
			return NumberConversionResult::FormatError;
		}
		++count;

		// 計算する前にオーバーフローを確認する
		if (num < overflowMax ||					// 例) uint8(max 255) のとき、num は 25 より小さければOK
			(num == overflowMax && d <= baseMax)) {	// 例) num が 25 のときは d が 5 以下であればOK
			num = num * base + d;
		}
		else {
			isOverflow = true;
		}

		// 次の文字へ
		++p;
		if (p >= end) { break; }
	}

	if (outEndPtr != NULL) {
		*outEndPtr = p;
	}

	// オーバーフローしていたら最大値に丸めておく
	NumberConversionResult result = NumberConversionResult::Success;
	if (isOverflow)
	{
		num = unsignedMax;
		result = NumberConversionResult::Overflow;
	}
	else if (!reqUnsigned)	// signed のオーバフローチェック
	{
		if (isNeg && num > (TUnigned)(-signedMin))
		{
			num = (TUnigned)(-signedMin);
			result = NumberConversionResult::Overflow;
		}
		else if (!isNeg && num > (TUnigned)signedMax)
		{
			num = signedMax;
			result = NumberConversionResult::Overflow;
		}
	}

	// マイナス符合がある場合は signed 値を unsigned として返し、
	// 呼び出し側で signed にキャストして返す。
	if (isNeg) {
		num = (TUnigned)(-(TSigned)num);
	}

	if (outNumber != NULL) { *outNumber = num; }
	return result;
}

template<typename TChar>
bool StringHelper::match(const TChar* pattern, const TChar* str)
{
	switch (*pattern)
	{
	case '\0':
		return '\0' == *str;
	case '*':
		return match(pattern + 1, str) || (('\0' != *str) && match(pattern, str + 1));
	case '?':
		return ('\0' != *str) && match(pattern + 1, str + 1);
	default:
		return (*pattern == *str) && match(pattern + 1, str + 1);
	}
}
template bool StringHelper::match<char>(const char* pattern, const char* str);
template bool StringHelper::match<wchar_t>(const wchar_t* pattern, const wchar_t* str);
template bool StringHelper::match<Char>(const Char* pattern, const Char* str);

template<typename TChar>
int8_t StringHelper::toInt8(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint8_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int8_t, uint8_t>(str, len, base, false, INT8_MIN, INT8_MAX, UINT8_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return (int8_t)n;
}
template int8_t StringHelper::toInt8<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template int8_t StringHelper::toInt8<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template int8_t StringHelper::toInt8<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
uint8_t StringHelper::toUInt8(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint8_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int8_t, uint8_t>(str, len, base, true, INT8_MIN, INT8_MAX, UINT8_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return n;
}
template uint8_t StringHelper::toUInt8<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template uint8_t StringHelper::toUInt8<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template uint8_t StringHelper::toUInt8<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
int16_t StringHelper::toInt16(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint16_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int16_t, uint16_t>(str, len, base, false, INT16_MIN, INT16_MAX, UINT16_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return (int16_t)n;
}
template int16_t StringHelper::toInt16<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template int16_t StringHelper::toInt16<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template int16_t StringHelper::toInt16<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
uint16_t StringHelper::toUInt16(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint16_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int16_t, uint16_t>(str, len, base, true, INT16_MIN, INT16_MAX, UINT16_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return n;
}
template uint16_t StringHelper::toUInt16<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template uint16_t StringHelper::toUInt16<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template uint16_t StringHelper::toUInt16<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
int32_t StringHelper::toInt32(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint32_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int32_t, uint32_t>(str, len, base, false, INT32_MIN, INT32_MAX, UINT32_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return (int32_t)n;
}
template int32_t StringHelper::toInt32<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template int32_t StringHelper::toInt32<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template int32_t StringHelper::toInt32<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
uint32_t StringHelper::toUInt32(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint32_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int32_t, uint32_t>(str, len, base, true, INT32_MIN, INT32_MAX, UINT32_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return n;
}
template uint32_t StringHelper::toUInt32<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template uint32_t StringHelper::toUInt32<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template uint32_t StringHelper::toUInt32<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
int64_t StringHelper::toInt64(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint64_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int64_t, uint64_t>(str, len, base, false, INT64_MIN, INT64_MAX, UINT64_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return (int64_t)n;
}
template int64_t StringHelper::toInt64<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template int64_t StringHelper::toInt64<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template int64_t StringHelper::toInt64<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
uint64_t StringHelper::toUInt64(const TChar* str, int len, int base, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	uint64_t n;
	NumberConversionResult r = StrToNumInternal<TChar, int64_t, uint64_t>(str, len, base, true, INT64_MIN, INT64_MAX, UINT64_MAX, outEndPtr, &n);
	if (outResult != NULL) { *outResult = r; }
	return n;
}
template uint64_t StringHelper::toUInt64<char>(const char* str, int len, int base, const char** outEndPtr, NumberConversionResult* outResult);
template uint64_t StringHelper::toUInt64<wchar_t>(const wchar_t* str, int len, int base, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template uint64_t StringHelper::toUInt64<Char>(const Char* str, int len, int base, const Char** outEndPtr, NumberConversionResult* outResult);

#ifdef _WIN32
static bool g_localeInitialized = false;
static _locale_t g_locale;

static _locale_t GetCLocale()
{
	if (!g_localeInitialized)
	{
		g_locale = _create_locale(LC_ALL, "C");
		g_localeInitialized = true;
	}
	return g_locale;
}

static double StrToD_L(const char* str, char** endptr, _locale_t locale) { return _strtod_l(str, endptr, locale); }
static double StrToD_L(const wchar_t* str, wchar_t** endptr, _locale_t locale) { return _wcstod_l(str, endptr, locale); }

#else
static bool g_localeInitialized = false;

class CLocale
{
public:
	locale_t m_locale;
	CLocale() : m_locale(0) { }
	~CLocale()
	{
		if (m_locale != NULL) {
			freelocale(m_locale);
		}
	}
};
static CLocale g_cLocale;

static locale_t GetCLocale()
{
	if (!g_localeInitialized)
	{
		g_cLocale.m_locale = newlocale(LC_ALL_MASK, "C", NULL);
		g_localeInitialized = true;
	}
	return g_cLocale.m_locale;
}
static double StrToD_L(const char* str, char** endptr, locale_t locale) { return strtod_l(str, endptr, locale); }
//static double StrToD_L(const wchar_t* str, wchar_t** endptr, locale_t locale) { return wcstod_l(str, endptr, locale); }

#endif

template<typename TChar>
double StringHelper::toDouble(const TChar* str, int len, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	if (outResult != NULL) { *outResult = NumberConversionResult::Success; }

	if (str == NULL) {
		if (outResult != NULL) { *outResult = NumberConversionResult::ArgsError; }
		return 0.0;
	}

	len = static_cast<int>((len < 0 ? StringHelper::strlen(str) : len));
	if (len >= 512) {
		if (outResult != NULL) { *outResult = NumberConversionResult::ArgsError; }
		return 0.0;
	}

	// 標準関数の strtod は長さを渡せないので一時バッファにコピーして終端\0にする。
	// 最大長さはとりあえず 512。
	// IEEE 形式では仮数部の桁数は 2^53=9007199254740992 で16桁で、指数部は 308。
	// IBM 形式では仮数部の桁数は 2^24=16777216 で8桁で、指数部は 16^63で、7.237005577332262213973186563043e+75。
	// 0 は 308 個並べられることになるが、512 文字分のサイズがあれば十分。
	char tmp[512] = { 0 };
	copySimpleAsciiString(tmp, 512, str, len);
	tmp[len] = '\0';

	char* end;
	errno = 0;
	double v = StrToD_L(tmp, &end, GetCLocale());

	if (outEndPtr != NULL) { *outEndPtr = str + (end - tmp); }

	if (errno == ERANGE || v == HUGE_VAL || v == -HUGE_VAL) {
		if (outResult != NULL) { *outResult = NumberConversionResult::Overflow; }
		return v;
	}
	return v;
}
template double StringHelper::toDouble<char>(const char* str, int len, const char** outEndPtr, NumberConversionResult* outResult);
template double StringHelper::toDouble<wchar_t>(const wchar_t* str, int len, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template double StringHelper::toDouble<Char>(const Char* str, int len, const Char** outEndPtr, NumberConversionResult* outResult);

template<typename TChar>
float StringHelper::toFloat(const TChar* str, int len, const TChar** outEndPtr, NumberConversionResult* outResult)
{
	double value = toDouble(str, len, outEndPtr, outResult);

	// double に変換するとオーバーフローする場合はエラーを出力する
	if (outResult != nullptr &&
		*outResult == NumberConversionResult::Success &&
		-FLT_MAX <= value && value <= FLT_MAX)
	{
		*outResult = NumberConversionResult::Overflow;
	}

	return (float)value;
}
template float StringHelper::toFloat<char>(const char* str, int len, const char** outEndPtr, NumberConversionResult* outResult);
template float StringHelper::toFloat<wchar_t>(const wchar_t* str, int len, const wchar_t** outEndPtr, NumberConversionResult* outResult);
template float StringHelper::toFloat<Char>(const Char* str, int len, const Char** outEndPtr, NumberConversionResult* outResult);

int StringHelper::int64ToString(int64_t value, char format, char* outStr, int bufSize)
{
	ln::detail::StdCharArrayBuffer<char> b(outStr, bufSize);
	std::basic_ostream<char, std::char_traits<char> > os(&b);
	//os.imbue(*formatter.m_locale);

	if (format == 'd' || format == 'D')
	{
	}
	else if (format == 'x' || format == 'X')
	{
		os << std::hex;
		if (format == 'X') os << std::uppercase;
	}

	os << value;
	return static_cast<int>(b.length());
}

int StringHelper::uint64ToString(uint64_t value, char format, char* outStr, int bufSize)
{
	ln::detail::StdCharArrayBuffer<char> b(outStr, bufSize);
	std::basic_ostream<char, std::char_traits<char> > os(&b);
	//os.imbue(*formatter.m_locale);

	if (format == 'd' || format == 'D')
	{
	}
	else if (format == 'x' || format == 'X')
	{
		os << std::hex;
		if (format == 'X') os << std::uppercase;
	}

	os << value;
	return static_cast<int>(b.length());
}

int StringHelper::doubleToString(double value, char format, int precision, char* outStr, int bufSize)
{
	ln::detail::StdCharArrayBuffer<char> b(outStr, bufSize);
	std::basic_ostream<char, std::char_traits<char> > os(&b);
	//os.imbue(*formatter.m_locale);

	if (format == 'f' || format == 'F')
	{
		os << std::fixed;
	}
	else if (format == 'e' || format == 'E')
	{
		os << std::scientific;
		if (format == 'E') os << std::uppercase;
	}
	if (precision >= 0)
	{
		os << std::setprecision(precision);
	}

	os << value;
	return static_cast<int>(b.length());
}

//==============================================================================
// StringHelper

namespace detail {

size_t UStringHelper::strlen(const Char* str)
{
	if (!str) return 0;
	size_t count = 0;
	for (; *str; ++str) ++count;
	return count;
}

int UStringHelper::compare(const Char* str1, const Char* str2)
{
	str1 = (str1) ? str1 : _TT("");
	str2 = (str2) ? str2 : _TT("");
	for (; *str1 && *str2; ++str1, ++str2)
	{
		if (*str1 != *str2)
		{
			return (*str1 < *str2) ? -1 : 1;
		}
	}

	if (*str1 != *str2)
		return (*str1 < *str2) ? -1 : 1;
	else
		return 0;
}

template<typename TChar, typename TValue>
static void toStringIntX(TValue v, TChar* outStr, int size)
{
	char buf[64];
	detail::StdCharArrayBuffer<char> b(buf, 64);
	std::ostream os(&b);
	os << v;
	const char* str = b.GetCStr();
	int i = 0;
	for (; *str && i < (size - 1); ++str, ++i)
	{
		outStr[i] = *str;
	}
	outStr[i] = '\0';
}

template<typename TChar>
void UStringHelper::toStringInt8(int8_t v, TChar* outStr, int size)
{
	toStringIntX((int32_t)v, outStr, size);
}
template void UStringHelper::toStringInt8<char>(int8_t v, char* outStr, int size);
template void UStringHelper::toStringInt8<wchar_t>(int8_t v, wchar_t* outStr, int size);
template void UStringHelper::toStringInt8<Char>(int8_t v, Char* outStr, int size);


} // namespace detail
} // namespace ln
