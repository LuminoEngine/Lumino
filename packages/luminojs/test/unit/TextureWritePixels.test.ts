import { beforeEach, describe, expect, it } from "vitest";
import { GraphicsContext } from "../../src/GraphicsContext";
import { Texture } from "../../src/Texture";
import { Material } from "../../src/Material";
import { API, Runtime } from "../../src/Runtime";
import { Result } from "../../src/types";

// Texture.writePixels の JS 層の責務 (保持データへの反映、書き込み範囲の合成、ensure 時の一括アップロード) を、
// WASM モジュールをモックしてテストする。GPU への書き込みそのものは gtest (Test_Graphics.cpp) で検証する。

//------------------------------------------------------------------------------
// フェイク WASM モジュール
//------------------------------------------------------------------------------

const HEAP_SIZE = 4096;

function createFakeModule() {
    const buffer = new ArrayBuffer(HEAP_SIZE);
    return {
        buffer,
        nextPtr: 16, // 先頭は Runtime._returnPtr (0) 用に空けておく
        _malloc(size: number): number {
            const p = this.nextPtr;
            this.nextPtr += (size + 7) & ~7;
            return p;
        },
        _free(): void {},
        HEAPU8: new Uint8Array(buffer),
        HEAPU32: new Uint32Array(buffer),
        HEAPF32: new Float32Array(buffer),
        UTF8ToString(): string { return ""; },
        cwrap(): never { throw new Error("not supported in fake module"); },
    };
}

interface WriteCall {
    tex: number;
    x: number;
    y: number;
    width: number;
    height: number;
    pixels: number[];
}

//------------------------------------------------------------------------------
// テスト本体
//------------------------------------------------------------------------------

describe("Texture.writePixels", () => {
    let fake: ReturnType<typeof createFakeModule>;
    let createCount: number;
    let writeCalls: WriteCall[];
    let ctx: GraphicsContext;
    let bindCalls: number[];

    beforeEach(() => {
        fake = createFakeModule();
        (Runtime as unknown as { module: unknown }).module = fake;
        createCount = 0;
        writeCalls = [];

        API.LNTexture2D_CreateFromPixels = ((
            _ctx: number, _w: number, _h: number, _fmt: number, _pix: number, _size: number, out: number,
        ): number => {
            createCount++;
            new DataView(fake.buffer).setUint32(out, 0x00060000 + createCount, true);
            return Result.OK;
        }) as never;
        API.LNTexture2D_WritePixels = ((
            _ctx: number, tex: number, x: number, y: number, width: number, height: number,
            pix: number, size: number,
        ): number => {
            writeCalls.push({ tex, x, y, width, height, pixels: Array.from(fake.HEAPU8.subarray(pix, pix + size)) });
            return Result.OK;
        }) as never;
        API.LNObject_Release = ((): number => Result.OK) as never;
        API.LNMaterial_CreateFromBuiltinShader = ((_ctx: number, _shader: number, out: number): number => {
            new DataView(fake.buffer).setUint32(out, 0x00070001, true);
            return Result.OK;
        }) as never;
        bindCalls = [];
        API.LNMaterial_SetMainTexture = ((_mat: number, tex: number): number => {
            bindCalls.push(tex);
            return Result.OK;
        }) as never;

        ctx = new GraphicsContext();
        ctx._setHandle(0x00050001, false);
    });

    /** 1 ピクセル 4 バイトの w x h テクスチャ。各ピクセルの R にインデックスを入れておく。 */
    function createTexture(w: number, h: number): { texture: Texture; data: Uint8Array } {
        const data = new Uint8Array(w * h * 4);
        for (let i = 0; i < w * h; i++) data[i * 4] = i;
        return { texture: Texture.createFromPixels(data, w, h), data };
    }

    /** 全チャンネルを value で埋めた w x h のピクセル列。 */
    function fill(w: number, h: number, value: number): Uint8Array {
        return new Uint8Array(w * h * 4).fill(value);
    }

    it("保持しているピクセルデータの矩形領域へ行ごとに反映する", () => {
        const { texture, data } = createTexture(3, 2);
        texture.writePixels(1, 0, 2, 2, fill(2, 2, 200));

        const r = (x: number, y: number) => data[(y * 3 + x) * 4];
        expect([r(0, 0), r(1, 0), r(2, 0)]).toEqual([0, 200, 200]);
        expect([r(0, 1), r(1, 1), r(2, 1)]).toEqual([3, 200, 200]);
    });

    it("GPU 側が作られる前の書き込みは、作成時のアップロードに含まれ WritePixels を呼ばない", () => {
        const { texture } = createTexture(2, 2);
        texture.writePixels(0, 0, 1, 1, fill(1, 1, 9));
        texture.ensure(ctx);
        expect(createCount).toBe(1);
        expect(writeCalls).toHaveLength(0);
    });

    it("作成後の書き込みはハンドルを作り直さず、次の ensure でまとめて 1 回だけ書き込む", () => {
        const { texture } = createTexture(4, 4);
        texture.ensure(ctx);
        const handle = texture.handle;

        texture.writePixels(1, 1, 1, 1, fill(1, 1, 10));
        texture.writePixels(2, 2, 1, 1, fill(1, 1, 20));
        expect(writeCalls).toHaveLength(0); // 書き込み時点では GPU に触れない

        texture.ensure(ctx);
        expect(createCount).toBe(1);
        expect(texture.handle).toBe(handle);
        expect(writeCalls).toHaveLength(1);
        const call = writeCalls[0];
        expect([call.tex, call.x, call.y, call.width, call.height]).toEqual([handle, 1, 1, 2, 2]);
        // 2 つの書き込みを囲む矩形を、行を詰めて送っている。
        const red = [0, 1, 2, 3].map((i) => call.pixels[i * 4]);
        expect(red).toEqual([10, 6, 9, 20]);

        texture.ensure(ctx);
        expect(writeCalls).toHaveLength(1); // 反映済みなら再度は書き込まない
    });

    it("evict 後の書き込みは作り直しに含まれ WritePixels を呼ばない", () => {
        const { texture } = createTexture(2, 2);
        texture.ensure(ctx);
        texture.writePixels(0, 0, 1, 1, fill(1, 1, 5));
        texture.evict();
        texture.ensure(ctx);
        expect(createCount).toBe(2);
        expect(writeCalls).toHaveLength(0);
    });

    it("範囲外の矩形とデータサイズの不一致は例外になる", () => {
        const { texture } = createTexture(2, 2);
        expect(() => texture.writePixels(1, 0, 2, 1, fill(2, 1, 0))).toThrow(/out of texture bounds/);
        expect(() => texture.writePixels(0, 0, 0, 1, new Uint8Array(0))).toThrow(/out of texture bounds/);
        expect(() => texture.writePixels(0, 0, 2, 2, fill(1, 1, 0))).toThrow(/size mismatch/);
    });

    it("Material が参照するテクスチャの書き込みは、パラメータ変更が無くても Material の ensure でアップロードされる", () => {
        const { texture } = createTexture(2, 2);
        const material = Material.createUnlit();
        material.setMainTexture(texture);
        material.ensure(ctx);

        texture.writePixels(0, 0, 1, 1, fill(1, 1, 7));
        material.ensure(ctx);
        expect(writeCalls).toHaveLength(1);
        expect(bindCalls).toHaveLength(1); // ハンドルは変わらないので再バインドしない
    });

    it("Material が参照するテクスチャが evict されたら、作り直したハンドルをバインドし直す", () => {
        const { texture } = createTexture(2, 2);
        const material = Material.createUnlit();
        material.setMainTexture(texture);
        material.ensure(ctx);

        texture.evict();
        material.ensure(ctx);
        expect(createCount).toBe(2);
        expect(bindCalls).toEqual([0x00060001, 0x00060002]);
    });
});
