import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Texture } from "./Texture";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { CullMode } from "./types";

type MaterialSource =
    | { kind: "unlit" }
    | { kind: "compiled"; data: Uint8Array };

export class Material extends LuminoObject implements ResidentResource {
    private _source: MaterialSource;
    private _mainTexture: Texture | null = null;
    private _namedTextures: Map<string, Texture> = new Map();
    private _color: [number, number, number, number] | null = null;
    private _float4s: Map<string, [number, number, number, number]> = new Map();
    private _cullMode: CullMode | null = null;
    private _dirty = false;
    private _paramsDirty = false;
    private _lastUsedFrame = 0;

    get lastUsedFrame(): number { return this._lastUsedFrame; }

    constructor(source: MaterialSource) {
        super();
        this._source = source;
    }

    /** Create a material using the built-in Unlit shader. GPU 側生成は遅延。 */
    static createUnlit(): Material {
        const mat = new Material({ kind: "unlit" });
        mat._dirty = true;
        return mat;
    }

    /** Create a material from pre-compiled shader binary (.lcsh). GPU 側生成は遅延。 */
    static createFromCompiledShader(data: Uint8Array): Material {
        const mat = new Material({ kind: "compiled", data });
        mat._dirty = true;
        return mat;
    }

    /** Set the main (diffuse) texture. (shadow state only; applied at ensure time) */
    setMainTexture(texture: Texture): void {
        this._mainTexture = texture;
        this._paramsDirty = true;
    }

    /** Set the base color (RGBA, 0-1). */
    setColor(r: number, g: number, b: number, a: number): void {
        this._color = [r, g, b, a];
        this._paramsDirty = true;
    }

    /** Set a float4 shader parameter by name. */
    setFloat4(name: string, values: [number, number, number, number]): void {
        this._float4s.set(name, [values[0], values[1], values[2], values[3]]);
        this._paramsDirty = true;
    }

    /** Bind a texture to a named shader uniform. */
    setNamedTexture(name: string, texture: Texture): void {
        this._namedTextures.set(name, texture);
        this._paramsDirty = true;
    }

    /** Set face culling mode. */
    setCullMode(mode: CullMode): void {
        this._cullMode = mode;
        this._paramsDirty = true;
    }

    /**
     * @internal Called by Renderer (or Mesh.ensure) at draw time. Creates the
     * C++ Material on first use and applies any shadowed parameter changes.
     */
    ensure(ctx: GraphicsContext): void {
        if (this._handle === 0) {
            this._createGpuMaterial(ctx);
            // All params must be applied after creation, regardless of prior
            // _paramsDirty state.
            this._paramsDirty = true;
        }

        if (this._paramsDirty) {
            this._applyParams(ctx);
            this._paramsDirty = false;
        }

        this._dirty = false;
        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /** @internal Release the GPU material; next ensure() rebuilds and re-applies params. */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
        this._paramsDirty = true;
    }

    override dispose(): void {
        this.evict();
        this._mainTexture = null;
        this._namedTextures.clear();
        this._float4s.clear();
    }

    //--------------------------------------------------------------------------

    private _createGpuMaterial(ctx: GraphicsContext): void {
        if (this._source.kind === "unlit") {
            const handle = Runtime.safeCallWithReturnHandle((out) =>
                (API.LNMaterial_CreateUnlit as (
                    ctx: number, out: number,
                ) => number)(ctx.handle, out));
            this._setHandle(handle, true);
        } else {
            const data = this._source.data;
            const m = Runtime.module;
            const ptr = m._malloc(data.byteLength);
            try {
                m.HEAPU8.set(data, ptr);
                const handle = Runtime.safeCallWithReturnHandle((out) =>
                    (API.LNMaterial_CreateFromCompiledShader as (
                        ctx: number, data: number, size: number, out: number,
                    ) => number)(ctx.handle, ptr, data.byteLength, out));
                this._setHandle(handle, true);
            } finally {
                m._free(ptr);
            }
        }
    }

    private _applyParams(ctx: GraphicsContext): void {
        if (this._mainTexture) {
            this._mainTexture.ensure(ctx);
            if (this._mainTexture.handle !== 0) {
                Runtime.safeCall(() =>
                    (API.LNMaterial_SetMainTexture as (
                        mat: number, tex: number,
                    ) => number)(this._handle, this._mainTexture!.handle));
            }
        }

        for (const [name, tex] of this._namedTextures) {
            tex.ensure(ctx);
            if (tex.handle === 0) continue;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetNamedTexture as (
                    mat: number, name: string, tex: number,
                ) => number)(this._handle, name, tex.handle));
        }

        if (this._color) {
            const c = this._color;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetColor as (
                    mat: number, r: number, g: number, b: number, a: number,
                ) => number)(this._handle, c[0], c[1], c[2], c[3]));
        }

        if (this._float4s.size > 0) {
            const m = Runtime.module;
            const ptr = m._malloc(16);
            try {
                const floatOffset = ptr >> 2;
                for (const [name, v] of this._float4s) {
                    m.HEAPF32[floatOffset]     = v[0];
                    m.HEAPF32[floatOffset + 1] = v[1];
                    m.HEAPF32[floatOffset + 2] = v[2];
                    m.HEAPF32[floatOffset + 3] = v[3];
                    Runtime.safeCall(() =>
                        (API.LNMaterial_SetFloat4 as (
                            mat: number, name: string, values: number,
                        ) => number)(this._handle, name, ptr));
                }
            } finally {
                m._free(ptr);
            }
        }

        if (this._cullMode !== null) {
            const mode = this._cullMode;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetCullMode as (
                    mat: number, mode: number,
                ) => number)(this._handle, mode));
        }
    }
}
