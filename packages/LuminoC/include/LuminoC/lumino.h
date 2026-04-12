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
// Phase 0 smoke test
//------------------------------------------------------------------------------

/**
 * Phase 0 動作確認用。stdout に "Lumino HelloTest: <value>" を出力して value を返す。
 * ランタイム初期化等は一切不要で、Emscripten のツールチェインとシンボルエクスポートの
 * 疎通確認のためだけに使用する。
 */
extern LUMINO_API int32_t LNHelloTest(int32_t value);

/**
 * このバイナリのビルド情報を文字列で返します。
 * 内容は未定ですが、ビルド日時やコミットハッシュなどが含まれる予定です。
 * 正しいバイナリが読み込まれているかの確認用です。
 */
extern LUMINO_API const char* LNBuildInfo_GetBuildTimestamp();

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
// Enums
//------------------------------------------------------------------------------

/** Graphics Backend. */
typedef enum LNGraphicsBackend {
    LN_GRAPHICS_BACKEND_DEFAULT = 0,
    LN_GRAPHICS_BACKEND_VULKAN = 1,
    LN_GRAPHICS_BACKEND_WEBGPU = 2,
} LNGraphicsBackend;

/**
 * レンダーパスのアタッチメントのロード操作。
 * ゼロ初期化時のデフォルトは LN_LOAD_OP_CLEAR。
 */
typedef enum LNLoadOp {
    /**
     * 内容をクリアします。
     */
    LN_LOAD_OP_CLEAR     = 0,
    /**
     * 既存の内容を保持します。
     */
    LN_LOAD_OP_LOAD      = 1,
    /**
     * 内容不定 (パフォーマンス最適化)
     * ポストプロセスや G-Buffer のように出力先の全ピクセルを書き換える時に指定できます。
     */
    LN_LOAD_OP_DONT_CARE = 2,

    LN_LOAD_OP__FORCE32 = 0x7FFFFFFF,
} LNLoadOp;

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
     /** 使用するグラフィックスバックエンド (デフォルト: LN_GRAPHICS_BACKEND_DEFAULT) */
    LNGraphicsBackend preferredBackend;
    /** デバッグ用のバリデーションレイヤーを有効にするか */
    LNBool enableValidation;
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
 * HTML canvas に紐づく Window を作成します。Web 専用。
 * canvasSelector は CSS セレクタ (例: "#my_canvas")。
 * デスクトップビルドでは LN_ERROR_UNKNOWN を返します。
 * @param[in]  canvasSelector CSS セレクタで指定された canvas
 * @param[in]  width          キャンバスの幅 (ピクセル)
 * @param[in]  height         キャンバスの高さ (ピクセル)
 * @param[out] outHandle      作成されたウィンドウのハンドル
 */
extern LUMINO_API LNResult LNWindow_CreateFromCanvas(
    const char* canvasSelector,
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
 * @param[out] outQuit 終了フラグ (LN_TRUE でループ終了)
 */
extern LUMINO_API LNResult LNWindow_ProcessEvents(LNHandle handle, LNBool* outQuit);

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

/** 同時に指定できるカラーアタッチメントの最大数 */
#define LN_MAX_COLOR_ATTACHMENTS 8

//------------------------------------------------------------------------------
// Render pass descriptor structs
//------------------------------------------------------------------------------

/**
 * カラーアタッチメントの設定。
 * @note renderTarget が LN_NULL_HANDLE の場合、バックバッファが使用されます。
 */
typedef struct LNColorAttachmentDesc {
    LNHandle renderTarget;  /**< 描画先テクスチャ (LN_NULL_HANDLE = バックバッファ) */
    float    clearColor[4]; /**< クリアカラー RGBA (loadOp == CLEAR 時に使用) */
    LNLoadOp loadOp;        /**< ロード操作 (デフォルト: LN_LOAD_OP_CLEAR) */
} LNColorAttachmentDesc;

/**
 * デプス・ステンシルアタッチメントの設定。
 * @note depthBuffer が LN_NULL_HANDLE の場合、バックバッファのデプスバッファが使用されます。
 */
typedef struct LNDepthStencilAttachmentDesc {
    LNHandle depthBuffer;     /**< デプスバッファ (LN_NULL_HANDLE = バックバッファのデプス) */
    float    clearDepth;      /**< クリアデプス値 (デフォルト: 1.0f、要 LNRenderPassDesc_Init) */
    uint32_t clearStencil;    /**< クリアステンシル値 (デフォルト: 0) */
    LNLoadOp depthLoadOp;     /**< デプスのロード操作 (デフォルト: LN_LOAD_OP_CLEAR) */
    LNLoadOp stencilLoadOp;   /**< ステンシルのロード操作 (デフォルト: LN_LOAD_OP_CLEAR) */
} LNDepthStencilAttachmentDesc;

/**
 * レンダーパスの設定。
 * LNRenderPassDesc_Init で初期化してから使用してください。
 */
typedef struct LNRenderPassDesc {
    uint32_t colorAttachmentCount; /**< 使用するカラーアタッチメント数 (0 の場合バックバッファを使用) */
    LNColorAttachmentDesc colorAttachments[LN_MAX_COLOR_ATTACHMENTS]; /**< カラーアタッチメント配列 */
    LNDepthStencilAttachmentDesc depthStencil;     /**< デプス・ステンシルアタッチメント */
} LNRenderPassDesc;

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

/**
 * フレームの描画を開始します。内部で Renderer の beginFrame も呼び出します。
 * 返された renderer, colorBuffer, depthBuffer ハンドルは graphicsContext が管理するため、
 * LNObject_Release を呼ぶ必要はありません。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outRenderer     Renderer のハンドル
 * @param[out] outColorBuffer  現フレームのバックバッファ (SwapChain のカラーテクスチャ) のハンドル
 * @param[out] outDepthBuffer  現フレームのデプスバッファのハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    LNHandle* outRenderer,
    LNHandle* outColorBuffer,
    LNHandle* outDepthBuffer);

/**
 * LNRenderPassDesc を適切なデフォルト値で初期化します。
 * (clearDepth=1.0f, clearStencil=0, loadOp=CLEAR, renderTarget=NULL_HANDLE)
 * @param[out] desc 初期化するデスクリプタ
 */
extern LUMINO_API void LNRenderPassDesc_Init(LNRenderPassDesc* desc);

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
 * 作成されたテクスチャは LNRenderPassDesc の renderTarget に指定して描画先として使用でき、
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
 * @param[in] texture  Texture のハンドル
 */
extern LUMINO_API LNResult LNMaterial_SetMainTexture(
    LNHandle material,
    LNHandle texture
);

extern LUMINO_API LNResult LNMaterial_SetFloat4(
    LNHandle material,
    const char* name,
    const float* values
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
 * レンダーパスを開始します。LNRenderPassDesc で描画先やクリア方法を指定します。
 * desc は LNRenderPassDesc_Init で初期化してから使用してください。
 * renderTarget / depthBuffer が LN_NULL_HANDLE の場合、バックバッファが使用されます。
 * camera が有効な場合、カメラデータを set=0 View UBO に自動アップロードします。
 * @param[in] renderer         Renderer のハンドル
 * @param[in] graphicsContext  GraphicsContext のハンドル (バックバッファ取得用)
 * @param[in] desc             レンダーパスの設定
 * @param[in] camera           カメラのハンドル (LN_NULL_HANDLE でカメラなし — ポストエフェクト用)
 */
extern LUMINO_API LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer,
    LNHandle graphicsContext,
    const LNRenderPassDesc* desc,
    LNHandle camera
);

/**
 * レンダーパスを終了します。
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_EndRenderPass(LNHandle renderer);

/**
 * メッシュ描画コマンドを内部コマンドバッファに蓄積します。
 * 蓄積されたコマンドは LNRenderer_EndRenderPass 時に自動的にソート→バッチ化→描画されます。
 * メッシュに設定されたマテリアルを使用します。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] mesh       メッシュのハンドル
 * @param[in] transform  LNTransform へのポインタ (NULL で単位変換)
 * @param[in] zIndex     ソート優先度
 */
extern LUMINO_API LNResult LNRenderer_DrawMesh(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform,
    int32_t zIndex
);

/**
 * メッシュを即座に描画します (バッチ化なし)。
 * レンダーパス内で即時に GPU コマンドを発行します。
 * @param[in] renderer   Renderer のハンドル
 * @param[in] mesh       メッシュのハンドル
 * @param[in] transform  LNTransform へのポインタ (NULL で単位変換)
 */
extern LUMINO_API LNResult LNRenderer_DrawMeshImmediate(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform
);

/**
 * スプライト描画コマンドを内部コマンドバッファに蓄積します。
 * 蓄積されたコマンドは LNRenderer_EndRenderPass 時に自動的にソート→バッチ化→描画されます。
 * @param[in] renderer  Renderer のハンドル
 * @param[in] material  マテリアルのハンドル
 * @param[in] zIndex    ソート優先度
 * @param[in] posX,posY,posZ  位置
 * @param[in] sizeW,sizeH     サイズ
 * @param[in] uvX,uvY,uvW,uvH UV 矩形
 * @param[in] colorR,colorG,colorB,colorA 頂点カラー
 * @param[in] rotation  Z 軸回転 (ラジアン)
 */
extern LUMINO_API LNResult LNRenderer_DrawSprite(
    LNHandle renderer,
    LNHandle material,
    int32_t  zIndex,
    float posX, float posY, float posZ,
    float sizeW, float sizeH,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA,
    float rotation
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
// LNDrawCommandBuffer (deprecated — use LNRenderer_DrawMesh / LNRenderer_DrawSprite)
//------------------------------------------------------------------------------

/**
 * @deprecated LNRenderer_DrawMesh / LNRenderer_DrawSprite を使用してください。
 * DrawCommandBuffer を作成します。
 * @param[out] outHandle 作成された DrawCommandBuffer のハンドル
 */
extern LUMINO_API LNResult LNDrawCommandBuffer_Create(LNHandle* outHandle);

/**
 * DrawCommandBuffer をクリアします。フレーム先頭で呼び出してください。
 * @param[in] buffer DrawCommandBuffer のハンドル
 */
extern LUMINO_API LNResult LNDrawCommandBuffer_Clear(LNHandle buffer);

/**
 * スプライト描画コマンドを追加します。
 * @param[in] buffer    DrawCommandBuffer のハンドル
 * @param[in] material  マテリアルのハンドル
 * @param[in] zIndex    ソート優先度
 * @param[in] posX,posY,posZ  位置
 * @param[in] sizeW,sizeH     サイズ
 * @param[in] uvX,uvY,uvW,uvH UV 矩形
 * @param[in] colorR,colorG,colorB,colorA 頂点カラー
 * @param[in] rotation  Z 軸回転 (ラジアン)
 */
extern LUMINO_API LNResult LNDrawCommandBuffer_DrawSprite(
    LNHandle buffer,
    LNHandle material,
    int32_t  zIndex,
    float posX, float posY, float posZ,
    float sizeW, float sizeH,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA,
    float rotation);

/**
 * スプライト描画コマンドを一括追加します。
 */
typedef struct LNSpriteCommand {
    float posX, posY, posZ;
    float sizeW, sizeH;
    float uvX, uvY, uvW, uvH;
    float colorR, colorG, colorB, colorA;
    float rotation;
    int32_t zIndex;
} LNSpriteCommand;

extern LUMINO_API LNResult LNDrawCommandBuffer_DrawSprites(
    LNHandle buffer,
    LNHandle material,
    const LNSpriteCommand* sprites,
    uint32_t count);

/**
 * サブメッシュ単位の描画コマンドを追加します。
 * @param[in] buffer        DrawCommandBuffer のハンドル
 * @param[in] mesh          メッシュのハンドル
 * @param[in] submeshIndex  サブメッシュインデックス
 * @param[in] material      マテリアルのハンドル
 * @param[in] transform     LNTransform へのポインタ (NULL で単位変換)
 * @param[in] zIndex        ソート優先度
 */
extern LUMINO_API LNResult LNDrawCommandBuffer_DrawSubMesh(
    LNHandle buffer,
    LNHandle mesh,
    uint32_t submeshIndex,
    LNHandle material,
    const LNTransform* transform,
    int32_t zIndex);

/**
 * メッシュの全サブメッシュを一括で描画コマンドに追加します (便利 API)。
 * @param[in] buffer    DrawCommandBuffer のハンドル
 * @param[in] mesh      メッシュのハンドル
 * @param[in] transform LNTransform へのポインタ (NULL で単位変換)
 * @param[in] zIndex    ソート優先度
 */
extern LUMINO_API LNResult LNDrawCommandBuffer_DrawMesh(
    LNHandle buffer,
    LNHandle mesh,
    const LNTransform* transform,
    int32_t zIndex);

//------------------------------------------------------------------------------
// LNBatchProcessor (deprecated — use LNRenderer_DrawMesh / LNRenderer_DrawSprite)
//------------------------------------------------------------------------------

/**
 * @deprecated LNRenderer_DrawMesh / LNRenderer_DrawSprite を使用してください。
 * BatchProcessor を作成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[out] outHandle       作成された BatchProcessor のハンドル
 */
extern LUMINO_API LNResult LNBatchProcessor_Create(
    LNHandle graphicsContext,
    LNHandle* outHandle);

/**
 * DrawCommandBuffer の内容をソート→バッチ化→描画します。
 * beginRenderPass ～ endRenderPass の間で呼び出してください。
 * @param[in] batchProcessor BatchProcessor のハンドル
 * @param[in] renderer       Renderer のハンドル
 * @param[in] commandBuffer  DrawCommandBuffer のハンドル
 */
extern LUMINO_API LNResult LNBatchProcessor_Flush(
    LNHandle batchProcessor,
    LNHandle renderer,
    LNHandle commandBuffer);

//------------------------------------------------------------------------------
// LNDebug
//------------------------------------------------------------------------------

/**
 * グラフィックスプロファイリング情報。
 */
typedef struct LNGraphicsProfilering {
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
    LNGraphicsProfilering* outProfiler);

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
