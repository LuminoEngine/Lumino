import { LuminoObject } from "./LuminoObject";
import { Matrix4x4 } from "./Matrix4x4";
import { API, Runtime } from "./Runtime";

export class Camera extends LuminoObject {
    /**
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
     * カメラに 2D 用の正射影投影 (左上原点、Y軸下向き) を設定します。
     *
     * 画面座標 (0, 0) が左上、(width, height) が右下に対応します。
     * ビュー行列は単位行列に設定されます。このカメラで `Renderer.drawSprite` を
     * 行うと、スプライト頂点が左上原点、Y軸下向きのレイアウトで自動生成されます
     * (UV 反転やマテリアルのカリング設定変更は不要です)。
     *
     * 注意: このカメラで 3D メッシュ (`Renderer.drawMesh`) を描画する場合は、
     * Y 軸反転によりワインディングが反転するため、マテリアルの CullMode を
     * None に設定するか、フロントフェイス向きを CW に揃える必要があります。
     *
     * pivotX, pivotY (0..1) で原点 (画面座標 (0,0)) の位置を指定できます。
     * pivot=(0,0) で左上、(0.5,0.5) で画面中央、(1,1) で右下が原点になります。
     *
     * @param width    画面幅 (ピクセル)
     * @param height   画面高さ (ピクセル)
     * @param nearClip ニアクリップ距離
     * @param farClip  ファークリップ距離
     * @param pivotX   原点の水平位置 (0..1)。0=左, 0.5=中央, 1=右 (既定 0)
     * @param pivotY   原点の垂直位置 (0..1)。0=上, 0.5=中央, 1=下 (既定 0)
     */
    setOrthographic2D(
        width: number, height: number, nearClip: number, farClip: number,
        pivotX = 0, pivotY = 0,
    ): void {
        Runtime.safeCall(() =>
            (API.LNCamera_SetOrthographic2D as (
                cam: number, width: number, height: number, near: number, far: number,
                pivotX: number, pivotY: number,
            ) => number)(this._handle, width, height, nearClip, farClip, pivotX, pivotY));
    }

    /**
     * ビュー行列を視点、注視点、上方向ベクトルで設定します。
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
     * カメラのビュー行列とプロジェクション行列を直接設定します。
     * @param view ビュー行列 (列優先 float x 16)
     * @param proj プロジェクション行列 (列優先 float x 16)
     * @param is2D 2D モードフラグ。`true` で 2D カメラ (スプライトが左上原点、Y軸下向き
     *             のレイアウトで自動生成される)。既定は `false`
     */
    setMatrices(view: Matrix4x4, proj: Matrix4x4, is2D = false): void {
        const m = Runtime.module;
        const byteLen = 16 * 4; // 行列 1 つあたり 64 バイト
        const ptr = m._malloc(byteLen * 2);
        try {
            const heap = new Float32Array(m.HEAPU8.buffer, ptr, 32);
            heap.set(view.m, 0);
            heap.set(proj.m, 16);
            Runtime.safeCall(() =>
                (API.LNCamera_SetMatrices as (
                    cam: number, viewPtr: number, projPtr: number, is2D: number,
                ) => number)(this._handle, ptr, ptr + byteLen, is2D ? 1 : 0));
        } finally {
            m._free(ptr);
        }
    }
}
