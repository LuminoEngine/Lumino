import { beforeEach, describe, expect, it } from "vitest";
import { GraphicsContext } from "../../src/GraphicsContext";
import { Texture } from "../../src/Texture";
import { API, Runtime } from "../../src/Runtime";
import { Result, TextureFormat } from "../../src/types";
import type { ResidentResource } from "../../src/ResidencyManager";

// GraphicsContext.beginFrame のデバイスロスト復旧フローを、WASM モジュールを
// モックしてテストする。C++ 側の自動復旧そのものは gtest (Test_DeviceLost.cpp)
// で検証されるため、ここでは JS 層の責務 (null 返却、invalidateAll、RT 再作成、
// onDeviceRestored フック) のみを対象とする。

//------------------------------------------------------------------------------
// フェイク WASM モジュール
//------------------------------------------------------------------------------

const HEAP_SIZE = 4096;

interface FakeModule {
    buffer: ArrayBuffer;
    nextPtr: number;
    _malloc(size: number): number;
    _free(ptr: number): void;
    HEAPU8: Uint8Array;
    HEAPU32: Uint32Array;
    HEAPF32: Float32Array;
    UTF8ToString(ptr: number): string;
    cwrap(): never;
}

function createFakeModule(): FakeModule {
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

//------------------------------------------------------------------------------
// テスト本体
//------------------------------------------------------------------------------

describe("GraphicsContext device lost recovery", () => {
    let fake: FakeModule;
    /** LNGraphicsContext_BeginFrame が返す結果のキュー。空になったら LN_OK。 */
    let beginFrameResults: number[];
    let rendererHandle: number;
    let releasedHandles: number[];
    let createdRTHandles: number[];
    let nextRTHandle: number;

    beforeEach(() => {
        fake = createFakeModule();
        // Runtime.module を直接差し替える (initialize は呼ばない)
        (Runtime as unknown as { module: unknown }).module = fake;

        beginFrameResults = [];
        rendererHandle = 0x00010001;
        releasedHandles = [];
        createdRTHandles = [];
        nextRTHandle = 0x00020001;

        API.LNGraphicsContext_BeginFrame = ((
            _ctx: number, _w: number, _h: number,
            outRenderer: number, outColor: number, outDepth: number,
        ): number => {
            const rc = beginFrameResults.length > 0 ? beginFrameResults.shift()! : Result.OK;
            if (rc === Result.OK) {
                const view = new DataView(fake.buffer);
                view.setUint32(outRenderer, rendererHandle, true);
                view.setUint32(outColor, 0x00030001, true);
                view.setUint32(outDepth, 0x00040001, true);
            }
            return rc;
        }) as never;

        API.LNObject_Release = ((h: number): number => {
            releasedHandles.push(h);
            return Result.OK;
        }) as never;

        API.LNTexture2D_CreateRenderTargetEx = ((
            _ctx: number, _w: number, _h: number, _fmt: number, out: number,
        ): number => {
            const handle = nextRTHandle++;
            createdRTHandles.push(handle);
            new DataView(fake.buffer).setUint32(out, handle, true);
            return Result.OK;
        }) as never;
    });

    function createContext(): GraphicsContext {
        const ctx = new GraphicsContext();
        ctx._setHandle(0x00050001, false);
        return ctx;
    }

    it("デバイスロスト中の beginFrame は null を返し、フックはまだ呼ばれない", () => {
        const ctx = createContext();
        let restoredCount = 0;
        ctx.onDeviceRestored = () => restoredCount++;

        beginFrameResults = [Result.ERROR_DEVICE_LOST, Result.ERROR_DEVICE_LOST];
        expect(ctx.beginFrame()).toBeNull();
        expect(ctx.beginFrame()).toBeNull();
        expect(restoredCount).toBe(0);
    });

    it("復旧完了フレームで常駐リソースが evict され、onDeviceRestored が一度だけ呼ばれる", () => {
        const ctx = createContext();
        let restoredCount = 0;
        ctx.onDeviceRestored = () => restoredCount++;

        // 常駐リソース (Texture / Mesh / Material 相当) のモック
        const resource: ResidentResource & { evictedCount: number } = {
            handle: 123,
            lastUsedFrame: 0,
            evictedCount: 0,
            evict() {
                this.evictedCount++;
                this.handle = 0;
            },
        };
        ctx.residencyManager.register(resource);

        // ロスト → 復旧のシーケンス
        beginFrameResults = [Result.ERROR_DEVICE_LOST];
        expect(ctx.beginFrame()).toBeNull();

        const frame = ctx.beginFrame(); // 復旧完了 (LN_OK)
        expect(frame).not.toBeNull();
        expect(frame!.renderer.handle).toBe(rendererHandle);
        expect(resource.evictedCount).toBe(1);
        expect(restoredCount).toBe(1);

        // 以降の正常フレームではフックは呼ばれない
        expect(ctx.beginFrame()).not.toBeNull();
        expect(restoredCount).toBe(1);
    });

    it("復旧完了フレームで RenderTarget が生成情報から再作成される (内容はリセット)", () => {
        const ctx = createContext();

        const rt = Texture.createRenderTargetEx(ctx, 64, 64, TextureFormat.RGBA8_UNORM);
        const oldHandle = rt.handle;
        expect(createdRTHandles).toHaveLength(1);

        beginFrameResults = [Result.ERROR_DEVICE_LOST];
        expect(ctx.beginFrame()).toBeNull();
        expect(ctx.beginFrame()).not.toBeNull(); // 復旧完了

        // 旧ハンドルが解放され、新しいハンドルで作り直されている
        expect(releasedHandles).toContain(oldHandle);
        expect(createdRTHandles).toHaveLength(2);
        expect(rt.handle).toBe(createdRTHandles[1]);
        expect(rt.handle).not.toBe(oldHandle);

        // dispose すると復旧対象から外れる
        rt.dispose();
        beginFrameResults = [Result.ERROR_DEVICE_LOST];
        expect(ctx.beginFrame()).toBeNull();
        expect(ctx.beginFrame()).not.toBeNull();
        expect(createdRTHandles).toHaveLength(2); // 再作成されない
    });
});
