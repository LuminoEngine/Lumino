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
// Value structs
//------------------------------------------------------------------------------

/**
 * 標準頂点 (64 bytes, ln::Vertex と同一レイアウト)。
 */
typedef struct LNVertex {
    float posX, posY, posZ;
    float normX, normY, normZ;
    float u, v;
    float colorR, colorG, colorB, colorA;
    float tanX, tanY, tanZ, tanW;
} LNVertex;

/**
 * サブメッシュ (インデックスバッファの部分範囲 + マテリアルインデックス)。
 */
typedef struct LNSubMesh {
    uint32_t indexOffset;
    uint32_t indexCount;
    uint32_t materialIndex;
} LNSubMesh;

/**
 * TRS トランスフォーム。
 */
typedef struct LNTransform {
    float posX, posY, posZ;
    float rotX, rotY, rotZ, rotW;   /* quaternion (x, y, z, w) */
    float scaleX, scaleY, scaleZ;
} LNTransform;

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

/**
 * 画像ファイルから 2D テクスチャを読み込みます (PNG, JPG, BMP, TGA 等)。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  filePath        画像ファイルパス (UTF-8)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_LoadFromFile(
    LNHandle graphicsContext,
    const char* filePath,
    LNHandle* outHandle
);

//------------------------------------------------------------------------------
// LNMaterial
//------------------------------------------------------------------------------

/**
 * Unlit マテリアルを作成します (テクスチャ * カラー、ライティングなし)。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outHandle       作成されたマテリアルのハンドル
 */
extern LUMINO_API LNResult LNMaterial_CreateUnlit(
    LNHandle graphicsContext,
    LNHandle* outHandle
);

/**
 * マテリアルのベースカラーを設定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] r,g,b,a  カラー成分 (0.0 - 1.0)
 */
extern LUMINO_API LNResult LNMaterial_SetColor(
    LNHandle material,
    float r, float g, float b, float a
);

/**
 * マテリアルのベーステクスチャを設定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] texture  Texture2D のハンドル
 */
extern LUMINO_API LNResult LNMaterial_SetTexture(
    LNHandle material,
    LNHandle texture
);

/**
 * パラメータ変更後に GPU バインドグループを再構築します。
 * Set* 呼び出し後、描画前に呼ぶ必要があります。
 * @param[in] material         マテリアルのハンドル
 * @param[in] graphicsContext  GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNMaterial_UpdateBindGroup(
    LNHandle material,
    LNHandle graphicsContext
);

//------------------------------------------------------------------------------
// LNMesh
//------------------------------------------------------------------------------

/**
 * 頂点・インデックスデータからメッシュを作成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  vertices        LNVertex 配列
 * @param[in]  vertexCount     頂点数
 * @param[in]  indices         uint32_t インデックス配列
 * @param[in]  indexCount       インデックス数
 * @param[in]  submeshes       LNSubMesh 配列
 * @param[in]  submeshCount     サブメッシュ数
 * @param[out] outHandle        作成されたメッシュのハンドル
 */
extern LUMINO_API LNResult LNMesh_Create(
    LNHandle graphicsContext,
    const LNVertex* vertices,
    uint32_t vertexCount,
    const uint32_t* indices,
    uint32_t indexCount,
    const LNSubMesh* submeshes,
    uint32_t submeshCount,
    LNHandle* outHandle
);

/**
 * メッシュの指定マテリアルスロットにマテリアルを設定します。
 * @param[in] mesh            メッシュのハンドル
 * @param[in] materialIndex   マテリアルスロットインデックス
 * @param[in] material        マテリアルのハンドル
 */
extern LUMINO_API LNResult LNMesh_SetMaterial(
    LNHandle mesh,
    uint32_t materialIndex,
    LNHandle material
);

//------------------------------------------------------------------------------
// LNCamera
//------------------------------------------------------------------------------

/**
 * デフォルト設定のカメラを作成します。
 * @param[out] outHandle 作成されたカメラのハンドル
 */
extern LUMINO_API LNResult LNCamera_Create(LNHandle* outHandle);

/**
 * カメラに透視投影を設定します。
 * @param[in] camera   カメラのハンドル
 * @param[in] fovY     垂直視野角 (ラジアン)
 * @param[in] aspect   アスペクト比 (幅 / 高さ)
 * @param[in] nearClip ニアクリップ距離
 * @param[in] farClip  ファークリップ距離
 */
extern LUMINO_API LNResult LNCamera_SetPerspective(
    LNHandle camera,
    float fovY, float aspect, float nearClip, float farClip
);

/**
 * カメラの注視点を設定します。
 * @param[in] camera                   カメラのハンドル
 * @param[in] eyeX, eyeY, eyeZ        視点位置
 * @param[in] targetX, targetY, targetZ 注視点
 * @param[in] upX, upY, upZ            上方向ベクトル
 */
extern LUMINO_API LNResult LNCamera_SetLookAt(
    LNHandle camera,
    float eyeX, float eyeY, float eyeZ,
    float targetX, float targetY, float targetZ,
    float upX, float upY, float upZ
);

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

/**
 * Renderer を作成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outHandle       作成された Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_Create(
    LNHandle graphicsContext,
    LNHandle* outHandle
);

/**
 * フレームを開始します。フレーム毎のアロケータをリセットし、コマンドバッファを取得します。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_BeginFrame(LNHandle renderer);

/**
 * フレームを終了し、記録したコマンドを GPU に送信します。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_EndFrame(LNHandle renderer);

/**
 * レンダーパスを開始します。スワップチェインのカラー/デプスターゲットを使用します。
 * @param[in] renderer         Renderer のハンドル
 * @param[in] graphicsContext  GraphicsContext のハンドル (現フレームのターゲット取得用)
 * @param[in] r,g,b,a          クリアカラー
 */
extern LUMINO_API LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer,
    LNHandle graphicsContext,
    float r, float g, float b, float a
);

/**
 * レンダーパスを終了します。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_EndRenderPass(LNHandle renderer);

/**
 * カメラからビューバインドグループ (set=0) を作成します。
 * ViewParamsUBO を構築し、Renderer の viewBindGroupLayout を使って BindGroup を作成します。
 * ライトパラメータはゼロ (Unlit レンダリング用)。
 * @param[in]  renderer         Renderer のハンドル
 * @param[in]  graphicsContext  GraphicsContext のハンドル
 * @param[in]  camera           カメラのハンドル
 * @param[out] outBindGroup     作成されたバインドグループのハンドル
 */
extern LUMINO_API LNResult LNRenderer_CreateViewBindGroup(
    LNHandle renderer,
    LNHandle graphicsContext,
    LNHandle camera,
    LNHandle* outBindGroup
);

/**
 * 生の行列とカメラ位置からビューバインドグループ (set=0) を作成します。
 * ライトパラメータはゼロ。
 * @param[in]  renderer           Renderer のハンドル
 * @param[in]  graphicsContext    GraphicsContext のハンドル
 * @param[in]  viewProjMatrix     列優先 4x4 行列 (16 floats)
 * @param[in]  cameraPosX,Y,Z     カメラのワールド位置
 * @param[out] outBindGroup        作成されたバインドグループのハンドル
 */
extern LUMINO_API LNResult LNRenderer_CreateViewBindGroupFromMatrix(
    LNHandle renderer,
    LNHandle graphicsContext,
    const float* viewProjMatrix,
    float cameraPosX, float cameraPosY, float cameraPosZ,
    LNHandle* outBindGroup
);

/**
 * 現在のレンダーパスにビューバインドグループ (set=0) を設定します。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] bindGroup  CreateViewBindGroup で作成したバインドグループのハンドル
 */
extern LUMINO_API LNResult LNRenderer_SetViewBindGroup(
    LNHandle renderer,
    LNHandle bindGroup
);

/**
 * メッシュを描画します。メッシュに設定されたマテリアルを使用します。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] mesh       メッシュのハンドル
 * @param[in] transform  LNTransform へのポインタ (NULL で単位変換)
 */
extern LUMINO_API LNResult LNRenderer_DrawMesh(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform
);

/**
 * 明示的なマテリアルを使ってメッシュを描画します。
 * メッシュ自身のマテリアルは無視されます。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] mesh       メッシュのハンドル
 * @param[in] transform  LNTransform へのポインタ (NULL で単位変換)
 * @param[in] material   マテリアルのハンドル
 */
extern LUMINO_API LNResult LNRenderer_DrawMeshWithMaterial(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform,
    LNHandle material
);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
