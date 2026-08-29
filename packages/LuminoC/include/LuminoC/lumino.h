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
// LNLogger
//------------------------------------------------------------------------------

/**
 * ログ出力レベルを設定します。設定したレベル未満のログは破棄されます。 (Default: LN_LOG_LEVEL_INFO)
 *
 * この関数はランタイムの初期化を必要としません。初期化処理のログを制御したい場合は
 * LNInstance_Initialize より前に呼び出してください。
 *
 * @param[in] level 出力するログの下限レベル
 */
extern LUMINO_API LNResult LNLogger_SetLevel(LNLogLevel level);

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
 * デスクトップ専用。Web (WASM) ビルドでは GLFW ベースの Window を作成できないため
 * LN_ERROR_UNKNOWN を返します (Web では LNWindow_CreateFromCanvas を使用してください)。
 *
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
 *
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
 *
 * @param[in]  handle            ウィンドウのハンドル
 * @param[out] outGraphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNWindow_GetGraphicsContext(LNHandle handle, LNHandle* outGraphicsContext);

/**
 * ウィンドウのイベントを処理します。
 *
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
 * 同一 zIndex 内での描画順 (二次ソート)。
 * zIndex 自体は常に主キー (エンジンが指定するレイヤ/レンダーキュー番号) であり、
 * 本モードはその中での並びだけを決めます。
 * 距離はカメラ位置からのユークリッド距離ではなくビュー平面からの距離 (ビュー空間 Z) で
 * 計算されるため、Perspective / Orthographic の両方で正しく機能します。
 * @note 深度ソートはカメラを指定した RenderPass でのみ有効です。
 */
typedef enum LNSortMode {
    /** 投入順 (描画順 = 呼び出し順)。既定値。ソート方針はアプリ側が制御します。 */
    LN_SORT_MODE_STABLE = 0,
    /** 手前→奥 (ビュー平面からの距離が小さい順)。不透明の overdraw 削減向け。 */
    LN_SORT_MODE_FRONT_TO_BACK = 1,
    /** 奥→手前 (ビュー平面からの距離が大きい順)。半透明の正しいアルファ合成向け。 */
    LN_SORT_MODE_BACK_TO_FRONT = 2,
} LNSortMode;

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
    /**
     * 同一 zIndex 内のスプライト/メッシュの描画順 (デフォルト: LN_SORT_MODE_STABLE = 投入順)。
     * FRONT_TO_BACK / BACK_TO_FRONT はカメラ指定時のみ有効。
     */
    LNSortMode sortMode;
} LNRenderPassDesc;

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

/**
 * フレームの描画を開始します。内部で Renderer の beginFrame も呼び出します。
 * width/height が前フレームと異なる場合、SwapChain と深度バッファを自動的にリサイズします。
 * 返された renderer, colorBuffer, depthBuffer ハンドルは graphicsContext が管理するため、
 * LNObject_Release を呼ぶ必要はありません。
 *
 * デバイスロスト (GPU ドライバ更新、ブラウザのタブ復帰等) が検出された場合、
 * 本関数は LN_ERROR_DEVICE_LOST を返します。復旧は Lumino 内部で自動的に進行し、
 * 本関数の呼び出しが復旧処理を 1 ステップずつ進めるため、クライアントは
 * フレームループを回し続けるだけでよく、復旧が完了すると再び LN_OK を返します。
 *
 * ハンドルの契約: Window / GraphicsContext / Camera のハンドルはロストをまたいで
 * 有効です。Texture / Mesh / Material のハンドルは stale となり、描画に渡しても
 * 無視 (スキップ) されるため、復旧後に LNObject_Release で解放して作り直して
 * ください (Release はロスト中も常に成功します)。RenderTarget / DepthStencil の
 * 内容は復旧時に失われます。
 *
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
 *
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
 *
 * Web (WASM) ビルドでは非対応で、常に LN_ERROR_NOT_SUPPORTED を返します。
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
 *
 * Web (WASM) ビルドでは非対応で、常に LN_ERROR_NOT_SUPPORTED を返します。
 *
 * @param[in] graphicsContext GraphicsContext のハンドル
 */
extern LUMINO_API LNResult LNGraphicsContext_RequestCaptureBackbuffer(LNHandle graphicsContext);

/**
 * 直前のフレームのバックバッファの内容を RGBA8 ピクセルデータとして取得します。
 * 事前に (EndFrame の前に) LNGraphicsContext_RequestCaptureBackbuffer を呼び、
 * その後 LNGraphicsContext_EndFrame を呼んでから本関数を呼び出してください。
 * 返されるポインタは、次の LNGraphicsContext_CaptureBackbuffer 呼び出しまで有効です。
 *
 * Web (WASM) ビルドでは非対応で、常に LN_ERROR_NOT_SUPPORTED を返します
 * (outData に nullptr、outWidth/outHeight に 0 が設定されます)。
 *
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

/**
 * 列優先 4x4 行列 (ln::Matrix4x4 と同一レイアウト)。
 *
 * 要素は m[col * 4 + row] でアクセスし、平行移動成分は m[12], m[13], m[14]。
 * 点の変換は列ベクトル規約 (world = M * (v, 1)) です。
 *
 * シーングラフのワールド変換のように、TRS では表現できない合成結果
 * (非一様スケール + 回転によるシア等) も指定できます。
 */
typedef struct LNMatrix {
    float m[16];
} LNMatrix;

//------------------------------------------------------------------------------
// LNTexture2D
//------------------------------------------------------------------------------

/**
 * 2D テクスチャを作成します。
 *
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
 *
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
 *
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
 *
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
 *
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
 *
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
 *
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
 *
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
 *
 * @param[in] pixels 解放するピクセルデータへのポインタ
 */
extern LUMINO_API LNResult LNImage_FreePixels(const void* pixels);

//------------------------------------------------------------------------------
// LNShader
//------------------------------------------------------------------------------

/**
 * コンパイル済みシェーダ (.lcsh) のバイナリデータから Shader を作成します。
 *
 * Shader は GPU シェーダモジュールとパイプラインレイアウトを保持する共有オブジェクトです。
 * LNMaterial_CreateFromShader で作成した Material はこれらを共有するため、
 * 同一シェーダの Material を何個作っても GPU リソースは増えません。
 *
 * フレーム内で異なるパラメータを使いたい場合や、テクスチャごとに Material を
 * 分けたい場合は、Shader を 1 つ作って Material を量産してください。
 * 共有できているかは LNGraphicsProfiler::shaderPassCount で確認できます。
 *
 * @param[in]  graphicsContext GraphicsContext のハンドル
 * @param[in]  data            コンパイル済みシェーダのバイナリデータ
 * @param[in]  size            バイナリデータのサイズ (バイト)
 * @param[out] outHandle       作成されたシェーダのハンドル
 */
extern LUMINO_API LNResult LNShader_CreateFromCompiledShader(
    LNHandle graphicsContext,
    const void* data,
    uint32_t size,
    LNHandle* outHandle
);

/**
 * シェーダソースファイル (.slang) を実行時コンパイルして Shader を作成します。
 * デスクトップ環境 (LUMINO_USE_SLANG が有効な場合) のみ使用可能です。
 * テストやサンプルプログラムでの利用を想定しており、本番環境では
 * LNShader_CreateFromCompiledShader の使用を推奨します。
 *
 * @param[in]  graphicsContext  GraphicsContext のハンドル
 * @param[in]  shaderFilePath   コンパイルする .slang ファイルのパス
 * @param[in]  searchPathOrNull lumino.slang を含むディレクトリのパス (NULL 可)
 * @param[out] outHandle        作成されたシェーダのハンドル
 */
extern LUMINO_API LNResult LNShader_CreateFromShaderSourceFile(
    LNHandle graphicsContext,
    const char* shaderFilePath,
    const char* searchPathOrNull,
    LNHandle* outHandle
);

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

/** テクスチャのフィルタリング方法 */
typedef enum LNTextureFilterMode {
    /** 最近傍 (ドット絵の拡大表示向け) */
    LN_TEXTURE_FILTER_MODE_NEAREST = 0,
    /** 線形補間 (デフォルト) */
    LN_TEXTURE_FILTER_MODE_LINEAR  = 1,
} LNTextureFilterMode;

/** テクスチャ座標が 0.0 - 1.0 の範囲外になったときの回り込み方法 */
typedef enum LNTextureAddressMode {
    /** 繰り返し (タイリングする模様やノイズテクスチャ向け) */
    LN_TEXTURE_ADDRESS_MODE_REPEAT          = 0,
    /** 反転繰り返し */
    LN_TEXTURE_ADDRESS_MODE_MIRRORED_REPEAT = 1,
    /** 端のピクセルを引き伸ばす (デフォルト) */
    LN_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE   = 2,
} LNTextureAddressMode;

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
 *
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
 * 作成済みの Shader からマテリアルを作成します。
 *
 * GPU シェーダモジュールとパイプラインレイアウトは Shader が保持しているものを
 * 共有するため、同一 Shader から Material を何個作っても GPU リソースは増えません。
 *
 * @param[in]  shader    LNShader_CreateFromCompiledShader 等で作成したシェーダのハンドル
 * @param[out] outHandle 作成されたマテリアルのハンドル
 */
extern LUMINO_API LNResult LNMaterial_CreateFromShader(
    LNHandle shader,
    LNHandle* outHandle
);

/**
 * コンパイル済みシェーダ (.lcsh) のバイナリデータからマテリアルを作成します。
 *
 * この関数は呼び出しごとに GPU シェーダモジュールとパイプラインレイアウトを
 * 新規生成します。同一シェーダから複数の Material を作る場合は
 * LNShader_CreateFromCompiledShader と LNMaterial_CreateFromShader を
 * 使用してください。
 *
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
 *
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
 * マテリアルのベースカラーを設定します。
 *
 * @param[in] material マテリアルのハンドル
 * @param[in] r,g,b,a  カラー成分 (0.0 - 1.0)
 */
extern LUMINO_API LNResult LNMaterial_SetColor(
    LNHandle material,
    float r, float g, float b, float a
);

/**
 * マテリアルのメインテクスチャを設定します。
 *
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
 *
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
 * マテリアル全体のテクスチャサンプリング方法を設定します。
 *
 * このマテリアルが参照する全テクスチャの既定値になります。個別のテクスチャだけ
 * 変えたい場合は LNMaterial_SetNamedSamplerState で上書きしてください。
 *
 * 既定は LN_TEXTURE_FILTER_MODE_LINEAR + LN_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE です。
 * ドット絵を拡大表示する場合は LN_TEXTURE_FILTER_MODE_NEAREST を、
 * タイリングする模様やノイズテクスチャには LN_TEXTURE_ADDRESS_MODE_REPEAT を
 * 指定します。
 *
 * @param[in] material    マテリアルのハンドル
 * @param[in] filterMode  拡大/縮小フィルタ
 * @param[in] addressMode UV が 0.0 - 1.0 の範囲外に出たときの回り込み方法
 */
extern LUMINO_API LNResult LNMaterial_SetSamplerState(
    LNHandle material,
    LNTextureFilterMode filterMode,
    LNTextureAddressMode addressMode
);

/**
 * 名前付きテクスチャ 1 スロットのサンプリング方法を設定します。
 * LNMaterial_SetSamplerState によるマテリアル単位の設定を上書きします。
 *
 * name にはシェーダの uniform Texture2D 変数名 (例: "u_sceneColor") を指定します。
 * ペアになる SamplerState 変数名 (例: "u_sceneColorSampler") ではありません。
 *
 * @param[in] material    マテリアルのハンドル
 * @param[in] name        シェーダ内のテクスチャバインディング名 (UTF-8)
 * @param[in] filterMode  拡大/縮小フィルタ
 * @param[in] addressMode UV が 0.0 - 1.0 の範囲外に出たときの回り込み方法
 */
extern LUMINO_API LNResult LNMaterial_SetNamedSamplerState(
    LNHandle material,
    const char* name,
    LNTextureFilterMode filterMode,
    LNTextureAddressMode addressMode
);

/**
 * マテリアルのブレンドモードを設定します。
 *
 * @param[in] material  マテリアルのハンドル
 * @param[in] blendMode 合成方法
 */
extern LUMINO_API LNResult LNMaterial_SetBlendMode(
    LNHandle material,
    LNBlendMode blendMode
);

/**
 * マテリアルのカリングモードを設定します。
 *
 * @param[in] material  マテリアルのハンドル
 * @param[in] cullMode  カリングモード
 */
extern LUMINO_API LNResult LNMaterial_SetCullMode(
    LNHandle material,
    LNCullMode cullMode
);

/**
 * マテリアルのデプステスト有効/無効を設定します。
 *
 * @param[in] material マテリアルのハンドル
 * @param[in] enabled  LN_TRUE: 有効, LN_FALSE: 無効
 */
extern LUMINO_API LNResult LNMaterial_SetDepthTestEnabled(
    LNHandle material,
    LNBool enabled
);

/**
 * マテリアルのデプス書き込み有効/無効を設定します。
 *
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
 *
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
 *
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
 *
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
 *
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
 *
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
 *
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
 *
 * @param[out] outHandle 作成されたカメラのハンドル
 */
extern LUMINO_API LNResult LNCamera_Create(LNHandle* outHandle);

/**
 * カメラに透視投影を設定します。
 *
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
 *
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
 *
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
 *
 * @param[in] camera       カメラのハンドル
 * @param[in] viewMatrix   ビュー行列 (float x 16, 列優先)
 * @param[in] projMatrix   プロジェクション行列 (float x 16, 列優先)
 * @param[in] is2D         2D モードフラグ (LN_TRUE: 2D, LN_FALSE: 3D)
 */
extern LUMINO_API LNResult LNCamera_SetMatrices(
    LNHandle camera,
    const float* viewMatrix,
    const float* projMatrix,
    LNBool is2D
);

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

/**
 * レンダーパスを開始します。LNRenderPassDesc で描画先やクリア方法を指定します。
 * desc は LNRenderPassDesc_Init で初期化してから使用してください。
 * renderTarget / depthBuffer が LN_NULL_HANDLE の場合、バックバッファが使用されます。
 * camera が有効な場合、カメラデータを set=0 View UBO に自動アップロードします。
 *
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
 *
 * @param[in] renderer Renderer のハンドル
 */
extern LUMINO_API LNResult LNRenderer_EndRenderPass(LNHandle renderer);

/**
 * メッシュ描画コマンドを内部コマンドバッファに蓄積します。
 * 蓄積されたコマンドは LNRenderer_EndRenderPass 時に自動的にソート→バッチ化→描画されます。
 * メッシュに設定されたマテリアルを使用します。
 *
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
 *
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
 *
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
 *
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
 * スプライトは size と pivot で定義されるローカル矩形を offset だけずらし、transform で
 * ワールド空間へ配置して描画します (world = transform * (localCorner + offset))。
 * 位置・回転・スケールは transform に畳み込んでください (LNRenderer_DrawMesh と同じく、
 * クライアント側のシーングラフから求めたワールド行列をそのまま渡せます)。
 * size / pivot / uv / color / offset はスプライトローカルのプロパティです。
 *
 * offset は、Tilemap / Tiling Sprite / 9-Sliced Sprite のように 1 ノード内で多数の
 * スプライトを描く際、transform (= ノードのワールド行列) を共有しつつタイルごとの位置だけを
 * 軽量に変えるためのオフセットです (行列を毎回作らずに済みます)。
 *
 * @param[in] renderer  Renderer のハンドル
 * @param[in] material  マテリアルのハンドル
 * @param[in] zIndex    ソート優先度
 * @param[in] transform LNMatrix へのポインタ (NULL で単位変換)。ワールド変換行列。
 * @param[in] offsetX,offsetY ノードローカル空間でのスプライト位置 (transform 適用前に加算)。
 *                            矩形上の pivot 位置がこの座標に配置されます。
 * @param[in] sizeW,sizeH     サイズ (ローカル)
 * @param[in] pivotX,pivotY   矩形上の基準点 (0.0〜1.0)。(0,0)=視覚的な左上, (0.5,0.5)=中央, (1,1)=右下。
 *                            この点が offset 位置に配置され、transform の回転軸にもなります。
 *                            CanvasRenderingContext2D の fillRect のように左上原点で描きたい場合は (0,0) を指定します。
 * @param[in] uvX,uvY,uvW,uvH UV 矩形
 * @param[in] colorR,colorG,colorB,colorA 頂点カラー
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
    const LNMatrix* transform,
    float offsetX, float offsetY,
    float sizeW, float sizeH,
    float pivotX, float pivotY,
    float uvX, float uvY, float uvW, float uvH,
    float colorR, float colorG, float colorB, float colorA
);

/**
 * ステンシルマスクをプッシュします。
 * maskMesh をステンシルバッファにのみ描画し（カラー書き込みなし）、
 * 以降の DrawMesh 呼び出しはマスク領域内のみ描画されます。
 * ネスト可能（内部でステンシル参照値をインクリメント）。
 *
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
 *
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

    /**
     * 生存しているシェーダパスの数 (プロセス全体、フレームでリセットされません)。
     *
     * 1 パスが GPU シェーダモジュール 2 個 (頂点/フラグメント) と
     * パイプラインレイアウト 1 個を所有するため、
     * 「シェーダモジュール数 = この値 x 2」「パイプラインレイアウト数 = この値」です。
     *
     * LNShader_CreateFromCompiledShader で作った 1 つの Shader から
     * LNMaterial_CreateFromShader で Material を複数作った場合、
     * Material を増やしてもこの値は増えません。逆に
     * LNMaterial_CreateFromCompiledShader は呼び出しごとに増えます。
     * ビルトインシェーダの分も含まれるため、絶対値ではなく
     * 「Material を増やしたときの増分」で確認してください。
     */
    int32_t shaderPassCount;

    /**
     * マテリアル BindGroup キャッシュのエントリ数 (フレームでリセットされません)。
     * Material の破棄でエビクトされるため、増え続ける場合は Material が
     * どこかで保持されたままになっています。
     */
    int32_t materialCacheCount;
} LNGraphicsProfiler;

/**
 * グラフィックスプロファイリング情報を取得します。
 * LNGraphicsContext_EndFrame の後に呼び出すと前フレームの計測値が得られます。
 * (drawCallCount は LNGraphicsContext_BeginFrame でリセットされるため、
 *  BeginFrame - EndFrame の間に呼び出すとその時点までの途中経過になります)
 *
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
 *
 * @param[in] graphicsContext GraphicsContext のハンドル
 * @param[in] str            描画する文字列 (UTF-8/ASCII)
 */
extern LUMINO_API LNResult LNDebug_Print(
    LNHandle graphicsContext,
    const char* str);

/**
 * 指定した公開構造体の sizeof (バイト数) を返します。
 * C API と各言語バインディング間の構造体レイアウト同期を実行時に検証するための
 * デバッグ用関数です。ランタイムの初期化は不要です。
 *
 * 構造体サイズはポインタ幅などプラットフォームによって異なります
 * (例: LNRenderPassDesc は const char* を含むため wasm32 と x64 で異なる)。
 * このため、バインディング側は自身が想定する定数値をハードコードで比較するのではなく、
 * 実行中のバイナリに本関数で問い合わせた値と照合してください。
 *
 * @param[in]  structName 構造体名 (lumino.h の typedef 名。例: "LNRenderPassDesc")
 * @param[out] outSize    sizeof の結果 (バイト数)
 * @return 成功時は LN_OK。未知の構造体名の場合は LN_ERROR_INVALID_ARGUMENT。
 */
extern LUMINO_API LNResult LNDebug_GetStructSize(
    const char* structName,
    uint32_t* outSize);

/**
 * デバイスロストをシミュレートします。テスト用。
 * 以後の LNGraphicsContext_BeginFrame をはじめとする GPU 依存の API は
 * LN_ERROR_DEVICE_LOST を返します。内部の自動復旧が完了すると
 * LNGraphicsContext_BeginFrame は再び LN_OK を返すようになります。
 *
 * @param[in] deep LN_TRUE の場合、WebGPU では実際にデバイスを破棄し、
 *                 後続 API のエラー挙動まで再現します (Vulkan では無視され、
 *                 ロストフラグを立てるのみ)。
 */
extern LUMINO_API LNResult LNDebug_SimulateDeviceLost(LNBool deep);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // LUMINO_API_H_
