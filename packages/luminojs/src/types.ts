/** Lumino C-API result codes. 失敗時は例外として投げられます。 */
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

/** Graphics backend selection. */
export enum GraphicsBackend {
    Default = 0,
    Vulkan = 1,
    WebGPU = 2,
}

/**
 * Render pass attachment load operation.
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
 * 同一 zIndex 内での描画順 (二次ソート)。matches C `LNSortMode`。
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

/** Blend mode (matches C LNBlendMode). 合成方法。 */
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

/** Face culling mode (matches C LNCullMode). ポリゴンのカリングモード。 */
export enum CullMode {
    /** No culling - render both sides. カリングなし (両面描画)。 */
    None  = 0,
    /** Cull front faces. 前面をカリング。 */
    Front = 1,
    /** Cull back faces (default). 背面をカリング (デフォルト)。 */
    Back  = 2,
}

/** Texture filter mode (matches C `LNTextureFilterMode`). テクスチャのフィルタリング方法。 */
export enum TextureFilterMode {
    /** 最近傍。ドット絵の拡大表示向け。 */
    Nearest = 0,
    /** 線形補間 (デフォルト)。 */
    Linear  = 1,
}

/**
 * Texture address mode (matches C `LNTextureAddressMode`).
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

/** Built-in shader kind (matches C `LNBuiltinShader`). ビルトインシェーダの種類。 */
export enum BuiltinShader {
    /** Unlit: テクスチャ * カラー、ライティングなし。 */
    Unlit       = 0,
    /** BasicLit: Blinn-Phong、1方向ライト。 */
    BasicLit    = 1,
    /** StencilMask: ステンシルバッファへの書き込み専用。 */
    StencilMask = 2,
}

/** Texture format (matches C LNTextureFormat). */
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

/** Opaque object handle (uint32). */
export type Handle = number;

/** Null handle sentinel. */
export const LN_NULL_HANDLE: Handle = 0;

/** Maximum number of simultaneous color attachments. */
export const LN_MAX_COLOR_ATTACHMENTS = 8;

/**
 * Describes a single color attachment for a render pass.
 * カラーアタッチメントの設定。
 * @note `renderTarget` が未設定の場合、バックバッファが使用されます。
 */
export interface ColorAttachmentDesc {
    /** Render target handle (`LN_NULL_HANDLE` = back-buffer). */
    renderTarget?: Handle;
    /** RGBA クリアカラー。`loadOp` が `Clear` のときに使用されます。デフォルト `[0, 0, 0, 1]`。 */
    clearColor?: [number, number, number, number];
    /** Load operation. Default `LoadOp.Clear`. */
    loadOp?: LoadOp;
}

/**
 * Describes the depth-stencil attachment for a render pass.
 * デプス・ステンシルアタッチメントの設定。
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
 * Full render pass descriptor (mirrors C `LNRenderPassDesc`).
 * レンダーパスの設定。
 */
export interface RenderPassDesc {
    /** カラーアタッチメント配列。未設定または空配列の場合、バックバッファが使用されます。 */
    colorAttachments?: ColorAttachmentDesc[];
    /** デプス・ステンシルアタッチメント。 */
    depthStencil?: DepthStencilAttachmentDesc;
    /**
     * Shader pass name to select (e.g. "GBuffer"). NULL/empty = "Forward".
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

/** Options for `Runtime.initialize`. */
export interface RuntimeOptions {
    /** Path (or URL) to `LuminoC.wasm`. Forwarded to Emscripten `locateFile`. */
    wasmPath?: string;
    /** Callback for stdout lines from the C runtime. */
    print?: (text: string) => void;
    /** Callback for stderr lines from the C runtime. */
    printErr?: (text: string) => void;
    /** Enable graphics validation layer. */
    enableValidation?: boolean;
}

//------------------------------------------------------------------------------
// C struct layout constants (wasm32, 4-byte aligned)
//------------------------------------------------------------------------------

/** Byte size of `LNColorAttachmentDesc` in wasm memory. */
export const SIZEOF_COLOR_ATTACHMENT_DESC = 24;
// Layout: renderTarget(u32,0) clearColor(f32x4,4) loadOp(u32,20)

/** Byte size of `LNDepthStencilAttachmentDesc` in wasm memory. */
export const SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC = 20;
// Layout: depthBuffer(u32,0) clearDepth(f32,4) clearStencil(u32,8)
//         depthLoadOp(u32,12) stencilLoadOp(u32,16)

/** Byte size of `LNRenderPassDesc` in wasm memory. */
export const SIZEOF_RENDER_PASS_DESC = 224;
// Layout: colorAttachmentCount(u32,0)
//         colorAttachments[8](24*8=192, offset 4)
//         depthStencil(20, offset 196)
//         shaderPassName(ptr,216)
//         sortMode(u32,220)

/** Byte size of `LNInstanceInitializeSettings` in wasm memory. */
export const SIZEOF_INSTANCE_INIT_SETTINGS = 8;
// Layout: preferredBackend(u32,0) enableValidation(u32,4)

/** Byte size of `LNVertex` in wasm memory. */
export const SIZEOF_VERTEX = 64;
// Layout: posX,posY,posZ(f32x3,0) normX,normY,normZ(f32x3,12)
//         u,v(f32x2,24) colorR,G,B,A(f32x4,32) tanX,Y,Z,W(f32x4,48)

/** Byte size of `LNSubMesh` in wasm memory. */
export const SIZEOF_SUBMESH = 12;
// Layout: indexOffset(u32,0) indexCount(u32,4) materialIndex(u32,8)

/** Byte size of `LNTransform` in wasm memory. */
export const SIZEOF_TRANSFORM = 40;
// Layout: posX,posY,posZ(f32x3,0) rotX,rotY,rotZ,rotW(f32x4,12)
//         scaleX,scaleY,scaleZ(f32x3,28)

/** Byte size of `LNMatrix` in wasm memory (列優先 4x4, float[16])。 */
export const SIZEOF_MATRIX = 64;

/** Byte size of `LNGraphicsProfiler` in wasm memory. */
export const SIZEOF_GRAPHICS_PROFILER = 12;
// Layout: drawCallCount(i32,0) fps(f32,4) lastFrameTimeMs(f32,8)

/**
 * Vertex data matching C `LNVertex` (64 bytes).
 * 標準頂点データ (64 bytes)。
 */
export interface Vertex {
    position: [number, number, number];
    normal: [number, number, number];
    uv: [number, number];
    color: [number, number, number, number];
    tangent: [number, number, number, number];
}

/**
 * Sub-mesh descriptor matching C `LNSubMesh` (12 bytes).
 * サブメッシュ (インデックスバッファの部分範囲とマテリアルインデックス)。
 */
export interface SubMesh {
    indexOffset: number;
    indexCount: number;
    materialIndex: number;
}

/**
 * Graphics profiling counters matching C `LNGraphicsProfiler` (12 bytes).
 * グラフィックスのプロファイリング情報。`GraphicsContext.getProfiler()` で取得します。
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
}

/**
 * TRS transform matching C `LNTransform` (40 bytes).
 * TRS トランスフォーム。
 */
export interface Transform {
    position: [number, number, number];
    rotation: [number, number, number, number]; // quaternion (x, y, z, w)
    scale: [number, number, number];
}
