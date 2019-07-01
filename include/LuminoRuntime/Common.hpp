#pragma once

#if !defined(LN_API)
    #if defined(_WIN32)
        #define LN_API __declspec(dllexport)
    #elif defined(_WIN32) && defined(LUMINO_DLL)
        #define LN_API __declspec(dllimport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LN_API __attribute__((visibility("default")))
    #else
        #define LN_API
    #endif
#endif


#define LNI_FUNC_TRY_BEGIN    try {
#define LNI_FUNC_TRY_END_RETURN    } \
    catch (ln::Exception& e) { \
        return ln::detail::RuntimeManager::procException(&e); \
    } \
    catch (...) { \
        return LN_ERROR_UNKNOWN; \
    } \
	return LN_SUCCESS;

#define LNI_BOOL_TO_LNBOOL(x)    (x) ? LN_TRUE : LN_FALSE
#define LNI_LNBOOL_TO_BOOL(x)    (x != LN_FALSE)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
    /**< 偽 */
    LN_FALSE = 0,

    /**< 真 */
    LN_TRUE = 1,

} LnBool;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


