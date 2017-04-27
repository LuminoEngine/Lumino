
#pragma once

//------------------------------------------------------------------------------
// flags

// Debug ビルドフラグ
#if defined(DEBUG) || defined(_DEBUG)
	#define LN_DEBUG
#endif

#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(Q_OS_WIN))
	#define LN_OS_WIN32
#endif

// プラットフォーム
#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__)
    #define LN_OS_WIN32
#elif defined(__APPLE__) || defined(__APPLE_CC__) || defined(__OSX__)
    #define LN_OS_MAC
	#define LN_OS_FAMILY_UNIX
#else	// とりあえず
	#define LN_OS_LINUX
	#define LN_X11
	#define LN_OS_FAMILY_UNIX
#endif

// コンパイラ
#if defined(__GNUC__)
	#define LN_GNUC
#elif defined(_MSC_VER)
	#define LN_MSVC
#else
	#error "invalid compiler"
#endif

// アーキテクチャ
#if defined(_WIN64) || defined(__x86_64__)
	#define LN_X64
#endif

// スレッドライブラリ
#if defined(__CYGWIN__) || !defined(LN_OS_WIN32)
	#define LN_THREAD_POSIX
#else
	#define LN_THREAD_WIN32
#endif

// 文字コードセット
#if defined (UNICODE) || defined(_UNICODE)
	#ifndef LN_UNICODE
		#define LN_UNICODE
	#endif
#else
	#ifndef LN_MBS
		#define LN_MBS
	#endif
#endif

// DLL exports
#if defined(LUMINO_DLL) && defined(LUMINO_BUILD_DLL)
	/* LUMINO_DLL と LUMINO_BUILD_DLL の同時定義は禁止。
	* LUMINO_DLL はライブラリをDLLとして使うとき、
	* LUMINO_BUILD_DLL はライブラリ本体をビルドするときにのみ使用する。
	*/
	#error "You must not have both LUMINO_DLL and LUMINO_BUILD_DLL defined"
#endif

#if defined(_WIN32) && defined(LUMINO_BUILD_DLL)
	#define LUMINO_EXPORT	__declspec(dllexport)

#elif defined(_WIN32) && defined(LUMINO_DLL)
	#define LUMINO_EXPORT	__declspec(dllimport)

#elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
	#define LUMINO_EXPORT	__attribute__((visibility("default")))

#else
	#define LUMINO_EXPORT

#endif

// 内部的な Copy on write 操作をスレッドセーフにするか
#define LN_INTERNAL_COW_THREAD_SAFE

// C++11 有効
#define LN_CPP11

#if !defined(_MSC_VER) || _MSC_VER >= 1900
	#define LN_CONSTEXPR	constexpr
#else
	#define LN_CONSTEXPR
#endif

//------------------------------------------------------------------------------
// defines

#if defined(LN_MSVC)
	#define LN_STDCALL	__stdcall
	#define LN_CDECL	__cdecl
#else
	#define LN_STDCALL	
	#define LN_CDECL	
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
	#define LN_NOEXCEPT
#else
	#define LN_NOEXCEPT noexcept
#endif

#ifndef LN_INTERNAL_ACCESS
	#define LN_INTERNAL_ACCESS				private
#endif
#ifndef LN_PROTECTED_INTERNAL_ACCESS
	#define LN_PROTECTED_INTERNAL_ACCESS	protected
#endif
#ifndef LN_CONSTRUCT_ACCESS
	#define LN_CONSTRUCT_ACCESS \
		template<class T, typename... TArgs> \
		friend ln::RefPtr<T> ln::NewObject(TArgs&&... args); \
			protected
#endif

#ifdef LN_UNICODE
	#define LN_AFX_FUNCNAME(name) name##W
#else
	#define LN_AFX_FUNCNAME(name) name##A
#endif

#define LN_NAMESPACE_BEGIN	namespace ln {
#define LN_NAMESPACE_END	}

// コピーコンストラクタと=演算子関数を無効にするマクロ。
// これはクラスのprivate:宣言内で使うべき。
#define LN_DISALLOW_COPY_AND_ASSIGN(typeName) \
	typeName(const typeName&); \
	void operator=(const typeName&)

//----------------------------------------------------------------------
// include
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#if defined(LN_OS_WIN32)
#else
	#include <limits.h>		// for _PC_PATH_MAX (on Ubuntu)
	#include <pthread.h>
#endif

//----------------------------------------------------------------------
// const

// ファイルパスの最大文字数
#if defined(LN_OS_WIN32)
	#define LN_MAX_PATH		260//MAX_PATH
#elif defined(PATH_MAX)
	#define LN_MAX_PATH		PATH_MAX
#elif defined(_PC_PATH_MAX)
	#define LN_MAX_PATH		_PC_PATH_MAX
#else
	#error invalid LN_MAX_PATH
#endif

#ifdef LN_DEBUG
	#define LN_ASSERT(x)	assert(x);
#else
	#define LN_ASSERT(x)
#endif

#ifndef LN_SAFE_FREE
	#define LN_SAFE_FREE( p ) { if ( p ) { free( ( p ) ); ( p ) = NULL; } }
#endif

#ifndef LN_SAFE_DELETE
	#define LN_SAFE_DELETE( p ) { if ( p ) { delete ( p ); ( p ) = NULL; } }
#endif

#ifndef LN_SAFE_DELETE_ARRAY
	#define LN_SAFE_DELETE_ARRAY( p ) { if ( p ) { delete[] ( p ); ( p ) = NULL; } }
#endif

/// 配列の要素数を返すマクロ
#ifndef LN_ARRAY_SIZE_OF
	#define LN_ARRAY_SIZE_OF(ary)     ( sizeof(ary) / sizeof( ary[0] ) )
#endif

//----------------------------------------------------------------------
// Binding / Refrection
#define LN_CLASS(...)
#define LN_STRUCT(...)
#define LN_ENUM_(...)
#define LN_FIELD(...)
#define LN_METHOD(...)
#define LN_DELEGATE(...)

//----------------------------------------------------------------------
// Base include

#include <string.h>

#include "Typedef.h"
#include "Memory.h"
#include "CRT.h"

/** Root namespace. */
LN_NAMESPACE_BEGIN

class ImplAccessor;

template<typename BaseType = char>	// GCC では GenericString 内部に定義できないので外に出している
class GenericStringTraits
{
public:
	typedef char	XCHAR;	// this
	typedef wchar_t	YCHAR;	// other
};
template<>
class GenericStringTraits<wchar_t>
{
public:
	typedef wchar_t	XCHAR;	// this
	typedef char	YCHAR;	// other
};

/** 大文字と小文字の区別指定 */
enum class CaseSensitivity
{
	CaseSensitive = 0,	/**< 大文字と小文字を区別する */
	CaseInsensitive,	/**< 大文字と小文字を区別しない */
};

/** String::Split() の出力方法 */
enum class StringSplitOptions
{
	None = 0,			/**< 出力は空の文字列を含む */
	RemoveEmptyEntries,	/**< 出力は空の文字列を含まない */
};

/** */
enum class NumberConversionResult
{
	Success = 0,
	ArgsError,
	FormatError,
	Overflow,
};

/** */
enum class Result	// TODO: ちょっと一般的な名前すぎ
{
	Success = 0,
	ArgumentError,
};


LN_NAMESPACE_END
