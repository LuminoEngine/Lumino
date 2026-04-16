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

    /**
     * Begin a new frame. Must be paired with `endFrame()`.
     *
     * The returned `renderer`, `colorBuffer`, and `depthBuffer` handles are
     * owned by the C-side GraphicsContext - do **not** call `dispose()` on them.
     */
    async beginFrame(): Promise<FrameInfo> {
        const m = Runtime.module;

        // We need 3 out-handles (12 bytes).
        const outPtr = m._malloc(12);
        try {
            await Runtime.safeCallAsync(() =>
                (API.LNGraphicsContext_BeginFrame as (
                    ctx: number, r: number, c: number, d: number,
                ) => number | Promise<number>)(
                    this._handle, outPtr, outPtr + 4, outPtr + 8));

            // Read the three handles. Re-derive view after await (grow-safe).
            const buf = m.HEAPU8.buffer;
            const outView = new Uint32Array(buf, outPtr, 3);
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
}
