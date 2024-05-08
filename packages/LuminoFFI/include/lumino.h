#ifndef LUMINO_API_H_
#define LUMINO_API_H_
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
#define LN_DEFINE_HANDLE(object) typedef struct object##_T* object

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

extern LUMINO_API LNResult LNMatrix_SetIdentity(LNMatrix* pMatrix);
// extern LUMINO_API LNResult LNSpriteRenderer_Translate(LNMatrix* matrix, float x, float y, float z);
// extern LUMINO_API LNResult LNSpriteRenderer_Translate(LNMatrix* matrix, float x, float y, float z);

//==============================================================================
//
//==============================================================================

extern LUMINO_API LNResult LNRuntime_Initialize();
extern LUMINO_API void LNRuntime_Terminate();

extern LUMINO_API LNResult LNGraphicsContext_CreateFromCurrentOpenGLContext(int32_t width, int32_t height, LNHandle* outReturn);

/**
 * 描画フレームを開始します。
 *
 * @param[in] graphicsContext : 対象 LNGraphicsContext のハンドル。
 * @param[in] width  : 描画先のバックバッファのテクセル幅。
 * @param[in] height : 描画先のバックバッファのテクセル高さ。
 * 
 * 描画フレームは 1 フレーム分の一連の描画コマンドの構築処理です。
 * LNGraphicsContext_BeginFrame でネイティブのコマンドリストへの記録を開始し、
 * LNGraphicsContext_EndFrame で記録を終了します。
 * そして LNGraphicsContext_Present でコマンドリストを GPU へ送信します。
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext, int32_t width, int32_t height);

/**
 * 描画フレームを終了します。
 *
 * 内部的には ネイティブのコマンドリストへの記録を終了します。
 * 外部コンテキストを使っている場合、それぞれの方法で GPU への送信を行う必要があります。
 */
extern LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext);

/**
 * バックバッファをフロントバッファに転送します。
 *
 * 外部コンテキストを使っている場合、この関数は使えません。
 *
 * BeginFrame, EndFrame, Present の流れは LNGraphicsContext_BeginFrame のコメントを参照してください。
 */
extern LUMINO_API LNResult LNGraphicsContext_Present(LNHandle graphicsContext);

//extern LUMINO_API LNResult LNGraphicsContext_Release(LNHandle* handle);

extern LUMINO_API LNResult LNRenderingCommandList_Create(LNHandle graphicsContext, LNHandle* outRenderingCommandList);
extern LUMINO_API LNResult LNRenderingCommandList_Reset(LNHandle renderingCommandList, LNHandle renderingViewPoint, LNHandle graphicsContext_);

/**
 * RenderingCommandList に記録されているコマンドリストを GPUCommandList に変換します。
 * 
 * @param[in] renderingCommandList : 対象の RenderingCommandList のハンドル。
 * @param[in] sceneRenderingPass : 対象の RenderingCommandList のハンドル。
 * @param[in] graphicsContext : 対象の RenderingCommandList のハンドル。
 */
extern LUMINO_API LNResult LNRenderingCommandList_Submit(LNHandle renderingCommandList, LNHandle sceneRenderingPass, LNHandle graphicsContext);

// いわゆるカメラ情報
extern LUMINO_API LNResult LNSceneRenderingViewPoint_Create(LNHandle* outRenderingViewPoint);
extern LUMINO_API LNResult LNSceneRenderingViewPoint_SetupPerspective2D(LNHandle renderingViewPoint, float x, float y, float z, float width, float height, float nearZ, float farZ);

extern LUMINO_API LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass);

//==============================================================================
// LNMaterial
//==============================================================================
LN_DEFINE_HANDLE(LNMaterial);
extern LUMINO_API LNResult LNMaterial_Create(LNMaterial* outMaterial);
extern LUMINO_API LNResult LNMaterial_Release(LNMaterial material);

//==============================================================================
// 
//==============================================================================
/** 3D 空間での基準方向を表す値 */
typedef enum LNSpriteBaseDirection {
    /** X + 方向(右向き) */
    LN_SPRITE_BASE_DIRECTION_XPLUS = 0,

    /**  Y+ 向 (上向き) */
    LN_SPRITE_BASE_DIRECTION_YPLUS = 1,

    /**  Z+ 向 (奥向き) */
    LN_SPRITE_BASE_DIRECTION_ZPLUS = 2,

    /**  X- 向 (左向き) */
    LN_SPRITE_BASE_DIRECTION_XMINUS = 3,

    /**  Y- 向 (下向き) */
    LN_SPRITE_BASE_DIRECTION_YMINUS = 4,

    /**  Z- 向 (手前向き) */
    LN_SPRITE_BASE_DIRECTION_ZMINUS = 5,

    /**  2D 基本方向 (Z+ 向、左上原点) */
    LN_SPRITE_BASE_DIRECTION_BASIC2D = 6,

    LN_SPRITE_BASE_DIRECTION_MAX_ENUM = 0x7FFFFFFF,
} LNSpriteBaseDirection;

/** ビルボードの計算方法 */
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

extern LUMINO_API LNResult LNSpriteRenderer_Get(LNHandle* outSpriteRenderer);
extern LUMINO_API LNResult LNSpriteRenderer_BeginBatch(LNHandle spriteRenderer, LNHandle renderingCommandList, LNMaterial material, const LNMatrix* transform);
extern LUMINO_API LNResult LNSpriteRenderer_EndBatch(LNHandle spriteRenderer);
extern LUMINO_API LNResult LNSpriteRenderer_DrawSprite(LNHandle spriteRenderer, const LNMatrix* localTransformOrNull, float width, float height, float anchorRatioX, float anchorRatioY, float uvRectX, float uvRectY, float uvRectW, float uvRectH, float r, float g, float b, float a, LNSpriteBaseDirection baseDirection, LNBillboardType billboardType);

//==============================================================================
// LNSpriteTextRenderer
//==============================================================================
#if 0
extern LUMINO_API LNResult LNSpriteTextRenderer_Get(LNHandle* outSpriteTextRenderer);
extern LUMINO_API LNResult LNSpriteTextRenderer_BeginBatch(LNHandle spriteTextRenderer, LNHandle renderingCommandList, LNHandle material, const LNMatrix* transform);
extern LUMINO_API LNResult LNSpriteTextRenderer_EndBatch(LNHandle spriteTextRenderer);
extern LUMINO_API LNResult LNSpriteTextRenderer_DrawFillText(LNHandle spriteTextRenderer, const LNMatrix* localTransformOrNull, const char* text);
#endif

//==============================================================================
// LNObject
//==============================================================================

extern LUMINO_API LNResult LNObject_Release(LNHandle obj);
extern LUMINO_API LNResult LNObject_Retain(LNHandle obj);
extern LUMINO_API LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
