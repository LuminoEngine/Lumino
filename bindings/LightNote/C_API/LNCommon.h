
#pragma once

//------------------------------------------------------------------------------
// Flags

// UNICODE 使用フラグ
#if defined (UNICODE) || defined(_UNICODE)
	#define LN_UNICODE
#else
	#define LN_MBS
#endif

// DLL
#if defined(LN_BUILD_DLL)
	#define LN_API			__declspec(dllexport)
#else
	#define LN_API
#endif


//------------------------------------------------------------------------------
// typedef

#include <stdint.h>
typedef intptr_t		LNHandle;

#ifdef LN_UNICODE
	typedef wchar_t		TCHAR;
	#define __T(x)      L ## x
#else
	typedef char		TCHAR;
	#define __T(x)      x
#endif

#define _T(x)			__T(x)

//------------------------------------------------------------------------------
// C_API

// デフォルト引数定義の細工
#ifdef LN_DISABLE_AT_DOXY_BUILD
	// Doxygen 実行時は何も定義しない
	// Doxygen 設定 [Preprcessor] > [PREDEFINED] >「LN_DEFAULT_ARG(x)= =x」によって
	// ドキュメント上はデフォルト引数が存在することになる。
/*
	[Preprcessor] > [MACRO_EXPANSION] … ON
	[Preprcessor] > [PREDEFINED] … "LN_DISABLE_AT_DOXY_BUILD"
	[Preprcessor] > [PREDEFINED] … "LN_DEFAULT_ARG(x)= =x"
*/
#else
	#ifdef __cplusplus
		// C++ ビルド時はデフォルト引数を有効にする
		#define LN_DEFAULT_ARG(x) =x
	#else
		// C ビルド時は無効にする
		#define LN_DEFAULT_ARG(x)
	#endif
#endif

#define LN_MODULE(x)
#define LN_MODULE_END
#define LN_CLASS(...)			/*LNOTE_TYPE_INFO_DECL(LNSound);*/
#define LN_STATIC_CLASS(x)
#define LN_STRUCT_CLASS(x)
#define LN_EXTENSION_CLASS(x)
#define LN_CLASS_END

#define LN_INSTANCE_API		LN_API
#define LN_INTERNAL_API		LN_API
#define LN_STATIC_API		LN_API

#define LN_LIBRARY_INITIALIZER
#define LN_LIBRARY_TERMINATOR
#define LN_ATTR_CONSTRUCTOR
#define LN_ATTR_PROPERTY
#define LN_ATTR_EVENT
//#define LN_ATTR_STRUCT_CONSTRUCTOR

#define LN_HANDLE(x) LNHandle
#define LN_OUT
