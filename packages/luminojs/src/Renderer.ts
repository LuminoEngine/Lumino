import { LuminoObject } from "./LuminoObject";
import type { Camera } from "./Camera";
import type { GraphicsContext } from "./GraphicsContext";
import type { Material } from "./Material";
import type { Mesh } from "./Mesh";
import { API, Runtime } from "./Runtime";
import {
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
    // LNRenderPassDesc 用の事前確保 WASM バッファ (220 バイト)。
    // 初回使用時に遅延生成し、Renderer が dispose されるときに解放する。
    private _descPtr = 0;
    private _descView: DataView | null = null;
    private _descHeapBuf: ArrayBufferLike | null = null;

    // LNTransform 用の事前確保 WASM バッファ (40 バイト)。
    private _transformPtr = 0;
    private _transformView: DataView | null = null;
    private _transformHeapBuf: ArrayBufferLike | null = null;

    // shaderPassName 用の一時文字列ポインタ (BeginRenderPass 後に解放する)。
    private _lastShaderPassNamePtr = 0;

    // beginRenderPass/endRenderPass 間にバインドされるコンテキスト。
    // draw 系メソッドがリソース引数の Residency `ensure()` を呼ぶために使用する。
    private _boundCtx: GraphicsContext | null = null;

    /**
     * レンダーパスを開始する。
     * `renderTarget` / `depthBuffer` が未設定の場合、バックバッファが使用されます。
     * カメラが有効な場合、カメラデータを View UBO に自動アップロードします。
     *
     * @param ctx    この Renderer を所有する GraphicsContext。
     * @param desc   レンダーパスディスクリプタ (カラーアタッチメント、デプス、クリア値など)。
     * @param camera カメラ。
     */
    beginRenderPass(ctx: GraphicsContext, desc: RenderPassDesc, camera: Camera): void {
        this._boundCtx = ctx;
        const ptr = this._serializeDesc(desc);
        try {
            Runtime.safeCall(() =>
                (API.LNRenderer_BeginRenderPass as (
                    r: number, ctx: number, d: number, cam: number,
                ) => number)(this._handle, ctx.handle, ptr, camera.handle));
        } finally {
            if (this._lastShaderPassNamePtr) {
                Runtime.module._free(this._lastShaderPassNamePtr);
                this._lastShaderPassNamePtr = 0;
            }
        }
    }

    /** 現在のレンダーパスを終了する。 */
    endRenderPass(): void {
        Runtime.safeCall(() =>
            (API.LNRenderer_EndRenderPass as (r: number) => number)(this._handle));
        this._boundCtx = null;
    }

    /**
     * メッシュの描画コマンドを内部バッファに蘊積する。
     * 蘊積されたコマンドは `endRenderPass()` 時に自動的にソート・バッチ化・描画されます。
     * メッシュに設定されたマテリアルが使用されます。
     *
     * @param mesh      描画する Mesh。
     * @param transform TRS トランスフォーム。
     * @param zIndex    ソート優先度 (デフォルト 0)。
     */
    drawMesh(mesh: Mesh, transform: Transform, zIndex = 0): void {
        if (!this._boundCtx) throw new Error("Renderer.drawMesh called outside of a render pass");
        mesh.ensure(this._boundCtx);
        if (mesh.handle === 0) return;
        const ptr = this._serializeTransform(transform);
        Runtime.safeCall(() =>
            (API.LNRenderer_DrawMesh as (
                r: number, mesh: number, t: number, z: number,
            ) => number)(this._handle, mesh.handle, ptr, zIndex));
    }

    /**
     * ステンシルマスクをプッシュする。
     * mesh をステンシルバッファにのみ描画し（カラー書き込みなし）、以降の描画はマスク領域内のみ出力される。
     * ネスト可 (内部で stencil 参照値をインクリメント)。popStencilMask と対で呼ぶこと。
     *
     * レンダーパスの depthStencil にステンシル付きバッファが必要。
     *
     * @param mesh      マスク形状のメッシュ (アルファマスクテクスチャ付き)。
     * @param transform マスクメッシュの TRS。
     * @param material  マスク描画用マテリアル (StencilMask シェーダ推奨)。
     */
    pushStencilMask(mesh: Mesh, transform: Transform, material: Material): void {
        if (!this._boundCtx) throw new Error("Renderer.pushStencilMask called outside of a render pass");
        mesh.ensure(this._boundCtx);
        material.ensure(this._boundCtx);
        if (mesh.handle === 0 || material.handle === 0) return;
        const ptr = this._serializeTransform(transform);
        Runtime.safeCall(() =>
            (API.LNRenderer_PushStencilMask as (
                r: number, mesh: number, t: number, mat: number,
            ) => number)(this._handle, mesh.handle, ptr, material.handle));
    }

    /**
     * 直前の `pushStencilMask()` と対になるマスクを解除する。
     * 内部でマスクメッシュを再描画してステンシル値をデクリメントします。
     */
    popStencilMask(): void {
        if (!this._boundCtx) throw new Error("Renderer.popStencilMask called outside of a render pass");
        Runtime.safeCall(() =>
            (API.LNRenderer_PopStencilMask as (r: number) => number)(this._handle));
    }

    /**
     * スプライトの描画コマンドを内部バッファに蘊積する (バッチ処理)。
     * 蘊積されたコマンドは `endRenderPass()` 時に自動的にソート・バッチ化・描画されます。
     *
     * @note `drawMesh()` と比べ、同一マテリアルで多数のスプライトを描画する場合は大幅に高速です。
     * 内部でバッチングを行いドローコール数を削減するため、タイルマップなど大量のスプライト描画に適しています。
     *
     * @param material  使用する Material。
     * @param zIndex    ソート優先度。
     * @param posX      X 座標。
     * @param posY      Y 座標。
     * @param posZ      Z 座標。
     * @param sizeW     スプライト幅。
     * @param sizeH     スプライト高さ。
     * @param pivotX    矩形上の基準点 X (0.0〜1.0)。0=左, 0.5=中央, 1=右。posX がこの位置に一致し、回転軸にもなる。
     * @param pivotY    矩形上の基準点 Y (0.0〜1.0)。0=上, 0.5=中央, 1=下。posY がこの位置に一致し、回転軸にもなる。
     * @param uvX       UV 矩形の X。
     * @param uvY       UV 矩形の Y。
     * @param uvW       UV 矩形の幅。
     * @param uvH       UV 矩形の高さ。
     * @param colorR    頂点カラー R。
     * @param colorG    頂点カラー G。
     * @param colorB    頂点カラー B。
     * @param colorA    頂点カラー A。
     * @param rotation  Z 軸回転 (ラジアン)。
     */
    drawSprite(
        material: Material, zIndex: number,
        posX: number, posY: number, posZ: number,
        sizeW: number, sizeH: number,
        pivotX: number, pivotY: number,
        uvX: number, uvY: number, uvW: number, uvH: number,
        colorR: number, colorG: number, colorB: number, colorA: number,
        rotation: number,
    ): void {
        if (!this._boundCtx) throw new Error("Renderer.drawSprite called outside of a render pass");
        material.ensure(this._boundCtx);
        if (material.handle === 0) return;
        Runtime.safeCall(() =>
            (API.LNRenderer_DrawSprite as (
                r: number, mat: number, z: number,
                px: number, py: number, pz: number,
                sw: number, sh: number,
                pvx: number, pvy: number,
                ux: number, uy: number, uw: number, uh: number,
                cr: number, cg: number, cb: number, ca: number,
                rot: number,
            ) => number)(
                this._handle, material.handle, zIndex,
                posX, posY, posZ,
                sizeW, sizeH,
                pivotX, pivotY,
                uvX, uvY, uvW, uvH,
                colorR, colorG, colorB, colorA,
                rotation,
            ));
    }

    /**
     * 指定したマテリアルでフルスクリーン矩形を描画する (ポストプロセス用)。
     * NDC [-1,1]×[-1,1] をカバーする矩形を描画します。
     * @param material 使用するマテリアル。
     */
    drawScreenRect(material: Material): void {
        if (!this._boundCtx) throw new Error("Renderer.drawScreenRect called outside of a render pass");
        material.ensure(this._boundCtx);
        if (material.handle === 0) return;
        Runtime.safeCall(() =>
            (API.LNRenderer_DrawScreenRect as (
                r: number, mat: number,
            ) => number)(this._handle, material.handle));
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
    // Private: RenderPassDesc を事前確保した WASM メモリへシリアライズする
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
     * `Transform` を WASM 線形メモリに書き込み、そのポインタを返す。
     *
     * C レイアウト (wasm32, 40 バイト):
     * ```
     * offset 0:  float posX, posY, posZ        (12 バイト)
     * offset 12: float rotX, rotY, rotZ, rotW  (16 バイト)
     * offset 28: float scaleX, scaleY, scaleZ  (12 バイト)
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
     * `RenderPassDesc` を WASM 線形メモリに書き込み、そのポインタを返す。
     *
     * C レイアウト (wasm32, 4 バイトアライン、合計 216 バイト):
     * ```
     * offset 0:   uint32_t colorAttachmentCount
     * offset 4:   LNColorAttachmentDesc colorAttachments[8]  (各 24 バイト)
     *   アタッチメントごと:
     *     +0  uint32_t renderTarget
     *     +4  float    clearColor[4]
     *     +20 uint32_t loadOp
     * offset 196: LNDepthStencilAttachmentDesc depthStencil  (20 バイト)
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

        // 構造体全体をゼロクリアする (安全なデフォルト値: 全ゼロ = CLEAR、
        // renderTarget=NULL_HANDLE、clearDepth は後で設定する)。
        const bytes = new Uint8Array(v.buffer, this._descPtr, SIZEOF_RENDER_PASS_DESC);
        bytes.fill(0);

        // --- カラーアタッチメント ---
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

        // --- デプス/ステンシル ---
        const dsBase = 4 + LN_MAX_COLOR_ATTACHMENTS * SIZEOF_COLOR_ATTACHMENT_DESC; // 196
        const ds = desc.depthStencil;
        v.setUint32(dsBase + 0,  ds?.depthBuffer   ?? LN_NULL_HANDLE, true);
        v.setFloat32(dsBase + 4, ds?.clearDepth     ?? 1.0, true);
        v.setUint32(dsBase + 8,  ds?.clearStencil   ?? 0, true);
        v.setUint32(dsBase + 12, ds?.depthLoadOp    ?? LoadOp.Clear, true);
        v.setUint32(dsBase + 16, ds?.stencilLoadOp  ?? LoadOp.Clear, true);

        // --- shaderPassName (offset 216 の const char*) ---
        if (desc.shaderPassName) {
            const encoder = new TextEncoder();
            const encoded = encoder.encode(desc.shaderPassName);
            const strPtr = Runtime.module._malloc(encoded.length + 1);
            Runtime.module.HEAPU8.set(encoded, strPtr);
            Runtime.module.HEAPU8[strPtr + encoded.length] = 0; // ヌル終端
            v.setUint32(216, strPtr, true);
            this._lastShaderPassNamePtr = strPtr;
        } else {
            v.setUint32(216, 0, true);
            this._lastShaderPassNamePtr = 0;
        }

        return this._descPtr;
    }
}
