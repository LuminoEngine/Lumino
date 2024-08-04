
export class Matrix {
    private readonly _m: Float32Array;

    /**
     * Creates a new Matrix with identity-matrix.
     */
    public constructor() {
        this._m = new Float32Array(16);
        this.setIdentity();
    }
    
    /**
     * Gets the internal data of the matrix
     */
    public get m(): Float32Array { return this._m; }

    public set(m11: number, m12: number, m13: number, m14: number,
        m21: number, m22: number, m23: number, m24: number,
        m31: number, m32: number, m33: number, m34: number,
        m41: number, m42: number, m43: number, m44: number): void {
        const m = this._m;
        m[0] = m11; m[1] = m12; m[2] = m13; m[3] = m14;
        m[4] = m21; m[5] = m22; m[6] = m23; m[7] = m24;
        m[8] = m31; m[9] = m32; m[10] = m33; m[11] = m34;
        m[12] = m41; m[13] = m42; m[14] = m43; m[15] = m44;
    }

    public setIdentity(): void {
        const m = this._m;
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    /**
     * Translate this matrix.
     * @param x defines the translation on X axis
     * @param y defines the translation on Y axis
     * @param z defines the translation on Z axis
     */
    public translate(x: number, y: number, z: number): void {
        const m = this._m;
        m[12] += x;
        m[13] += y;
        m[14] += z;
    }

    // [1][1] -> [5]
    // [2][0] -> [8]
    // [2][1] -> [9]
    // [3][0] -> [12]
    // [3][1] -> [13]

    public rotateZ(r: number): void {
        // see: Matrix::rotateZ()
        const c = Math.cos(r);
        const s = Math.sin(r);
        const m = this._m;
        let mx0 = m[0];
        m[0] = mx0 * c + m[1] * -s;
        m[1] = mx0 * s + m[1] * c;

        mx0 = m[4];
        m[4] = mx0 * c + m[5] * -s;
        m[5] = mx0 * s + m[5] * c;

        mx0 = m[8];
        m[8] = mx0 * c + m[9] * -s;
        m[9] = mx0 * s + m[9] * c;

        mx0 = m[12];
        m[12] = mx0 * c + m[13] * -s;
        m[13] = mx0 * s + m[13] * c;
    }


    /**
     * Scale this matrix.
     * @param x defines the translation on X axis
     * @param y defines the translation on Y axis
     * @param z defines the translationon Z axis
     */
    public scale(x: number, y: number, z: number): void {
        const m = this._m;
        m[0] *= x;
        m[1] *= y;
        m[2] *= z;
        m[4] *= x;
        m[5] *= y;
        m[6] *= z;
        m[8] *= x;
        m[9] *= y;
        m[10] *= z;
        m[12] *= x;
        m[13] *= y;
        m[14] *= z;
    }

    /**
     * Multiply this matrix by another matrix. (result(this) = this * other)
     */
    public multiply(other: Matrix, result?: Matrix): void {
        result = result || this;
        const m = this._m;
        const om = other.m;
        const rm = result.m;

        const tm0 = m[0], tm1 = m[1], tm2 = m[2], tm3 = m[3];
        const tm4 = m[4], tm5 = m[5], tm6 = m[6], tm7 = m[7];
        const tm8 = m[8], tm9 = m[9], tm10 = m[10], tm11 = m[11];
        const tm12 = m[12], tm13 = m[13], tm14 = m[14], tm15 = m[15];

        const om0 = om[0], om1 = om[1], om2 = om[2], om3 = om[3];
        const om4 = om[4], om5 = om[5], om6 = om[6], om7 = om[7];
        const om8 = om[8], om9 = om[9], om10 = om[10], om11 = om[11];
        const om12 = om[12], om13 = om[13], om14 = om[14], om15 = om[15];

        rm[0] = tm0 * om0 + tm1 * om4 + tm2 * om8 + tm3 * om12;
        rm[1] = tm0 * om1 + tm1 * om5 + tm2 * om9 + tm3 * om13;
        rm[2] = tm0 * om2 + tm1 * om6 + tm2 * om10 + tm3 * om14;
        rm[3] = tm0 * om3 + tm1 * om7 + tm2 * om11 + tm3 * om15;

        rm[4] = tm4 * om0 + tm5 * om4 + tm6 * om8 + tm7 * om12;
        rm[5] = tm4 * om1 + tm5 * om5 + tm6 * om9 + tm7 * om13;
        rm[6] = tm4 * om2 + tm5 * om6 + tm6 * om10 + tm7 * om14;
        rm[7] = tm4 * om3 + tm5 * om7 + tm6 * om11 + tm7 * om15;

        rm[8] = tm8 * om0 + tm9 * om4 + tm10 * om8 + tm11 * om12;
        rm[9] = tm8 * om1 + tm9 * om5 + tm10 * om9 + tm11 * om13;
        rm[10] = tm8 * om2 + tm9 * om6 + tm10 * om10 + tm11 * om14;
        rm[11] = tm8 * om3 + tm9 * om7 + tm10 * om11 + tm11 * om15;

        rm[12] = tm12 * om0 + tm13 * om4 + tm14 * om8 + tm15 * om12;
        rm[13] = tm12 * om1 + tm13 * om5 + tm14 * om9 + tm15 * om13;
        rm[14] = tm12 * om2 + tm13 * om6 + tm14 * om10 + tm15 * om14;
        rm[15] = tm12 * om3 + tm13 * om7 + tm14 * om11 + tm15 * om15;
    }

    /**
     * Copy the values from another matrix into this matrix.
     */
    public copyFrom(other: Matrix): void {
        this._m.set(other.m);
    }
}
