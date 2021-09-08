#pragma once

#if !defined(LN_FLAT_API)
    #if defined(_WIN32) && defined(LUMINO_BUILD_DLL)
        #define LN_FLAT_API __declspec(dllexport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LN_FLAT_API __attribute__((visibility("default")))
    #else
        #define LN_FLAT_API
    #endif
#endif


#define LNI_FUNC_TRY_BEGIN    try {
//#define LNI_FUNC_TRY_END_RETURN    } \
//    catch (ln::Exception& e) { \
//        return ln::Runtime::processException(&e); \
//    } \
//    catch (...) { \
//        return LN_ERROR_UNKNOWN; \
//    } \
//	return LN_OK;
#define LNI_FUNC_TRY_END_RETURN    } \
    catch (ln::Exception& e) { \
        return ln::Runtime::processException(&e); \
    } \
	return LN_OK;
// NOTE: HSP ランタイムは エラーを例外で通知していくので、catch (...) で処理してしまうとよくわからないままプロセスが終了してしまう。

#define LNI_BOOL_TO_LNBOOL(x)    (x) ? LN_TRUE : LN_FALSE
#define LNI_LNBOOL_TO_BOOL(x)    (x != LN_FALSE)

#define LNI_CREATE_OBJECT(out, type, initFunc, ...)     { auto ptr = ::ln::makeObject<type>(__VA_ARGS__); *out = ::ln::Runtime::makeObjectWrap(ptr, true); }
#define LNI_HANDLE_TO_OBJECT(type, h)					static_cast<type*>((h) ? ::ln::Runtime::getObject(h) : nullptr)
#define LNI_OBJECT_TO_HANDLE(obj)						::ln::Runtime::makeObjectWrap(obj, false)
#define LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(obj)	::ln::Runtime::makeObjectWrap(obj, true)
#define LNI_STRING_TO_STRPTR_UTF16(str)                 ::ln::Runtime::getUTF16StringPtr(str)
#define LNI_STRING_TO_STRPTR_A(str)                     ::ln::Runtime::getAStringPtr(str)
#define LNI_ASTRPTR_TO_STRING(str)                      ::ln::String::fromCString(str, -1, ::ln::Runtime::getAStringEncoding())

#define LNI_REFERENCE_RETAINED (1)
#define LNI_REFERENCE_RELEASED (2)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//------------------------------------------------------------------------------
#include <stdint.h>

/** Lumino のオブジェクトを識別するための値です。0 (LN_NULL_HANDLE) は無効値です。 */
typedef uint32_t LNHandle;

typedef void* LNUserData;

//typedef char16_t LNChar;
typedef char32_t LNChar;

#define LN_NULL_HANDLE 0

/** 結果・エラーコード */
typedef enum tagLNResult
{
    /** 成功 */
	LN_OK = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

	/**  */
	LN_RUNTIME_UNINITIALIZED = -2,

    /**  */
    LN_ERROR_INVALID_ARGUMENT = -3,

} LNResult;

/** 真偽値 */
typedef enum tagLNBool
{
    /** 偽 */
    LN_FALSE = 0,

    /** 真 */
    LN_TRUE = 1,

} LNBool;

///** ログの通知レベル */
//typedef enum tagLNLogLevel
//{
//    LN_LOG_LEVEL_UNKNOWN = 0,
//    LN_LOG_LEVEL_VERBOSE,
//    LN_LOG_LEVEL_DEBUG,
//    LN_LOG_LEVEL_INFO,
//    LN_LOG_LEVEL_WARN,
//    LN_LOG_LEVEL_ERROR,
//    LN_LOG_LEVEL_FATAL,
//
//} LNLogLevel;

//==============================================================================
// Internal API

typedef void(*LNRuntimeFinalizedCallback)();
typedef void(*LNReferenceCountTrackerCallback)(LNHandle handle, int method, int count);
typedef void(*LNRuntimeGetTypeInfoIdCallback)(LNHandle handle, int* outTypeInfoId);

typedef struct tagLNRuntimeSettings
{
    LNRuntimeFinalizedCallback runtimeFinalizedCallback;
    LNReferenceCountTrackerCallback referenceCountTrackerCallback;
    LNRuntimeGetTypeInfoIdCallback runtimeGetTypeInfoIdCallback;

} LNRuntimeSettings;

extern LN_FLAT_API void LNRuntime_Initialize(const LNRuntimeSettings* settings);
extern LN_FLAT_API void LNRuntime_Finalize();
inline const char* LNRuntime_GetLastErrorMessage() { return ""; }  // TODO:
extern LN_FLAT_API void LNRuntime_SetManagedObjectId(LNHandle handle, int64_t id);
extern LN_FLAT_API int64_t LNRuntime_GetManagedObjectId(LNHandle handle);
extern LN_FLAT_API int64_t LNRuntime_GetManagedTypeInfoId(LNHandle handle);
//extern LN_FLAT_API void LNRuntime_SetReferenceCountTracker(LNReferenceCountTrackerCallback callback);
extern LN_FLAT_API void LNRuntime_SetReferenceTrackEnabled(LNHandle handle);
//extern LN_FLAT_API void LNRuntime_SetRuntimeFinalizedCallback(LNRuntimeFinalizedCallback callback);
//extern LN_FLAT_API void LNRuntime_SetRuntimeCreateInstanceCallback(LNRuntimeCreateInstanceCallback callback);
//extern LN_FLAT_API void LNRuntime_SetRuntimeGetTypeInfoIdCallback(LNRuntimeGetTypeInfoIdCallback callback);
extern LN_FLAT_API void LNRuntime_RunAppInternal(LNHandle app);
extern LN_FLAT_API void LNRuntime_DumpInfo();

extern LN_FLAT_API void LNInternalEngineSettings_SetEngineResourcesPathA(const char* path);

typedef void(*LNTypeInfoCreateInstanceCallback)(int typeInfoId, LNHandle* outHandle);

extern LN_FLAT_API LNResult LNTypeInfo_Acquire(const LNChar* typeName, int32_t* outTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_AcquireA(const char* typeName, int32_t* outTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_Find(const LNChar* typeName, int32_t* outTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_FindA(const char* typeName, int32_t* outTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_SetBaseClass(int32_t typeInfoId, int32_t baseClassTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_SetCreateInstanceCallback(int32_t typeInfoId, LNTypeInfoCreateInstanceCallback callback);
extern LN_FLAT_API LNResult LNTypeInfo_SetManagedTypeInfoId(int32_t typeInfoId, int32_t managedTypeInfoId);
extern LN_FLAT_API LNResult LNTypeInfo_GetManagedTypeInfoId(int32_t typeInfoId, int32_t* outManagedTypeInfoId);

typedef uint64_t LNSubinstanceId;
typedef LNSubinstanceId(*LNSubinstanceAllocFunc)(LNHandle object);
typedef void(*LNSubinstanceFreeFunc)(LNHandle object, LNSubinstanceId subinstanceId);

//==============================================================================

/**
	@brief	全てのオブジェクトのベースクラスです。
*/

/**
	@brief		オブジェクトを解放します。
	@param[in]	obj	: オブジェクトハンドル
	@details	指定されたオブジェクトの参照を解放します。
*/
LN_FLAT_API LNResult LNObject_Release(LNHandle obj);

/**
 *  @brief      オブジェクトの参照を取得します。
 *  @param[in]  obj	: オブジェクトハンドル
 *  @details    指定されたオブジェクトの参照カウントをインクリメントします。
 */
LN_FLAT_API LNResult LNObject_Retain(LNHandle obj);

/**
 *  @brief      ネイティブオブジェクトの参照カウントを取得します。これは内部的に使用される関数です。
 *  @param[in]  obj	: オブジェクトハンドル
 */
LN_FLAT_API LNResult LNObject_GetReferenceCount(LNHandle object, int* outReturn);
LN_FLAT_API int32_t _LNObject_GetReferenceCount(LNHandle obj);

// class LNWS_XXXX のインスタンスに対して set できる。
// Engine 内部で new されたインスタンスに対して呼び出すことはできない。
// Managed 側で Engine のクラスを継承して新たな型を作ったとき、それを登録するために使用する。
LN_FLAT_API LNResult LNObject_SetTypeInfoId(LNHandle obj, int typeInfoId);

//==============================================================================

//LN_FLAT_API void LNLog_SetLevel(LNLogLevel level);
//LN_FLAT_API void LNLog_AllocConsole(LNLogLevel level);
//LN_FLAT_API void LNLog_Write(LNLogLevel level, const LNChar* tag, const LNChar* text);
//LN_FLAT_API void LNLog_WriteA(LNLogLevel level, const char* tag, const char* text);
//LN_FLAT_API void LNLog_PrintA(LNLogLevel level, const char* tag, const char* format, ...);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


