﻿#pragma once

#if !defined(LN_FLAT_API)
    #if defined(_WIN32)
        #define LN_FLAT_API __declspec(dllexport)
    #elif defined(_WIN32) && defined(LUMINO_DLL)
        #define LN_FLAT_API __declspec(dllimport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LN_FLAT_API __attribute__((visibility("default")))
    #else
        #define LN_FLAT_API
    #endif
#endif


#define LNI_FUNC_TRY_BEGIN    try {
#define LNI_FUNC_TRY_END_RETURN    } \
    catch (ln::Exception& e) { \
        return ln::Runtime::processException(&e); \
    } \
    catch (...) { \
        return LN_ERROR_UNKNOWN; \
    } \
	return LN_SUCCESS;

#define LNI_BOOL_TO_LNBOOL(x)    (x) ? LN_TRUE : LN_FALSE
#define LNI_LNBOOL_TO_BOOL(x)    (x != LN_FALSE)

#define LNI_CREATE_OBJECT(out, type, initFunc, ...) { auto ptr = ::ln::makeObject<type>(__VA_ARGS__); *out = ::ln::Runtime::makeObjectWrap(ptr); }
#define LNI_HANDLE_TO_OBJECT(type, h)               static_cast<type*>((h) ? ::ln::Runtime::getObject(h) : nullptr)
#define LNI_OBJECT_TO_HANDLE(obj)					::ln::Runtime::makeObjectWrap(obj)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//------------------------------------------------------------------------------
#include <stdint.h>
typedef intptr_t LnHandle;
typedef void* LnUserData;
typedef char16_t LnChar;

#define LN_NULL_HANDLE 0

/** 結果・エラーコード */
typedef enum tagLnResult
{
    /** 成功 */
	LN_SUCCESS = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

} LnResult;

/** 真偽値 */
typedef enum tagLnBool
{
    /** 偽 */
    LN_FALSE = 0,

    /** 真 */
    LN_TRUE = 1,

} LnBool;

inline const char* LnRuntime_GetLastErrorMessage() { return ""; }  // TODO
extern LN_FLAT_API void LnRuntime_SetManagedObjectId(LnHandle handle, int64_t id);
extern LN_FLAT_API int64_t LnRuntime_GetManagedObjectId(LnHandle handle);
extern LN_FLAT_API int64_t LnRuntime_GetManagedTypeInfoId(LnHandle handle);

//==============================================================================

/**
	@brief	全てのオブジェクトのベースクラスです。
*/

/**
	@brief		オブジェクトを解放します。
	@param[in]	obj	: オブジェクトハンドル
	@details	指定されたオブジェクトの参照を解放します。
*/
LN_FLAT_API LnResult LnObject_Release(LnHandle obj);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

