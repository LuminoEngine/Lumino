import { describe, expect, it } from "vitest";
import { Matrix4x4 } from "../../src/Matrix4x4";

// docs/graphics-conventions.md の規約: 列優先 (column-major) レイアウト。
//   | m[0] m[4] m[8]  m[12] |
//   | m[1] m[5] m[9]  m[13] |
//   | m[2] m[6] m[10] m[14] |
//   | m[3] m[7] m[11] m[15] |
// 平行移動成分は最終「列」(m[12], m[13], m[14]) に入る (最終行の m[3], m[7], m[11] ではない)。

function toArray(m: Matrix4x4): number[] {
    return Array.from(m.m);
}

// toEqual は -0 と +0 を区別してしまう (例: -Math.sin(0) === -0) ため、
// 数値行列の比較には toBeCloseTo ベースの比較を用いる。
function expectMatrixCloseTo(actual: Matrix4x4, expected: number[]): void {
    const a = toArray(actual);
    expect(a.length).toBe(expected.length);
    for (let i = 0; i < expected.length; i++) {
        expect(a[i]).toBeCloseTo(expected[i], 6);
    }
}

describe("Matrix4x4.makeTranslation", () => {
    it("平行移動成分を列優先レイアウトの m[12..14] に書き込む", () => {
        const m = Matrix4x4.makeTranslation(10, 20, 30);
        expect(m.m[12]).toBe(10);
        expect(m.m[13]).toBe(20);
        expect(m.m[14]).toBe(30);
        // 行優先であれば平行移動は m[3], m[7], m[11] に入るはずだが、そこは 0 のまま。
        expect(m.m[3]).toBe(0);
        expect(m.m[7]).toBe(0);
        expect(m.m[11]).toBe(0);
    });

    it("それ以外は単位行列のまま", () => {
        const m = Matrix4x4.makeTranslation(1, 2, 3);
        expectMatrixCloseTo(m, [
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            1, 2, 3, 1,
        ]);
    });
});

describe("Matrix4x4.makeScale", () => {
    it("対角成分にスケール値を書き込む", () => {
        const m = Matrix4x4.makeScale(2, 3, 4);
        expectMatrixCloseTo(m, [
            2, 0, 0, 0,
            0, 3, 0, 0,
            0, 0, 4, 0,
            0, 0, 0, 1,
        ]);
    });
});

describe("Matrix4x4.makeRotationZ", () => {
    it("angle=0 で単位行列になる", () => {
        const m = Matrix4x4.makeRotationZ(0);
        expectMatrixCloseTo(m, [
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        ]);
    });

    it("angle=PI/2 で X 軸が Y 軸へ回転する (反時計回り)", () => {
        const m = Matrix4x4.makeRotationZ(Math.PI / 2);
        // 列優先: 列0 (m[0..3]) = X 軸の回転後の向き。
        expect(m.m[0]).toBeCloseTo(0, 6);
        expect(m.m[1]).toBeCloseTo(1, 6);
        expect(m.m[4]).toBeCloseTo(-1, 6);
        expect(m.m[5]).toBeCloseTo(0, 6);
        expect(m.m[10]).toBe(1);
        expect(m.m[15]).toBe(1);
    });
});

describe("Matrix4x4.multiply", () => {
    it("this = this * rhs (列ベクトル規約: 先に rhs、その後 this を適用)", () => {
        // T * S: 点にはまず S (スケール) が適用され、その後 T (平行移動) が適用される
        // ( (T*S)*v == T*(S*v) )。
        const t = Matrix4x4.makeTranslation(1, 2, 3);
        const s = Matrix4x4.makeScale(2, 2, 2);
        t.multiply(s);
        expectMatrixCloseTo(t, [
            2, 0, 0, 0,
            0, 2, 0, 0,
            0, 0, 2, 0,
            1, 2, 3, 1, // 平行移動はスケールの影響を受けない
        ]);
    });

    it("単位行列との積は元の行列のまま", () => {
        const identity = new Matrix4x4();
        const m = Matrix4x4.makeTranslation(5, 6, 7);
        const before = toArray(m);
        m.multiply(identity);
        expect(toArray(m)).toEqual(before);
    });
});

describe("Matrix4x4.makeOrthographic", () => {
    it("C++ Matrix4x4::ortho 相当の正射影行列を生成する", () => {
        const width = 800;
        const height = 600;
        const nearZ = 0.1;
        const farZ = 100;
        const m = Matrix4x4.makeOrthographic(width, height, nearZ, farZ);

        expect(m.m[0]).toBeCloseTo(2 / width, 6);
        expect(m.m[5]).toBeCloseTo(2 / height, 6);
        expect(m.m[10]).toBeCloseTo(1 / (nearZ - farZ), 6);
        expect(m.m[14]).toBeCloseTo(nearZ / (nearZ - farZ), 6);
        expect(m.m[15]).toBe(1);

        // 非対角・非平行移動成分は 0
        for (const i of [1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13]) {
            expect(m.m[i]).toBe(0);
        }
    });

    it("Z=nearZ が NDC の 0、Z=farZ が NDC の 1 に写像される (深度範囲 [0,1])", () => {
        // 右手座標系でカメラは既定で -Z 方向を見る (docs/graphics-conventions.md)。
        // そのためカメラ前方 (nearZ〜farZ の視錐台内) にある点のビュー空間 Z は負になる。
        // perspectiveRH_ZO と同様、ortho も -nearZ -> NDC 0, -farZ -> NDC 1 に写像する。
        const nearZ = 1;
        const farZ = 10;
        const m = Matrix4x4.makeOrthographic(2, 2, nearZ, farZ);
        // z' = z * m[10] + m[14] (列優先の第3行を参照)
        const zAtNear = -nearZ * m.m[10] + m.m[14];
        const zAtFar = -farZ * m.m[10] + m.m[14];
        expect(zAtNear).toBeCloseTo(0, 6);
        expect(zAtFar).toBeCloseTo(1, 6);
    });
});
