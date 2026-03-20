import { DepthBuffer } from "./DepthBuffer";
import { RenderTexture } from "./RenderTargetTexture";
import { API, Handle, Runtime } from "../Runtime";
import { SceneRenderPass } from "./SceneRenderPass";
import { IColor } from "src/math/Color";
import { GraphicsViewPoint } from "./GraphicsViewPoint";
import { MAX_RENDER_TARGETS } from "../types";

export abstract class GraphicsContext {
    public _handle: Handle;
    private _currentColorBuffer: RenderTexture;
    private _currentDepthBuffer: DepthBuffer;

    public get handle(): Handle {
        return this._handle;
    }

    /**
     * Get the current color buffer.
     * 
     * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an RenderPass.
     */
    public get currentColorBuffer(): RenderTexture {
        return this._currentColorBuffer;
    }

    /**
     * Get the current depth buffer.
     * 
     * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an RenderPass.
     */
    public get currentDepthBuffer(): DepthBuffer {
        return this._currentDepthBuffer;
    }

    protected constructor() {
        this._handle = 0;
        this._currentColorBuffer = new RenderTexture(this);
        this._currentDepthBuffer = new DepthBuffer(this);
    }

    /** @see LNGraphicsContext_BeginFrame */
    public beginFrame(width: number, height: number): void {
        const handle1 = Runtime.module._malloc(4);
        const handle2 = Runtime.module._malloc(4);
        Runtime.safeCall(() => API.LNGraphicsContext_BeginFrame(this._handle, width, height, handle1, handle2));
        const value1 = Runtime.module.getValue(handle1, "i32");
        const value2 = Runtime.module.getValue(handle2, "i32");
        this._currentColorBuffer._setHandle(value1, false);
        this._currentDepthBuffer._setHandle(value2, false);
        Runtime.module._free(handle1);
        Runtime.module._free(handle2);
    }

    /** @see LNGraphicsContext_EndFrame */
    public endFrame(): void {
        API.LNGraphicsContext_EndFrame(this.handle);
    }
    
    public beginSceneRenderPass(descriptor: BeginRederPassDescriptor, viewPoint: GraphicsViewPoint): SceneRenderPass {
        const desc = API.LNRenderPassDescriptor_Get();
        const count = descriptor.renderTargets.length;
        if (count <= 0 || count > MAX_RENDER_TARGETS) {
            throw new Error("Invalid render target count.");
        }

        // RenderTargets.
        for (let i = 0; i < count; i++) {
            const attachment = descriptor.renderTargets[i];
            let r, g, b, a;
            if (attachment.clearColor) {
                const c = attachment.clearColor;
                r = c.r;
                g = c.g;
                b = c.b;
                a = c.a;
            } else {
                r = 0;
                g = 0;
                b = 0;
                a = 0;
            }
            API.LNRenderPassDescriptor_SetRenderTarget(
                desc, i, attachment.renderTarget.handle,
                r, g, b, a,
                attachment.clearColor !== undefined ? 1 : 0
            );
        }

        // DepthBuffer.
        if (descriptor.depthBuffer) {
            const attachment = descriptor.depthBuffer;
            const clearDepth = attachment.clearDepth !== undefined ? attachment.clearDepth: 1;
            const clearStencil = attachment.clearStencil !== undefined ? attachment.clearStencil : 0;
            API.LNRenderPassDescriptor_SetDepthBuffer(
                desc, attachment.depthBuffer.handle,
                clearDepth,
                clearStencil,
                attachment.clearDepth !== undefined ? 1 : 0,
                attachment.clearStencil !== undefined ? 1 : 0,
            );
        }
        
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNGraphicsContext_BeginSceneRenderPass(this.handle, desc, viewPoint.handle, r));
        const renderPass = new SceneRenderPass(this);
        renderPass._setHandle(handle, false);
        return renderPass;
    }
    
    public endSceneRenderPass(sceneRenderPass: SceneRenderPass): void {
        API.LNGraphicsContext_EndSceneRenderPass(this.handle, sceneRenderPass.handle);
    }
}


export interface BeginRederPassDescriptor {
    renderTargets: {
        renderTarget: RenderTexture;
        clearColor?: IColor; // default: { r: 0, g: 0, b: 0, a: 0 }
    }[];
    depthBuffer?: {
        depthBuffer: DepthBuffer;
        clearDepth?: number;   // 0.0-1.0, default:1.0
        clearStencil?: number; // 0x00-0xFF (0-255), default:0
    }
}
