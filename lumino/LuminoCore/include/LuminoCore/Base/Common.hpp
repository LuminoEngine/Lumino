// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <assert.h>
#include <errno.h>
#include <string>
#include <vector>
#include <atomic>
#include <locale>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <functional>
#include <filesystem>
#include <functional>

//------------------------------------------------------------------------------
// Configuration
//------------------------------------------------------------------------------
#define LN_EXPERIMENTAL_VARIANT_SERIALIZATION 0

//------------------------------------------------------------------------------
// flags
//------------------------------------------------------------------------------

// Debug build flag
#if defined(DEBUG) || defined(_DEBUG)
#define LN_DEBUG
#endif

// platform
#if defined(__EMSCRIPTEN__)
#define LN_EMSCRIPTEN
#define LN_OS_FAMILY_UNIX
#elif defined(__ANDROID__)
#define LN_OS_ANDROID
#define LN_OS_FAMILY_UNIX
#elif (defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__) || defined(Q_OS_WIN))
#define LN_OS_WIN32
#define LN_OS_DESKTOP
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#define LN_OS_DARWIN
#define LN_OS_FAMILY_UNIX
#else
#define LN_OS_LINUX
#define LN_X11
#define LN_OS_FAMILY_UNIX
#define LN_OS_DESKTOP
#endif

#if defined(LN_OS_DARWIN)
#	include <TargetConditionals.h>
#	if (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#		define LN_OS_IOS
#	elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#		define LN_OS_MAC
#       define LN_OS_DESKTOP
#	endif
#endif


// compiler
#if defined(__GNUC__)
#define LN_GNUC
#elif defined(_MSC_VER)
#define LN_MSVC
#else
#error "invalid compiler"
#endif

// arch
#if defined(_WIN64) || defined(__x86_64__)
#define LN_X64
#endif

// thread library
#if defined(__CYGWIN__) || !defined(LN_OS_WIN32)
#define LN_THREAD_POSIX
#else
#define LN_THREAD_WIN32
#endif

// charcode set
#if defined(UNICODE) || defined(_UNICODE)
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

//#if defined(_WIN32) && defined(LUMINO_BUILD_DLL)
//#define LN_API __declspec(dllexport)
//#elif defined(_WIN32) && defined(LUMINO_DLL)
//#define LN_API __declspec(dllimport)
//#elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
//#define LN_API __attribute__((visibility("default")))
//#else
#define LN_API
//#endif

//------------------------------------------------------------------------------
// string
//------------------------------------------------------------------------------

#define LN_STRING_WITH_TYPESAFEFORMAT
#define LN_STRING_WITH_PATH

#define LN_USTRING
//#define LN_USTRING16
#define LN_USTRING32 1

//#ifndef LN_STRING_STRICT_CONVERSION
//#define LN_STRING_FUZZY_CONVERSION
//#endif

#if LN_USTRING32
#define __LT(x) U##x
#define _TT(x) U##x
#define _LT(x) __LT(x)
#define TTCHAR char32_t
//#define _U(x) (u##x)
namespace ln {
using Char = char32_t;
using UStdString = std::u32string;
}

#elif LN_USTRING16

#ifdef LN_USTRING16
#define __LT(x) u##x
#define _TT(x) u##x
#define _LT(x) __LT(x)
#define TTCHAR char16_t
//#define _U(x) (u##x)
namespace ln {
using Char = char16_t;
using UStdString = std::u16string;
}

#else
#define __LT(x) L##x
#define _TT(x) L##x
#define _LT(x) __LT(x)
#define TTCHAR wchar_t
//#define _U(x) (L##x)
namespace ln {
using Char = wchar_t;
}

#endif
#else
#endif


namespace ln {
	
#if !defined(_T)
#define _T(x) _TT(x) 
#endif

#ifdef _WIN32
#define _CT(x) L##x
#define CChar wchar_t
#else
#define _CT(x) x
#define CChar char
#endif

/** 大文字と小文字の区別指定 */
enum class CaseSensitivity
{
	/** 大文字と小文字を区別する */
	CaseSensitive,

	/** 大文字と小文字を区別しない */
	CaseInsensitive,
};

/** String::Split() の出力方法 */
enum class StringSplitOptions
{
	/** 出力は空の文字列を含む */
	None,

	/** 出力は空の文字列を含まない */
	RemoveEmptyEntries,
};

namespace detail {
void notifyFatalError(const char* file, int line, const char* message) noexcept;
} // namespace detail
} // namespace ln

//------------------------------------------------------------------------------
// CHECK macro
//------------------------------------------------------------------------------

#ifdef LN_DEBUG
#	define	LN_DCHECK_ENABLED
#endif

#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#	define LN_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#	define LN_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
#	define LN_LIKELY(expr) (1 == !!(expr))
#	define LN_UNLIKELY(expr) (0 == !!(expr))
#endif

#ifndef LN_CHECK_ABORT
#	include <stdlib.h>
#	define LN_CHECK_ABORT ::ln::detail::notifyFatalError(__FILE__, __LINE__, "abort.");
#endif

#ifndef LN_CHECK
#	define LN_CHECK(expr) if (!LN_LIKELY(expr)) { printf("%s:%u: abort: %s\n", (__FILE__), (unsigned)(__LINE__), (#expr)); LN_CHECK_ABORT; }
#endif

#ifdef LN_DCHECK_ENABLED
#	define LN_DCHECK(...) LN_CHECK(__VA_ARGS__);
#else
#	define LN_DCHECK(expr) ((void)(expr));
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

#ifndef LN_OPERATOR_NEW
#define LN_OPERATOR_NEW(size)		::operator new(size)
#endif

#ifndef LN_OPERATOR_DELETE
#define LN_OPERATOR_DELETE(ptr) {	::operator delete(ptr); (ptr) = 0; }
#endif

#ifndef LN_INTERNAL_ACCESS
#define LN_INTERNAL_ACCESS				private
#endif

#ifndef LN_PROTECTED_INTERNAL_ACCESS
#define LN_PROTECTED_INTERNAL_ACCESS	protected
#endif

#ifndef LN_ARRAY_SIZE_OF
#define LN_ARRAY_SIZE_OF(ary) ( sizeof(ary) / sizeof( ary[0] ) )
#endif

#ifndef LN_MEMBER_OFFSETOF
#define LN_MEMBER_OFFSETOF(type, member) (reinterpret_cast<std::size_t>(&reinterpret_cast<char const volatile&>(((type*)0)->member)))
#endif

//------------------------------------------------------------------------------
#include <stdint.h>
#include <math.h>       // for macOS on AzurePipelines. TODO: https://github.com/PointCloudLibrary/pcl/issues/2601

#if WCHAR_MAX <= 0xffff
#define LN_WCHAR_16
#elif WCHAR_MAX <= 0xffffffff
#define LN_WCHAR_32
#else
#error "size of wchar_t is too large."
#endif

/** Lumino namespace */
namespace ln {

using byte_t = unsigned char;

#ifdef LN_WCHAR_16
	typedef uint8_t		UTF8;
	typedef	wchar_t		UTF16;	// for watch by debugger
	typedef uint32_t	UTF32;
#else
	typedef uint8_t		UTF8;
	typedef	uint16_t	UTF16;
	typedef uint32_t	UTF32;
#endif

} // namespace ln

#define LN_NEW new

// This should be used within the private: declaration of the class
#define LN_DISALLOW_COPY_AND_ASSIGN(typeName) \
	typeName(const typeName&) = delete; \
	void operator=(const typeName&) = delete

// increment COM reference count
#define LN_COM_SAFE_ADDREF(p)	{ if (p) { (p)->AddRef(); } }

// decrement COM reference count
#define LN_COM_SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p)= nullptr; } }

// constexpr macro
#if defined(__clang__)
#define LN_CONSTEXPR constexpr
#elif defined(__GNUC__) || defined(__GNUG__)
#define LN_CONSTEXPR
#elif !defined(_MSC_VER) || _MSC_VER >= 1900
#define LN_CONSTEXPR constexpr
#else
#define LN_CONSTEXPR
#endif

// noexcept macro
#if defined(_MSC_VER) && _MSC_VER < 1900
#define LN_NOEXCEPT
#else
#define LN_NOEXCEPT noexcept
#endif

#ifndef LN_MAX
#define LN_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef LN_MIN
#define LN_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//----------------------------------------------------------------------
// Binding / Refrection
#define LN_CLASS(...)
#define LN_STRUCT(...)
#define LN_ENUM(...)
#define LN_FIELD(...)

/**
 * @param OverloadPostfix = "Name"
 * @param Property
 */
#define LN_METHOD(...)
#define LN_DELEGATE(...)
#define LN_PROMISE(...)

#define LN_LAST_ELEMENT_MARKER(x) x

