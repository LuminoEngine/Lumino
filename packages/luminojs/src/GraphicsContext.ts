import { LuminoObject } from "./LuminoObject";
import { Renderer } from "./Renderer";
import { ResidencyManager } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { Result, type Handle } from "./types";
import type { Texture } from "./Texture";

/**
 * Return value of `GraphicsContext.beginFrame()`.
 * `GraphicsContext.beginFrame()` の戳り値。
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

    /**
     * デバイスロストからの自動復旧が完了した直後に呼ばれるフック。
     * Texture / Mesh / Material は自動的に再アップロードされるが、
     * RenderTarget / DepthStencil の内容はリセットされるため、
     * 内容の再構築が必要なアプリはここで再レンダリングを行う。
     */
    onDeviceRestored?: () => void;

    /** @internal Frame counter incremented at the start of each beginFrame(). */
    get currentFrame(): number { return this._currentFrame; }

    /** @internal Resource residency tracker owned by this context. */
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
     * @param options        幅・高さ (ピクセル) の上書き指定。省略時は canvas のサイズを使用。
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
     * Begin a new frame. Must be paired with `endFrame()`.
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
     * The returned `renderer`, `colorBuffer`, and `depthBuffer` handles are
     * owned by the C-side GraphicsContext - do **not** call `dispose()` on them.
     */
    beginFrame(): FrameInfo | null {
        const m = Runtime.module;

        this._currentFrame++;
        this._residencyManager.gc(this._currentFrame);

        // Get current canvas dimensions
        let width = 0;
        let height = 0;
        if (this._canvas) {
            width = this._canvas.width;
            height = this._canvas.height;
        }

        // We need 3 out-handles (12 bytes).
        const outPtr = m._malloc(12);
        try {
            const rc = (API.LNGraphicsContext_BeginFrame as (
                ctx: number, w: number, h: number,
                r: number, c: number, d: number,
            ) => number)(
                this._handle, width, height,
                outPtr, outPtr + 4, outPtr + 8);

            if (rc === Result.ERROR_DEVICE_LOST) {
                // デバイスロスト検出・自動復旧待ち。BeginFrame の呼び出し自体が
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

            // Cache the Renderer instance across frames.
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

    /** Finish the current frame and present.
     * フレームの描画を終了し、画面に表示します。
     * `beginFrame()` と対になるように呼び出してください。
     */
    endFrame(): void {
        Runtime.safeCall(() =>
            (API.LNGraphicsContext_EndFrame as (h: number) => number)(this._handle));
    }

    override dispose(): void {
        this._residencyManager.disposeAll();
        this._externalTextures.clear();
        this._renderer = null;
        if (this._windowHandle !== 0) {
            (API.LNObject_Release as (h: number) => number)(this._windowHandle);
            this._windowHandle = 0;
        }
        this._handle = 0;
    }
}
