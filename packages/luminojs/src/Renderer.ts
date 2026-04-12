import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import { API, Runtime } from "./Runtime";
import {
    type Handle,
    type RenderPassDesc,
    LN_NULL_HANDLE,
    LN_MAX_COLOR_ATTACHMENTS,
    LoadOp,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_COLOR_ATTACHMENT_DESC,
} from "./types";

export class Renderer extends LuminoObject {
    // Pre-allocated WASM buffer for LNRenderPassDesc (216 bytes).
    // Created lazily on first use; freed when the Renderer is disposed.
    private _descPtr = 0;
    private _descView: DataView | null = null;
    private _descHeapBuf: ArrayBufferLike | null = null;

    /**
     * Begin a render pass.
     *
     * @param ctx    The GraphicsContext that owns this Renderer.
     * @param desc   Render pass descriptor (color attachments, depth, clear values…).
     * @param camera Camera handle, or `LN_NULL_HANDLE` (default).
     */
    beginRenderPass(ctx: GraphicsContext, desc: RenderPassDesc, camera: Handle = LN_NULL_HANDLE): void {
        const ptr = this._serializeDesc(desc);
        Runtime.safeCall(() =>
            (API.LNRenderer_BeginRenderPass as (
                r: number, ctx: number, d: number, cam: number,
            ) => number)(this._handle, ctx.handle, ptr, camera));
    }

    /** End the current render pass. */
    endRenderPass(): void {
        Runtime.safeCall(() =>
            (API.LNRenderer_EndRenderPass as (r: number) => number)(this._handle));
    }

    override dispose(): void {
        if (this._descPtr) {
            Runtime.module._free(this._descPtr);
            this._descPtr = 0;
            this._descView = null;
        }
        super.dispose();
    }

    //--------------------------------------------------------------------------
    // Private: serialize RenderPassDesc into pre-allocated WASM memory
    //--------------------------------------------------------------------------

    private _ensureDescBuffer(): void {
        if (!this._descPtr) {
            this._descPtr = Runtime.module._malloc(SIZEOF_RENDER_PASS_DESC);
        }
        const buf = Runtime.module.HEAPU8.buffer;
        if (!this._descView || this._descHeapBuf !== buf) {
            this._descHeapBuf = buf;
            this._descView = new DataView(buf, this._descPtr, SIZEOF_RENDER_PASS_DESC);
        }
    }

    /**
     * Write a `RenderPassDesc` into WASM linear memory and return its pointer.
     *
     * C layout (wasm32, 4-byte aligned, total 216 bytes):
     * ```
     * offset 0:   uint32_t colorAttachmentCount
     * offset 4:   LNColorAttachmentDesc colorAttachments[8]  (each 24 bytes)
     *   per-attachment:
     *     +0  uint32_t renderTarget
     *     +4  float    clearColor[4]
     *     +20 uint32_t loadOp
     * offset 196: LNDepthStencilAttachmentDesc depthStencil  (20 bytes)
     *     +0  uint32_t depthBuffer
     *     +4  float    clearDepth
     *     +8  uint32_t clearStencil
     *     +12 uint32_t depthLoadOp
     *     +16 uint32_t stencilLoadOp
     * ```
     */
    private _serializeDesc(desc: RenderPassDesc): number {
        this._ensureDescBuffer();
        const v = this._descView!;

        // Zero-fill the whole struct first (safe defaults: all zeros ≡ CLEAR,
        // renderTarget=NULL_HANDLE, clearDepth will be set below).
        const bytes = new Uint8Array(v.buffer, this._descPtr, SIZEOF_RENDER_PASS_DESC);
        bytes.fill(0);

        // --- Color attachments ---
        const attachments = desc.colorAttachments ?? [];
        const count = Math.min(attachments.length, LN_MAX_COLOR_ATTACHMENTS);
        v.setUint32(0, count, true);

        for (let i = 0; i < count; i++) {
            const a = attachments[i];
            const base = 4 + i * SIZEOF_COLOR_ATTACHMENT_DESC;
            v.setUint32(base + 0, a.renderTarget ?? LN_NULL_HANDLE, true);
            const c = a.clearColor ?? [0, 0, 0, 1];
            v.setFloat32(base + 4,  c[0], true);
            v.setFloat32(base + 8,  c[1], true);
            v.setFloat32(base + 12, c[2], true);
            v.setFloat32(base + 16, c[3], true);
            v.setUint32(base + 20, a.loadOp ?? LoadOp.Clear, true);
        }

        // --- Depth/stencil ---
        const dsBase = 4 + LN_MAX_COLOR_ATTACHMENTS * SIZEOF_COLOR_ATTACHMENT_DESC; // 196
        const ds = desc.depthStencil;
        v.setUint32(dsBase + 0,  ds?.depthBuffer   ?? LN_NULL_HANDLE, true);
        v.setFloat32(dsBase + 4, ds?.clearDepth     ?? 1.0, true);
        v.setUint32(dsBase + 8,  ds?.clearStencil   ?? 0, true);
        v.setUint32(dsBase + 12, ds?.depthLoadOp    ?? LoadOp.Clear, true);
        v.setUint32(dsBase + 16, ds?.stencilLoadOp  ?? LoadOp.Clear, true);

        return this._descPtr;
    }
}
