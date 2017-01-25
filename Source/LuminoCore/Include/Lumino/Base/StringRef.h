
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
		m_len = str.GetLength();
		m_string = str.GetCore();
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
		m_str = path.GetString().c_str();
		m_len = path.GetString().GetLength();
	}


	GenericStringRef(GenericStringRef const&) = default;
	//GenericStringRef(GenericStringRef&&) = default;	// VS2013 がムーブコンストラクタの default を生成しないのでとりあえず無しで。http://stackoverflow.com/questions/24573963/move-constructor-invalid-type-for-defaulted-constructor-vs-2013
	GenericStringRef& operator =(GenericStringRef const&) = default;
	//GenericStringRef& operator =(GenericStringRef&&) = default;
	~GenericStringRef() = default;

	const TChar* GetBegin() const { return m_str + m_pos; }
	const TChar* GetEnd() const { return m_str + m_pos + m_len; }

	bool IsNullOrEmpty() const { return (m_str == nullptr || m_len <= 0); }
	bool IsEmpty() const { return m_len <= 0; }
	int GetLength() const { return m_len; }

	int Compare(const TChar* str, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return StringTraits::Compare(GetBegin(), m_len, str, -1, count, cs); }

	const TChar& operator[](int index) const  { return *(m_str + m_pos + index); }
	
	bool Equals(const GenericStringRef<TChar>& str) const { return Compare(str.GetBegin(), str.GetLength()) == 0; }
	
	void Detach() LN_NOEXCEPT
	{
		m_str = nullptr;
		m_pos = 0;
		m_len = 0;
	}
	void Attach(const TChar* str, int pos, int len) LN_NOEXCEPT
	{
		m_str = str;
		m_pos = pos;
		m_len = len;
	}

	int CopyTo(TChar* dest, int destLen) const
	{
		LN_THROW(destLen > m_len, OutOfRangeException);
		StringTraits::tstrcpy(dest, destLen, m_str + m_pos);
		dest[m_len] = '\0';
		return m_len;
	}

	int CopyToLocal8Bit(char* dest, int destLen) const;

	GenericString<TChar> Mid(int start, int count) const
	{
		return StringTraits::Mid(GetBegin(), start, count);
	}

	/** この文字列参照のコピーを持つ文字列を作成します。*/
	GenericString<TChar> ToString() const;

private:
	const TChar*						m_str;
	int									m_pos;
	int									m_len;
	detail::GenericStringCore<TChar>*	m_string;

	template<typename T> friend class GenericString;
};

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericStringRef<TChar>::ToString() const
{
	return GenericString<TChar>(GetBegin(), GetLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator==(const TChar* left, const GenericStringRef<TChar>& right)
{
	return right.Equals(left);
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator==(const GenericStringRef<TChar>& left, const TChar* right)
{
	return left.Equals(right);
}

//------------------------------------------------------------------------------
template<typename TChar>
inline bool operator!=(const GenericStringRef<TChar>& left, const GenericStringRef<TChar>& right)
{
	return !right.Equals(left);
}


typedef GenericStringRef<TCHAR>		StringRef;
typedef GenericStringRef<char>		StringRefA;
typedef GenericStringRef<wchar_t>	StringRefW;

LN_NAMESPACE_END
