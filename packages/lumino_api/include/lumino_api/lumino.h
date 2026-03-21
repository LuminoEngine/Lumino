// clang-format off
#ifndef LUMINO_API_H_
#define LUMINO_API_H_

#include <stdint.h>

//------------------------------------------------------------------------------
// Export / visibility macros
//------------------------------------------------------------------------------
#if !defined(LUMINO_API)
    #if defined(__EMSCRIPTEN__)
        #include <emscripten.h>
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

//------------------------------------------------------------------------------
// Result / error codes
//------------------------------------------------------------------------------
typedef enum LNResult {
    /** 成功 */
    LN_OK = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

    /** ランタイム未初期化 */
    LN_RUNTIME_UNINITIALIZED = -2,

    /** 無効な引数 */
    LN_ERROR_INVALID_ARGUMENT = -3,

    /** 無効なハンドル */
    LN_ERROR_INVALID_HANDLE = -4,
} LNResult;

//------------------------------------------------------------------------------
// Handle type
//------------------------------------------------------------------------------

/**
 * オブジェクトハンドル。上位16bitが世代番号、下位16bitがインデックス。
 */
typedef uint32_t LNHandle;

/** 無効なハンドル */
#define LN_NULL_HANDLE ((LNHandle)0)

//------------------------------------------------------------------------------
// LNInstance
//------------------------------------------------------------------------------

/**
 * Lumino のインスタンスを初期化します。
 */
extern LUMINO_API LNResult LNInstance_Initialize();

/**
 * Lumino のインスタンスを終了します。
 */
extern LUMINO_API void LNInstance_Terminate();

//------------------------------------------------------------------------------
// LNObject
//------------------------------------------------------------------------------

/**
 * ハンドルに対応するオブジェクトを解放します。
 */
extern LUMINO_API LNResult LNObject_Release(LNHandle handle);

//------------------------------------------------------------------------------
// LNWindow
//------------------------------------------------------------------------------

/**
 * ウィンドウを作成します。内部で GraphicsContext も作成されます。
 * @param[in]  title     ウィンドウタイトル
 * @param[in]  width     幅 (ピクセル)
 * @param[in]  height    高さ (ピクセル)
 * @param[out] outHandle 作成されたウィンドウのハンドル
 */
extern LUMINO_API LNResult LNWindow_Create(
    const char* title,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle
);

/**
 * ウィンドウに関連付けられた GraphicsContext のハンドルを取得します。
 * 繰り返し呼び出しても同じハンドルを返します。
 * @param[in]  handle            ウィンドウのハンドル
 * @param[out] outGraphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNWindow_GetGraphicsContext(LNHandle handle, LNHandle* outGraphicsContext);

/**
 * ウィンドウのイベントを処理します。
 * @param[in]  handle      ウィンドウのハンドル
 * @param[out] outContinue ループ継続可否 (0 で終了)
 */
extern LUMINO_API LNResult LNWindow_ProcessEvents(LNHandle handle, int* outContinue);

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

/**
 * フレームの描画を開始します。
 * @param[in] graphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext);

/**
 * レンダーパスを開始します。指定したカラーでレンダーターゲットをクリアします。
 * @param[in] graphicsContext GraphicsContext のハンドル
 * @param[in] r               クリアカラー R (0.0 - 1.0)
 * @param[in] g               クリアカラー G (0.0 - 1.0)
 * @param[in] b               クリアカラー B (0.0 - 1.0)
 * @param[in] a               クリアカラー A (0.0 - 1.0)
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginRenderPass(
    LNHandle graphicsContext, float r, float g, float b, float a);

/**
 * レンダーパスを終了します。
 * @param[in] graphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_EndRenderPass(LNHandle graphicsContext);

/**
 * フレームの描画を終了し、画面に表示します。
 * @param[in] graphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext);

//------------------------------------------------------------------------------
// LNTexture2D
//------------------------------------------------------------------------------

/**
 * 2D テクスチャを作成します。
 * @param[in]  width     幅 (ピクセル)
 * @param[in]  height    高さ (ピクセル)
 * @param[in]  format    テクスチャフォーマット
 * @param[out] outHandle 作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_Create(
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle
);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
