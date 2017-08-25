
#pragma once
#include <cstddef>
#include "StringHelper.h"

LN_NAMESPACE_BEGIN

namespace detail { template<typename TChar> class GenericStringCore; }
template<typename TChar> class GenericPathName;

/**
	@brief	ある文字列に対する部分文字列の参照を保持します。
*/
template<typename TChar>
class GenericStringRef
{
public:
	GenericStringRef() : m_str(nullptr), m_pos(0), m_len(0), m_string(nullptr) {}
	GenericStringRef(std::nullptr_t) : GenericStringRef(){}
	GenericStringRef(const TChar* str)
		: GenericStringRef()
	{
		m_str = str;
		if (m_str != nullptr)
		{
			m_len = (int)StringTraits::tcslen(m_str);	// TODO: これ strlen じゃなくて templateの[N]で取ればもう少し高速化できそう
		}
	}
	GenericStringRef(const GenericString<TChar>& str)
		: GenericStringRef()
	{
		m_str = str.c_str();
		m_len = str.getLength();
		m_string = str.getCore();
	}
	GenericStringRef(const GenericString<TChar>& str, int startIndex)
		: GenericStringRef()
	{
		m_str = str.c_str() + startIndex;
		m_len = str.getLength() - startIndex;
		m_string = str.getCore();
	}
	GenericStringRef(const GenericString<TChar>& str, int startIndex, int length)
		: GenericStringRef()
	{
		m_str = str.c_str() + startIndex;
		m_len = length;
		m_string = str.getCore();
	}
	GenericStringRef(const TChar* begin, const TChar* end)
		: GenericStringRef()
	{
		if (begin != nullptr && end != nullptr && begin <= end)
		{
			m_str = begin;
			m_len = (int)(end - begin);
		}
	}
	GenericStringRef(const TChar* begin, int length)
		: GenericStringRef()
	{
		m_str = begin;
		m_len = length;
	}
	GenericStringRef(const GenericPathName<TChar>& path)
		: GenericStringRef()
	{
		m_str = path.getString().c_str();
		m_len = path.getString().getLength();
	}


	GenericStringRef(GenericStringRef const&) = default;
	//GenericStringRef(GenericStringRef&&) = default;	// VS2013 がムーブコンストラクタの default を生成しないのでとりあえず無しで。http://stackoverflow.com/questions/24573963/move-constructor-invalid-type-for-defaulted-constructor-vs-2013
	GenericStringRef& operator =(GenericStringRef const&) = default;
	//GenericStringRef& operator =(GenericStringRef&&) = default;
	~GenericStringRef() = default;

	const TChar* getBegin() const { return m_str + m_pos; }
	const TChar* getEnd() const { return m_str + m_pos + m_len; }

	bool IsNullOrEmpty() const { return (m_str == nullptr || m_len <= 0); }
	bool isEmpty() const { return m_len <= 0; }
	int getLength() const { return m_len; }

	int compare(const TChar* str, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return StringTraits::compare(getBegin(), m_len, str, -1, count, cs); }

	const TChar& operator[](int index) const  { return *(m_str + m_pos + index); }
	
	bool equals(const GenericStringRef<TChar>& str) const { return compare(str.getBegin(), str.getLength()) == 0; }
	
	void detach() LN_NOEXCEPT
	{
		m_str = nullptr;
		m_pos = 0;
		m_len = 0;
	}
	void attach(const TChar* str, int pos, int len) LN_NOEXCEPT
	{
		m_str = str;
		m_pos = pos;
		m_len = len;
	}

	int copyTo(TChar* dest, int destLen) const
	{
		LN_THROW(destLen > m_len, OutOfRangeException);
		StringTraits::tstrcpy(dest, destLen, m_str + m_pos);
		dest[m_len] = '\0';
		return m_len;
	}

	int copyToLocal8Bit(char* dest, int destLen) const;

	GenericString<TChar> mid(int start, int count) const
	{
		return StringTraits::mid(getBegin(), start, count);
	}

	/** この文字列参照のコピーを持つ文字列を作成します。*/
	GenericString<TChar> toString() const;

	size_t getHashCode() const;

private:
	const TChar*						m_str;
	int									m_pos;
	int									m_len;
	detail::GenericStringCore<TChar>*	m_string;

	template<typename T> friend class GenericString;
};

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericStringRef<TChar>::toString() const
{
	return GenericString<TChar>(getBegin(), getLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator==(const TChar* left, const GenericStringRef<TChar>& right)
{
	return right.equals(left);
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator==(const GenericStringRef<TChar>& left, const TChar* right)
{
	return left.equals(right);
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator!=(const GenericStringRef<TChar>& left, const GenericStringRef<TChar>& right)
{
	return !right.equals(left);
}


#ifndef LN_USTRING
typedef GenericStringRef<TCHAR>		StringRef;
#endif
typedef GenericStringRef<char>		StringRefA;
typedef GenericStringRef<wchar_t>	StringRefW;

LN_NAMESPACE_END
