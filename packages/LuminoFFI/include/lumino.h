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

extern LUMINO_API LNResult LNRenderingContext_Create(LNHandle graphicsContext, LNHandle* outReturn);
extern LUMINO_API LNResult LNRenderingContext_Reset(LNHandle renderingContext, LNHandle renderingViewPoint, LNHandle graphicsContext_);

// いわゆるカメラ情報
extern LUMINO_API LNResult LNSceneRenderingViewPoint_Create(LNHandle* outRenderingViewPoint);
extern LUMINO_API LNResult LNSceneRenderingViewPoint_SetupPerspective2D(LNHandle renderingViewPoint, float x, float y, float z, float width, float height, float nearZ, float farZ);

extern LUMINO_API LNResult LNUnlitSceneRenderingPass_Create(LNHandle* outUnlitSceneRenderingPass);

extern LUMINO_API LNResult LNObject_Release(LNHandle obj);
extern LUMINO_API LNResult LNObject_Retain(LNHandle obj);
extern LUMINO_API LNResult LNObject_GetReferenceCount(LNHandle obj, int32_t* outReturn);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
