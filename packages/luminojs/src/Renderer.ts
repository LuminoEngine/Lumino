import { LuminoObject } from "./LuminoObject";
import type { Camera } from "./Camera";
import type { GraphicsContext } from "./GraphicsContext";
import type { Material } from "./Material";
import type { Mesh } from "./Mesh";
import type { Matrix4x4 } from "./Matrix4x4";
import { API, Runtime } from "./Runtime";
import {
    type RenderPassDesc,
    type Transform,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_TRANSFORM,
    SIZEOF_MATRIX,
} from "./types";
import { writeRenderPassDesc, writeTransform } from "./serialize";

export class Renderer extends LuminoObject {
    // LNRenderPassDesc 用の事前確保 WASM バッファ。
    // 初回使用時に遅延生成し、Renderer が dispose されるときに解放する。
    private _descPtr = 0;
    private _descView: DataView | null = null;
    private _descHeapBuf: ArrayBufferLike | null = null;

    // LNTransform 用の事前確保 WASM バッファ (40 バイト)。
    private _transformPtr = 0;
    private _transformView: DataView | null = null;
    private _transformHeapBuf: ArrayBufferLike | null = null;

    // LNMatrix 用の事前確保 WASM バッファ (64 バイト)。
    private _matrixPtr = 0;

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
     * メッシュの描画コマンドを内部バッファに蓄積する。
     * 蓄積されたコマンドは `endRenderPass()` 時に自動的にソート、バッチ化、描画されます。
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
     * スプライトの描画コマンドを内部バッファに蓄積する (バッチ処理)。
     * 蓄積されたコマンドは `endRenderPass()` 時に自動的にソート、バッチ化、描画されます。
     *
     * スプライトは size と pivot で定義されるローカル矩形を `offset` だけずらし、
     * `transform` でワールド空間へ配置して描画します
     * (world = transform * (localCorner + offset))。
     * 位置、回転、スケールは `transform` に畳み込んでください
     * (クライアント側のシーングラフから求めたワールド行列をそのまま渡せます)。
     *
     * `offset` は、Tilemap / Tiling Sprite / 9-Sliced Sprite のように 1 ノード内で多数の
     * スプライトを描く際、`transform` (= ノードのワールド行列) を 1 インスタンス共有しつつ、
     * タイルごとの位置だけを軽量に変えるためのオフセットです (行列を毎回生成せずに済みます)。
     *
     * @note `drawMesh()` と比べ、同一マテリアルで多数のスプライトを描画する場合は大幅に高速です。
     * 内部でバッチングを行いドローコール数を削減するため、タイルマップなど大量のスプライト描画に適しています。
     *
     * @param material  使用する Material。
     * @param zIndex    ソート優先度。
     * @param transform ワールド変換行列。
     * @param offsetX   ノードローカル空間でのスプライト位置 X (transform 適用前に加算)。矩形上の pivot 位置がここに来る。
     * @param offsetY   ノードローカル空間でのスプライト位置 Y (transform 適用前に加算)。矩形上の pivot 位置がここに来る。
     * @param sizeW     スプライト幅 (ローカル)。
     * @param sizeH     スプライト高さ (ローカル)。
     * @param pivotX    矩形上の基準点 X (0.0-1.0)。0=左, 0.5=中央, 1=右。offset 位置に一致し、回転軸にもなる。
     * @param pivotY    矩形上の基準点 Y (0.0-1.0)。0=上, 0.5=中央, 1=下。offset 位置に一致し、回転軸にもなる。
     * @param uvX       UV 矩形の X。
     * @param uvY       UV 矩形の Y。
     * @param uvW       UV 矩形の幅。
     * @param uvH       UV 矩形の高さ。
     * @param colorR    頂点カラー R。
     * @param colorG    頂点カラー G。
     * @param colorB    頂点カラー B。
     * @param colorA    頂点カラー A。
     */
    drawSprite(
        material: Material, zIndex: number,
        transform: Matrix4x4,
        offsetX: number, offsetY: number,
        sizeW: number, sizeH: number,
        pivotX: number, pivotY: number,
        uvX: number, uvY: number, uvW: number, uvH: number,
        colorR: number, colorG: number, colorB: number, colorA: number,
    ): void {
        if (!this._boundCtx) throw new Error("Renderer.drawSprite called outside of a render pass");
        material.ensure(this._boundCtx);
        if (material.handle === 0) return;
        const ptr = this._serializeMatrix(transform);
        Runtime.safeCall(() =>
            (API.LNRenderer_DrawSprite as (
                r: number, mat: number, z: number,
                t: number,
                ox: number, oy: number,
                sw: number, sh: number,
                pvx: number, pvy: number,
                ux: number, uy: number, uw: number, uh: number,
                cr: number, cg: number, cb: number, ca: number,
            ) => number)(
                this._handle, material.handle, zIndex,
                ptr,
                offsetX, offsetY,
                sizeW, sizeH,
                pivotX, pivotY,
                uvX, uvY, uvW, uvH,
                colorR, colorG, colorB, colorA,
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
        if (this._matrixPtr) {
            Runtime.module._free(this._matrixPtr);
            this._matrixPtr = 0;
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
     * `Matrix4x4` (列優先 float[16]) を WASM 線形メモリに書き込み、そのポインタを返す。
     */
    private _serializeMatrix(m: Matrix4x4): number {
        if (!this._matrixPtr) {
            this._matrixPtr = Runtime.module._malloc(SIZEOF_MATRIX);
        }
        // HEAPF32 はメモリ成長で差し替わるため、毎回最新を参照する。
        Runtime.module.HEAPF32.set(m.m, this._matrixPtr >> 2);
        return this._matrixPtr;
    }

    /**
     * `Transform` を WASM 線形メモリに書き込み、そのポインタを返す。
     * レイアウトとテストは `./serialize.ts` の `writeTransform` を参照。
     */
    private _serializeTransform(t: Transform): number {
        this._ensureTransformBuffer();
        writeTransform(this._transformView!, t);
        return this._transformPtr;
    }

    /**
     * `RenderPassDesc` を WASM 線形メモリに書き込み、そのポインタを返す。
     * レイアウトとテストは `./serialize.ts` の `writeRenderPassDesc` を参照。
     * shaderPassName の文字列は WASM ヒープへの確保が必要なため、ここで行う
     * (呼び出し後の解放は `beginRenderPass` の finally ブロックで行われる)。
     */
    private _serializeDesc(desc: RenderPassDesc): number {
        this._ensureDescBuffer();

        let strPtr = 0;
        if (desc.shaderPassName) {
            const encoder = new TextEncoder();
            const encoded = encoder.encode(desc.shaderPassName);
            strPtr = Runtime.module._malloc(encoded.length + 1);
            Runtime.module.HEAPU8.set(encoded, strPtr);
            Runtime.module.HEAPU8[strPtr + encoded.length] = 0; // ヌル終端
        }
        this._lastShaderPassNamePtr = strPtr;

        writeRenderPassDesc(this._descView!, desc, strPtr);
        return this._descPtr;
    }
}
