// clang-format off
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

typedef intptr_t LNStructHandle;

typedef int8_t LNBool;
#define LN_TRUE 1
#define LN_FALSE 0

 
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

//==============================================================================
//
//==============================================================================
//typedef enum LNClearFlags {
//    LN_CLEAR_FLAGS_NONE  = 0x0000,
//    LN_CLEAR_FLAGS_COLOR = 0x0001,
//    LN_CLEAR_FLAGS_DEPTH = 0x0002,
//    LN_CLEAR_FLAGS_STENCIL = 0x0004,
//    LN_CLEAR_FLAGS_ALL = LN_CLEAR_FLAGS_COLOR | LN_CLEAR_FLAGS_DEPTH | LN_CLEAR_FLAGS_STENCIL,
//    LN_CLEAR_FLAGS__FORCE32 = 0x7FFFFFFF,
//} LNClearFlags;

typedef struct LNRenderPassRenderTargetDescriptor {
    LNHandle renderTarget;
	float clearColor[4]; // [R, G, B, A]
    LNBool clearEnable;
} LNRenderPassRenderTargetDescriptor;

typedef struct LNRenderPassDepthBufferDescriptor {
    LNHandle depthBuffer;
	float clearDepth;
	int32_t clearStencil;
    LNBool clearDepthEnable;
    LNBool clearStencilEnable;
} LNRenderPassDepthBufferDescriptor;

typedef struct LNRenderPassDescriptor {
    LNRenderPassRenderTargetDescriptor renderTargets[8];
    LNRenderPassDepthBufferDescriptor depthBuffer;
} LNRenderPassDescriptor;

extern LUMINO_API LNStructHandle LNRenderPassDescriptor_Get();
extern LUMINO_API void LNRenderPassDescriptor_SetRenderTarget(LNStructHandle handle, int32_t index, LNHandle renderTarget, float clearR, float clearG, float clearB, float clearA, LNBool clearEnable);
extern LUMINO_API void LNRenderPassDescriptor_SetDepthBuffer(LNStructHandle handle, LNHandle depthBuffer, float clearDepth, int32_t clearStencil, LNBool clearDepthEnable, LNBool clearStencilEnable);


//==============================================================================
//
//==============================================================================

extern LUMINO_API LNResult LNRuntime_Initialize();
extern LUMINO_API void LNRuntime_Terminate();

/**
 * Get the current color buffer.
 *
 * @param[out] outRenderTarget: LNRenderTargetTexture wrapped around the backbuffer.
 * 
 * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an LNRenderPass.
 */
extern LUMINO_API LNResult LNGraphicsContext_GetCurrentColorBuffer(LNHandle graphicsContext, LNHandle* outRenderTarget);

/**
 * Get the current depth buffer.
 *
 * @param[out] outDepthBuffer: LNDepthBuffer wrapped around the backbuffer.
 * 
 * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an LNRenderPass.
 */
extern LUMINO_API LNResult LNGraphicsContext_GetCurrentDepthBuffer(LNHandle graphicsContext, LNHandle* outDepthBuffer);

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
 * Sends the command list recorded in GraphicsCommandList to the GPU.
 */
extern LUMINO_API LNResult LNGraphicsContext_SubmitCommandList(LNHandle graphicsContext, LNHandle graphicsCommandList);

/**
 * バックバッファをフロントバッファに転送します。
 *
 * 外部コンテキストを使っている場合、この関数は使えません。
 *
 * BeginFrame, EndFrame, Present の流れは LNGraphicsContext_BeginFrame のコメントを参照してください。
 */
extern LUMINO_API LNResult LNGraphicsContext_Present(LNHandle graphicsContext);

//extern LUMINO_API LNResult LNGraphicsContext_Release(LNHandle* handle);

/**
　* Creates a new GraphicsCommandList.
　*/
extern LUMINO_API LNResult LNGraphicsCommandList_Create(LNHandle graphicsContext, LNHandle* outGraphicsCommandList);
extern LUMINO_API LNResult LNGraphicsCommandList_Reset(LNHandle renderingCommandList, LNHandle renderingViewPoint);


extern LUMINO_API LNResult LNGraphicsCommandList_BeginRenderPass(LNHandle renderingCommandList, LNRenderPassDescriptor descriptor, LNHandle* outRenderPass);

// いわゆるカメラ情報
extern LUMINO_API LNResult LNGraphicsViewPoint_Create(LNHandle* outGraphicsViewPoint);
extern LUMINO_API LNResult LNGraphicsViewPoint_SetupPerspective2D(LNHandle graphicsViewPoint, float x, float y, float z, float width, float height, float nearZ, float farZ);

extern LUMINO_API LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass);


//==============================================================================
// LNGLGraphicsContext
//==============================================================================

/**
 * Creates a LNGLGraphicsContext from the current OpenGL Context.
 */
extern LUMINO_API LNResult LNGLGraphicsContext_CreateFromCurrentGL(int32_t width, int32_t height, LNHandle* outGraphicsContext);


//==============================================================================
// LNRenderPass
//==============================================================================

extern LUMINO_API LNResult LNRenderPass_End(LNHandle renderPass);
 
//==============================================================================
// LNTexture
//==============================================================================

/**
 * Create a texture with the specified size.
 */
extern LUMINO_API LNResult LNTexture2D_Create(int32_t width, int32_t height, LNHandle* outTexture2D);

/**
 * 画像ファイルデータからテクスチャを作成します。
 */
extern LUMINO_API LNResult LNTexture2D_CreateFromImageFileData(const uint8_t* data, int32_t length, LNHandle* outTexture2D);

extern LUMINO_API LNResult LNTexture2D_GetContext(LNHandle texture2D, LNHandle* outTextureRenderingContext);

//==============================================================================
// LNMaterial
//==============================================================================
extern LUMINO_API LNResult LNMaterial_Create(LNHandle* outMaterial);
extern LUMINO_API LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle texture);

//==============================================================================
// LNTextureRenderingContext
//==============================================================================

extern LUMINO_API LNResult LNTextureRenderingContext_FillText(LNHandle textureRenderingContext);

extern LUMINO_API LNResult LNTextureRenderingContext_StrokeText(LNHandle textureRenderingContext);

//==============================================================================
// 
//==============================================================================
/** A value that represents a reference direction in 3D space. */
typedef enum LNSpriteBaseDirection {
    /** X+ (右向き) */
    LN_SPRITE_BASE_DIRECTION_XPLUS = 0,

    /** Y+ (上向き) */
    LN_SPRITE_BASE_DIRECTION_YPLUS = 1,

    /** Z+ (奥向き) */
    LN_SPRITE_BASE_DIRECTION_ZPLUS = 2,

    /** X- (左向き) */
    LN_SPRITE_BASE_DIRECTION_XMINUS = 3,

    /** Y- (下向き) */
    LN_SPRITE_BASE_DIRECTION_YMINUS = 4,

    /** Z- (手前向き) */
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

extern LUMINO_API LNResult LNSpriteRenderer_Get(LNHandle* outSpriteRenderer);
extern LUMINO_API LNResult LNSpriteRenderer_BeginBatch(
    LNHandle spriteRenderer,
    LNHandle graphicsCommandList,
    LNHandle material,
    const LNMatrix* transform);
extern LUMINO_API LNResult LNSpriteRenderer_EndBatch(LNHandle spriteRenderer);
extern LUMINO_API LNResult LNSpriteRenderer_DrawSprite(LNHandle spriteRenderer, const LNMatrix* localTransformOrNull, float width, float height, float anchorRatioX, float anchorRatioY, float uvRectX, float uvRectY, float uvRectW, float uvRectH, float r, float g, float b, float a, LNSpriteBaseDirection baseDirection, LNBillboardType billboardType);

//==============================================================================
// LNSpriteTextRenderer
//==============================================================================
extern LUMINO_API LNResult LNSpriteTextRenderer_Get(LNHandle* outSpriteTextRenderer);
extern LUMINO_API LNResult LNSpriteTextRenderer_BeginBatch(LNHandle spriteTextRenderer, LNHandle graphicsCommandList, LNHandle material, const LNMatrix* transform);
extern LUMINO_API LNResult LNSpriteTextRenderer_EndBatch(LNHandle spriteTextRenderer);
#if 0
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
