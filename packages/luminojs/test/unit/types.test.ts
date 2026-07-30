import { describe, expect, it } from "vitest";
import {
    SIZEOF_COLOR_ATTACHMENT_DESC,
    SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_INSTANCE_INIT_SETTINGS,
    SIZEOF_VERTEX,
    SIZEOF_SUBMESH,
    SIZEOF_TRANSFORM,
    SIZEOF_MATRIX,
    SIZEOF_GRAPHICS_PROFILER,
    LN_MAX_COLOR_ATTACHMENTS,
} from "../../src/types";

// これらの SIZEOF_* 定数は C 側の構造体 (lumino.h) と手動で同期されている
// (実行時の照合は Runtime._verifyStructLayouts が担当、改善案 #10)。
// ここでは「手計算とコード上の定数値が一致しているか」を固定し、
// 定数だけをうっかり書き換えるリグレッションを検出する。

describe("types.ts SIZEOF_* layout constants", () => {
    it("SIZEOF_COLOR_ATTACHMENT_DESC = renderTarget(u32,4) + clearColor(f32x4,16) + loadOp(u32,4)", () => {
        expect(SIZEOF_COLOR_ATTACHMENT_DESC).toBe(4 + 16 + 4);
        expect(SIZEOF_COLOR_ATTACHMENT_DESC).toBe(24);
    });

    it("SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC = depthBuffer(4) + clearDepth(4) + clearStencil(4) + depthLoadOp(4) + stencilLoadOp(4)", () => {
        expect(SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC).toBe(4 + 4 + 4 + 4 + 4);
        expect(SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC).toBe(20);
    });

    it("SIZEOF_RENDER_PASS_DESC = count(4) + colorAttachments[8]*24(192) + depthStencil(20) + shaderPassName ptr(4) + sortMode(4)", () => {
        expect(LN_MAX_COLOR_ATTACHMENTS).toBe(8);
        const expected =
            4 +
            LN_MAX_COLOR_ATTACHMENTS * SIZEOF_COLOR_ATTACHMENT_DESC +
            SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC +
            4 + // shaderPassName (const char*)
            4;  // sortMode (uint32_t)
        expect(expected).toBe(224);
        expect(SIZEOF_RENDER_PASS_DESC).toBe(expected);
    });

    it("SIZEOF_INSTANCE_INIT_SETTINGS = preferredBackend(4) + enableValidation(4)", () => {
        expect(SIZEOF_INSTANCE_INIT_SETTINGS).toBe(4 + 4);
        expect(SIZEOF_INSTANCE_INIT_SETTINGS).toBe(8);
    });

    it("SIZEOF_VERTEX = position(f32x3,12) + normal(f32x3,12) + uv(f32x2,8) + color(f32x4,16) + tangent(f32x4,16)", () => {
        expect(SIZEOF_VERTEX).toBe(12 + 12 + 8 + 16 + 16);
        expect(SIZEOF_VERTEX).toBe(64);
    });

    it("SIZEOF_SUBMESH = indexOffset(4) + indexCount(4) + materialIndex(4)", () => {
        expect(SIZEOF_SUBMESH).toBe(4 + 4 + 4);
        expect(SIZEOF_SUBMESH).toBe(12);
    });

    it("SIZEOF_TRANSFORM = position(f32x3,12) + rotation(f32x4,16) + scale(f32x3,12)", () => {
        expect(SIZEOF_TRANSFORM).toBe(12 + 16 + 12);
        expect(SIZEOF_TRANSFORM).toBe(40);
    });

    it("SIZEOF_MATRIX = float[16] (列優先 4x4)", () => {
        expect(SIZEOF_MATRIX).toBe(16 * 4);
        expect(SIZEOF_MATRIX).toBe(64);
    });

    it("SIZEOF_GRAPHICS_PROFILER = drawCallCount(i32,4) + fps(f32,4) + lastFrameTimeMs(f32,4)", () => {
        expect(SIZEOF_GRAPHICS_PROFILER).toBe(4 + 4 + 4);
        expect(SIZEOF_GRAPHICS_PROFILER).toBe(12);
    });
});
