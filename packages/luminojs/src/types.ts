/** Lumino C-API の結果コード。失敗時は例外として投げられます。 */
export enum Result {
    /** 成功 */
    OK = 0,
    /** 不明なエラー */
    ERROR_UNKNOWN = -1,
    /** ランタイム未初期化 */
    RUNTIME_UNINITIALIZED = -2,
    /** 無効な引数 */
    ERROR_INVALID_ARGUMENT = -3,
    /** 無効なハンドル */
    ERROR_INVALID_HANDLE = -4,
    /** 現在のビルド/プラットフォームでは未対応の機能 */
    ERROR_NOT_SUPPORTED = -5,
    /**
     * GPU デバイスロスト。復旧は Lumino 内部で自動的に行われるため、
     * このフレームの描画をスキップして次のフレームで再試行する。
     */
    ERROR_DEVICE_LOST = -6,
}

/**
 * ログ出力レベル 。値が大きいほど重大です。
 */
export enum LogLevel {
    /** 関数の入口や分岐のトレース。 */
    TRACE = 0,
    /** デバッグに有用な情報。 */
    DEBUG = 1,
    /** Info に加えた詳細情報。 */
    VERBOSE = 2,
    /** 通常運用で有用な情報 (デフォルト)。 */
    INFO = 3,
    /** 問題になる可能性がある状況。 */
    WARNING = 4,
    /** 処理を続行できない障害。 */
    ERROR = 5,
    /** プログラム実行が不可能な状況。 */
    FATAL = 6,
    /** すべてのログを破棄します。 */
    DISABLE = 7,
}

/** グラフィックスバックエンドの選択。 */
export enum GraphicsBackend {
    DEFAULT = 0,
    VULKAN = 1,
    WEBGPU = 2,
}

/**
 * レンダーパスのアタッチメントのロード操作。ゼロ初期化時のデフォルトは `Clear`。
 */
export enum LoadOp {
    /** 内容をクリアします。 */
    Clear = 0,
    /** 既存の内容を保持します。 */
    Load = 1,
    /**
     * 内容不定 (パフォーマンス最適化)。
     * ポストプロセスや G-Buffer のように出力先の全ピクセルを書き換える場合に指定できます。
     */
    DontCare = 2,
}

/**
 * 同一 zIndex 内での描画順 (二次ソート)。C の `LNSortMode` に対応します。
 * zIndex 自体は常に主キー (エンジンが指定するレイヤ/レンダーキュー番号) であり、
 * 本モードはその中での並びだけを決めます。
 * 距離はカメラ位置からのユークリッド距離ではなくビュー平面からの距離 (ビュー空間 Z) で
 * 計算されるため、Perspective / Orthographic の両方で正しく機能します。
 * @note 深度ソートはカメラを指定した RenderPass でのみ有効です。
 */
export enum SortMode {
    /** 投入順 (描画順 = 呼び出し順)。既定値。ソート方針はアプリ側が制御します。 */
    Stable = 0,
    /** 手前→奥 (ビュー平面からの距離が小さい順)。不透明の overdraw 削減向け。 */
    FrontToBack = 1,
    /** 奥→手前 (ビュー平面からの距離が大きい順)。半透明の正しいアルファ合成向け。 */
    BackToFront = 2,
}

/** 合成方法 (C の LNBlendMode に対応)。 */
export enum BlendMode {
    /** 通常 */
    Normal   = 0,
    /** アルファブレンド (RGB をアルファブレンドし、A を加算合成) */
    Alpha    = 1,
    /** 加算合成 */
    Add      = 2,
    /** 減算合成 */
    Subtract = 3,
    /** 乗算合成 */
    Multiply = 4,
}

/** ポリゴンのカリングモード (C の LNCullMode に対応)。 */
export enum CullMode {
    /** カリングなし (両面描画)。 */
    None  = 0,
    /** 前面をカリング。 */
    Front = 1,
    /** 背面をカリング (デフォルト)。 */
    Back  = 2,
}

/** テクスチャのフィルタリング方法 (C の `LNTextureFilterMode` に対応)。 */
export enum TextureFilterMode {
    /** 最近傍。ドット絵の拡大表示向け。 */
    Nearest = 0,
    /** 線形補間 (デフォルト)。 */
    Linear  = 1,
}

/**
 * テクスチャのアドレスモード (C の `LNTextureAddressMode` に対応)。
 * テクスチャ座標が 0.0 - 1.0 の範囲外になったときの回り込み方法。
 */
export enum TextureAddressMode {
    /** 繰り返し。タイリングする模様やノイズテクスチャ向け。 */
    Repeat         = 0,
    /** 反転繰り返し。 */
    MirroredRepeat = 1,
    /** 端のピクセルを引き伸ばす (デフォルト)。 */
    ClampToEdge    = 2,
}

/** ビルトインシェーダの種類 (C の `LNBuiltinShader` に対応)。 */
export enum BuiltinShader {
    /** Unlit: テクスチャ * カラー、ライティングなし。 */
    Unlit       = 0,
    /** BasicLit: Blinn-Phong、1方向ライト。 */
    BasicLit    = 1,
    /** StencilMask: ステンシルバッファへの書き込み専用。 */
    StencilMask = 2,
}

/** テクスチャフォーマット (C の LNTextureFormat に対応)。 */
export enum TextureFormat {
    Undefined       = 0,
    BGRA8_UNORM     = 1,
    BGRA8_UNORM_SRGB = 2,
    RGBA8_UNORM     = 3,
    RGBA8_UNORM_SRGB = 4,
    R8_UNORM        = 7,
    RG8_UNORM       = 8,
    RGBA16_FLOAT    = 9,
    RGBA32_FLOAT    = 10,
}

/** 不透明なオブジェクトハンドル (uint32)。 */
export type Handle = number;

/** NULL ハンドルを表す番兵値。 */
export const LN_NULL_HANDLE: Handle = 0;

/** 同時に使用できるカラーアタッチメントの最大数。 */
export const LN_MAX_COLOR_ATTACHMENTS = 8;

/**
 * カラーアタッチメントの設定。
 * @note `renderTarget` が未設定の場合、バックバッファが使用されます。
 */
export interface ColorAttachmentDesc {
    /** レンダーターゲットのハンドル (`LN_NULL_HANDLE` = バックバッファ)。 */
    renderTarget?: Handle;
    /** RGBA クリアカラー。`loadOp` が `Clear` のときに使用されます。デフォルト `[0, 0, 0, 1]`。 */
    clearColor?: [number, number, number, number];
    /** ロード操作。デフォルト `LoadOp.Clear`。 */
    loadOp?: LoadOp;
}

/**
 * デプスステンシルアタッチメントの設定。
 * @note `depthBuffer` が未設定の場合、バックバッファのデプスバッファが使用されます。
 */
export interface DepthStencilAttachmentDesc {
    /** デプスバッファ (未設定 = バックバッファのデプス)。 */
    depthBuffer?: Handle;
    /** クリアデプス値。デフォルト: 1.0。 */
    clearDepth?: number;
    /** クリアステンシル値。デフォルト: 0。 */
    clearStencil?: number;
    /** デプスのロード操作。デフォルト: `LoadOp.Clear`。 */
    depthLoadOp?: LoadOp;
    /** ステンシルのロード操作。デフォルト: `LoadOp.Clear`。 */
    stencilLoadOp?: LoadOp;
}

/**
 * レンダーパスの設定 (C の `LNRenderPassDesc` に対応)。
 */
export interface RenderPassDesc {
    /** カラーアタッチメント配列。未設定または空配列の場合、バックバッファが使用されます。 */
    colorAttachments?: ColorAttachmentDesc[];
    /** デプスステンシルアタッチメント。 */
    depthStencil?: DepthStencilAttachmentDesc;
    /**
     * マテリアルから優先的に選択する ShaderPass の名前。
     * 未設定または空文字列の場合は "Forward" が使用されます。
     * マテリアルがこの名前の ShaderPass を持たない場合、そのメッシュの描画はスキップされます。
     * 代表的なパス名: "Forward", "GBuffer", "Shadow" など。
     */
    shaderPassName?: string;
    /**
     * 同一 zIndex 内のスプライト/メッシュの描画順 (デフォルト: `SortMode.Stable` = 投入順)。
     * `FrontToBack` / `BackToFront` はカメラ指定時のみ有効。
     */
    sortMode?: SortMode;
}

/**
 * `Runtime.decodeImage` のデコード結果。
 * GPU 非依存の純 CPU 経路 (stb_image) でデコードされた RGBA8 画像です。
 */
export interface DecodedImage {
    /** 画像の幅 (ピクセル)。 */
    width: number;
    /** 画像の高さ (ピクセル)。 */
    height: number;
    /**
     * RGBA8 ピクセル列 (長さ = width * height * 4)。
     * WASM ヒープからコピー済みの JS 所有バッファです。
     */
    pixels: Uint8Array;
}

/** `Runtime.initialize` のオプション。 */
export interface RuntimeOptions {
    /** `LuminoC.wasm` のパス (または URL)。Emscripten の `locateFile` に渡されます。 */
    wasmPath?: string;
    /** C ランタイムの stdout 出力行を受け取るコールバック。 */
    print?: (text: string) => void;
    /** C ランタイムの stderr 出力行を受け取るコールバック。 */
    printErr?: (text: string) => void;
    /** グラフィックスのバリデーションレイヤを有効にします。 */
    enableValidation?: boolean;
    /**
     * ログ出力レベル。省略時は `LogLevel.Info`。
     * ランタイム初期化中のログも対象になります。
     */
    logLevel?: LogLevel;
}

//------------------------------------------------------------------------------
// C 構造体のレイアウト定数 (wasm32, 4 バイトアライン)
//------------------------------------------------------------------------------

/** wasm メモリ上の `LNColorAttachmentDesc` のバイトサイズ。 */
export const SIZEOF_COLOR_ATTACHMENT_DESC = 24;
// レイアウト: renderTarget(u32,0) clearColor(f32x4,4) loadOp(u32,20)

/** wasm メモリ上の `LNDepthStencilAttachmentDesc` のバイトサイズ。 */
export const SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC = 20;
// レイアウト: depthBuffer(u32,0) clearDepth(f32,4) clearStencil(u32,8)
//         depthLoadOp(u32,12) stencilLoadOp(u32,16)

/** wasm メモリ上の `LNRenderPassDesc` のバイトサイズ。 */
export const SIZEOF_RENDER_PASS_DESC = 224;
// レイアウト: colorAttachmentCount(u32,0)
//         colorAttachments[8](24*8=192, offset 4)
//         depthStencil(20, offset 196)
//         shaderPassName(ptr,216)
//         sortMode(u32,220)

/** wasm メモリ上の `LNInstanceInitializeSettings` のバイトサイズ。 */
export const SIZEOF_INSTANCE_INIT_SETTINGS = 8;
// レイアウト: preferredBackend(u32,0) enableValidation(u32,4)

/** wasm メモリ上の `LNVertex` のバイトサイズ。 */
export const SIZEOF_VERTEX = 64;
// レイアウト: posX,posY,posZ(f32x3,0) normX,normY,normZ(f32x3,12)
//         u,v(f32x2,24) colorR,G,B,A(f32x4,32) tanX,Y,Z,W(f32x4,48)

/** wasm メモリ上の `LNSubMesh` のバイトサイズ。 */
export const SIZEOF_SUBMESH = 12;
// レイアウト: indexOffset(u32,0) indexCount(u32,4) materialIndex(u32,8)

/** wasm メモリ上の `LNTransform` のバイトサイズ。 */
export const SIZEOF_TRANSFORM = 40;
// レイアウト: posX,posY,posZ(f32x3,0) rotX,rotY,rotZ,rotW(f32x4,12)
//         scaleX,scaleY,scaleZ(f32x3,28)

/** wasm メモリ上の `LNMatrix` のバイトサイズ (列優先 4x4, float[16])。 */
export const SIZEOF_MATRIX = 64;

/** wasm メモリ上の `LNGraphicsProfiler` のバイトサイズ。 */
export const SIZEOF_GRAPHICS_PROFILER = 20;
// レイアウト: drawCallCount(i32,0) fps(f32,4) lastFrameTimeMs(f32,8)
//         shaderPassCount(i32,12) materialCacheCount(i32,16)

/**
 * 標準頂点データ (C の `LNVertex` に対応、64 bytes)。
 */
export interface Vertex {
    position: [number, number, number];
    normal: [number, number, number];
    uv: [number, number];
    color: [number, number, number, number];
    tangent: [number, number, number, number];
}

/**
 * サブメッシュ (インデックスバッファの部分範囲とマテリアルインデックス)。C の `LNSubMesh` に対応 (12 bytes)。
 */
export interface SubMesh {
    indexOffset: number;
    indexCount: number;
    materialIndex: number;
}

/**
 * グラフィックスのプロファイリング情報 (C の `LNGraphicsProfiler` に対応、20 bytes)。
 * `GraphicsContext.getProfiler()` で取得します。
 */
export interface GraphicsProfiler {
    /**
     * ドローコール数。
     * `beginFrame()` でリセットされ、描画のたびに加算されます。
     * スプライトのバッチングが効いていれば、スプライト枚数を増やしても
     * この値はほとんど増えません。
     */
    drawCallCount: number;
    /** 実際のフレームレート (FPS)。直前フレームの所要時間から算出した瞬間値です。 */
    fps: number;
    /** 直前フレームの所要時間 (ミリ秒)。 */
    lastFrameTimeMs: number;
    /**
     * 生存しているシェーダパスの数 (プロセス全体。フレームでリセットされません)。
     *
     * 1 パスが GPU シェーダモジュール 2 個 (頂点/フラグメント) と
     * パイプラインレイアウト 1 個を所有するため、
     * 「シェーダモジュール数 = この値 x 2」「パイプラインレイアウト数 = この値」です。
     *
     * 1 つの `Shader` から `Material.createFromShader` で Material を複数作った場合、
     * Material を増やしてもこの値は増えません (共有できている証拠になります)。
     * 逆に `Material.createFromCompiledShader` は Material ごとに増えます。
     * ビルトインシェーダの分も含まれるため、絶対値ではなく
     * 「Material を増やしたときの増分」で確認してください。
     */
    shaderPassCount: number;
    /**
     * マテリアル BindGroup キャッシュのエントリ数 (フレームでリセットされません)。
     * Material の破棄でエビクトされるため、増え続ける場合は Material が
     * どこかで保持されたままになっています。
     */
    materialCacheCount: number;
}

/**
 * TRS トランスフォーム (C の `LNTransform` に対応、40 bytes)。
 */
export interface Transform {
    position: [number, number, number];
    rotation: [number, number, number, number]; // クォータニオン (x, y, z, w)
    scale: [number, number, number];
}
