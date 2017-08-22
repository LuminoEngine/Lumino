
#pragma once
#include <atomic>
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class UStringCore; }
using UChar = char16_t;

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

}

class UStringRef;

/**
	@brief		文字列を表すクラスです。

	@note		[Experimental] null と空文字は区別しない。空文字扱い。null を表現したければ optional と組み合わせる。
				
*/
class UString
{
public:
	UString();
	~UString();
	UString(const UString& str);
	UString(UString&& str) LN_NOEXCEPT;
	UString& operator=(const UString& str);
	UString& operator=(UString&& str) LN_NOEXCEPT;

	UString(const UString& str, int begin);
	UString(const UString& str, int begin, int length);
	UString(const UChar* str);
	UString(const UChar* str, int begin);
	UString(const UChar* str, int begin, int length);
	UString(int count, UChar ch);
	UString(const char* str);

	UString& operator=(UChar ch);
	UString& operator=(const UChar* str);

	/** 文字列が空であるかを確認します。 */
	bool isEmpty() const;

	/** C 言語としての文字列表現を取得します。 */
	const UChar* c_str() const;

	/** 文字列の長さを取得します。 */
	int getLength() const;

	/** 文字列をクリアします。 */
	void clear();

	/**
		@brief		文字列を検索し、見つかった最初の文字のインデックスを返します。
		@param[in]	str			: 検索文字列
		@param[in]	startIndex	: 検索を開始するインデックス (省略した場合は先頭から)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		見つからなかった場合は -1。str が空文字列である場合は 0。
	*/
	int indexOf(const UStringRef& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int indexOf(UChar ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;





	template<typename... TArgs>
	static UString format(const UStringRef& format, const TArgs&... args);


	bool isSSO() const LN_NOEXCEPT { return detail::getLSB<7>(static_cast<uint8_t>(m_data.sso.length)); }
	bool isNonSSO() const LN_NOEXCEPT { return !detail::getLSB<7>(static_cast<uint8_t>(m_data.sso.length)); }

private:
	static std::size_t const SSOCapacity = 15;//31;//sizeof(uint32_t) * 4 / sizeof(UChar) - 1;

	void init() LN_NOEXCEPT;
	void copy(const UString& str);
	void move(UString&& str) LN_NOEXCEPT;
	void allocBuffer(int length);
	void assign(const UChar* str);
	void assign(const UChar* str, int begin);
	void assign(const UChar* str, int begin, int length);
	void assign(int count, UChar ch);
	void assignFromCStr(const char* str, int length = -1);
	void checkDetachShared();
	UChar* getBuffer();
	void setSSOLength(int len);
	int getSSOLength() const;
	void setNonSSO();

	union Data
	{
		detail::UStringCore*	core;

		struct SSO
		{
			UChar		buffer[SSOCapacity];
			UChar		length;	// y---xxxx	: x=size y:flag(0=non sso,1=sso)
		} sso;
	} m_data;
};

/**
	@brief		
*/
class UStringHelper
{
public:
	static size_t strlen(const UChar* str);
	static int compare(const UChar* str1, const UChar* str2);

	template<typename TChar>
	static void toStringInt8(int8_t v, TChar* outStr, int size);
};

/**
	@brief		
*/
class UStringRef
{
public:
	UStringRef()
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = nullptr;
		m_u.length = 0;
	}

	UStringRef(const UString& str)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = str.c_str();
		m_u.length = str.getLength();
	}

	//template<std::size_t N>
	//UStringRef(const UChar (&str)[N])
	//{
	//	m_type = detail::UStringType::UChar;
	//	m_u.str = str;
	//	m_u.length = N;
	//	if (m_u.str[N - 1] == '\0')
	//	{
	//		m_u.length--;
	//	}
	//}

	UStringRef(const UChar* str)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = str;
		m_u.length = UStringHelper::strlen(str);
	}

	UStringRef(const char* str)
	{
		m_type = detail::UStringRefSource::ByChar;
		m_c.str = UString(str);
	}

	UStringRef(const UStringRef& str)
		: UStringRef()
	{
		copy(str);
	}

	UStringRef& operator=(const UStringRef& str)
	{
		copy(str);
	}

	~UStringRef()
	{
		clear();
	}

	int getLength() const
	{
		return (m_type == detail::UStringRefSource::ByUChar) ? m_u.length : m_c.str.getLength();
	}

	const UChar* data() const
	{
		return (m_type == detail::UStringRefSource::ByUChar) ? m_u.str : m_c.str.c_str();
	}

private:
	void clear()
	{
		switch (m_type)
		{
		case detail::UStringRefSource::ByUChar:
			break;
		case detail::UStringRefSource::ByChar:
			m_c.str.~UString();
			break;
		default:
			break;
		}
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = nullptr;
		m_u.length = 0;
	}

	void copy(const UStringRef& str)
	{
		clear();
		m_type = str.m_type;
		switch (m_type)
		{
		case detail::UStringRefSource::ByUChar:
			m_u.str = str.m_u.str;
			m_u.length = str.m_u.length;
			break;
		case detail::UStringRefSource::ByChar:
			m_c.str = str.m_c.str;
			break;
		default:
			break;
		}
	}

	detail::UStringRefSource	m_type;
	union
	{
		struct UCharData
		{
			const UChar*		str;
			int					length;
		} m_u;

		struct CharData
		{
			UString	str;
		} m_c;
	};
};

/**
	@brief		
*/
class UStringBuilder
{
public:

private:
};


/**
	@brief		
*/
class UStringFormatter
{
public:

private:
	//const std::locale& locale;
};


namespace fmt {

typedef void(*FormatFunc)(UStringFormatter* formatter, const void* arg, void* format_str_ptr);

template<typename Formatter>
void format_arg(Formatter&, ...)
{
	static_assert(false,
		"Cannot format argument. To enable the use of ostream "
		"operator<< include fmt/ostream.h. Otherwise provide "
		"an overload of format_arg.");
}

namespace detail {

#if defined(_MSC_VER) && _MSC_VER <= 1800 // VS2013
#	define LN_FMT_BRACED_INIT_WORKAROUND(x) (x)
#else
#	define LN_FMT_BRACED_INIT_WORKAROUND(x) {x}
#endif

template<typename T>
FormatFunc selectFormatFunc(const T& v)
{
	return &format_arg;
}



//static GenericString<TChar> Format(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam, TValue value)
template<typename TChar, typename TValue>
void formatInternal_Numeric(UStringFormatter* formatter, const TValue* arg, void* format_str_ptr)
{
	//TChar buf[64];
	//detail::StdCharArrayBuffer<TChar> b(buf, 64);
	//std::basic_ostream<TChar, std::char_traits<TChar> > os(&b);
	//os.imbue(locale);

	//int32_t precision = -1;
	//if (!formatParam.isEmpty())
	//{
	//	NumberConversionResult result;
	//	const TChar* dummy;
	//	precision = StringTraits::toInt32(formatParam.getBegin(), formatParam.getLength(), 10, &dummy, &result);
	//	LN_THROW(result == NumberConversionResult::Success, InvalidFormatException);
	//}

	//if (format.isEmpty())
	//{
	//}
	//else if (format.getLength() == 1)
	//{
	//	if (format[0] == 'd' || format[0] == 'D')
	//	{
	//		if (precision >= 0)
	//		{
	//			// 0埋め
	//			os << std::setfill((TChar)'0') << std::setw(precision);
	//		}
	//	}
	//	else if (format[0] == 'x' || format[0] == 'X')
	//	{
	//		os << std::hex;
	//		if (format[0] == 'X') { os << std::uppercase; }

	//		if (precision >= 0)
	//		{
	//			// 0埋め
	//			os << std::setfill((TChar)'0') << std::setw(precision);
	//		}
	//	}
	//	else if (format[0] == 'f' || format[0] == 'F')
	//	{
	//		os << std::fixed;

	//		if (precision >= 0)
	//		{
	//			// 小数点以下の精度
	//			os << std::setprecision(precision);
	//		}
	//	}
	//	else if (format[0] == 'e' || format[0] == 'E')
	//	{
	//		os << std::scientific;
	//		if (format[0] == 'E') { os << std::uppercase; }

	//		if (precision >= 0)
	//		{
	//			// 小数点以下の精度
	//			os << std::setprecision(precision);
	//		}
	//	}
	//}
	//else
	//{
	//	LN_THROW(0, InvalidFormatException);
	//}
	//// http://sla0.jp/2012/04/cpp%E3%81%AEiostream%E3%83%95%E3%82%A9%E3%83%BC%E3%83%9E%E3%83%83%E3%83%88%E6%8C%87%E5%AE%9A%E6%97%A9%E8%A6%8B%E8%A1%A8/

	//os << value;
	//return GenericString<TChar>(b.GetCStr());
}












template<typename TChar>
class FormatArg
{
public:
	//template<typename T>
	//	FormatArg(const T& value)
	//	: m_value(static_cast<const void*>(&value))
	//	//, m_formatImpl(&FormatImpl<T>)	// T に応じた変換関数のポインタ
	//{
	//}

	//GenericString<TChar> DoFormat(const std::locale& locale, const GenericStringRef<TChar>& format, const GenericStringRef<TChar>& formatParam) const
	//{
	//	return m_formatImpl(locale, format, formatParam, m_value);
	//}

//#define LN_FMT_MAKE_VALUE()
	FormatArg(const int& value) : m_value(static_cast<const void*>(&value)), m_format(&formatInternal_Int32) {}

	template <typename T>
	FormatArg(const T &value)
		: m_value(static_cast<const void*>(&value))
		, m_format(&format_custom_arg<T>)
	{}

	template <typename T>
	static void format_custom_arg(UStringFormatter* formatter, const void* arg, void* format_str_ptr)
	{
		format_arg(*formatter,
			format_str_ptr,
			arg);
	}


	static void formatInternal_Int32(UStringFormatter* formatter, const void* arg, void* format_str_ptr)
	{
		formatInternal_Numeric<TChar, int>(formatter, reinterpret_cast<const int*>(arg), format_str_ptr);
	}


private:

	const void*	m_value;
	FormatFunc	m_format;
};

// base class of variadic args
template<typename TChar>
class FormatList
{
public:
	FormatList()
		: m_argList(nullptr)
		, m_count(0)
	{}

	const FormatArg<TChar>& GetArg(int index) const { return m_argList[index]; }
	int getCount() const { return m_count; }

protected:
	const FormatArg<TChar>* m_argList;
	int m_count;
};

// variadic args (N > 0)
template<typename TChar, int N>
class FormatListN : public FormatList<TChar>
{
public:
	template<typename... Args>
	FormatListN(const Args&... args)
		: FormatList<TChar>()
		, m_argListInstance LN_FMT_BRACED_INIT_WORKAROUND({ FormatArg<TChar>(args)... })	// extract to -> {FormatArg(e1), FormatArg(e2), FormatArg(e3)} http://en.cppreference.com/w/cpp/language/parameter_pack
	{
		static_assert(sizeof...(args) == N, "Invalid args count.");
		m_argList = &m_argListInstance[0];
		m_count = N;
	}

private:
	std::array<FormatArg<TChar>, N> m_argListInstance;
};

// variadic args (N = 0)
template<typename TChar>
class FormatListN<TChar, 0> : public FormatList<TChar>
{
public:
	FormatListN() : FormatList<TChar>(0, 0) {}
};


template<typename TChar, typename... Args>
static FormatListN<TChar, sizeof...(Args)> makeArgList(const Args&... args)
{
	return FormatListN<TChar, sizeof...(args)>(args...);
}

} // namespace detail
} // namespace fmt



namespace detail {

class UStringCore
{
public:

	UStringCore() LN_NOEXCEPT
		: m_refCount(1)
		, m_str(nullptr)
		, m_capacity(0)
		, m_length(0)
	{}

	~UStringCore() LN_NOEXCEPT
	{
		delete m_str;
	}

	bool isShared() const LN_NOEXCEPT { return (m_refCount > 1); }
	void addRef() LN_NOEXCEPT { ++m_refCount; }
	void release() LN_NOEXCEPT
	{
		--m_refCount;
		if (m_refCount == 0)
		{
			delete this;
		}
	}

	UChar* get() LN_NOEXCEPT { return m_str; }
	const UChar* get() const LN_NOEXCEPT { return m_str; }
	int getLength() const LN_NOEXCEPT { return m_length; }
	//void assign(const UChar* str, int begin, int length)
	//{
	//	assert(str);
	//	assert(begin >= 0);
	//	assert(length >= 0);
	//	alloc(length);
	//	memcpy(m_str, str + begin, sizeof(UChar) * length);
	//	m_str[length] = '\0';
	//}
	void alloc(int length)
	{
		assert(length >= 0);
		int size = length + 1;
		if (m_capacity < size)
		{
			delete m_str;
			m_str = LN_NEW UChar[size];
			m_capacity = size;
		}
		m_length = length;
	}
	void clear() { m_length = 0; }

private:
	std::atomic<int>	m_refCount;
	UChar*				m_str;
	int					m_capacity;
	int					m_length;
};

} // namespace detail

//==============================================================================
// String
//==============================================================================

inline const UChar* UString::c_str() const
{
	return (isSSO()) ? m_data.sso.buffer : ((m_data.core) ? m_data.core->get() : u"");
}

inline int UString::getLength() const
{
	return (isSSO()) ? getSSOLength() : ((m_data.core) ? m_data.core->getLength() : 0);
}

template<typename... TArgs>
inline UString UString::format(const UStringRef& format, const TArgs&... args)
{
	auto argList = ln::fmt::detail::makeArgList<UChar>(args...);
	return UString();
}

inline bool operator==(const UChar* lhs, const UString& rhs)
{
	return UStringHelper::compare(lhs, rhs.c_str()) == 0;
}

LN_NAMESPACE_END
