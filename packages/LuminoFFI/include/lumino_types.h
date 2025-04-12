// clang-format off
#ifndef LUMINO_TYPES_H_
#define LUMINO_TYPES_H_
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#if !defined(LUMINO_API)
    #if defined(__EMSCRIPTEN__)
        #define LUMINO_API EMSCRIPTEN_KEEPALIVE
    #elif defined(_WIN32) && defined(LUMINO_BUILD_DLL)
        #define LUMINO_API __declspec(dllexport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LUMINO_API __attribute__((visibility("default")))
    #else
        #define LUMINO_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** 結果・エラーコード */
typedef enum LNResult {
    /** 成功 */
    LN_OK = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

    /**  */
    LN_RUNTIME_UNINITIALIZED = -2,

    /**  */
    LN_ERROR_INVALID_ARGUMENT = -3,

} LNResult;

/** Lumino のオブジェクトを識別するための値です。0 (LN_NULL_HANDLE) は無効値です。 */
typedef int32_t LNHandle;
#define LN_NULL_HANDLE 0

typedef intptr_t LNStructHandle;

/** UTF-8 char. */
typedef char LNChar;

typedef int8_t LNBool;
#define LN_TRUE 1
#define LN_FALSE 0

//==============================================================================
// LNVector2
//==============================================================================
typedef struct LNVector2 {
    float x;
    float y;
} LNVector2;

//==============================================================================
// LNVector3
//==============================================================================
typedef struct LNVector3 {
    float x;
    float y;
    float z;
} LNVector3;

//==============================================================================
//
//==============================================================================
typedef struct LNMatrix {
    union {
        struct
        {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
        float m[4][4];
    };
} LNMatrix;

//extern LUMINO_API LNStructHandle LNMatrix_New();
//extern LUMINO_API void LNMatrix_Delete(LNStructHandle matrix);
extern LUMINO_API void LNMatrix_SetIdentity(LNMatrix* pMatrix);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_TYPES_H_
