import { beforeEach, describe, expect, it } from "vitest";
import { GraphicsContext } from "../../src/GraphicsContext";
import { API, Runtime } from "../../src/Runtime";
import { Result, TextureFormat } from "../../src/types";
import { createFakeModule, type FakeModule } from "./fakeModule";

// GraphicsContext の一時レンダーターゲットのプール (acquire / release / beginFrame での破棄) を、
// WASM モジュールをモックしてテストする。

describe("GraphicsContext temporary render targets", () => {
    let fake: FakeModule;
    let createdHandles: number[];
    let releasedHandles: number[];
    let nextHandle: number;

    beforeEach(() => {
        fake = createFakeModule();
        (Runtime as unknown as { module: unknown }).module = fake;
        createdHandles = [];
        releasedHandles = [];
        nextHandle = 0x00020001;

        API.LNGraphicsContext_BeginFrame = ((
            _ctx: number, _w: number, _h: number, outRenderer: number,
        ): number => {
            new DataView(fake.buffer).setUint32(outRenderer, 0x00010001, true);
            return Result.OK;
        }) as never;
        API.LNObject_Release = ((h: number): number => {
            releasedHandles.push(h);
            return Result.OK;
        }) as never;
        API.LNTexture2D_CreateRenderTargetEx = ((
            _ctx: number, _w: number, _h: number, _fmt: number, out: number,
        ): number => {
            const handle = nextHandle++;
            createdHandles.push(handle);
            new DataView(fake.buffer).setUint32(out, handle, true);
            return Result.OK;
        }) as never;
    });

    function createContext(): GraphicsContext {
        const ctx = new GraphicsContext();
        ctx._setHandle(0x00050001, false);
        return ctx;
    }

    it("返却したものは同じサイズとフォーマットの acquire で再利用される", () => {
        const ctx = createContext();
        const a = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        ctx.releaseTemporaryRenderTarget(a);
        const b = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        expect(b).toBe(a);
        expect(createdHandles).toHaveLength(1);
    });

    it("貸し出し中のもの、サイズやフォーマットが違うものは再利用しない", () => {
        const ctx = createContext();
        const a = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        const b = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        ctx.releaseTemporaryRenderTarget(a);
        ctx.releaseTemporaryRenderTarget(b);
        const c = ctx.acquireTemporaryRenderTarget(32, 32, TextureFormat.RGBA8_UNORM);
        const d = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA16_FLOAT);
        expect(new Set([a, b, c, d]).size).toBe(4);
        expect(createdHandles).toHaveLength(4);
    });

    it("返却されたまま 60 フレーム使われなかったものは beginFrame で破棄される", () => {
        const ctx = createContext();
        const rt = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        const handle = rt.handle;
        ctx.releaseTemporaryRenderTarget(rt);

        for (let i = 0; i < 60; i++) ctx.beginFrame();
        expect(releasedHandles).not.toContain(handle);
        ctx.beginFrame();
        expect(releasedHandles).toContain(handle);

        // 破棄した後の acquire は新しく作る。
        expect(ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM)).not.toBe(rt);
        expect(createdHandles).toHaveLength(2);
    });

    it("貸し出し中のものは何フレーム経っても破棄されない", () => {
        const ctx = createContext();
        const rt = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        for (let i = 0; i < 100; i++) ctx.beginFrame();
        expect(releasedHandles).not.toContain(rt.handle);
    });

    it("借りていないテクスチャや二重の返却は例外になる", () => {
        const ctx = createContext();
        const rt = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        ctx.releaseTemporaryRenderTarget(rt);
        expect(() => ctx.releaseTemporaryRenderTarget(rt)).toThrow();
    });

    it("dispose で貸し出し中のものも含めてすべて解放される", () => {
        const ctx = createContext();
        const a = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        const b = ctx.acquireTemporaryRenderTarget(64, 32, TextureFormat.RGBA8_UNORM);
        ctx.releaseTemporaryRenderTarget(b);
        const handles = [a.handle, b.handle];
        ctx.dispose();
        expect(releasedHandles).toEqual(expect.arrayContaining(handles));
    });
});
