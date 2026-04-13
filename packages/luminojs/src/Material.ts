import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Texture } from "./Texture";
import { API, Runtime } from "./Runtime";

export class Material extends LuminoObject {
    /** Create a material using the built-in Unlit shader. */
    static createUnlit(ctx: GraphicsContext): Material {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNMaterial_CreateUnlit as (
                ctx: number, out: number,
            ) => number)(ctx.handle, out));
        const mat = new Material();
        mat._setHandle(handle, true);
        return mat;
    }

    /** Set the main (diffuse) texture. */
    setMainTexture(texture: Texture): void {
        Runtime.safeCall(() =>
            (API.LNMaterial_SetMainTexture as (
                mat: number, tex: number,
            ) => number)(this._handle, texture.handle));
    }

    /** Set the base color (RGBA, 0-1). */
    setColor(r: number, g: number, b: number, a: number): void {
        Runtime.safeCall(() =>
            (API.LNMaterial_SetColor as (
                mat: number, r: number, g: number, b: number, a: number,
            ) => number)(this._handle, r, g, b, a));
    }
}
