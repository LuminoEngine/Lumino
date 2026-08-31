import {
    Result,
    GraphicsBackend,
    LogLevel,
    type Handle,
    type RuntimeOptions,
    type DecodedImage,
    SIZEOF_INSTANCE_INIT_SETTINGS,
    SIZEOF_COLOR_ATTACHMENT_DESC,
    SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_VERTEX,
    SIZEOF_SUBMESH,
    SIZEOF_TRANSFORM,
    SIZEOF_MATRIX,
    SIZEOF_GRAPHICS_PROFILER,
} from "./types";
import { Logger } from "./Logger";

//------------------------------------------------------------------------------
// Emscripten Module の型 (使用する部分のみ)
//------------------------------------------------------------------------------

interface EmscriptenModule {
    cwrap(ident: string, returnType: string | null, argTypes: string[], opts?: { async?: boolean }): (...args: unknown[]) => unknown;
    _malloc(size: number): number;
    _free(ptr: number): void;
    UTF8ToString(ptr: number): string;
    HEAPU8: Uint8Array;
    HEAPU32: Uint32Array;
    HEAPF32: Float32Array;
}

type ModuleFactory = (opts?: Record<string, unknown>) => Promise<EmscriptenModule>;

//------------------------------------------------------------------------------
// 内部用 cwrap バインディング
//------------------------------------------------------------------------------

/** @internal cwrap した C 関数の参照を保持する。 */
export const API: Record<string, (...args: never[]) => unknown> = {};

//------------------------------------------------------------------------------
// Runtime
//------------------------------------------------------------------------------

export class Runtime {
    /** 生の Emscripten モジュール。高度な用途向けに公開する。 */
    static module: EmscriptenModule;

    // 単一の out パラメータ読み取り用に事前確保した 4 バイトのスロット。
    private static _returnPtr = 0;
    private static _returnView: Uint32Array | undefined;
    private static _heapBuffer: ArrayBufferLike | undefined;

    /** `initialize` が呼ばれたかどうか。 */
    static get initialized(): boolean {
        return !!this.module;
    }

    /**
     * このバイナリがビルドされた日時文字列を返す。
     * ランタイム初期化不要。正しい WASM が読み込まれているかの確認用。
     */
    static getBuildTimestamp(): string {
        const ptr = API.LNBuildInfo_GetBuildTimestamp() as number;
        return this.module.UTF8ToString(ptr);
    }

    /**
     * Emscripten の WASM モジュールをロードし、すべての C-API シンボルをバインドする。
     */
    static async initialize(options?: RuntimeOptions): Promise<void> {
        if (this.initialized) return;

        // ログレベルは WASM のロード中に出るログにも効かせるため、まず JS 側へ反映する。
        // ネイティブ側への反映は cwrap 完了後 (後述)。
        if (options?.logLevel !== undefined) {
            Logger.setLevel(options.logLevel);
        }

        // 動的 import。このファイルはビルド済み TS 出力と同じ lib/ に置かれる。
        const { default: LuminoC } = await import("./LuminoC.mjs") as { default: ModuleFactory };

        const moduleOpts: Record<string, unknown> = {};
        if (options?.wasmPath) {
            const wasmPath = options.wasmPath;
            moduleOpts["locateFile"] = (path: string) =>
                path.endsWith(".wasm") ? wasmPath : path;
        }

        // print 関数を設定する。
        let printFunc: ((text: string) => void) | undefined = options?.print;
        let printErrFunc: ((text: string) => void) | undefined = options?.printErr;
        if (!printFunc)  {
            // ネイティブ側の Logger 出力は stdout に流れてくる。
            printFunc = (text: string) => Logger.writeNativeLine(text);
        }
        if (!printErrFunc)  {
            // stderr には Emscripten 自身の abort / assert などが流れてくる。
            printErrFunc = (text: string) => Logger.error(text);
        }
        moduleOpts["print"]    = printFunc;
        moduleOpts["printErr"] = printErrFunc;

        const preferredBackend: GraphicsBackend = options?.backend ?? GraphicsBackend.WEBGPU;

        this.module = await LuminoC(moduleOpts);

        // out パラメータ読み取り用の 4 バイトスロットを事前確保する。
        this._returnPtr = this.module._malloc(4);

        // すべての C-API 関数を cwrap でバインドする。
        this._bindAPI();

        // ログレベルをネイティブ側にも反映する。
        if (options?.logLevel !== undefined) {
            this.setLogLevel(options.logLevel);
        }

        Logger.verbose("Build:", Runtime.getBuildTimestamp());

        // C API と TS 側の構造体レイアウト同期を検証する。
        // 不一致があれば以降のシリアライズがヒープ外読み書き (未定義動作) を
        // 起こすため、ここで即座に例外を投げて早期に検出する。
        this._verifyStructLayouts();

        // Lumino のグラフィックスインスタンスを初期化する。
        const m = this.module;
        const ptr = m._malloc(SIZEOF_INSTANCE_INIT_SETTINGS);
        // canvasSelector は WebGL2 バックエンドが参照する。C 側は初期化中しか読まないため、
        // 呼び出し後に解放してよい。
        let selectorPtr = 0;
        if (options?.canvasSelector) {
            const encoded = new TextEncoder().encode(options.canvasSelector);
            selectorPtr = m._malloc(encoded.length + 1);
            m.HEAPU8.set(encoded, selectorPtr);
            m.HEAPU8[selectorPtr + encoded.length] = 0; // ヌル終端
        }
        const view = new DataView(m.HEAPU8.buffer, ptr, SIZEOF_INSTANCE_INIT_SETTINGS);
        view.setUint32(0, preferredBackend, true);
        view.setUint32(4, options?.enableValidation ? 1 : 0, true);
        view.setUint32(8, selectorPtr, true);

        try {
            await this.safeCallAsync(() =>
                (API.LNInstance_Initialize as (p: number) => number | Promise<number>)(ptr));
        } finally {
            if (ptr) m._free(ptr);
            if (selectorPtr) m._free(selectorPtr);
        }
    }

    /**
     * ログ出力レベルを設定する。設定したレベル未満のログは破棄される。
     * `LogLevel.Off` ですべてのログ出力を停止できる。
     * JS 側 (Logger) とネイティブ側の両方に反映される。
     *
     * ランタイムの初期化 (グラフィックスインスタンス) は不要だが、WASM モジュールの
     * ロードは必要なため `initialize()` 完了後に呼ぶこと。初期化処理そのもののログを
     * 制御したい場合は `initialize({ logLevel })` を使う。
     */
    static setLogLevel(level: LogLevel): void {
        Logger.setLevel(level);
        this.safeCall(() => (API.LNLogger_SetLevel as (level: number) => number)(level));
    }

    /** Lumino ランタイムを終了する。 */
    static async terminate(): Promise<void> {
        await (API.LNInstance_Terminate as () => Promise<void>)();
    }

    //--------------------------------------------------------------------------
    // 画像デコード (GPU 非依存)
    //--------------------------------------------------------------------------

    /**
     * PNG / JPEG などの画像バイト列をデコードして RGBA8 ピクセルを返す。
     * stb_image による純 CPU 処理で、WebGPU (GPU) には依存しない。
     *
     * WASM モジュールがロード済み (= `initialize()` 済み) であることが前提。
     * デコード自体はグラフィックスインスタンスの初期化を必要としないが、
     * ヒープアクセスのために module のロードは必要となる。
     *
     * @param data エンコード済み画像バイト列 (PNG / JPEG など)
     * @returns デコード結果。ピクセルは WASM ヒープからコピー済みの JS 所有バッファ。
     */
    static decodeImage(data: Uint8Array): DecodedImage {
        if (!this.module) {
            throw new Error(
                "Runtime.decodeImage: WASM モジュールが未ロードです。先に initialize() を呼んでください。");
        }
        const m = this.module;

        // 入力データを WASM ヒープへコピーする。
        const dataPtr = m._malloc(data.length);
        // 出力用の 4 スロット (outWidth, outHeight, outPixelsPtr, outPixelsSize) を確保する。
        const outPtr = m._malloc(16);
        try {
            m.HEAPU8.set(data, dataPtr);

            const decode = API.LNImage_DecodeFromMemory as
                (data: number, size: number, outW: number, outH: number,
                    outPx: number, outSize: number) => number;
            const rc = decode(dataPtr, data.length, outPtr, outPtr + 4, outPtr + 8, outPtr + 12);
            if (rc !== Result.OK) {
                throw new Error(`Lumino C-API error: LNImage_DecodeFromMemory (${rc})`);
            }

            // stbi_load はヒープを成長させることがあるため、呼び出し後に view を取り直す。
            const out = new Uint32Array(m.HEAPU8.buffer, outPtr, 4);
            const width = out[0];
            const height = out[1];
            const pixelsPtr = out[2];
            const pixelsSize = out[3];

            // C 側が確保したピクセルを JS 所有のバッファへコピーする。
            const pixels = m.HEAPU8.slice(pixelsPtr, pixelsPtr + pixelsSize);

            // C 側のピクセルバッファを解放する。
            (API.LNImage_FreePixels as (p: number) => number)(pixelsPtr);

            return { width, height, pixels };
        } finally {
            m._free(dataPtr);
            m._free(outPtr);
        }
    }

    //--------------------------------------------------------------------------
    // 戻り値ポインタのヘルパー (HEAP バッファの成長に対応)
    //--------------------------------------------------------------------------

    /**
     * 事前確保した戻り値ポインタと、それを指す Uint32Array ビューを返す。
     * WASM メモリの成長で背後の ArrayBuffer が変わっていた場合は
     * ビューを自動的に作り直す。
     */
    static getReturnPointerInfo(): [ptr: number, view: Uint32Array] {
        const buf = this.module.HEAPU8.buffer;
        if (!this._returnView || this._heapBuffer !== buf) {
            this._heapBuffer = buf;
            this._returnView = new Uint32Array(buf, this._returnPtr, 1);
        }
        return [this._returnPtr, this._returnView];
    }

    //--------------------------------------------------------------------------
    // 安全呼び出しラッパー (同期)
    //--------------------------------------------------------------------------

    /** C 関数を呼び出し、結果が OK でなければ例外を投げる。 */
    static safeCall(fn: () => number): void {
        const rc = fn();
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
    }

    /** 事前確保した out ポインタ経由でハンドルを返す C 関数を呼び出す。 */
    static safeCallWithReturnHandle(fn: (ptr: number) => number): Handle {
        const [ptr, view] = this.getReturnPointerInfo();
        const rc = fn(ptr);
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
        return view[0];
    }

    //--------------------------------------------------------------------------
    // 安全呼び出しラッパー (非同期 - ASYNCIFY)
    //--------------------------------------------------------------------------

    /** `safeCall` の非同期版。 */
    static async safeCallAsync(fn: () => number | Promise<number>): Promise<void> {
        const rc = await fn();
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
    }

    /** `safeCallWithReturnHandle` の非同期版。 */
    static async safeCallWithReturnHandleAsync(fn: (ptr: number) => number | Promise<number>): Promise<Handle> {
        const [ptr, view] = this.getReturnPointerInfo();
        const rc = await fn(ptr);
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
        // await 後にビューを取り直す (HEAP が成長している可能性がある)。
        const [, freshView] = this.getReturnPointerInfo();
        return freshView[0];
    }

    //--------------------------------------------------------------------------
    // Private: cwrap bindings
    //--------------------------------------------------------------------------

    private static _bindAPI(): void {

        // NOTE: ccall と cwrap の違い
        // ccall
        // const result = Module.ccall('Add', 'number', ['number', 'number'], [1, 2]);
        // cwrap
        // const add = Module.cwrap('Add', 'number', ['number', 'number']);
        // const result = add(4, 5);

        const m = this.module;
        const cw = m.cwrap.bind(m);

        // Phase 0
        API.LNHelloTest = cw("LNHelloTest", "number", ["number"]);
        API.LNBuildInfo_GetBuildTimestamp = cw("LNBuildInfo_GetBuildTimestamp", "number", []);

        // Logger
        API.LNLogger_SetLevel = cw("LNLogger_SetLevel", "number", ["number"]);

        // Debug (ABI レイアウト検証 / プロファイリング)
        API.LNDebug_GetStructSize        = cw("LNDebug_GetStructSize",        "number", ["string", "number"]);
        API.LNDebug_GetGraphicsProfiler  = cw("LNDebug_GetGraphicsProfiler",  "number", ["number", "number"]);

        // Instance
        API.LNInstance_Initialize = cw("LNInstance_Initialize", "number", ["number"], { async: true });
        API.LNInstance_Terminate  = cw("LNInstance_Terminate",  null,     [],         { async: true });

        // Object
        API.LNObject_Release = cw("LNObject_Release", "number", ["number"]);

        // Window
        API.LNWindow_CreateFromCanvas   = cw("LNWindow_CreateFromCanvas",   "number", ["string", "number", "number", "number"], { async: true });
        API.LNWindow_GetGraphicsContext = cw("LNWindow_GetGraphicsContext", "number", ["number", "number"]);

        // GraphicsContext
        API.LNGraphicsContext_BeginFrame = cw("LNGraphicsContext_BeginFrame", "number", ["number", "number", "number", "number", "number", "number"]);
        API.LNGraphicsContext_EndFrame   = cw("LNGraphicsContext_EndFrame",   "number", ["number"]);

        // RenderPassDesc
        API.LNRenderPassDesc_Init = cw("LNRenderPassDesc_Init", null, ["number"]);

        // Renderer
        API.LNRenderer_BeginRenderPass = cw("LNRenderer_BeginRenderPass", "number", ["number", "number", "number", "number"]);
        API.LNRenderer_EndRenderPass   = cw("LNRenderer_EndRenderPass",   "number", ["number"]);
        API.LNRenderer_DrawMesh        = cw("LNRenderer_DrawMesh",        "number", ["number", "number", "number", "number"]);
        API.LNRenderer_DrawScreenRect  = cw("LNRenderer_DrawScreenRect",  "number", ["number", "number"]);
        API.LNRenderer_DrawSprite      = cw("LNRenderer_DrawSprite",      "number", ["number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNRenderer_PushStencilMask = cw("LNRenderer_PushStencilMask", "number", ["number", "number", "number", "number"]);
        API.LNRenderer_PopStencilMask  = cw("LNRenderer_PopStencilMask",  "number", ["number"]);

        // Texture
        API.LNTexture2D_CreateFromPixels    = cw("LNTexture2D_CreateFromPixels",    "number", ["number", "number", "number", "number", "number", "number", "number"]);
        API.LNTexture2D_CreateRenderTargetEx = cw("LNTexture2D_CreateRenderTargetEx", "number", ["number", "number", "number", "number", "number"]);
        API.LNTexture2D_CreateDepthStencil   = cw("LNTexture2D_CreateDepthStencil",   "number", ["number", "number", "number", "number"]);

        // Image (GPU 非依存の画像デコード)
        API.LNImage_DecodeFromMemory = cw("LNImage_DecodeFromMemory", "number", ["number", "number", "number", "number", "number", "number"]);
        API.LNImage_FreePixels       = cw("LNImage_FreePixels",       "number", ["number"]);

        // Shader
        API.LNShader_CreateFromCompiledShader    = cw("LNShader_CreateFromCompiledShader",    "number", ["number", "number", "number", "number"]);

        // Material
        API.LNMaterial_CreateFromBuiltinShader  = cw("LNMaterial_CreateFromBuiltinShader",  "number", ["number", "number", "number"]);
        API.LNMaterial_CreateFromShader          = cw("LNMaterial_CreateFromShader",          "number", ["number", "number"]);
        API.LNMaterial_CreateFromCompiledShader  = cw("LNMaterial_CreateFromCompiledShader",  "number", ["number", "number", "number", "number"]);
        API.LNMaterial_SetMainTexture            = cw("LNMaterial_SetMainTexture",            "number", ["number", "number"]);
        API.LNMaterial_SetColor                  = cw("LNMaterial_SetColor",                  "number", ["number", "number", "number", "number", "number"]);
        API.LNMaterial_SetFloat4                 = cw("LNMaterial_SetFloat4",                 "number", ["number", "string", "number"]);
        API.LNMaterial_SetNamedTexture           = cw("LNMaterial_SetNamedTexture",           "number", ["number", "string", "number"]);
        API.LNMaterial_SetSamplerState           = cw("LNMaterial_SetSamplerState",           "number", ["number", "number", "number"]);
        API.LNMaterial_SetNamedSamplerState      = cw("LNMaterial_SetNamedSamplerState",      "number", ["number", "string", "number", "number"]);
        API.LNMaterial_SetCullMode               = cw("LNMaterial_SetCullMode",               "number", ["number", "number"]);
        API.LNMaterial_SetBlendMode              = cw("LNMaterial_SetBlendMode",              "number", ["number", "number"]);
        API.LNMaterial_SetDepthTestEnabled       = cw("LNMaterial_SetDepthTestEnabled",       "number", ["number", "number"]);
        API.LNMaterial_SetDepthWriteEnabled      = cw("LNMaterial_SetDepthWriteEnabled",      "number", ["number", "number"]);

        // Mesh
        API.LNMesh_Create      = cw("LNMesh_Create",      "number", ["number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNMesh_SetMaterial = cw("LNMesh_SetMaterial", "number", ["number", "number", "number"]);

        // Camera
        API.LNCamera_Create         = cw("LNCamera_Create",         "number", ["number"]);
        API.LNCamera_SetPerspective = cw("LNCamera_SetPerspective", "number", ["number", "number", "number", "number", "number"]);
        API.LNCamera_SetOrthographic2D = cw("LNCamera_SetOrthographic2D", "number", ["number", "number", "number", "number", "number", "number", "number"]);
        API.LNCamera_SetLookAt      = cw("LNCamera_SetLookAt",      "number", ["number", "number", "number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNCamera_SetMatrices    = cw("LNCamera_SetMatrices",    "number", ["number", "number", "number", "number"]);
    }

    //--------------------------------------------------------------------------
    // Private: ABI レイアウト検証
    //--------------------------------------------------------------------------

    /**
     * 実行中の WASM バイナリに問い合わせた構造体サイズと、types.ts の SIZEOF_*
     * 定数を照合する。構造体サイズはプラットフォーム (ポインタ幅) で異なるため、
     * 定数値をハードコードで比較するのではなく、常に実バイナリへ問い合わせる。
     *
     * 不一致は lumino.h と types.ts のレイアウト同期崩れを意味し、シリアライズ時の
     * ヒープ外読み書き (未定義動作) につながるため、初期化時に即例外とする。
     */
    private static _verifyStructLayouts(): void {
        // [構造体名, TS 側が想定するサイズ]。types.ts の SIZEOF_* を全て網羅する。
        const expected: ReadonlyArray<readonly [name: string, size: number]> = [
            ["LNInstanceInitializeSettings", SIZEOF_INSTANCE_INIT_SETTINGS],
            ["LNColorAttachmentDesc",        SIZEOF_COLOR_ATTACHMENT_DESC],
            ["LNDepthStencilAttachmentDesc", SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC],
            ["LNRenderPassDesc",             SIZEOF_RENDER_PASS_DESC],
            ["LNVertex",                     SIZEOF_VERTEX],
            ["LNSubMesh",                    SIZEOF_SUBMESH],
            ["LNTransform",                  SIZEOF_TRANSFORM],
            ["LNMatrix",                     SIZEOF_MATRIX],
            ["LNGraphicsProfiler",           SIZEOF_GRAPHICS_PROFILER],
        ];

        const getStructSize = API.LNDebug_GetStructSize as
            (name: string, outPtr: number) => number;

        const mismatches: string[] = [];
        for (const [name, tsSize] of expected) {
            const [ptr] = this.getReturnPointerInfo();
            const rc = getStructSize(name, ptr);
            if (rc !== Result.OK) {
                // WASM が構造体名を認識しない = lumino.h への登録漏れ。
                throw new Error(
                    `ABI レイアウト検証に失敗しました: WASM が構造体 "${name}" を認識しません ` +
                    `(rc=${rc})。LuminoAPI.cpp の LNDebug_GetStructSize に登録されているか確認してください。`);
            }
            // WASM メモリ成長に備え、呼び出し後に view を取り直す。
            const [, view] = this.getReturnPointerInfo();
            const wasmSize = view[0];
            if (wasmSize !== tsSize) {
                mismatches.push(`${name}: types.ts=${tsSize} bytes, WASM=${wasmSize} bytes`);
            }
        }

        if (mismatches.length > 0) {
            throw new Error(
                "ABI レイアウト不整合を検出しました。lumino.h の構造体レイアウトと " +
                "packages/luminojs/src/types.ts の SIZEOF_* 定数が一致していません:\n" +
                mismatches.map((m) => "  - " + m).join("\n") +
                "\ntypes.ts の SIZEOF_* とシリアライズ処理 (Renderer.ts の _serializeDesc 等) を更新してください。");
        }
    }
}
