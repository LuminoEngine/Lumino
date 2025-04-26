
export interface IVector2 {
    x: number;
    y: number;
}


class Vector2 {
    public x: number;
    public y: number;

    constructor() {
        this.x = 0;
        this.y = 0;
    }

    // static fromArray(arr: number[]): Vector2 {
    //     return new Vector2(arr[0], arr[1]);
    // }

    // toArray(): number[] {
    //     return [this.x, this.y];
    // }

    // add(v: Vector2): Vector2 {
    //     return new Vector2(this.x + v.x, this.y + v.y);
    // }

    // subtract(v: Vector2): Vector2 {
    //     return new Vector2(this.x - v.x, this.y - v.y);
    // }

    // multiply(scalar: number): Vector2 {
    //     return new Vector2(this.x * scalar, this.y * scalar);
    // }

    // divide(scalar: number): Vector2 {
    //     return new Vector2(this.x / scalar, this.y / scalar);
    // }
}
