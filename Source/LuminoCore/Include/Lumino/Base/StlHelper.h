
#pragma once
#include <vector>
#include <algorithm>

LN_NAMESPACE_BEGIN
template<typename TChar> class GenericStringRef;
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
	static bool removeAll(std::vector<T, TAllocator>& vec, const T& value)
    {
        size_t n = vec.size();
        vec.erase( std::remove( vec.begin(), vec.end(), value ), vec.end() );
        return n != vec.size();
    }

	// vector から等しい要素をすべて削除する
	template <class T, typename TAllocator, typename TPred>
	static bool removeAll(std::vector<T, TAllocator>& vec, TPred pred)
	{
		size_t n = vec.size();
		vec.erase(std::remove_if(vec.begin(), vec.end(), pred), vec.end());
		return n != vec.size();
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

// STL カスタムアロケータ
//		アロケータのシンプルな実装は http://www.codeproject.com/Articles/4795/C-Standard-Allocator-An-Introduction-and-Implement が参考になる。
//		しかし、VisualC++12.0ではこの実装だと アロケータをテンプレートのデフォルト引数にできなかった。("コンパイラ内部エラー" となる)
//		環境依存の実装が必要なのだと思われるが、愚直に実装しても他のプラットフォームに移植できないので std::allocator から派生させている。
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




	
template<typename TString>
struct StringCaseInsensitiveLess
{
	typedef typename TString::CharType CharType;
	typedef GenericStringRef<CharType> StringRef;

	LN_CONSTEXPR bool operator()(const TString& left, const TString& right) const
	{
		return left.compare(right.c_str(), -1, CaseSensitivity::CaseInsensitive) < 0;
	}
	// String and char*
	LN_CONSTEXPR bool operator()(const TString& left, const CharType* right) const
	{
		return left.compare(right, -1, CaseSensitivity::CaseInsensitive) < 0;
	}
	LN_CONSTEXPR bool operator()(const CharType* left, const TString& right) const
	{
		return right.compare(left, -1, CaseSensitivity::CaseInsensitive) > 0;
	}
	// String and StringRef
	LN_CONSTEXPR bool operator()(const TString& left, const StringRef& right) const
	{
		return left.compare(right.getBegin(), -1, CaseSensitivity::CaseInsensitive) < 0;
	}
	LN_CONSTEXPR bool operator()(const StringRef& left, const TString& right) const
	{
		return right.compare(left.getBegin(), -1, CaseSensitivity::CaseInsensitive) > 0;
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

} // namespace detail
LN_NAMESPACE_END
