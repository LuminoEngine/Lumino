
#pragma once
#include <atomic>
#include "Common.h"
#include "StringBuilder.h"

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
	UString(const UChar* str, int length);
	UString(int count, UChar ch);
	UString(const char* str);

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
	static UString format(const UStringRef& format, TArgs&&... args);
	template<typename... TArgs>
	static UString format(const Locale& locale, const UStringRef& format, TArgs&&... args);	/**< @overload Format */

	
	/**
		@brief		この文字列と、指定した文字列を比較します。
		@param[in]	str1		: 比較文字列
		@param[in]	index1		: str1 内の部分文字列の開始位置
		@param[in]	str2		: 比較文字列
		@param[in]	index2		: str2 内の部分文字列の開始位置
		@param[in]	length		: 比較する文字数 (-1 の場合、GetLength() の値を使用します)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		str1 が str2 より小さい → 0 より小さい値
					str1 と str2 が等しい   → 0
					str1 が str2 より大きい → 0 より大きい値
	*/
	static int compare(const UStringRef& str1, int index1, const UStringRef& str2, int index2, int length = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive);

	bool isSSO() const LN_NOEXCEPT { return detail::getLSB<7>(static_cast<uint8_t>(m_data.sso.length)); }
	bool isNonSSO() const LN_NOEXCEPT { return !detail::getLSB<7>(static_cast<uint8_t>(m_data.sso.length)); }

	UString& operator=(const UStringRef& rhs);
	UString& operator=(const UChar* rhs);
	UString& operator=(UChar ch);

	UString& operator+=(const UString& rhs);
	UString& operator+=(const UStringRef& rhs);
	UString& operator+=(const UChar* rhs);
	UString& operator+=(UChar rhs);

private:
	static std::size_t const SSOCapacity = 15;//31;//sizeof(uint32_t) * 4 / sizeof(UChar) - 1;

	void init() LN_NOEXCEPT;
	void release() LN_NOEXCEPT;
	void copy(const UString& str);
	void move(UString&& str) LN_NOEXCEPT;
	void allocBuffer(int length);
	void resizeBuffer(int length);
	void assign(const UChar* str);
	void assign(const UChar* str, int length);
	void assign(int count, UChar ch);
	void assignFromCStr(const char* str, int length = -1);
	void checkDetachShared();
	UChar* getBuffer();
	void setSSOLength(int len);
	int getSSOLength() const;
	void setNonSSO();
	void append(const UChar* str, int length);

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




namespace fmt {

template<typename TChar>
class GenericFormatStringRef
{
public:
	GenericFormatStringRef();
	GenericFormatStringRef(const TChar* begin, const TChar* end);
	GenericFormatStringRef(const GenericFormatStringRef& str);

	bool isEmpty() const { return m_length == 0; }
	int getLength() const { return m_length; }
	const TChar* begin() const { return m_str; }
	const TChar* end() const { return m_str + m_length; }

	const TChar& operator[](int index) const { return *(m_str + index); }

private:
	const TChar*	m_str;
	int				m_length;
};

template<typename TChar>
class GenericFormatStringBuilder
{
public:
	GenericFormatStringBuilder();

	void clear();
	void appendChar(TChar ch);
	void appendChar(TChar ch, int count);
	void appendString(const TChar* str);
	void appendString(const TChar* str, int length);
	void appendString(const UString& str);
	const TChar* c_str() const;
	int getLength() const;

private:
	void appendIntenal(const TChar* str, int length);

	ByteBuffer	m_buffer;
	size_t		m_bufferUsed;
};

template<typename TChar>
class GenericStringFormatter
{
public:
	GenericStringFormatter()
		: m_error()
		, m_errorPos(0)
	{}
	~GenericStringFormatter() {}

	void reportError(const char* message, int pos) { m_error = message; m_errorPos = pos; }
	bool hasError() const { return !m_error.empty(); }

public:	// TODO
	const std::locale* m_locale;
	GenericFormatStringBuilder<TChar>	m_sb;
	GenericFormatStringRef<TChar>	m_formatString;
	GenericFormatStringRef<TChar>	m_precision;

private:
	std::string	m_error;
	int			m_errorPos;
};

template<typename Formatter>
void formatArg(Formatter&, ...)
{
	static_assert(false, "[Lumino format string error] Cannot format argument. Please overload formatArg.");
}

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
	void resize(int length)
	{
		assert(length >= 0);
		int size = length + 1;
		if (m_capacity < size)
		{
			UChar* oldStr = m_str;
			int oldLen = m_length;
			
			m_str = LN_NEW UChar[size];
			m_capacity = size;

			if (oldStr != nullptr)
			{
				memcpy(m_str, oldStr, std::min(length, oldLen) * sizeof(UChar));
				delete oldStr;
			}

		}
		m_str[length] = '\0';
		m_length = length;
	}
	void clear()
	{
		if (m_str != nullptr)
		{
			m_str[0] = '\0';
		}
		m_length = 0;
	}

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
inline UString UString::format(const UStringRef& format, TArgs&&... args)
{
	auto argList = ln::fmt::detail::makeArgList<UChar>(std::forward<TArgs>(args)...);
	fmt::GenericFormatStringBuilder<UChar> sb;
	if (fmt::detail::formatInternal<UChar>(Locale::getC(), &sb, format.data(), format.getLength(), argList))
	{
		return UString(sb.c_str(), sb.getLength());
	}
	else
	{
		return UString();
	}
}

template<typename... TArgs>
inline UString UString::format(const Locale& locale, const UStringRef& format, TArgs&&... args)
{
	auto argList = ln::fmt::detail::makeArgList<UChar>(std::forward<TArgs>(args)...);
	fmt::GenericFormatStringBuilder<UChar> sb;
	if (fmt::detail::formatInternal<UChar>(locale, &sb, format.data(), format.getLength(), argList))
	{
		return UString(sb.c_str(), sb.getLength());
	}
	else
	{
		return UString();
	}
}

inline UString& UString::operator=(const UStringRef& rhs)
{
	assign(rhs.data(), rhs.getLength());
	return *this;
}

inline UString& UString::operator=(const UChar* rhs)
{
	assign(rhs);
	return *this;
}

inline UString& UString::operator=(UChar ch)
{
	assign(&ch, 1);
	return *this;
}

inline UString& UString::operator+=(const UString& rhs)
{
	append(rhs.c_str(), rhs.getLength());
	return *this;
}

inline UString& UString::operator+=(const UStringRef& rhs)
{
	append(rhs.data(), rhs.getLength());
	return *this;
}

inline UString& UString::operator+=(const UChar* rhs)
{
	append(rhs, UStringHelper::strlen(rhs));
	return *this;
}

inline UString& UString::operator+=(UChar rhs)
{
	append(&rhs, 1);
	return *this;
}

inline bool operator==(const UChar* lhs, const UString& rhs)
{
	return UStringHelper::compare(lhs, rhs.c_str()) == 0;
}

inline bool operator==(const UString& lhs, const UString& rhs)
{
	return UString::compare(lhs, 0, rhs, 0) == 0;
}

inline bool operator==(const UString& lhs, const UChar* rhs)
{
	return UStringHelper::compare(lhs.c_str(), rhs) == 0;
}

inline bool operator!=(const UChar* lhs, const UString& rhs)
{
	return !operator==(lhs, rhs);
}

inline bool operator!=(const UString& lhs, const UString& rhs)
{
	return !operator==(lhs, rhs);
}

inline bool operator!=(const UString& lhs, const UChar* rhs)
{
	return !operator==(lhs, rhs);
}

#include "StringFormat.inl"

LN_NAMESPACE_END
