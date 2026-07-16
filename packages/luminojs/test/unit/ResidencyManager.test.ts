import { describe, expect, it } from "vitest";
import { ResidencyManager, type ResidentResource } from "../../src/ResidencyManager";

// ResidencyManager は WASM Runtime に依存しない (Handle は単なる number として
// 扱われるだけで、Runtime.module や cwrap は参照しない)。そのためモックの
// ResidentResource を実装するだけで、実際の WASM ロードなしにテストできる。
class MockResource implements ResidentResource {
    handle: number;
    lastUsedFrame: number;
    evictedCount = 0;

    constructor(handle: number, lastUsedFrame: number) {
        this.handle = handle;
        this.lastUsedFrame = lastUsedFrame;
    }

    evict(): void {
        this.evictedCount++;
        this.handle = 0;
    }
}

describe("ResidencyManager.gc", () => {
    it("しきい値 (デフォルト300) 以下のフレーム差では evict しない", () => {
        const rm = new ResidencyManager();
        const r = new MockResource(1, /* lastUsedFrame */ 100);
        rm.register(r);

        rm.gc(/* currentFrame */ 100 + rm.threshold); // 差はちょうど threshold (超過ではない)
        expect(r.evictedCount).toBe(0);
        expect(r.handle).toBe(1);
    });

    it("しきい値を超えたフレーム差で evict する", () => {
        const rm = new ResidencyManager();
        const r = new MockResource(1, 100);
        rm.register(r);

        rm.gc(100 + rm.threshold + 1); // 差が threshold を超える
        expect(r.evictedCount).toBe(1);
        expect(r.handle).toBe(0);
    });

    it("handle が 0 (未ロード) のリソースは evict 対象にならない", () => {
        const rm = new ResidencyManager();
        const r = new MockResource(0, 0);
        rm.register(r);

        rm.gc(100000);
        expect(r.evictedCount).toBe(0);
    });

    it("threshold はカスタマイズでき、gc の判定に反映される", () => {
        const rm = new ResidencyManager();
        rm.threshold = 10;
        const r = new MockResource(1, 0);
        rm.register(r);

        rm.gc(10); // 差 = 10、超過していない
        expect(r.evictedCount).toBe(0);

        rm.gc(11); // 差 = 11、超過
        expect(r.evictedCount).toBe(1);
    });

    it("unregister されたリソースは gc の対象から外れる", () => {
        const rm = new ResidencyManager();
        const r = new MockResource(1, 0);
        rm.register(r);
        rm.unregister(r);

        rm.gc(100000);
        expect(r.evictedCount).toBe(0);
    });

    it("複数リソースのうち、しきい値を超えたものだけを evict する", () => {
        const rm = new ResidencyManager();
        const old = new MockResource(1, 0);
        const fresh = new MockResource(2, 250);
        rm.register(old);
        rm.register(fresh);

        rm.gc(301); // old: 差301 (超過) / fresh: 差51 (超過せず)
        expect(old.evictedCount).toBe(1);
        expect(fresh.evictedCount).toBe(0);
    });
});

describe("ResidencyManager.disposeAll", () => {
    it("handle を持つ全リソースを evict し、追跡を空にする", () => {
        const rm = new ResidencyManager();
        const a = new MockResource(1, 0);
        const b = new MockResource(2, 0);
        const unloaded = new MockResource(0, 0);
        rm.register(a);
        rm.register(b);
        rm.register(unloaded);

        rm.disposeAll();

        expect(a.evictedCount).toBe(1);
        expect(b.evictedCount).toBe(1);
        expect(unloaded.evictedCount).toBe(0); // 未ロードなので evict 不要

        // 追跡が空になっているため、以降の gc は何もしない。
        a.evictedCount = 0;
        rm.gc(999999);
        expect(a.evictedCount).toBe(0);
    });
});
