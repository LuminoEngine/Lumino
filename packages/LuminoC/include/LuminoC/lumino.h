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
    
typedef uint32_t LNBool;
#define LN_FALSE 0U
#define LN_TRUE 1U

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
typedef struct LNInstanceInitializeSettings {
    LNBool enableValidation; /**< デバッグ用のバリデーションレイヤーを有効にするか */
} LNInstanceInitializeSettings;

/**
 * Lumino のインスタンスを初期化します。
 */
extern LUMINO_API LNResult LNInstance_Initialize(const LNInstanceInitializeSettings* settings);

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
 * フレームの描画を開始します。内部で Renderer の beginFrame も呼び出します。
 * 返された renderer ハンドルは graphicsContext が管理するため、
 * LNObject_Release を呼ぶ必要はありません。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outRenderer     Renderer のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext, LNHandle* outRenderer);

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
 * 内部で Renderer の endFrame と GPU コマンド送信も行います。
 * LNGraphicsContext_BeginFrame を呼んだフレームに対応して呼び出してください。
 */
extern LUMINO_API LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext);

/**
 * @internal GPU の処理が完了するまで待機します。
 * 通常は必要ありません。主にライブラリ内部動作の問題調査に使用します。
 */
extern LUMINO_API LNResult LNGraphicsContext_WaitIdle(LNHandle graphicsContext);

/**
 * 直前のフレームのバックバッファの内容を RGBA8 ピクセルデータとして取得します。
 * LNGraphicsContext_EndFrame の後に呼び出してください。
 * 返されるポインタは、次の LNGraphicsContext_CaptureBackbuffer 呼び出しまで有効です。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outData         ピクセルデータの先頭ポインタ
 * @param[out] outWidth        画像の幅 (ピクセル)
 * @param[out] outHeight       画像の高さ (ピクセル)
 */
extern LUMINO_API LNResult LNGraphicsContext_CaptureBackbuffer(
    LNHandle graphicsContext,
    const uint8_t** outData,
    int32_t* outWidth,
    int32_t* outHeight
);

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
 * レンダーターゲットテクスチャを作成します。
 * 内部で BGRA8Unorm カラーテクスチャと Depth24Stencil8 深度テクスチャを生成します。
 * 作成されたテクスチャは LNRenderer_BeginRenderPassToTexture で描画先として使用でき、
 * 描画後は LNMaterial_SetMainTexture でマテリアルに設定してサンプリングできます。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  width           幅 (ピクセル)
 * @param[in]  height          高さ (ピクセル)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_CreateRenderTarget(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
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

/** ビルトインシェーダの種類 */
typedef enum LNBuiltinShader {
    /** Unlit: テクスチャ * カラー、ライティングなし */
    LN_BUILTIN_SHADER_UNLIT       = 0,
    /** BasicLit: Blinn-Phong、1方向ライト */
    LN_BUILTIN_SHADER_BASIC_LIT   = 1,
    /** StencilMask: ステンシルバッファへの書き込み専用 */
    LN_BUILTIN_SHADER_STENCIL_MASK = 2,
} LNBuiltinShader;

/**
 * ビルトインシェーダを指定してマテリアルを作成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  shader          使用するビルトインシェーダ
 * @param[out] outHandle       作成されたマテリアルのハンドル
 */
extern LUMINO_API LNResult LNMaterial_CreateFromBuiltinShader(
    LNHandle graphicsContext,
    LNBuiltinShader shader,
    LNHandle* outHandle
);

/**
 * コンパイル済みシェーダ (.lcsh) のバイナリデータからマテリアルを作成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  data            コンパイル済みシェーダのバイナリデータ
 * @param[in]  size            バイナリデータのサイズ (バイト)
 * @param[out] outHandle       作成されたマテリアルのハンドル
 */
extern LUMINO_API LNResult LNMaterial_CreateFromCompiledShader(
    LNHandle graphicsContext,
    const void* data,
    uint32_t size,
    LNHandle* outHandle
);

/**
 * シェーダソースファイル (.slang) を実行時コンパイルしてマテリアルを作成します。
 * デスクトップ環境 (LUMINO_USE_SLANG が有効な場合) のみ使用可能です。
 * テストやサンプルプログラムでの利用を想定しており、本番環境では
 * LNMaterial_CreateFromCompiledShader の使用を推奨します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  shaderFilePath  コンパイルする .slang ファイルのパス
 * @param[in]  searchPathOrNull      lumino.slang を含むディレクトリのパス (NULL 可)
 * @param[out] outHandle       作成されたマテリアルのハンドル
 */
// extern LUMINO_API LNResult LNMaterial_CreateFromShaderSourceFile(
//     LNHandle graphicsContext,
//     const char* shaderFilePath,
//     const char* searchPathOrNull,
//     LNHandle* outHandle
// );

/**
 * @deprecated LNMaterial_CreateFromBuiltinShader(ctx, LN_BUILTIN_SHADER_UNLIT, out) を使用してください。
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
 * マテリアルのメインテクスチャを設定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] texture  Texture2D のハンドル
 */
extern LUMINO_API LNResult LNMaterial_SetMainTexture(
    LNHandle material,
    LNHandle texture
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
 * 毎フレーム CPU から更新可能な動的メッシュを作成します。
 * 内部で host-visible なバッファを確保します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  maxVertexCount  最大頂点数
 * @param[in]  maxIndexCount   最大インデックス数
 * @param[out] outHandle        作成されたメッシュのハンドル
 */
extern LUMINO_API LNResult LNMesh_CreateDynamic(
    LNHandle graphicsContext,
    uint32_t maxVertexCount,
    uint32_t maxIndexCount,
    LNHandle* outHandle
);

/**
 * 動的メッシュの頂点データを更新します。
 * @param[in] mesh         メッシュのハンドル
 * @param[in] firstVertex  書き込み開始頂点インデックス
 * @param[in] vertices     LNVertex 配列
 * @param[in] count        頂点数
 */
extern LUMINO_API LNResult LNMesh_UpdateVertices(
    LNHandle mesh,
    uint32_t firstVertex,
    const LNVertex* vertices,
    uint32_t count
);

/**
 * 動的メッシュのインデックスデータを更新します。
 * @param[in] mesh        メッシュのハンドル
 * @param[in] firstIndex  書き込み開始インデックスオフセット
 * @param[in] indices     uint32_t インデックス配列
 * @param[in] count       インデックス数
 */
extern LUMINO_API LNResult LNMesh_UpdateIndices(
    LNHandle mesh,
    uint32_t firstIndex,
    const uint32_t* indices,
    uint32_t count
);

/**
 * メッシュのサブメッシュ配列を差し替えます。
 * マテリアルスロットは必要に応じて拡張されます。
 * @param[in] mesh          メッシュのハンドル
 * @param[in] submeshes     LNSubMesh 配列
 * @param[in] submeshCount  サブメッシュ数
 */
extern LUMINO_API LNResult LNMesh_SetSubMeshes(
    LNHandle mesh,
    const LNSubMesh* submeshes,
    uint32_t submeshCount
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
 * カメラに正射影投影を設定します。
 * @param[in] camera   カメラのハンドル
 * @param[in] width    投影幅
 * @param[in] height   投影高さ
 * @param[in] nearClip ニアクリップ距離
 * @param[in] farClip  ファークリップ距離
 */
extern LUMINO_API LNResult LNCamera_SetOrthographic(
    LNHandle camera,
    float width, float height, float nearClip, float farClip
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
 * レンダーパスを開始します。カメラデータを set=0 View UBO に自動アップロードします。
 * @param[in] renderer         Renderer のハンドル
 * @param[in] graphicsContext  GraphicsContext のハンドル (現フレームのターゲット取得用)
 * @param[in] camera           カメラのハンドル (LN_NULL_HANDLE でカメラなし — ポストエフェクト用)
 * @param[in] r,g,b,a          クリアカラー
 */
extern LUMINO_API LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer,
    LNHandle graphicsContext,
    LNHandle camera,
    float r, float g, float b, float a
);

/**
 * レンダーターゲットテクスチャへのレンダーパスを開始します。
 * LNTexture2D_CreateRenderTarget で作成したテクスチャを描画先として使用します。
 * 対応する LNRenderer_EndRenderPass 呼び出し時に、テクスチャは自動的に
 * シェーダ読み取り可能なレイアウトに遷移されます。
 * @param[in] renderer           Renderer のハンドル
 * @param[in] renderTargetTexture LNTexture2D_CreateRenderTarget で作成したテクスチャのハンドル
 * @param[in] camera             カメラのハンドル (LN_NULL_HANDLE でカメラなし)
 * @param[in] r,g,b,a            クリアカラー
 */
extern LUMINO_API LNResult LNRenderer_BeginRenderPassToTexture(
    LNHandle renderer,
    LNHandle renderTargetTexture,
    LNHandle camera,
    float r, float g, float b, float a
);

/**
 * レンダーパスを終了します。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_EndRenderPass(LNHandle renderer);

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

/**
 * ステンシルマスクをプッシュします。
 * maskMesh をステンシルバッファにのみ描画し（カラー書き込みなし）、
 * 以降の DrawMesh 呼び出しはマスク領域内のみ描画されます。
 * ネスト可能（内部でステンシル参照値をインクリメント）。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] mesh       マスク形状のメッシュ（アルファマスクテクスチャ付き）
 * @param[in] transform  LNTransform へのポインタ (NULL で単位変換)
 * @param[in] material   マスク描画に使うマテリアルのハンドル
 */
extern LUMINO_API LNResult LNRenderer_PushStencilMask(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform,
    LNHandle material
);

/**
 * 直前の PushStencilMask に対応するマスクを解除します。
 * 内部でマスクメッシュを再描画してステンシル値をデクリメントします。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_PopStencilMask(LNHandle renderer);

//------------------------------------------------------------------------------
// LNDebug
//------------------------------------------------------------------------------

/**
 * グラフィックスプロファイリング情報。
 */
typedef struct LNGraphicsProfiler {
    /** 現在フレームのドローコール数。 */
    int32_t drawCallCount;

    /** 実際のフレームレート (FPS)。 */
    float fps;

    /** 直前フレームの所要時間 (ミリ秒)。 */
    float lastFrameTimeMs;
} LNGraphicsProfiler;

/**
 * グラフィックスプロファイリング情報を取得します。
 * LNGraphicsContext_EndFrame の後に呼び出すと前フレームの計測値が得られます。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outProfiler    プロファイリング情報の出力先
 */
extern LUMINO_API LNResult LNDebug_GetGraphicsProfiler(
    LNHandle graphicsContext,
    LNGraphicsProfiler* outProfiler);

/**
 * デバッグ文字列を画面左上に描画します。
 * ASCII 文字のみ使用可能で、最大 512 文字まで蓄積できます。
 * 実際の描画は LNGraphicsContext_EndFrame の内部で行われます。
 * @param[in] graphicsContext GraphicsContext のハンドル
 * @param[in] str            描画する文字列 (UTF-8/ASCII)
 */
extern LUMINO_API LNResult LNDebug_Print(
    LNHandle graphicsContext,
    const char* str);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
