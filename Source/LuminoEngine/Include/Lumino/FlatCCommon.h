#pragma once


#if defined(LUMINO_BUILD_DLL) && defined(LN_MSVC)
#define LN_API	__declspec(dllexport)
#else
#define LN_API
#endif

#ifdef LN_UNICODE
typedef wchar_t		LNChar;
#define __T(x)      L ## x
#else
typedef char		LNChar;
#define __T(x)      x
#endif

#define LN_OK	LN_RESULTCODE_OK

typedef void* LNUserData;

/** ê^ãUíl */
typedef enum tagLNBool
{
	/** ãU */
	LN_FALSE	= 0,

	/** ê^ */
	LN_TRUE		= 1,

} LNBool;

typedef enum
{
	LN_OK = 0,

} LNResultCode;

