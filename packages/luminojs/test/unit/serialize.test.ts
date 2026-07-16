import { describe, expect, it } from "vitest";
import { writeRenderPassDesc, writeTransform } from "../../src/serialize";
import {
    LoadOp,
    SortMode,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_TRANSFORM,
    type RenderPassDesc,
    type Transform,
} from "../../src/types";

// writeRenderPassDesc / writeTransform は Renderer.ts の _serializeDesc /
// _serializeTransform から切り出した WASM 非依存の純関数。
// 実際の WASM ヒープの代わりにプレーンな ArrayBuffer を DataView で渡すだけで
// オフセット計算を検証できる (Runtime のモックは不要)。

function makeView(size: number): DataView {
    return new DataView(new ArrayBuffer(size));
}

describe("writeTransform", () => {
    it("position/rotation/scale を LNTransform のオフセット通りに書き込む", () => {
        const view = makeView(SIZEOF_TRANSFORM);
        const t: Transform = {
            position: [1, 2, 3],
            rotation: [4, 5, 6, 7],
            scale: [8, 9, 10],
        };
        writeTransform(view, t);

        expect(view.getFloat32(0, true)).toBe(1);
        expect(view.getFloat32(4, true)).toBe(2);
        expect(view.getFloat32(8, true)).toBe(3);
        expect(view.getFloat32(12, true)).toBe(4);
        expect(view.getFloat32(16, true)).toBe(5);
        expect(view.getFloat32(20, true)).toBe(6);
        expect(view.getFloat32(24, true)).toBe(7);
        expect(view.getFloat32(28, true)).toBe(8);
        expect(view.getFloat32(32, true)).toBe(9);
        expect(view.getFloat32(36, true)).toBe(10);
    });
});

describe("writeRenderPassDesc", () => {
    it("colorAttachments 未指定時はゼロクリアされ、count=0 になる", () => {
        const view = makeView(SIZEOF_RENDER_PASS_DESC);
        // 事前にゴミを書いておき、ゼロクリアされることを確認する。
        for (let i = 0; i < SIZEOF_RENDER_PASS_DESC; i++) view.setUint8(i, 0xff);

        writeRenderPassDesc(view, {}, 0);

        expect(view.getUint32(0, true)).toBe(0); // colorAttachmentCount
        expect(view.getUint32(196, true)).toBe(0); // depthStencil.depthBuffer
        expect(view.getFloat32(200, true)).toBe(1.0); // depthStencil.clearDepth デフォルト
        expect(view.getUint32(216, true)).toBe(0); // shaderPassNamePtr
        expect(view.getUint32(220, true)).toBe(0); // sortMode デフォルト = Stable
    });

    it("colorAttachments を offset 4 + i*24 に書き込む", () => {
        const view = makeView(SIZEOF_RENDER_PASS_DESC);
        const desc: RenderPassDesc = {
            colorAttachments: [
                { renderTarget: 7, clearColor: [0.1, 0.2, 0.3, 0.4], loadOp: LoadOp.Load },
                { renderTarget: 9 },
            ],
        };
        writeRenderPassDesc(view, desc, 0);

        expect(view.getUint32(0, true)).toBe(2);

        // アタッチメント0: base = 4
        expect(view.getUint32(4 + 0, true)).toBe(7);
        expect(view.getFloat32(4 + 4, true)).toBeCloseTo(0.1, 6);
        expect(view.getFloat32(4 + 8, true)).toBeCloseTo(0.2, 6);
        expect(view.getFloat32(4 + 12, true)).toBeCloseTo(0.3, 6);
        expect(view.getFloat32(4 + 16, true)).toBeCloseTo(0.4, 6);
        expect(view.getUint32(4 + 20, true)).toBe(LoadOp.Load);

        // アタッチメント1: base = 4 + 24 = 28。clearColor 省略時は既定値 [0,0,0,1]。
        expect(view.getUint32(28 + 0, true)).toBe(9);
        expect(view.getFloat32(28 + 4, true)).toBe(0);
        expect(view.getFloat32(28 + 8, true)).toBe(0);
        expect(view.getFloat32(28 + 12, true)).toBe(0);
        expect(view.getFloat32(28 + 16, true)).toBe(1);
        expect(view.getUint32(28 + 20, true)).toBe(LoadOp.Clear);
    });

    it("colorAttachments は LN_MAX_COLOR_ATTACHMENTS (8件) で打ち切られる", () => {
        const view = makeView(SIZEOF_RENDER_PASS_DESC);
        const desc: RenderPassDesc = {
            colorAttachments: Array.from({ length: 10 }, (_, i) => ({ renderTarget: i + 1 })),
        };
        writeRenderPassDesc(view, desc, 0);
        expect(view.getUint32(0, true)).toBe(8);
    });

    it("depthStencil を offset 196 に書き込む", () => {
        const view = makeView(SIZEOF_RENDER_PASS_DESC);
        const desc: RenderPassDesc = {
            depthStencil: {
                depthBuffer: 3,
                clearDepth: 0.5,
                clearStencil: 2,
                depthLoadOp: LoadOp.DontCare,
                stencilLoadOp: LoadOp.Load,
            },
        };
        writeRenderPassDesc(view, desc, 0);

        const base = 196;
        expect(view.getUint32(base + 0, true)).toBe(3);
        expect(view.getFloat32(base + 4, true)).toBeCloseTo(0.5, 6);
        expect(view.getUint32(base + 8, true)).toBe(2);
        expect(view.getUint32(base + 12, true)).toBe(LoadOp.DontCare);
        expect(view.getUint32(base + 16, true)).toBe(LoadOp.Load);
    });

    it("shaderPassNamePtr を offset 216、sortMode を offset 220 に書き込む", () => {
        const view = makeView(SIZEOF_RENDER_PASS_DESC);
        writeRenderPassDesc(view, { sortMode: SortMode.BackToFront }, 0x1234);

        expect(view.getUint32(216, true)).toBe(0x1234);
        expect(view.getUint32(220, true)).toBe(SortMode.BackToFront);
    });
});
