// clang-format off
#ifndef LUMINO_API_H_
#define LUMINO_API_H_
#include "lumino_types.h"
#include "lumino_audio.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
// LNConfig
//==============================================================================

/** Graphics Backend. */
typedef enum LNGraphicsBackend {
    /** Default. */
    LN_GRAPHICS_BACKEND_DEFAULT = 0,
    
    LN_GRAPHICS_BACKEND_DIRECTX12 = 1,
    LN_GRAPHICS_BACKEND_VULKAN = 2,
    //N_GRAPHICS_BACKEND_METAL = 3,
    
    /** OpenGL. */
    LN_GRAPHICS_BACKEND_OPENGL = 4,

    /** WebGPU. */
    LN_GRAPHICS_BACKEND_WEBGPU = 5,
} LNGraphicsBackend;

extern LUMINO_API LNResult LNConfig_SetGraphicsBackend(LNGraphicsBackend value);

//==============================================================================
// LNInstance
//==============================================================================

/**
 * Initializes the Lumino instance.
 */
extern LUMINO_API LNResult LNInstance_Initialize();

/**
 * Terminates the Lumino instance.
 */
extern LUMINO_API void LNInstance_Terminate();

/**
 * Processes events that are present in the event queue.
 */
extern LUMINO_API LNResult LNInstance_ProcessEvents();

/**
 * Returns whether the application should quit.
 */
extern LUMINO_API LNResult LNInstance_ShouldQuit(LNBool* outQuit);
 
//==============================================================================
//
//==============================================================================

/**
 * Begin rendering the frame.
 * 
 * @param[int] graphicsContext : LNGraphicsContext handle.
 * @param[int] width : Backbuffer width.
 * @param[int] height : Backbuffer height.
 * @param[out] outRenderTarget : RenderTarget color buffer.
 * @param[out] outDepthBuffer : RenderTarget depth buffer.
 * @param[out] outCommandList : Graphics CommandList.
 * 
 * The back buffer will be resized if necessary.
 * but if you are using an external context, the back buffer will not be resized.
 *
 * outRenderTarget and outDepthBuffer is a special buffer that represents
 * the default Framebuffer and can only be used to attach to an LNRenderPass.
 * 
 * The frame must be ended by @see LNGraphicsContext_EndFrame.
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    int32_t width,
    int32_t height,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer
);

/**
 * Ends the frame.
 * 
 * @param[int] graphicsContext : LNGraphicsContext handle.
 */
extern LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext);

/**
 * 
 */
extern LUMINO_API LNResult LNGraphicsContext_CreateFromWebGPUCanvas(const char* selector, LNHandle* outGraphicsContext);

/**
 * Creates a LNGLGraphicsContext from the current OpenGL Context.
 */
extern LUMINO_API LNResult LNGraphicsContext_CreateFromCurrentGL(int32_t width, int32_t height, LNHandle* outGraphicsContext);


extern LUMINO_API LNResult LNGraphicsContext_BeginSceneRenderPass(LNHandle renderingCommandList, LNRenderPassDescriptor descriptor, LNHandle renderingViewPoint, LNHandle* outSceneRenderPass);

extern LUMINO_API LNResult LNGraphicsContext_EndSceneRenderPass(LNHandle renderingCommandList, LNHandle sceneRenderPass);


//==============================================================================
// LNSceneRenderPass
//==============================================================================
typedef struct LNDrawSpriteParams {
    LNHandle material;
    LNMatrix worldTransform;
    LNVector2 size;
    LNVector2 anchorRatio;
    LNRect uvRect;
    LNColor color;
    LNSpriteBaseDirection baseDirection;
    LNBillboardType billboardType;
} LNDrawSpriteParams;

/** @deprecated */
extern LUMINO_API LNResult LNSceneRenderPass_DrawSprite(
    LNHandle sceneRenderPass,
    const LNDrawSpriteParams* params);

/**
 * RenderItem を描画します。
 */
extern LUMINO_API LNResult LNSceneRenderPass_DrawRenderItem(
    LNHandle sceneRenderPass,
    LNHandle renderItem);

//==============================================================================
// LNMesh
//==============================================================================

extern LUMINO_API LNResult LNMesh_Create(LNHandle* outMesh);

// シンプルな四角形スプライトを表示する MeshSurface を追加します。
// ※1つのMeshSurfaceで複数のスプライトを一度に描きたい場合は、MeshSurfaceを手動で構築する必要があります。（その方法は未実装）
extern LUMINO_API LNResult LNMesh_AddSprite2DSurface(
    LNHandle mesh,
    LNHandle material,
    LNVector2 size,
    LNVector2 anchor,
    LNRect uvRect,
    LNColor color);

//==============================================================================
// LNRenderItem
//==============================================================================

/**
 * RenderItem を作成します。
 * 
 * @param[out] outRenderItem : 作成された描画アイテムのハンドル
 */
extern LUMINO_API LNResult LNRenderItem_Create(LNHandle* outRenderItem);

/**
 * 描画アイテムのワールド変換行列を設定します。
 * 
 * @param[in] renderItem : 描画アイテムのハンドル
 * @param[in] transform : 変換行列
 */
extern LUMINO_API LNResult LNRenderItem_SetTransform(
    LNHandle renderItem,
    const LNMatrix* transform);

/**
 */
extern LUMINO_API LNResult LNRenderItem_SetMesh(LNHandle renderItem, LNHandle mesh);


//==============================================================================
// LNDebug
//==============================================================================

/**
 * Prints a string.
 * 
 * Draws a string in the top left corner of the screen using the built-in bitmap font.

 * Note the following restrictions:
 * - Only ASCII characters can be used.
 * - A maximum of 512 characters can be drawn.
 * - The actual drawing takes place when LNGraphicsContext_EndFrame is called.
 */
extern LUMINO_API LNResult LNDebug_Print(LNHandle graphicsContext, const char* str);

typedef struct LNGraphicsProfilerng {
    int32_t drawCallCount;
    float actualFPS;
    float lastFrameTime;
} LNGraphicsProfilerng;

extern LUMINO_API LNResult LNDebug_GetGraphicsProfilerng(LNHandle graphicsContext, LNGraphicsProfilerng* outProfilerng);

//==============================================================================
//
//==============================================================================
// いわゆるカメラ情報
extern LUMINO_API LNResult LNViewPoint_Create(LNHandle* outGraphicsViewPoint);
extern LUMINO_API LNResult LNViewPoint_SetupOrtho2D(LNHandle graphicsViewPoint, float x, float y, float z, float width, float height, float nearZ, float farZ);

extern LUMINO_API LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass);



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

/**
 * Loads an image file and creates a texture.
 * 
 * @attention This function is a utility. It is only available in the desktop environment.
 */
extern LUMINO_API LNResult LNTexture2D_CreateFromImageFile(const char* filePathUTF8, LNHandle* outTexture2D);


extern LUMINO_API LNResult LNTexture2D_GetContext(LNHandle texture2D, LNHandle* outTextureRenderingContext);

//==============================================================================
// LNShader
//==============================================================================
extern LUMINO_API LNResult LNShader_CreateFromSourceCode(const void* code, int32_t length, LNHandle* outShader);

//==============================================================================
// LNMaterial
//==============================================================================
extern LUMINO_API LNResult LNMaterial_Create(LNHandle* outMaterial);

/**
 * Compile the shader source files to create the material.
 * 
 * @attention This function is a utility. It is only available in the desktop environment.
 */
extern LUMINO_API LNResult LNMaterial_CreateFromSourceFile(const char* filePathUTF8, LNHandle* outMaterial);

extern LUMINO_API LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle texture);
extern LUMINO_API LNResult LNMaterial_SetShader(LNHandle material, LNHandle shader);
extern LUMINO_API LNResult LNMaterial_FindParameterIndex(LNHandle material, const char* name, int32_t* outIndex);
extern LUMINO_API LNResult LNMaterial_SetFloat(LNHandle material, int32_t parameterIndex, float value);

/**
 * 2 つの Material が、同じ Batch で描画できるかを調べます。
 */
extern LUMINO_API LNResult LNMaterial_CanCombined(LNHandle material1, LNHandle material2, LNBool* outResult);

//==============================================================================
// LNTextureRenderingContext
//==============================================================================

extern LUMINO_API LNResult LNTextureRenderingContext_FillText(LNHandle textureRenderingContext);

extern LUMINO_API LNResult LNTextureRenderingContext_StrokeText(LNHandle textureRenderingContext);

//==============================================================================
// 
//==============================================================================

extern LUMINO_API LNResult LNBatchRenderer_Get(LNHandle* outSpriteRenderer);
extern LUMINO_API LNResult LNBatchRenderer_BeginBatch(
    LNHandle spriteRenderer,
    LNHandle graphicsCommandList,
    LNHandle material,
    const LNMatrix* transform);
extern LUMINO_API LNResult LNBatchRenderer_EndBatch(LNHandle spriteRenderer);

/**
 * Draw a sprite.
 * 
 * [r, g, b] is the vertex color, which is usually a multiplication color.
 * Specifying 0 will make the sprite black. Normally, specify 1.
 */
extern LUMINO_API LNResult LNBatchRenderer_DrawSprite_deprecated(
    LNHandle spriteRenderer,
    const LNMatrix* localTransformOrNull,
    float width, float height, float anchorRatioX, float anchorRatioY,
    float uvRectX, float uvRectY, float uvRectW, float uvRectH,
    float r, float g, float b, float a,
    LNSpriteBaseDirection baseDirection,
    LNBillboardType billboardType);

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
// LNWindow
//==============================================================================
extern LUMINO_API LNResult LNWindow_Create(int32_t width, int32_t height, const LNChar* title, LNHandle* outWindow);
extern LUMINO_API LNResult LNWindow_GetFramebufferSize(LNHandle window, int32_t* outWidth, int32_t* outHeight);
extern LUMINO_API LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outGraphicsContext);

/**
 * Transfers the back buffer to the front buffer.
 * 
 * The time since the last Present is waited to be the specified FPS.
 * 
 * This function is not available when using an external context.
 */
extern LUMINO_API LNResult LNWindow_Present(LNHandle window);

//==============================================================================
// LNObject
//==============================================================================

/**
 * Releases the reference to the object.
 */
extern LUMINO_API LNResult LNObject_Release(LNHandle obj);

/**
 * Retains the reference to the object.
 */
extern LUMINO_API LNResult LNObject_Retain(LNHandle obj);
extern LUMINO_API LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
