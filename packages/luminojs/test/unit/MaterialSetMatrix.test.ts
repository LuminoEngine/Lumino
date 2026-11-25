import { beforeEach, describe, expect, it } from "vitest";
import { GraphicsContext } from "../../src/GraphicsContext";
import { Material } from "../../src/Material";
import { Matrix4x4 } from "../../src/Matrix4x4";
import { API, Runtime } from "../../src/Runtime";
import { Result } from "../../src/types";
import { createFakeModule, type FakeModule } from "./fakeModule";

// Material.setMatrix の JS 層が Matrix4x4.m (列優先の 16 要素) を並べ替えずに C-API へ渡すことを確かめる。
// GPU 側の解釈 (mul(M, v) が A * v になること) は gtest (Test_Graphics.cpp の MaterialSetMatrix*) で検証する。

describe("Material.setMatrix", () => {
    let fake: FakeModule;
    let calls: { name: string; values: number[] }[];
    let ctx: GraphicsContext;

    beforeEach(() => {
        fake = createFakeModule();
        (Runtime as unknown as { module: unknown }).module = fake;
        calls = [];
        API.LNMaterial_CreateFromBuiltinShader = ((_ctx: number, _shader: number, out: number): number => {
            new DataView(fake.buffer).setUint32(out, 0x00070001, true);
            return Result.OK;
        }) as never;
        API.LNMaterial_SetMatrix = ((_mat: number, name: string, ptr: number): number => {
            calls.push({ name, values: Array.from(fake.HEAPF32.subarray(ptr >> 2, (ptr >> 2) + 16)) });
            return Result.OK;
        }) as never;
        ctx = new GraphicsContext();
        ctx._setHandle(0x00050001, false);
    });

    it("m の 16 要素をそのままの順で渡し、呼び出し時の値を保持する", () => {
        const a = new Matrix4x4();
        for (let i = 0; i < 16; i++) a.m[i] = i + 1;
        const expected = Array.from(a.m);

        const material = Material.createUnlit();
        material.setMatrix("u_matrix", a);
        a.m[12] = 100; // 呼び出し後の書き換えは反映されない
        material.ensure(ctx);

        expect(calls).toEqual([{ name: "u_matrix", values: expected }]);
    });
});
