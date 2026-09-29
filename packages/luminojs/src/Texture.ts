import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { Result, TextureFormat } from "./types";

type TextureSource =
    | { kind: "pixels"; data: Uint8Array; width: number; height: number; format: TextureFormat }
    | { kind: "rt"; format: TextureFormat }  // RenderTarget: 生成情報のみ保持 (内容は揮発)
    | { kind: "ds" }                         // DepthStencil: 生成情報のみ保持 (内容は揮発)
    | { kind: "external" };                  // その他 (Residency / 復旧の対象外)

/** writePixels で書き込まれ、まだ GPU へ反映していない範囲 (右端・下端は含まない)。 */
type PendingRegion = { left: number; top: number; right: number; bottom: number };

/** 1 ピクセルあたりのバイト数。 */
function bytesPerPixel(format: TextureFormat): number {
    switch (format) {
        case TextureFormat.R8_UNORM:     return 1;
        case TextureFormat.RG8_UNORM:    return 2;
        case TextureFormat.RGBA16_FLOAT: return 8;
        case TextureFormat.RGBA32_FLOAT: return 16;
        default:                         return 4;
    }
}

export class Texture extends LuminoObject implements ResidentResource {
    private _source: TextureSource = { kind: "external" };
    private _width = 0;
    private _height = 0;
    private _dirty = false;
    private _pendingRegion: PendingRegion | null = null;
    private _lastUsedFrame = 0;
    private _isResidencyTarget = false;
    // RT / DS の場合のみ: デバイスロスト復旧時の再作成用 (non-owning)
    private _externalCtx: GraphicsContext | null = null;

    get lastUsedFrame(): number { return this._lastUsedFrame; }
    get width(): number { return this._width; }
    get height(): number { return this._height; }

    /**
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
        tex._source = { kind: "rt", format };
        tex._externalCtx = ctx;
        ctx._registerExternalTexture(tex);
        return tex;
    }

    /**
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
        tex._source = { kind: "ds" };
        tex._externalCtx = ctx;
        ctx._registerExternalTexture(tex);
        return tex;
    }

    /**
     * 画像ファイルのバイト列 (PNG, JPG, BMP, TGA 等) からテクスチャを定義する。
     * 呼び出し時点で画像をデコードし、幅と高さが即座に参照可能になる。
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
     * デコード済みの生ピクセルデータ (createImageBitmap の結果など) からテクスチャを定義します。
     *
     * data はコピーせずにテクスチャが所有します。GPU へのアップロードやデバイスロストからの復旧に使うためです。
     * 呼び出し後は data の内容を書き換えないでください。また writePixels はこの配列へ書き込みます。
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

    /**
     * テクスチャの矩形領域へピクセルデータを書き込みます。引数の並びは WebGPU の queue.writeTexture に合わせています。
     * createFromPixels / loadFromMemory / loadFromURL で作成したテクスチャが対象です。
     *
     * 書き込みは保持しているピクセルデータへ反映し、GPU へは次に描画で使われたときにまとめてアップロードします。
     * 1 フレームに何度書き込んでも、アップロードは書き込んだ範囲を囲む矩形の 1 回だけです。
     * data はコピーされるため、呼び出し後に再利用して構いません。
     * @param x      書き込み先矩形の左端 (ピクセル)
     * @param y      書き込み先矩形の上端 (ピクセル)
     * @param width  書き込み先矩形の幅 (ピクセル)
     * @param height 書き込み先矩形の高さ (ピクセル)
     * @param data   ピクセルデータ (テクスチャのフォーマットに従い、行は上から下へ詰めて並べる)
     */
    writePixels(x: number, y: number, width: number, height: number, data: Uint8Array): void {
        const src = this._source;
        if (src.kind !== "pixels") {
            throw new Error("writePixels is only supported for textures created from pixels or images.");
        }
        if (width <= 0 || height <= 0 || x < 0 || y < 0 || x + width > src.width || y + height > src.height) {
            throw new Error(
                `Write region (${x}, ${y}, ${width}, ${height}) is out of texture bounds (${src.width} x ${src.height}).`);
        }
        const bpp = bytesPerPixel(src.format);
        const rowBytes = width * bpp;
        if (data.byteLength !== rowBytes * height) {
            throw new Error(`Pixel data size mismatch. (expected: ${rowBytes * height}, actual: ${data.byteLength})`);
        }

        // 保持しているピクセルデータへ行ごとに反映する。
        const stride = src.width * bpp;
        if (x === 0 && width === src.width) {
            src.data.set(data, y * stride);
        } else {
            for (let row = 0; row < height; row++) {
                src.data.set(data.subarray(row * rowBytes, (row + 1) * rowBytes), (y + row) * stride + x * bpp);
            }
        }

        // まだ GPU 側が無い場合は、次の ensure で保持データから作られるので範囲を記録しなくてよい。
        if (this._handle === 0) return;
        const r = this._pendingRegion;
        this._pendingRegion = r
            ? {
                left: Math.min(r.left, x),
                top: Math.min(r.top, y),
                right: Math.max(r.right, x + width),
                bottom: Math.max(r.bottom, y + height),
            }
            : { left: x, top: y, right: x + width, bottom: y + height };
    }

    /** URL から画像を取得してテクスチャを定義します。 */
    static async loadFromURL(url: string): Promise<Texture> {
        const resp = await fetch(url);
        if (!resp.ok) throw new Error(`Failed to fetch texture: ${resp.status} ${url}`);
        const buf = await resp.arrayBuffer();
        return Texture.loadFromMemory(new Uint8Array(buf));
    }

    /**
     * @internal 描画時に Renderer / Material から呼ばれ、GPU リソースの存在を保証する。
     * 繰り返し呼んでも安全で、必要なときだけアップロードする。
     */
    ensure(ctx: GraphicsContext): void {
        if (!this._isResidencyTarget) return;
        if (this._handle !== 0 && !this._dirty) {
            if (this._pendingRegion) this._uploadPendingRegion(ctx);
            this._lastUsedFrame = ctx.currentFrame;
            return;
        }

        if (this._handle !== 0) {
            // dirty による再アップロード: 先に古いハンドルを解放する。
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
        this._pendingRegion = null;
        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /** writePixels で書き込まれた範囲を、既存の GPU テクスチャへ書き込む。 */
    private _uploadPendingRegion(ctx: GraphicsContext): void {
        const src = this._source;
        const r = this._pendingRegion!;
        this._pendingRegion = null;
        if (src.kind !== "pixels") return;

        const bpp = bytesPerPixel(src.format);
        const stride = src.width * bpp;
        const width = r.right - r.left;
        const height = r.bottom - r.top;
        const rowBytes = width * bpp;
        const size = rowBytes * height;

        const m = Runtime.module;
        const ptr = m._malloc(size);
        try {
            if (width === src.width) {
                m.HEAPU8.set(src.data.subarray(r.top * stride, r.bottom * stride), ptr);
            } else {
                for (let row = 0; row < height; row++) {
                    const begin = (r.top + row) * stride + r.left * bpp;
                    m.HEAPU8.set(src.data.subarray(begin, begin + rowBytes), ptr + row * rowBytes);
                }
            }
            Runtime.safeCall(() =>
                (API.LNTexture2D_WritePixels as (
                    ctx: number, tex: number, x: number, y: number, w: number, h: number,
                    pix: number, size: number,
                ) => number)(ctx.handle, this._handle, r.left, r.top, width, height, ptr, size));
        } finally {
            m._free(ptr);
        }
    }

    /**
     * @internal ソースデータは保持したまま GPU リソースを解放する。
     * ResidencyManager の GC と dispose() から呼ばれる。
     */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
    }

    /**
     * @internal デバイスロスト復旧後に GraphicsContext から呼ばれる。
     * RenderTarget / DepthStencil を生成情報から作り直す。内容はリセットされるため、
     * 必要であればアプリが GraphicsContext.onDeviceRestored で再レンダリングする。
     */
    _recreateExternal(ctx: GraphicsContext): void {
        if (this._source.kind !== "rt" && this._source.kind !== "ds") return;
        if (this._handle !== 0) {
            (API.LNObject_Release as (h: number) => number)(this._handle);
            this._handle = 0;
        }
        if (this._source.kind === "rt") {
            const format = this._source.format;
            const handle = Runtime.safeCallWithReturnHandle((out) =>
                (API.LNTexture2D_CreateRenderTargetEx as (
                    ctx: number, w: number, h: number, fmt: number, out: number,
                ) => number)(ctx.handle, this._width, this._height, format, out));
            this._setHandle(handle, true);
        } else {
            const handle = Runtime.safeCallWithReturnHandle((out) =>
                (API.LNTexture2D_CreateDepthStencil as (
                    ctx: number, w: number, h: number, out: number,
                ) => number)(ctx.handle, this._width, this._height, out));
            this._setHandle(handle, true);
        }
    }

    override dispose(): void {
        if (this._isResidencyTarget) {
            this.evict();
            this._source = { kind: "external" };
            this._isResidencyTarget = false;
        } else {
            if (this._externalCtx) {
                this._externalCtx._unregisterExternalTexture(this);
                this._externalCtx = null;
            }
            super.dispose();
        }
    }
}
