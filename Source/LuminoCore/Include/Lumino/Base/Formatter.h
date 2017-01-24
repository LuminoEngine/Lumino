
#pragma once
#include <array>
#include <strstream>
#include <iomanip>
#include "Common.h"

#if defined(_MSC_VER) && _MSC_VER <= 1800 // VS2013
	#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) (x)
#else
	#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) {x}
#endif

LN_NAMESPACE_BEGIN

template<typename TChar>
class GenericPathName;

template<typename TChar, typename TKind, typename TValue>
struct Formatter;


// https://msdn.microsoft.com/ja-jp/library/txafckwd(v=vs.110).aspx#Anchor_1

namespace detail
{


struct FormatArgType
{
	struct KindArithmetic {};
	struct KindString {};
	struct KindPointer {};
};


template<typename TChar, typename T>
struct IsCharArray : std::false_type {};
template<typename TChar, std::size_t N>
struct IsCharArray<TChar, TChar[N]> : std::true_type{};
template<typename TChar, std::size_t N>
struct IsCharArray<TChar, const TChar[N]> : std::true_type{};


// 引数1つ分。データへの参照は void* で持つ
template<typename TChar>
class FormatArg
{
public:
	FormatArg() {}

	template<typename T>
	FormatArg(const T& value)
		: m_value(static_cast<const void*>(&value))
		, m_formatImpl(&FormatImpl<T>)	// T に応じた変換関数のポインタ
	{
	}

	GenericString<TChar> DoFormat(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam) const
	{
		return m_formatImpl(locale, format, formatParam, m_value);
	}

private:
	template<typename T>
	static GenericString<TChar> FormatImpl(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const void* value)
	{
		using typeKind = detail::StlHelper::first_enabled_t<
			std::enable_if<std::is_arithmetic<T>::value,					detail::FormatArgType::KindArithmetic>,
			std::enable_if<std::is_same<T, std::basic_string<TChar>>::value,detail::FormatArgType::KindString>,
			std::enable_if<std::is_same<T, GenericString<TChar>>::value,	detail::FormatArgType::KindString>,
			std::enable_if<std::is_same<T, GenericStringRef<TChar>>::value,	detail::FormatArgType::KindString>,
			std::enable_if<std::is_same<T, GenericPathName<TChar>>::value,	detail::FormatArgType::KindString>,
			std::enable_if<IsCharArray<TChar, T>::value,					detail::FormatArgType::KindPointer>,
			std::enable_if<IsCharArray<TChar, T>::value,					detail::FormatArgType::KindPointer>,
			std::enable_if<std::is_same<T, TChar*>::value,					detail::FormatArgType::KindPointer>,
			std::enable_if<std::is_same<T, const TChar*>::value,			detail::FormatArgType::KindPointer>,
			std::false_type>;
		// ここでエラーとなる場合、復号書式の引数リストに不正な型を指定している。
		static_assert(std::is_same<typeKind, std::false_type>::value == false, "Invalid format args. Please check the type of value specified in the composite format.");
		return Formatter<TChar, typeKind, T>::Format(locale, format, formatParam, *static_cast<const T*>(value));
	}

	const void* m_value;
	GenericString<TChar>(*m_formatImpl)(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const void* value);
};

// 引数リストのベースクラス
template<typename TChar>
class FormatList
{
public:
	FormatList(FormatArg<TChar>* argList, int count)
		: m_argList(argList), m_count(count)
	{}

	const FormatArg<TChar>& GetArg(int index) const { return m_argList[index]; }
	int GetCount() const { return m_count; }

private:
	const FormatArg<TChar>* m_argList;
	int m_count;
};

// 引数リスト
template<typename TChar, int N>
class FormatListN : public FormatList<TChar>
{
public:
	template<typename... Args>
	FormatListN(const Args&... args)
		: FormatList<TChar>(&m_argsStore[0], N)
		, m_argsStore LN_FORMAT_BRACED_INIT_WORKAROUND({ FormatArg<TChar>(args)... })	// この部分は → のように展開される {FormatArg(e1), FormatArg(e2), FormatArg(e3)} http://en.cppreference.com/w/cpp/language/parameter_pack
	{
		static_assert(sizeof...(args) == N, "Invalid args count.");
	}

private:
	std::array<FormatArg<TChar>, N> m_argsStore;
};

// 引数リスト0個の場合の特殊化
template<typename TChar>
class FormatListN<TChar, 0> : public FormatList<TChar>
{
public: FormatListN() : FormatList<TChar>(0, 0) {}
};

// 可変長引数から FormatList を作る
template<typename TChar, typename... Args>
static FormatListN<TChar, sizeof...(Args)> MakeArgList(const Args&... args)
{
	return FormatListN<TChar, sizeof...(args)>(args...);
}

// ostream が直接文字列配列に書き出せるようにする
template<typename TChar>
class StdCharArrayBuffer : public std::basic_streambuf<TChar, std::char_traits<TChar> >
{
public:
	TChar* m_begin;

	StdCharArrayBuffer(TChar* buffer, size_t bufferLength)
	{
		m_begin = buffer;
		TChar* bufferEnd = buffer + bufferLength;
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setp(buffer, bufferEnd);
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setg(buffer, buffer, bufferEnd);
	}

	const TChar* GetCStr()
	{
		*std::basic_streambuf<TChar, std::char_traits<TChar> >::pptr() = '\0';
		return m_begin;
	}
};

// 整数フォーマットに , を付ける
template<typename TChar>
class CommaNumpunct : public std::numpunct<TChar>
{
protected:
	virtual TChar do_thousands_sep() const
	{
		return LN_T(TChar, ',');
	}

	virtual std::basic_string<TChar> do_grouping() const
	{
		return LN_T(TChar, "\03");
	}
};

} // namespace detail











//------------------------------------------------------------------------------
/*
	以下、値を GenericString に変換するための特殊化関数群。
*/

//template<typename TChar, typename TKind, typename TValue>
//struct Formatter
//{
//	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const TValue& value)
//	{
//		// ここでエラーとなる場合、復号書式の引数リストに不正な型を指定している。
//		static_assert(0, "Invalid format args. Please check the type of value specified in the composite format.");
//		return GenericString<TChar>();
//	}
//};
//
// bool
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindArithmetic, bool>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, bool value)
	{
		if (value) {
			return GenericString<TChar>(LN_T(TChar, "True"));
		}
		else {
			return GenericString<TChar>(LN_T(TChar, "False"));
		}
	}
};

// TChar[]
template<typename TChar, std::size_t N>
struct Formatter<TChar, detail::FormatArgType::KindPointer, /*const*/ TChar[N]>	// VS2015
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const TChar value[N])
	{
		LN_THROW(format.IsEmpty(), InvalidFormatException);
		return GenericString<TChar>(value);
	}
};

// const TChar[]
template<typename TChar, std::size_t N>
struct Formatter<TChar, detail::FormatArgType::KindPointer, const TChar[N]>		// VS2013
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const TChar value[N])
	{
		LN_THROW(format.IsEmpty(), InvalidFormatException);
		return GenericString<TChar>(value);
	}
};

// TChar*
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindPointer, TChar*>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, TChar* value)
	{
		LN_THROW(format.IsEmpty(), InvalidFormatException);
		return GenericString<TChar>(value);
	}
};

// const TChar*
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindPointer, const TChar*>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const TChar* value)
	{
		LN_THROW(format.IsEmpty(), InvalidFormatException);
		return GenericString<TChar>(value);
	}
};

// std::basic_string
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindString, std::basic_string<TChar>>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const std::basic_string<TChar>& value)
	{
		return GenericString<TChar>(value.c_str());
	}
};

// GenericString
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindString, GenericString<TChar>>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const GenericString<TChar>& value)
	{
		return value;
	}
};

// GenericStringRef
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindString, GenericStringRef<TChar>>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, const GenericStringRef<TChar>& value)
	{
		return value;
	}
};

// char
template<typename TChar>
struct Formatter<TChar, detail::FormatArgType::KindArithmetic, char>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, char value)
	{
		return GenericString<TChar>((TChar)value);
	}
};

// 上記以外の算術型 (int や float)
template<typename TChar, typename TValue>
struct Formatter<TChar, detail::FormatArgType::KindArithmetic, TValue>
{
	static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, TValue value)
	{
		TChar buf[64];
		detail::StdCharArrayBuffer<TChar> b(buf, 64);
		std::basic_ostream<TChar, std::char_traits<TChar> > os(&b);
		os.imbue(locale);

		int32_t precision = -1;
		if (!formatParam.IsEmpty())
		{
			NumberConversionResult result;
			const TChar* dummy;
			precision = StringTraits::ToInt32(formatParam.GetBegin(), formatParam.GetLength(), 10, &dummy, &result);
			LN_THROW(result == NumberConversionResult::Success, InvalidFormatException);
		}

		if (format.IsEmpty())
		{
		}
		else if (format.GetLength() == 1)
		{
			if (format[0] == 'd' || format[0] == 'D')
			{
				if (precision >= 0)
				{
					// 0埋め
					os << std::setfill((TChar)'0') << std::setw(precision);
				}
			}
			else if (format[0] == 'x' || format[0] == 'X')
			{
				os << std::hex;
				if (format[0] == 'X') { os << std::uppercase; }

				if (precision >= 0)
				{
					// 0埋め
					os << std::setfill((TChar)'0') << std::setw(precision);
				}
			}
			else if (format[0] == 'f' || format[0] == 'F')
			{
				os << std::fixed;

				if (precision >= 0)
				{
					// 小数点以下の精度
					os << std::setprecision(precision);
				}
			}
			else if (format[0] == 'e' || format[0] == 'E')
			{
				os << std::scientific;
				if (format[0] == 'E') { os << std::uppercase; }

				if (precision >= 0)
				{
					// 小数点以下の精度
					os << std::setprecision(precision);
				}
			}
		}
		else
		{
			LN_THROW(0, InvalidFormatException);
		}
		// http://sla0.jp/2012/04/cpp%E3%81%AEiostream%E3%83%95%E3%82%A9%E3%83%BC%E3%83%9E%E3%83%83%E3%83%88%E6%8C%87%E5%AE%9A%E6%97%A9%E8%A6%8B%E8%A1%A8/

		os << value;
		return GenericString<TChar>(b.GetCStr());
	}
};



LN_NAMESPACE_END
