
#pragma once

#if defined(_MSC_VER)
//----------------------------------------------------------------------
// VisualC++

#if _MSC_VER >=1600 // VC2010(VC10.0)以降
	#include <stdint.h>
#else
	// integer types
	typedef signed char			int8_t;
	typedef unsigned char		uint8_t;
	typedef signed short		int16_t;
	typedef unsigned short		uint16_t;
	typedef signed int			int32_t;
	typedef unsigned int		uint32_t;
	typedef signed __int64		int64_t;
	typedef unsigned __int64	uint64_t;
	#if _INTEGRAL_MAX_BITS >= 128
		typedef signed __int128		int128_t;
		typedef unsigned __int128	uint128_t;
	#endif

	// intptr types
	#if _WIN64
		typedef __int64				intptr_t;
		typedef unsigned __int64	uintptr_t;
	#else
		typedef signed int			intptr_t;
		typedef unsigned int		uintptr_t;
	#endif

	#define INT8_MIN			-128
	#define INT8_MAX			127
	#define UINT8_MAX			255

	#define INT16_MIN			-32768
	#define INT16_MAX			32767
	#define UINT16_MAX			65535

	#define INT32_MIN			(-2147483647-1)
	#define INT32_MAX			2147483647
	#define UINT32_MAX			0xffffffff

	#define INT64_MIN			(-9223372036854775807-1)
	#define INT64_MAX			9223372036854775807
	#define UINT64_MAX			0xffffffffffffffff

	#if _INTEGRAL_MAX_BITS >= 128
	#define INT128_MIN			(-170141183460469231731687303715884105727i128-1)
	#define INT128_MAX			170141183460469231731687303715884105727i128
	#define UINT128_MAX			0xffffffffffffffffffffffffffffffffui128
	#endif

	#define INTPTR_MIN			(-2147483647-1)
	#define INTPTR_MAX			2147483647
	#define UINTPTR_MAX			0xffffffff
#endif

#else

//----------------------------------------------------------------------
// GCC
#include <stdint.h>
#include <locale.h>
#include <xlocale.h>    // for mac

#endif

// lib types
LN_NAMESPACE_BEGIN

typedef	uint8_t		byte_t;

#if defined(_MSC_VER)
typedef _locale_t	NativeLocale_t;
#else
typedef locale_t	NativeLocale_t;
#endif

LN_NAMESPACE_END



//==============================================================================
// TChar

#include <wchar.h>

#if WCHAR_MAX <= 0xffff
#define LN_WCHAR_16
#elif WCHAR_MAX <= 0xffffffff
#define LN_WCHAR_32
#else
#error "size of wchar_t is too large."
#endif


LN_NAMESPACE_BEGIN

#ifdef LN_WCHAR_16
typedef uint8_t		UTF8;
typedef	wchar_t		UTF16;	// デバッガで見ることができるので wchar_t にしておく
typedef uint32_t	UTF32;
#else
typedef uint8_t		UTF8;
typedef	uint16_t	UTF16;
typedef wchar_t		UTF32;
#endif

LN_NAMESPACE_END

//----------------------------------------------------------------------
// TChar Macros

#ifdef LN_UNICODE
	#ifndef _UNICODE
		#define _UNICODE
	#endif
	#ifndef UNICODE
		#define UNICODE
	#endif
#endif

#if defined(LN_OS_WIN32) && !defined(__CYGWIN__)
	#include <tchar.h>
#else

#ifdef LN_UNICODE
	typedef wchar_t		TCHAR;
	#define __T(x)      L ## x
    #define _tcslen		wcslen
    #define _tcsicmp    wcsicmp
	#define _tcscmp		wcscmp
	#define _tcsstr		wcsstr
	#define _tcscpy_s	wcscpy_s
	#define _tfopen_s	_wfopen_s
	#define _ftprintf	fwprintf
#else
	typedef char		TCHAR;
	#define __T(x)      x
	#define _tcslen		strlen
	#define _tcscmp		strcmp
    #define _tcsicmp    stricmp
	#define _tcsstr		strstr
	#define _tcscpy_s	strcpy_s
	#define _tfopen_s	_fopen_s
	#define _ftprintf	fprintf
#endif

#define _T(x)			__T(x)

#endif

