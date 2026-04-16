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

    /** Create a material from pre-compiled shader binary (.lcsh). */
    static async createFromCompiledShader(ctx: GraphicsContext, data: Uint8Array): Promise<Material> {
        const m = Runtime.module;
        const ptr = m._malloc(data.byteLength);
        try {
            m.HEAPU8.set(data, ptr);
            const handle = await Runtime.safeCallWithReturnHandleAsync((out) =>
                (API.LNMaterial_CreateFromCompiledShader as (
                    ctx: number, data: number, size: number, out: number,
                ) => number | Promise<number>)(ctx.handle, ptr, data.byteLength, out));
            const mat = new Material();
            mat._setHandle(handle, true);
            return mat;
        } finally {
            m._free(ptr);
        }
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

    /** Set a float4 shader parameter by name. */
    setFloat4(name: string, values: [number, number, number, number]): void {
        const m = Runtime.module;
        const ptr = m._malloc(16);
        try {
            const floatOffset = ptr >> 2;
            m.HEAPF32[floatOffset]     = values[0];
            m.HEAPF32[floatOffset + 1] = values[1];
            m.HEAPF32[floatOffset + 2] = values[2];
            m.HEAPF32[floatOffset + 3] = values[3];
            Runtime.safeCall(() =>
                (API.LNMaterial_SetFloat4 as (
                    mat: number, name: string, values: number,
                ) => number)(this._handle, name, ptr));
        } finally {
            m._free(ptr);
        }
    }

    /** Bind a texture to a named shader uniform. */
    setNamedTexture(name: string, texture: Texture): void {
        Runtime.safeCall(() =>
            (API.LNMaterial_SetNamedTexture as (
                mat: number, name: string, tex: number,
            ) => number)(this._handle, name, texture.handle));
    }
}
