
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
}
