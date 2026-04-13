/** Lumino C-API result codes. */
export enum Result {
    OK = 0,
    ERROR_UNKNOWN = -1,
    RUNTIME_UNINITIALIZED = -2,
    ERROR_INVALID_ARGUMENT = -3,
    ERROR_INVALID_HANDLE = -4,
}

/** Graphics backend selection. */
export enum GraphicsBackend {
    Default = 0,
    Vulkan = 1,
    WebGPU = 2,
}

/** Render pass attachment load operation. */
export enum LoadOp {
    Clear = 0,
    Load = 1,
    DontCare = 2,
}

/** Opaque object handle (uint32). */
export type Handle = number;

/** Null handle sentinel. */
export const LN_NULL_HANDLE: Handle = 0;

/** Maximum number of simultaneous color attachments. */
export const LN_MAX_COLOR_ATTACHMENTS = 8;

/** Options for `Instance.initialize`. */
export interface InstanceInitializeSettings {
    preferredBackend?: GraphicsBackend;
    enableValidation?: boolean;
}

/** Describes a single color attachment for a render pass. */
export interface ColorAttachmentDesc {
    /** Render target handle (`LN_NULL_HANDLE` = back-buffer). */
    renderTarget?: Handle;
    /** RGBA clear color. Default `[0, 0, 0, 1]`. */
    clearColor?: [number, number, number, number];
    /** Load operation. Default `LoadOp.Clear`. */
    loadOp?: LoadOp;
}

/** Describes the depth-stencil attachment for a render pass. */
export interface DepthStencilAttachmentDesc {
    depthBuffer?: Handle;
    clearDepth?: number;
    clearStencil?: number;
    depthLoadOp?: LoadOp;
    stencilLoadOp?: LoadOp;
}

/** Full render pass descriptor (mirrors C `LNRenderPassDesc`). */
export interface RenderPassDesc {
    colorAttachments?: ColorAttachmentDesc[];
    depthStencil?: DepthStencilAttachmentDesc;
}

/** Options for `Runtime.initialize`. */
export interface RuntimeOptions {
    /** Path (or URL) to `LuminoC.wasm`. Forwarded to Emscripten `locateFile`. */
    wasmPath?: string;
    /** Callback for stdout lines from the C runtime. */
    print?: (text: string) => void;
    /** Callback for stderr lines from the C runtime. */
    printErr?: (text: string) => void;
}

//------------------------------------------------------------------------------
// C struct layout constants (wasm32, 4-byte aligned)
//------------------------------------------------------------------------------

/** Byte size of `LNColorAttachmentDesc` in wasm memory. */
export const SIZEOF_COLOR_ATTACHMENT_DESC = 24;
// Layout: renderTarget(u32,0) clearColor(f32x4,4) loadOp(u32,20)

/** Byte size of `LNDepthStencilAttachmentDesc` in wasm memory. */
export const SIZEOF_DEPTH_STENCIL_ATTACHMENT_DESC = 20;
// Layout: depthBuffer(u32,0) clearDepth(f32,4) clearStencil(u32,8)
//         depthLoadOp(u32,12) stencilLoadOp(u32,16)

/** Byte size of `LNRenderPassDesc` in wasm memory. */
export const SIZEOF_RENDER_PASS_DESC = 216;
// Layout: colorAttachmentCount(u32,0)
//         colorAttachments[8](24*8=192, offset 4)
//         depthStencil(20, offset 196)

/** Byte size of `LNInstanceInitializeSettings` in wasm memory. */
export const SIZEOF_INSTANCE_INIT_SETTINGS = 8;
// Layout: preferredBackend(u32,0) enableValidation(u32,4)

/** Byte size of `LNVertex` in wasm memory. */
export const SIZEOF_VERTEX = 64;
// Layout: posX,posY,posZ(f32x3,0) normX,normY,normZ(f32x3,12)
//         u,v(f32x2,24) colorR,G,B,A(f32x4,32) tanX,Y,Z,W(f32x4,48)

/** Byte size of `LNSubMesh` in wasm memory. */
export const SIZEOF_SUBMESH = 12;
// Layout: indexOffset(u32,0) indexCount(u32,4) materialIndex(u32,8)

/** Byte size of `LNTransform` in wasm memory. */
export const SIZEOF_TRANSFORM = 40;
// Layout: posX,posY,posZ(f32x3,0) rotX,rotY,rotZ,rotW(f32x4,12)
//         scaleX,scaleY,scaleZ(f32x3,28)

/** Vertex data matching C `LNVertex` (64 bytes). */
export interface Vertex {
    position: [number, number, number];
    normal: [number, number, number];
    uv: [number, number];
    color: [number, number, number, number];
    tangent: [number, number, number, number];
}

/** Sub-mesh descriptor matching C `LNSubMesh` (12 bytes). */
export interface SubMesh {
    indexOffset: number;
    indexCount: number;
    materialIndex: number;
}

/** TRS transform matching C `LNTransform` (40 bytes). */
export interface Transform {
    position: [number, number, number];
    rotation: [number, number, number, number]; // quaternion (x, y, z, w)
    scale: [number, number, number];
}
