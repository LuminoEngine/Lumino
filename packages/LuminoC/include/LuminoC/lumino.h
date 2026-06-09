// clang-format off
#ifndef LUMINO_API_H_
#define LUMINO_API_H_
#include "lumino_types.h"

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
    /**
     * このレンダーパス内でマテリアルから優先的に選択する ShaderPass の名前。
     * NULL または空文字列の場合は "Forward" が使用されます。
     * マテリアルがこの名前の ShaderPass を持たない場合、
     * そのメッシュの描画はスキップされます (Unity の ShaderTagId と同じ挙動)。
     * 代表的なパス名: "Forward", "GBuffer", "Shadow" など。
     * 文字列は LNRenderer_BeginRenderPass の呼び出し中のみ有効である必要があります。
     */
    const char* shaderPassName;
} LNRenderPassDesc;

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

/**
 * フレームの描画を開始します。内部で Renderer の beginFrame も呼び出します。
 * width/height が前フレームと異なる場合、SwapChain と深度バッファを自動的にリサイズします。
 * 返された renderer, colorBuffer, depthBuffer ハンドルは graphicsContext が管理するため、
 * LNObject_Release を呼ぶ必要はありません。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  width          描画先の幅 (ピクセル)
 * @param[in]  height         描画先の高さ (ピクセル)
 * @param[out] outRenderer     Renderer のハンドル
 * @param[out] outColorBuffer  現フレームのバックバッファ (SwapChain のカラーテクスチャ) のハンドル
 * @param[out] outDepthBuffer  現フレームのデプスバッファのハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_BeginFrame(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
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
 * 現在のフレームのバックバッファをキャプチャするよう要求します。
 * LNGraphicsContext_EndFrame の前に呼び出してください。
 *
 * スワップチェーンイメージは present 後に acquire 解除されると一切使用できなくなる
 * (VUID UNASSIGNED-non-acquired-swapchain-image-used) ため、実際の読み戻しは
 * present 直前 (EndFrame 内) に行われます。読み戻した内容は EndFrame の後に
 * LNGraphicsContext_CaptureBackbuffer で取得します。
 * @param[in] graphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_RequestCaptureBackbuffer(LNHandle graphicsContext);

/**
 * 直前のフレームのバックバッファの内容を RGBA8 ピクセルデータとして取得します。
 * 事前に (EndFrame の前に) LNGraphicsContext_RequestCaptureBackbuffer を呼び、
 * その後 LNGraphicsContext_EndFrame を呼んでから本関数を呼び出してください。
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
 * レンダーターゲットテクスチャ (カラーのみ) を作成します。
 * BGRA8Unorm カラーテクスチャを生成します。
 * 深度バッファは LNTexture2D_CreateDepthStencil で別途作成し、
 * LNDepthStencilAttachmentDesc の depthBuffer に設定してください。
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
 * 指定フォーマットのレンダーターゲットテクスチャを作成します。
 * Sampled|RenderTarget のカラーテクスチャを生成します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  width           幅 (ピクセル)
 * @param[in]  height          高さ (ピクセル)
 * @param[in]  format          テクスチャフォーマット (LNTextureFormat)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_CreateRenderTargetEx(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle
);

/**
 * 深度ステンシルテクスチャを作成します。
 * 作成されたテクスチャは LNDepthStencilAttachmentDesc の depthBuffer に指定して使用します。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  width           幅 (ピクセル)
 * @param[in]  height          高さ (ピクセル)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_CreateDepthStencil(
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

/**
 * メモリ上の画像データから 2D テクスチャを読み込みます (PNG, JPG, BMP, TGA 等)。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  data            画像データへのポインタ
 * @param[in]  size            データサイズ (バイト)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_LoadFromMemory(
    LNHandle graphicsContext,
    const void* data,
    uint32_t size,
    LNHandle* outHandle
);

/**
 * デコード済みのピクセルデータから 2D テクスチャを作成します。
 * ブラウザの createImageBitmap 等でデコードした RGBA 等の生ピクセルを直接アップロードする用途。
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  width           幅 (ピクセル)
 * @param[in]  height          高さ (ピクセル)
 * @param[in]  format          テクスチャフォーマット (LNTextureFormat)
 * @param[in]  pixelData       ピクセルデータへのポインタ (format で指定されたフォーマットに従う)
 * @param[in]  dataSizeBytes   ピクセルデータサイズ (バイト)
 * @param[out] outHandle       作成されたテクスチャのハンドル
 */
extern LUMINO_API LNResult LNTexture2D_CreateFromPixels(
    LNHandle graphicsContext,
    uint32_t width,
    uint32_t height,
    uint32_t format,
    const void* pixelData,
    uint32_t dataSizeBytes,
    LNHandle* outHandle
);

//------------------------------------------------------------------------------
// LNImage
//------------------------------------------------------------------------------

/**
 * メモリ上の画像データ (PNG, JPG, BMP, TGA 等) をデコードし、
 * RGBA8 ピクセルデータとして返します。
 * GPU コンテキスト不要の純粋な CPU 処理です。
 * 返されたピクセルデータは LNImage_FreePixels で解放してください。
 * @param[in]  data          画像データへのポインタ
 * @param[in]  size          データサイズ (バイト)
 * @param[out] outWidth      画像の幅 (ピクセル)
 * @param[out] outHeight     画像の高さ (ピクセル)
 * @param[out] outPixels     デコードされた RGBA8 ピクセルデータへのポインタ
 * @param[out] outPixelsSize ピクセルデータのサイズ (バイト, = width * height * 4)
 */
extern LUMINO_API LNResult LNImage_DecodeFromMemory(
    const void* data,
    uint32_t size,
    uint32_t* outWidth,
    uint32_t* outHeight,
    const void** outPixels,
    uint32_t* outPixelsSize
);

/**
 * LNImage_DecodeFromMemory で確保されたピクセルデータを解放します。
 * @param[in] pixels 解放するピクセルデータへのポインタ
 */
extern LUMINO_API LNResult LNImage_FreePixels(const void* pixels);

//------------------------------------------------------------------------------
// LNMaterial
//------------------------------------------------------------------------------

/** 合成方法 */
typedef enum LNBlendMode {
    /** 通常 */
    LN_BLEND_MODE_NORMAL = 0,
    /** アルファブレンド (RGB をアルファブレンドし、A を加算合成) */
    LN_BLEND_MODE_ALPHA = 1,
    /** 加算合成 */
    LN_BLEND_MODE_ADD = 2,
    /** 減算合成 */
    LN_BLEND_MODE_SUBTRACT = 3,
    /** 乗算合成 */
    LN_BLEND_MODE_MULTIPLY = 4,
} LNBlendMode;

/** ポリゴンのカリングモード */
typedef enum LNCullMode {
    /** カリングなし (両面描画) */
    LN_CULL_MODE_NONE  = 0,
    /** 前面をカリング */
    LN_CULL_MODE_FRONT = 1,
    /** 背面をカリング (デフォルト) */
    LN_CULL_MODE_BACK  = 2,
} LNCullMode;

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
extern LUMINO_API LNResult LNMaterial_CreateFromShaderSourceFile(
    LNHandle graphicsContext,
    const char* shaderFilePath,
    const char* searchPathOrNull,
    LNHandle* outHandle
);

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

/**
 * シェーダバインディング名でテクスチャを設定します。
 * シェーダの uniform Texture2D 変数名（例: "u_sceneColor"）を指定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] name     シェーダ内のテクスチャバインディング名 (UTF-8)
 * @param[in] texture  Texture のハンドル
 */
extern LUMINO_API LNResult LNMaterial_SetNamedTexture(
    LNHandle material,
    const char* name,
    LNHandle texture
);

/**
 * マテリアルのブレンドモードを設定します。
 * @param[in] material  マテリアルのハンドル
 * @param[in] blendMode 合成方法
 */
extern LUMINO_API LNResult LNMaterial_SetBlendMode(
    LNHandle material,
    LNBlendMode blendMode
);

/**
 * マテリアルのカリングモードを設定します。
 * @param[in] material  マテリアルのハンドル
 * @param[in] cullMode  カリングモード
 */
extern LUMINO_API LNResult LNMaterial_SetCullMode(
    LNHandle material,
    LNCullMode cullMode
);

/**
 * マテリアルのデプステスト有効/無効を設定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] enabled  LN_TRUE: 有効, LN_FALSE: 無効
 */
extern LUMINO_API LNResult LNMaterial_SetDepthTestEnabled(
    LNHandle material,
    LNBool enabled
);

/**
 * マテリアルのデプス書き込み有効/無効を設定します。
 * @param[in] material マテリアルのハンドル
 * @param[in] enabled  LN_TRUE: 有効, LN_FALSE: 無効
 */
extern LUMINO_API LNResult LNMaterial_SetDepthWriteEnabled(
    LNHandle material,
    LNBool enabled
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
 * カメラに 2D 用の正射影投影 (左上原点・Y軸下向き) を設定します。
 * 画面座標 (0, 0) が左上、(width, height) が右下に対応します。
 * ビュー行列は単位行列に設定されます。
 * このカメラで LNRenderer_DrawSprite による描画を行うと、スプライト頂点が
 * 左上原点・Y軸下向きのレイアウトで自動生成されます (UV 反転やマテリアルの
 * カリング設定変更は不要です)。
 *
 * 注意: このカメラで 3D メッシュ (LNRenderer_DrawMesh) を描画する場合は、
 * Y 軸反転によりワインディングが反転するため、マテリアルの CullMode を
 * None に設定するか、フロントフェイス向きを CW に揃える必要があります。
 *
 * pivotX, pivotY (0..1) で原点 (画面座標 (0,0)) の位置を指定できます。
 * pivot=(0,0) で左上、(0.5,0.5) で画面中央、(1,1) で右下が原点になります。
 *
 * @param[in] camera   カメラのハンドル
 * @param[in] width    画面幅 (ピクセル)
 * @param[in] height   画面高さ (ピクセル)
 * @param[in] nearClip ニアクリップ距離
 * @param[in] farClip  ファークリップ距離
 * @param[in] pivotX   原点の水平位置 (0..1)。0=左, 0.5=中央, 1=右
 * @param[in] pivotY   原点の垂直位置 (0..1)。0=上, 0.5=中央, 1=下
 */
extern LUMINO_API LNResult LNCamera_SetOrthographic2D(
    LNHandle camera,
    float width, float height, float nearClip, float farClip,
    float pivotX, float pivotY
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

/**
 * カメラのビュー行列とプロジェクション行列を直接設定します。
 * @param[in] camera       カメラのハンドル
 * @param[in] viewMatrix   ビュー行列 (float x 16, 列優先)
 * @param[in] projMatrix   プロジェクション行列 (float x 16, 列優先)
 */
extern LUMINO_API LNResult LNCamera_SetMatrices(
    LNHandle camera,
    const float* viewMatrix,
    const float* projMatrix
);

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

/**
 * レンダーパスを開始します。LNRenderPassDesc で描画先やクリア方法を指定します。
 * desc は LNRenderPassDesc_Init で初期化してから使用してくださsい。
 * renderTarget / depthBuffer が LN_NULL_HANDLE の場合、バックバッファが使用されます。
 * camera が有効な場合、カメラデータを set=0 View UBO に自動アップロードします。
 * @param[in] renderer         Renderer のハンドル
 * @param[in] graphicsContext  GraphicsContext のハンドル (バックバッファ取得用)
 * @param[in] desc             レンダーパスの設定
 * @param[in] camera           カメラのハンドル (LN_NULL_HANDLE でカメラなし - ポストエフェクト用)
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
 * メッシュを指定マテリアルで即座に描画します (バッチ化なし)。
 * メッシュに設定されたマテリアルではなく、引数のマテリアルを使用します。
 * @param[in] renderer  Renderer のハンドル
 * @param[in] mesh      メッシュのハンドル
 * @param[in] transform ワールドトランスフォーム (NULL で単位行列)
 * @param[in] material  マテリアルのハンドル
 */
extern LUMINO_API LNResult LNRenderer_DrawMeshImmediateWithMaterial(
    LNHandle renderer,
    LNHandle mesh,
    const LNTransform* transform,
    LNHandle material
);

/**
 * フルスクリーン矩形を指定マテリアルで描画します。
 * ポストプロセス・スクリーンスペースエフェクト用です。
 * NDC [-1,1]x[-1,1] をカバーする矩形を描画します。
 * @param[in] renderer  Renderer のハンドル
 * @param[in] material  マテリアルのハンドル
 */
extern LUMINO_API LNResult LNRenderer_DrawScreenRect(
    LNHandle renderer,
    LNHandle material
);

/**
 * スプライト描画コマンドを内部コマンドバッファに蓄積します。
 *
 * @param[in] renderer  Renderer のハンドル
 * @param[in] material  マテリアルのハンドル
 * @param[in] zIndex    ソート優先度
 * @param[in] posX,posY,posZ  位置 (スプライト矩形上の pivot 位置がこの座標に一致します)
 * @param[in] sizeW,sizeH     サイズ
 * @param[in] pivotX,pivotY   矩形上の基準点 (0.0〜1.0)。(0,0)=視覚的な左上, (0.5,0.5)=中央, (1,1)=右下。
 *                            この点が posX,posY に配置され、rotation の回転軸にもなります。
 *                            CanvasRenderingContext2D の fillRect のように左上原点で描きたい場合は (0,0) を指定します。
 * @param[in] uvX,uvY,uvW,uvH UV 矩形
 * @param[in] colorR,colorG,colorB,colorA 頂点カラー
 * @param[in] rotation  Z 軸回転 (ラジアン)
 *
 * 蓄積されたコマンドは LNRenderer_EndRenderPass 時に自動的にソート→バッチ化→描画されます。
 *
 * ## DrawMesh vs DrawSprite
 * 例えば4頂点の四角形Mesh を 5000 個描画する場合、DrawMesh は 5000 ドローコールになりますが、DrawSprite は 1 ドローコールで描画できます。
 * Intel Core i9, GeForce RTX 3070 の環境でも顕著で、前者では 20ms 近くかかるのに対し、後者は 1ms 未満で描画できます。
 * もちろん自前で Mesh を作成して DrawMesh で描画することも可能ですが、DrawSprite は内部でバッチングを行うため、
 * タイルマップなど、同一マテリアルで多数のスプライトを描画する場合は DrawSprite が便利でしょう。
 */
extern LUMINO_API LNResult LNRenderer_DrawSprite(
    LNHandle renderer,
    LNHandle material,
    int32_t zIndex,
    float posX, float posY, float posZ,
    float sizeW, float sizeH,
    float pivotX, float pivotY,
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
// LNDrawCommandBuffer (deprecated - use LNRenderer_DrawMesh / LNRenderer_DrawSprite)
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
// LNBatchProcessor (deprecated - use LNRenderer_DrawMesh / LNRenderer_DrawSprite)
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
