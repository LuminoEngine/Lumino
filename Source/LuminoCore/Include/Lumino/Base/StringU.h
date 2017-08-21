
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

//enum class UStringType
//{
//	Internal,
//	Char,
//	WChar,
//};

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
	int indexOf(const UString& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int indexOf(UChar ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;


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
};

//class UStringRef
//{
//public:
//	UStringRef(const UString& str) LN_NOEXCEPT
//	{
//		m_str = str.c_str();
//		m_length = str.getLength();
//	}
//
//	UStringRef(const UChar* str) LN_NOEXCEPT
//	{
//		m_str = str;
//		m_length = UStringHelper::strlen(str);
//	}
//
//
//	//template<std::size_t N>
//	//UStringRef(const UChar str[N]) LN_NOEXCEPT
//	//{
//	//	m_str = str;
//	//	m_length = N;
//	//}
//
//private:
//	detail::UStringType		m_type;
//	int						m_length;
//	union
//	{
//		const UChar*	m_str;
//	};
//};

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

inline bool operator==(const UChar* lhs, const UString& rhs)
{
	return UStringHelper::compare(lhs, rhs.c_str()) == 0;
}

LN_NAMESPACE_END
