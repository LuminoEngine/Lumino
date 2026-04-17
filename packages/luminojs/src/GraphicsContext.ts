import { LuminoObject } from "./LuminoObject";
import { Renderer } from "./Renderer";
import { API, Runtime } from "./Runtime";
import type { Handle } from "./types";

/** Return value of `GraphicsContext.beginFrame()`. */
export interface FrameInfo {
    renderer: Renderer;
    colorBuffer: Handle;
    depthBuffer: Handle;
}

export class GraphicsContext extends LuminoObject {
    private _renderer: Renderer | null = null;
    private _windowHandle: Handle = 0;
    private _canvas: HTMLCanvasElement | null = null;

    /**
     * HTML `<canvas>` 要素を描画先として GraphicsContext を作成する。
     * Web のみ対応。
     * `width` / `height` を省略した場合は selector で見つけた canvas のサイズを使用する。
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
     * The returned `renderer`, `colorBuffer`, and `depthBuffer` handles are
     * owned by the C-side GraphicsContext - do **not** call `dispose()` on them.
     */
    beginFrame(): FrameInfo {
        const m = Runtime.module;

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
            Runtime.safeCall(() =>
                (API.LNGraphicsContext_BeginFrame as (
                    ctx: number, w: number, h: number,
                    r: number, c: number, d: number,
                ) => number)(
                    this._handle, width, height,
                    outPtr, outPtr + 4, outPtr + 8));

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

    /** Finish the current frame and present. */
    endFrame(): void {
        Runtime.safeCall(() =>
            (API.LNGraphicsContext_EndFrame as (h: number) => number)(this._handle));
    }

    override dispose(): void {
        this._renderer = null;
        if (this._windowHandle !== 0) {
            (API.LNObject_Release as (h: number) => number)(this._windowHandle);
            this._windowHandle = 0;
        }
        this._handle = 0;
    }
}
