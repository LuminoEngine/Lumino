
#pragma once
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "Common.hpp"

namespace ln {
class String;

namespace detail {

// STL ユーティリティ
class StlHelper
{
public:

	// item と一致する最初の要素を削除する
	template <class T, typename TItem/*, typename TAllocator*/>
	static bool remove(T& vec, const TItem& item)
	{
		typename T::iterator itr = vec.begin();
		typename T::iterator end = vec.end();
		for (; itr != end; ++itr)
		{
			if (*itr == item) {
				vec.erase(itr);
				return true;
			}
		}
		return false;
	}

	// vector から等しい要素をすべて削除する
	template <class T, typename TAllocator>
	static size_t removeAll(std::vector<T, TAllocator>& vec, const T& value)
    {
        size_t n = vec.size();
        vec.erase( std::remove( vec.begin(), vec.end(), value ), vec.end() );
        return n - vec.size();
    }

	// vector から等しい要素をすべて削除する
	template <class T, typename TAllocator, typename TPred>
	static size_t removeAll(std::vector<T, TAllocator>& vec, TPred pred)
	{
		size_t n = vec.size();
		vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
		return n - vec.size();
	}

	// std::vector に要素が含まれているかチェックする
	template <class T>
	static bool contains(const std::vector<T>& vec, const T& value)
    {
		return std::find( vec.begin(), vec.end(), value ) != vec.end();
	}




	template<typename ...Args>
	struct first_enabled {};

	template<typename T, typename ...Args>
	struct first_enabled<std::enable_if<true, T>, Args...> { using type = T; };
	template<typename T, typename ...Args>
	struct first_enabled<std::enable_if<false, T>, Args...> : first_enabled<Args...>{};
	template<typename T, typename ...Args>
	struct first_enabled<T, Args...> { using type = T; };

	template<typename ...Args>
	using first_enabled_t = typename first_enabled<Args...>::type;
};

template<class T>
class StlAllocator
	: public std::allocator<T> 
{
public:
	typedef T* pointer;					// need GCC
	typedef const T* const_pointer;		// need GCC
    typedef std::size_t size_type;		// need GCC
	
public:
	template<class U>
	struct rebind
	{
		typedef StlAllocator<U> other;
	};

public:
	StlAllocator() { }
	StlAllocator(const StlAllocator& x) { }

	template<class U>
	StlAllocator(const StlAllocator<U>& x) { }

	pointer allocate(size_type cnt, const_pointer hint = 0)
	{
		return reinterpret_cast<pointer>(::operator new(cnt * sizeof(T)));
	}

	void deallocate(pointer ptr, size_type n)
	{
		::operator delete(ptr);
	}
};

template<typename T>
struct SortedArrayBasicLess
{
	template<typename TOtherKey>
	LN_CONSTEXPR bool operator()(const T& left, const TOtherKey& right) const
	{
		return (left < right);
	}
};


// ostream が直接文字列配列に書き出せるようにする
template<typename TChar>
class StdCharArrayBuffer : public std::basic_streambuf<TChar, std::char_traits<TChar> >
{
public:
	TChar * m_begin;

	StdCharArrayBuffer(TChar* buffer, size_t bufferLength)
	{
		m_begin = buffer;
		TChar* bufferEnd = buffer + bufferLength;
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setp(buffer, bufferEnd);
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setg(buffer, buffer, bufferEnd);
	}

	int length() const
	{
		return std::basic_streambuf<TChar, std::char_traits<TChar> >::pptr() - m_begin;
	}

	const TChar* GetCStr()
	{
		*std::basic_streambuf<TChar, std::char_traits<TChar> >::pptr() = '\0';
		return m_begin;
	}
};


class StdStringHelper
{
public:

	static std::string makeStdString(const String& str);

	static std::string makeStdString(const char* str);

	static std::string makeStdString(const char* str, size_t len);

	static std::string makeStdString(const wchar_t* str);

	static std::string makeStdString(const wchar_t* str, size_t len);

	template <class TString, class A1, class A2>
	static TString cat(const A1& s1, const A2& s2)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2));
		result.append(s2);
		return result;
	}

	template <class TString, class A1, class A2, class A3>
	static TString cat(const A1& s1, const A2& s2, const A3& s3)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2) + len(s3));
		result.append(s2);
		result.append(s3);
		return result;
	}

	template <class TString, class A1, class A2, class A3, class A4>
	static TString cat(const A1& s1, const A2& s2, const A3& s3, const A4& s4)
	{
		TString result = s1;
		result.reserve(len(s1) + len(s2) + len(s3) + len(s4));
		result.append(s2);
		result.append(s3);
		result.append(s4);
		return result;
	}

	static size_t len(const char* s) { return strlen(s); }
	static size_t len(const std::string& s) { return s.length(); }
	static size_t len(const wchar_t* s) { return wcslen(s); }
	static size_t len(const std::wstring& s) { return s.length(); }

	template <class TString, class A1>
	static bool contains(const TString& s1, const A1& a1)
	{
		return s1.find(a1) != TString::npos;
	}
};

} // namespace detail
} // namespace ln
