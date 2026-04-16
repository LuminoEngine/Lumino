import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import { API, Runtime } from "./Runtime";
import type { TextureFormat } from "./types";

export class Texture extends LuminoObject {
    /** Create a render target texture with a specific format. */
    static createRenderTargetEx(ctx: GraphicsContext, width: number, height: number, format: TextureFormat): Texture {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNTexture2D_CreateRenderTargetEx as (
                ctx: number, w: number, h: number, fmt: number, out: number,
            ) => number)(ctx.handle, width, height, format, out));
        const tex = new Texture();
        tex._setHandle(handle, true);
        return tex;
    }

    /** Create a depth-stencil texture. */
    static createDepthStencil(ctx: GraphicsContext, width: number, height: number): Texture {
        const handle = Runtime.safeCallWithReturnHandle((out) =>
            (API.LNTexture2D_CreateDepthStencil as (
                ctx: number, w: number, h: number, out: number,
            ) => number)(ctx.handle, width, height, out));
        const tex = new Texture();
        tex._setHandle(handle, true);
        return tex;
    }

    /**
     * Create a texture from raw image data (PNG, etc.) already in memory.
     *
     * @param ctx  GraphicsContext handle owner.
     * @param data Image file bytes (e.g. PNG).
     */
    static async loadFromMemory(ctx: GraphicsContext, data: Uint8Array): Promise<Texture> {
        const m = Runtime.module;
        const ptr = m._malloc(data.byteLength);
        try {
            m.HEAPU8.set(data, ptr);
            const handle = await Runtime.safeCallWithReturnHandleAsync((out) =>
                (API.LNTexture2D_LoadFromMemory as (
                    ctx: number, data: number, size: number, out: number,
                ) => number | Promise<number>)(ctx.handle, ptr, data.byteLength, out));
            const tex = new Texture();
            tex._setHandle(handle, true);
            return tex;
        } finally {
            m._free(ptr);
        }
    }

    /**
     * Fetch an image from a URL and create a texture.
     *
     * @param ctx GraphicsContext handle owner.
     * @param url URL to fetch (relative or absolute).
     */
    static async loadFromURL(ctx: GraphicsContext, url: string): Promise<Texture> {
        const resp = await fetch(url);
        if (!resp.ok) throw new Error(`Failed to fetch texture: ${resp.status} ${url}`);
        const buf = await resp.arrayBuffer();
        return Texture.loadFromMemory(ctx, new Uint8Array(buf));
    }
}
