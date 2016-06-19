
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
#if defined(LN_BUILD_DLL) && defined(LN_MSVC)
	#define LN_API			__declspec(dllexport)
#else
	#define LN_API
#endif


//------------------------------------------------------------------------------
// typedef

#include <stdint.h>
typedef intptr_t		LNHandle;
typedef void*			LNUserData;

#ifdef LN_UNICODE
	typedef wchar_t		LNChar;
	#define __T(x)      L ## x
#else
	typedef char		LNChar;
	#define __T(x)      x
#endif

#define _T(x)			__T(x)

//------------------------------------------------------------------------------
// C_API

#define LN_DEFAULT_ARG(x)

#define LN_MODULE(x)
#define LN_MODULE_END
#define LN_CLASS(className, ...)			LN_API void className##_SetBindingTypeInfo(void* data);
#define LN_STATIC_CLASS(x)
#define LN_STRUCT_CLASS(x)
#define LN_EXTENSION_CLASS(x)		// BinderMaker によるクラス自動生成を行わないことを示す
#define LN_GENERIC_CLASS(x, ...)
#define LN_CLASS_END

#define LN_T1	void*
#define LN_T2	void*

#define LN_T1_HANDLE	LNHandle
#define LN_T2_HANDLE	LNHandle

#define LN_INSTANCE_API		LN_API
#define LN_INTERNAL_API		LN_API
#define LN_STATIC_API		LN_API

#define LN_ATTR_LIBRARY_INITIALIZER
#define LN_ATTR_LIBRARY_TERMINATOR
#define LN_ATTR_CONSTRUCTOR
#define LN_ATTR_PROPERTY
#define LN_ATTR_EVENT
#define LN_ATTR_OVERLOAD(ownerFunc)
//#define LN_ATTR_STRUCT_CONSTRUCTOR

#define LN_HANDLE(classType)				LNHandle
#define LN_GENERIC_HANDLE(classType, t1)	LNHandle
#define LN_OUT
