import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { Result, TextureFormat } from "./types";

type TextureSource =
    | { kind: "pixels"; data: Uint8Array; width: number; height: number; format: TextureFormat }
    | { kind: "external" };  // RenderTarget / DepthStencil - not a Residency target.

export class Texture extends LuminoObject implements ResidentResource {
    private _source: TextureSource = { kind: "external" };
    private _width = 0;
    private _height = 0;
    private _dirty = false;
    private _lastUsedFrame = 0;
    private _isResidencyTarget = false;

    get lastUsedFrame(): number { return this._lastUsedFrame; }
    get width(): number { return this._width; }
    get height(): number { return this._height; }

    /**
     * Create a render target texture with a specific format.
     * 指定フォーマットのレンダーターゲットテクスチャを作成します。
     * Sampled かつ RenderTarget として使用できます。(Residency 対象外、ctx 必須)
     * @param ctx    GraphicsContext
     * @param width  幅 (ピクセル)
     * @param height 高さ (ピクセル)
     * @param format テクスチャフォーマット
     */
    static createRenderTargetEx(ctx: GraphicsContext, width: number, height: number, format: TextureFormat): Texture {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNTexture2D_CreateRenderTargetEx as (
                ctx: number, w: number, h: number, fmt: number, out: number,
            ) => number)(ctx.handle, width, height, format, out));
        const tex = new Texture();
        tex._setHandle(handle, true);
        tex._width = width;
        tex._height = height;
        tex._source = { kind: "external" };
        return tex;
    }

    /**
     * Create a depth-stencil texture.
     * 深度ステンシルテクスチャを作成します。
     * 作成されたテクスチャは `DepthStencilAttachmentDesc.depthBuffer` に指定して使用します。(Residency 対象外、ctx 必須)
     * @param ctx    GraphicsContext
     * @param width  幅 (ピクセル)
     * @param height 高さ (ピクセル)
     */
    static createDepthStencil(ctx: GraphicsContext, width: number, height: number): Texture {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNTexture2D_CreateDepthStencil as (
                ctx: number, w: number, h: number, out: number,
            ) => number)(ctx.handle, width, height, out));
        const tex = new Texture();
        tex._setHandle(handle, true);
        tex._width = width;
        tex._height = height;
        tex._source = { kind: "external" };
        return tex;
    }

    /**
     * 画像ファイルのバイト列 (PNG, JPG, BMP, TGA 等) からテクスチャを定義する。
     * 呼び出し時点で画像をデコードし、幅・高さが即座に参照可能になる。
     * GPU アップロードは最初の描画時に遅延実行される。
     * @param data 画像ファイルのバイト列
     */
    static loadFromMemory(data: Uint8Array): Texture {
        const m = Runtime.module;
        const inPtr = m._malloc(data.byteLength);
        // out パラメータ用の一時バッファ (uint32 x 4 = 16 bytes)
        const outPtr = m._malloc(16);
        try {
            m.HEAPU8.set(data, inPtr);
            const rc = (API.LNImage_DecodeFromMemory as (
                data: number, size: number,
                outW: number, outH: number, outPix: number, outPixSize: number,
            ) => number)(inPtr, data.byteLength,
                outPtr, outPtr + 4, outPtr + 8, outPtr + 12);
            if (rc !== Result.OK) throw new Error(`LNImage_DecodeFromMemory failed: ${rc}`);

            const view = new Uint32Array(m.HEAPU32.buffer, outPtr, 4);
            const width = view[0];
            const height = view[1];
            const pixelsPtr = view[2];
            const pixelsSize = view[3];

            // デコード済みピクセルを JS 側にコピー (WASM メモリ節約)
            const pixels = new Uint8Array(pixelsSize);
            pixels.set(m.HEAPU8.subarray(pixelsPtr, pixelsPtr + pixelsSize));

            // C++ 側のデコードバッファを解放
            (API.LNImage_FreePixels as (p: number) => number)(pixelsPtr);

            return Texture.createFromPixels(pixels, width, height, TextureFormat.RGBA8_UNORM);
        } finally {
            m._free(outPtr);
            m._free(inPtr);
        }
    }

    /**
     * Define a texture from decoded pixel data (e.g., from createImageBitmap).
     * デコード済みの生ピクセルデータからテクスチャを定義します。
     * @param data   生ピクセルデータ (format で指定されたフォーマットに従う)
     * @param width  幅 (ピクセル)
     * @param height 高さ (ピクセル)
     * @param format テクスチャフォーマット (デフォルト: `TextureFormat.RGBA8_UNORM`)
     */
    static createFromPixels(
        data: Uint8Array,
        width: number,
        height: number,
        format: TextureFormat = TextureFormat.RGBA8_UNORM,
    ): Texture {
        const tex = new Texture();
        tex._source = { kind: "pixels", data, width, height, format };
        tex._width = width;
        tex._height = height;
        tex._dirty = true;
        tex._isResidencyTarget = true;
        return tex;
    }

    /** Fetch an image from a URL and define a texture. */
    static async loadFromURL(url: string): Promise<Texture> {
        const resp = await fetch(url);
        if (!resp.ok) throw new Error(`Failed to fetch texture: ${resp.status} ${url}`);
        const buf = await resp.arrayBuffer();
        return Texture.loadFromMemory(new Uint8Array(buf));
    }

    /**
     * @internal Called by Renderer / Material during draw time to guarantee the
     * GPU resource exists. Safe to call repeatedly; only uploads when needed.
     */
    ensure(ctx: GraphicsContext): void {
        if (!this._isResidencyTarget) return;
        if (this._handle !== 0 && !this._dirty) {
            this._lastUsedFrame = ctx.currentFrame;
            return;
        }

        if (this._handle !== 0) {
            // Dirty re-upload: release the old handle first.
            (API.LNObject_Release as (h: number) => number)(this._handle);
            this._handle = 0;
        }

        const m = Runtime.module;
        if (this._source.kind === "pixels") {
            const src = this._source;
            const ptr = m._malloc(src.data.byteLength);
            try {
                m.HEAPU8.set(src.data, ptr);
                const handle = Runtime.safeCallWithReturnHandle((out) =>
                    (API.LNTexture2D_CreateFromPixels as (
                        ctx: number, w: number, h: number, fmt: number,
                        pix: number, size: number, out: number,
                    ) => number)(ctx.handle, src.width, src.height, src.format,
                        ptr, src.data.byteLength, out));
                this._setHandle(handle, true);
            } finally {
                m._free(ptr);
            }
        }

        this._dirty = false;
        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /**
     * @internal Release the GPU resource while keeping source data.
     * Called by ResidencyManager on GC, and by dispose().
     */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
    }

    override dispose(): void {
        if (this._isResidencyTarget) {
            this.evict();
            this._source = { kind: "external" };
            this._isResidencyTarget = false;
        } else {
            super.dispose();
        }
    }
}
