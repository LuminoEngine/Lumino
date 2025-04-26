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

/** A value that represents a reference direction in 3D space. */
typedef enum LNSpriteBaseDirection {
    /** X+ (右向き) */
    LN_SPRITE_BASE_DIRECTION_XPLUS = 0,

    /** Y+ (上向き) */
    LN_SPRITE_BASE_DIRECTION_YPLUS = 1,

    /** Z+ */
    LN_SPRITE_BASE_DIRECTION_ZPLUS = 2,

    /** X- (左向き) */
    LN_SPRITE_BASE_DIRECTION_XMINUS = 3,

    /** Y- (下向き) */
    LN_SPRITE_BASE_DIRECTION_YMINUS = 4,

    /** Z- */
    LN_SPRITE_BASE_DIRECTION_ZMINUS = 5,

    /** 2D 基本方向 (Z+ 向、左上原点) */
    LN_SPRITE_BASE_DIRECTION_BASIC2D = 6,

    LN_SPRITE_BASE_DIRECTION_MAX_ENUM = 0x7FFFFFFF,
} LNSpriteBaseDirection;

/** How to calculate billboards.*/
typedef enum LNBillboardType {
    /** ビルボードの計算を行わない */
    LN_BILLBOARD_TYPE_NONE = 0,

    /** カメラ (ビュー行列) に対して正面を向く */
    LN_BILLBOARD_TYPE_TO_CAMERA_POINT = 1,

    /** スクリーン (ビュー平面) に対して正面を向く */
    LN_BILLBOARD_TYPE_TO_SCREEN = 2,

    /** Y 軸回転のみ行う */
    LN_BILLBOARD_TYPE_ROT_Y = 3,

    LN_BILLBOARD_TYPE_MAX_ENUM = 0x7FFFFFFF,
} LNBillboardType;

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
// LNMatrix
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

//==============================================================================
// LNRect
//==============================================================================
typedef struct LNRect {
    float x;
    float y;
    float width;
    float height;
} LNRect;

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_TYPES_H_
