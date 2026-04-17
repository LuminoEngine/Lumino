export interface Vec3 {
    x: number;
    y: number;
    z: number;
}

// multiply 用の一時バッファ (GC 回避)
const _mulTmp = new Float32Array(16);

/**
 * 列優先 4x4 行列。C++ ln::Matrix4x4 と同一レイアウト。
 * GC Pressure を抑えるため、各メソッドは this を変更して返す。
 */
export class Matrix4x4 {
    readonly m: Float32Array;

    public constructor() {
        this.m = new Float32Array([
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        ]);
    }

    public setIdentity(): this {
        const m = this.m;
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
        return this;
    }

    /** C++ Matrix4x4::perspectiveRH と同一 (RH, depth [-1,1]) */
    public static makePerspective(fovY: number, aspect: number, nearZ: number, farZ: number, result?: Matrix4x4): Matrix4x4 {
        result = result ?? new Matrix4x4();
        const tanHalf = Math.tan(fovY * 0.5);
        const m = result.m;
        m[0]  = 1.0 / (aspect * tanHalf);
        m[1]  = 0;
        m[2]  = 0;
        m[3]  = 0;
        m[4]  = 0;
        m[5]  = 1.0 / tanHalf;
        m[6]  = 0;
        m[7]  = 0;
        m[8]  = 0;
        m[9]  = 0;
        m[10] = -(farZ + nearZ) / (farZ - nearZ);
        m[11] = -1.0;
        m[12] = 0;
        m[13] = 0;
        m[14] = -(2.0 * farZ * nearZ) / (farZ - nearZ);
        m[15] = 0;
        return result;
    }

    /** C++ Matrix4x4::lookAtRH と同一 */
    public static makeLookAt(eye: Vec3, target: Vec3, up: Vec3 = { x: 0, y: 1, z: 0 }, result?: Matrix4x4): Matrix4x4 {
        result = result ?? new Matrix4x4();
        // zaxis = target - eye (注視点方向)
        let zx = target.x - eye.x;
        let zy = target.y - eye.y;
        let zz = target.z - eye.z;
        const zLen = Math.sqrt(zx * zx + zy * zy + zz * zz);
        if (zLen > 0) { zx /= zLen; zy /= zLen; zz /= zLen; }

        // xaxis = cross(zaxis, up)
        let xx = zy * up.z - zz * up.y;
        let xy = zz * up.x - zx * up.z;
        let xz = zx * up.y - zy * up.x;
        const xLen = Math.sqrt(xx * xx + xy * xy + xz * xz);
        if (xLen > 0) { xx /= xLen; xy /= xLen; xz /= xLen; }

        // yaxis = cross(xaxis, zaxis)
        const yx = xy * zz - xz * zy;
        const yy = xz * zx - xx * zz;
        const yz = xx * zy - xy * zx;

        const m = result.m;
        m[0]  = xx;  m[1]  = yx;  m[2]  = -zx; m[3]  = 0;
        m[4]  = xy;  m[5]  = yy;  m[6]  = -zy; m[7]  = 0;
        m[8]  = xz;  m[9]  = yz;  m[10] = -zz; m[11] = 0;
        m[12] = -(xx * eye.x + xy * eye.y + xz * eye.z);
        m[13] = -(yx * eye.x + yy * eye.y + yz * eye.z);
        m[14] =  (zx * eye.x + zy * eye.y + zz * eye.z);
        m[15] = 1;
        return result;
    }

    /** C++ Camera::setOrthographic / Matrix4x4::ortho と同一 */
    public static makeOrthographic(width: number, height: number, nearZ: number, farZ: number, result?: Matrix4x4): Matrix4x4 {
        result = result ?? new Matrix4x4();
        const m = result.m;
        m[0]  = 2.0 / width;
        m[1]  = 0; m[2]  = 0; m[3]  = 0;
        m[4]  = 0;
        m[5]  = 2.0 / height;
        m[6]  = 0; m[7]  = 0;
        m[8]  = 0; m[9]  = 0;
        m[10] = 1.0 / (nearZ - farZ);
        m[11] = 0;
        m[12] = 0; m[13] = 0;
        m[14] = nearZ / (nearZ - farZ);
        m[15] = 1.0;
        return result;
    }

    /** this = this * rhs (結果を this に書き込む) */
    multiply(rhs: Matrix4x4): this {
        const a = this.m;
        const b = rhs.m;
        for (let col = 0; col < 4; col++) {
            for (let row = 0; row < 4; row++) {
                let sum = 0;
                for (let k = 0; k < 4; k++) {
                    sum += a[k * 4 + row] * b[col * 4 + k];
                }
                _mulTmp[col * 4 + row] = sum;
            }
        }
        a.set(_mulTmp);
        return this;
    }
}
