import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Mesh } from "./Mesh";
import { API, Runtime } from "./Runtime";
import {
    type Handle,
    type RenderPassDesc,
    type Transform,
    LN_NULL_HANDLE,
    LN_MAX_COLOR_ATTACHMENTS,
    LoadOp,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_COLOR_ATTACHMENT_DESC,
    SIZEOF_TRANSFORM,
} from "./types";

export class Renderer extends LuminoObject {
    // Pre-allocated WASM buffer for LNRenderPassDesc (216 bytes).
    // Created lazily on first use; freed when the Renderer is disposed.
    private _descPtr = 0;
    private _descView: DataView | null = null;
    private _descHeapBuf: ArrayBufferLike | null = null;

    // Pre-allocated WASM buffer for LNTransform (40 bytes).
    private _transformPtr = 0;
    private _transformView: DataView | null = null;
    private _transformHeapBuf: ArrayBufferLike | null = null;

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

    /**
     * Submit a mesh draw command.
     *
     * @param mesh      Mesh to draw.
     * @param transform TRS transform.
     * @param zIndex    Sort priority (default 0).
     */
    drawMesh(mesh: Mesh, transform: Transform, zIndex = 0): void {
        const ptr = this._serializeTransform(transform);
        Runtime.safeCall(() =>
            (API.LNRenderer_DrawMesh as (
                r: number, mesh: number, t: number, z: number,
            ) => number)(this._handle, mesh.handle, ptr, zIndex));
    }

    override dispose(): void {
        if (this._descPtr) {
            Runtime.module._free(this._descPtr);
            this._descPtr = 0;
            this._descView = null;
        }
        if (this._transformPtr) {
            Runtime.module._free(this._transformPtr);
            this._transformPtr = 0;
            this._transformView = null;
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

    private _ensureTransformBuffer(): void {
        if (!this._transformPtr) {
            this._transformPtr = Runtime.module._malloc(SIZEOF_TRANSFORM);
        }
        const buf = Runtime.module.HEAPU8.buffer;
        if (!this._transformView || this._transformHeapBuf !== buf) {
            this._transformHeapBuf = buf;
            this._transformView = new DataView(buf, this._transformPtr, SIZEOF_TRANSFORM);
        }
    }

    /**
     * Write a `Transform` into WASM linear memory and return its pointer.
     *
     * C layout (wasm32, 40 bytes):
     * ```
     * offset 0:  float posX, posY, posZ        (12 bytes)
     * offset 12: float rotX, rotY, rotZ, rotW  (16 bytes)
     * offset 28: float scaleX, scaleY, scaleZ  (12 bytes)
     * ```
     */
    private _serializeTransform(t: Transform): number {
        this._ensureTransformBuffer();
        const v = this._transformView!;
        v.setFloat32(0,  t.position[0], true);
        v.setFloat32(4,  t.position[1], true);
        v.setFloat32(8,  t.position[2], true);
        v.setFloat32(12, t.rotation[0], true);
        v.setFloat32(16, t.rotation[1], true);
        v.setFloat32(20, t.rotation[2], true);
        v.setFloat32(24, t.rotation[3], true);
        v.setFloat32(28, t.scale[0], true);
        v.setFloat32(32, t.scale[1], true);
        v.setFloat32(36, t.scale[2], true);
        return this._transformPtr;
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
