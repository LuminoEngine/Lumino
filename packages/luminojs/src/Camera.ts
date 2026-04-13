import { LuminoObject } from "./LuminoObject";
import { API, Runtime } from "./Runtime";

export class Camera extends LuminoObject {
    /** Create a camera with default settings. */
    static create(): Camera {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNCamera_Create as (out: number) => number)(out));
        const cam = new Camera();
        cam._setHandle(handle, true);
        return cam;
    }

    /** Set perspective projection parameters. */
    setPerspective(fovY: number, aspect: number, near: number, far: number): void {
        Runtime.safeCall(() =>
            (API.LNCamera_SetPerspective as (
                cam: number, fov: number, aspect: number, near: number, far: number,
            ) => number)(this._handle, fovY, aspect, near, far));
    }

    /** Set the view matrix via eye / target / up vectors. */
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
}
