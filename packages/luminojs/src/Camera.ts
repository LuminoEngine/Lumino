import { LuminoObject } from "./LuminoObject";
import { Matrix4x4 } from "./Matrix4x4";
import { API, Runtime } from "./Runtime";

export class Camera extends LuminoObject {
    /**
     * Create a camera with default settings.
     * デフォルト設定のカメラを作成します。
     */
    static create(): Camera {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNCamera_Create as (out: number) => number)(out));
        const cam = new Camera();
        cam._setHandle(handle, true);
        return cam;
    }

    /**
     * Set perspective projection parameters.
     * カメラに透視投影を設定します。
     * @param fovY   垂直視野角 (ラジアン)
     * @param aspect アスペクト比 (幅 / 高さ)
     * @param near   ニアクリップ距離
     * @param far    ファークリップ距離
     */
    setPerspective(fovY: number, aspect: number, near: number, far: number): void {
        Runtime.safeCall(() =>
            (API.LNCamera_SetPerspective as (
                cam: number, fov: number, aspect: number, near: number, far: number,
            ) => number)(this._handle, fovY, aspect, near, far));
    }

    /**
     * Set the view matrix via eye / target / up vectors.
     * ビュー行列を視点・注視点・上方向ベクトルで設定します。
     * @param eyeX    視点位置 X
     * @param eyeY    視点位置 Y
     * @param eyeZ    視点位置 Z
     * @param targetX 注視点 X
     * @param targetY 注視点 Y
     * @param targetZ 注視点 Z
     * @param upX     上方向ベクトル X
     * @param upY     上方向ベクトル Y
     * @param upZ     上方向ベクトル Z
     */
    setLookAt(
        eyeX: number, eyeY: number, eyeZ: number,
        targetX: number, targetY: number, targetZ: number,
        upX: number, upY: number, upZ: number,
    ): void {
        Runtime.safeCall(() =>
            (API.LNCamera_SetLookAt as (
                cam: number,
                ex: number, ey: number, ez: number,
                tx: number, ty: number, tz: number,
                ux: number, uy: number, uz: number,
            ) => number)(this._handle, eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ));
    }

    /**
     * Set view and projection matrices directly.
     * カメラのビュー行列とプロジェクション行列を直接設定します。
     * @param view ビュー行列 (列優先 float x 16)
     * @param proj プロジェクション行列 (列優先 float x 16)
     */
    setMatrices(view: Matrix4x4, proj: Matrix4x4): void {
        const m = Runtime.module;
        const byteLen = 16 * 4; // 64 bytes per matrix
        const ptr = m._malloc(byteLen * 2);
        try {
            const heap = new Float32Array(m.HEAPU8.buffer, ptr, 32);
            heap.set(view.m, 0);
            heap.set(proj.m, 16);
            Runtime.safeCall(() =>
                (API.LNCamera_SetMatrices as (
                    cam: number, viewPtr: number, projPtr: number,
                ) => number)(this._handle, ptr, ptr + byteLen));
        } finally {
            m._free(ptr);
        }
    }
}
