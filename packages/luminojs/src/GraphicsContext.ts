import { LuminoObject } from "./LuminoObject";
import { Renderer } from "./Renderer";
import { ResidencyManager } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { readGraphicsProfiler } from "./serialize";
import { Result, SIZEOF_GRAPHICS_PROFILER, type GraphicsProfiler, type Handle } from "./types";
import type { Texture } from "./Texture";

/**
 * `GraphicsContext.beginFrame()` の戻り値。
 */
export interface FrameInfo {
    renderer: Renderer;
    /** 現フレームのバックバッファ (SwapChain のカラーテクスチャ)。 */
    colorBuffer: Handle;
    /** 現フレームのデプスバッファ。 */
    depthBuffer: Handle;
}

export class GraphicsContext extends LuminoObject {
    private _renderer: Renderer | null = null;
    private _windowHandle: Handle = 0;
    private _canvas: HTMLCanvasElement | null = null;
    private _currentFrame = 0;
    private _residencyManager = new ResidencyManager();
    private _deviceLostPending = false;
    private _externalTextures: Set<Texture> = new Set();
    /** getProfiler() 用の使い回しバッファ。毎フレーム呼ばれても malloc しないよう遅延確保する。 */
    private _profilerPtr = 0;

    /**
     * デバイスロストからの自動復旧が完了した直後に呼ばれるフック。
     * Texture / Mesh / Material は自動的に再アップロードされるが、
     * RenderTarget / DepthStencil の内容はリセットされるため、
     * 内容の再構築が必要なアプリはここで再レンダリングを行う。
     */
    onDeviceRestored?: () => void;

    /** @internal beginFrame() の先頭で加算されるフレームカウンタ。 */
    get currentFrame(): number { return this._currentFrame; }

    /** @internal このコンテキストが所有するリソースの常駐状態トラッカー。 */
    get residencyManager(): ResidencyManager { return this._residencyManager; }

    /** @internal RT / DS の復旧時再作成のための登録。Texture.createRenderTargetEx 等から呼ばれる。 */
    _registerExternalTexture(t: Texture): void {
        this._externalTextures.add(t);
    }

    /** @internal Texture.dispose から呼ばれる。 */
    _unregisterExternalTexture(t: Texture): void {
        this._externalTextures.delete(t);
    }

    /**
     * HTML `<canvas>` 要素を描画先として GraphicsContext を作成する。
     * Web のみ対応。デスクトップビルドでは例外を投げます。
     * `width` / `height` を省略した場合は selector で見つけた canvas のサイズを使用する。
     * @param canvasSelector 描画先 canvas の CSS セレクタ (例: `"#my_canvas"`)
     * @param options        幅と高さ (ピクセル) の上書き指定。省略時は canvas のサイズを使用。
     */
    static async createFromCanvas(
        canvasSelector: string,
        options?: { width?: number; height?: number },
    ): Promise<GraphicsContext> {
        let width  = options?.width;
        let height = options?.height;
        const canvas = document.querySelector(canvasSelector) as HTMLCanvasElement | null;
        if (!canvas) throw new Error(`createFromCanvas: canvas not found: "${canvasSelector}"`);
        if (width  === undefined) width  = canvas.width;
        if (height === undefined) height = canvas.height;
        const windowHandle = await Runtime.safeCallWithReturnHandleAsync((ptr) =>
            (API.LNWindow_CreateFromCanvas as (s: string, w: number, h: number, p: number) => number | Promise<number>)(
                canvasSelector, width!, height!, ptr,
            ));
        const ctxHandle = Runtime.safeCallWithReturnHandle((ptr) =>
            (API.LNWindow_GetGraphicsContext as (h: number, p: number) => number)(
                windowHandle, ptr));
        const ctx = new GraphicsContext();
        ctx._windowHandle = windowHandle;
        ctx._canvas = canvas;
        ctx._setHandle(ctxHandle, false);
        return ctx;
    }

    /**
     * 新しいフレームを開始します。`endFrame()` と対にして呼び出してください。
     *
     * Canvas の現在の width/height を自動取得し、サイズが変わっていれば
     * SwapChain と深度バッファを自動リサイズします。
     *
     * デバイスロスト中および自動復旧中は `null` を返します。アプリはこのフレームの
     * 描画をスキップし、次のフレームで再度呼び出してください (復旧は Lumino 内部で
     * 自動的に進行し、完了すると再び FrameInfo が返ります)。復旧完了時には
     * ロスト前のリソースの再アップロードが自動的に予約され、`onDeviceRestored`
     * フックが呼ばれます。
     *
     * 返される `renderer`, `colorBuffer`, `depthBuffer` の各ハンドルは
     * C 側の GraphicsContext が所有するため、`dispose()` を呼んでは**いけません**。
     */
    beginFrame(): FrameInfo | null {
        const m = Runtime.module;

        this._currentFrame++;
        this._residencyManager.gc(this._currentFrame);

        // canvas の現在のサイズを取得する
        let width = 0;
        let height = 0;
        if (this._canvas) {
            width = this._canvas.width;
            height = this._canvas.height;
        }

        // out ハンドルが 3 つ必要 (12 バイト)。
        const outPtr = m._malloc(12);
        try {
            const rc = (API.LNGraphicsContext_BeginFrame as (
                ctx: number, w: number, h: number,
                r: number, c: number, d: number,
            ) => number)(
                this._handle, width, height,
                outPtr, outPtr + 4, outPtr + 8);

            if (rc === Result.ERROR_DEVICE_LOST) {
                // デバイスロストを検出し、自動復旧を待つ。BeginFrame の呼び出し自体が
                // C++ 側の復旧ステートマシンを進めるため、アプリはフレームを
                // スキップしてループを回し続けるだけでよい。
                this._deviceLostPending = true;
                return null;
            }
            if (rc !== Result.OK) {
                throw new Error(`Lumino C-API error: LNGraphicsContext_BeginFrame (${rc})`);
            }
            if (this._deviceLostPending) {
                this._deviceLostPending = false;
                this._handleDeviceRestored();
            }

            const outView = new Uint32Array(m.HEAPU8.buffer, outPtr, 3);
            const rendererHandle = outView[0];
            const colorBuffer    = outView[1];
            const depthBuffer    = outView[2];

            // Renderer インスタンスはフレームをまたいでキャッシュする。
            if (!this._renderer || this._renderer.handle !== rendererHandle) {
                this._renderer = new Renderer();
                this._renderer._setHandle(rendererHandle, false);
            }

            return { renderer: this._renderer, colorBuffer, depthBuffer };
        } finally {
            m._free(outPtr);
        }
    }

    /**
     * デバイスロスト復旧完了後の最初のフレームで呼ばれる。
     * 旧デバイス上の stale リソースを解放し、次の描画時の ensure() による
     * 再アップロードを予約する。
     */
    private _handleDeviceRestored(): void {
        // Renderer は C++ 側で作り直されている。ハンドル比較でも追随するが明示的に破棄する。
        this._renderer = null;

        // ソースを持つリソース (Texture / Mesh / Material) は evict して
        // 次の描画時に自動再アップロードさせる。
        this._residencyManager.invalidateAll();

        // RenderTarget / DepthStencil は生成情報から構造だけ再作成する (内容はリセット)。
        for (const t of this._externalTextures) {
            t._recreateExternal(this);
        }

        // アプリが RT の内容再構築などを行うためのフック。
        this.onDeviceRestored?.();
    }

    /**
     * フレームの描画を終了し、画面に表示します。
     * `beginFrame()` と対になるように呼び出してください。
     */
    endFrame(): void {
        Runtime.safeCall(() =>
            (API.LNGraphicsContext_EndFrame as (h: number) => number)(this._handle));
    }

    /**
     * グラフィックスのプロファイリング情報 (ドローコール数 / FPS / フレーム時間) を取得します。
     *
     * **呼び出しタイミング**: `endFrame()` の後に呼び出してください。そうすると
     * 直前に描画し終えたフレームの計測値が得られます。`fps` と `lastFrameTimeMs` は
     * `endFrame()` の中で更新されるため、`beginFrame()` - `endFrame()` の間に
     * 呼び出すと 1 フレーム古い値になります。また `drawCallCount` は
     * `beginFrame()` でリセットされるため、フレームの途中で呼び出すと
     * その時点までの途中経過になります。
     *
     * スプライトのバッチングが効いているかは `drawCallCount` で確認できます。
     * バッチングされていればスプライト枚数を増やしてもこの値はほとんど増えません。
     *
     * ```ts
     * ctx.endFrame();
     * const p = ctx.getProfiler();
     * console.log(`draw calls: ${p.drawCallCount}, ${p.fps.toFixed(1)} fps`);
     * ```
     */
    getProfiler(): GraphicsProfiler {
        const m = Runtime.module;
        if (this._profilerPtr === 0) {
            this._profilerPtr = m._malloc(SIZEOF_GRAPHICS_PROFILER);
        }
        const rc = (API.LNDebug_GetGraphicsProfiler as (ctx: number, out: number) => number)(
            this._handle, this._profilerPtr);
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: LNDebug_GetGraphicsProfiler (${rc})`);
        }
        // WASM メモリ成長に備え、呼び出し後に view を作る。
        const view = new DataView(m.HEAPU8.buffer, this._profilerPtr, SIZEOF_GRAPHICS_PROFILER);
        return readGraphicsProfiler(view);
    }

    override dispose(): void {
        this._residencyManager.disposeAll();
        this._externalTextures.clear();
        this._renderer = null;
        if (this._profilerPtr !== 0) {
            Runtime.module._free(this._profilerPtr);
            this._profilerPtr = 0;
        }
        if (this._windowHandle !== 0) {
            (API.LNObject_Release as (h: number) => number)(this._windowHandle);
            this._windowHandle = 0;
        }
        this._handle = 0;
    }
}
