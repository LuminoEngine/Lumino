import { DepthBuffer } from "./DepthBuffer";
import { GraphicsContext } from "./GraphicsContext";
import { GraphicsViewPoint } from "./GraphicsViewPoint";
import { LuminoObject } from "./LuminoObject";
import { SceneRenderPass } from "./SceneRenderPass";
import { RenderTexture } from "./RenderTargetTexture";
import { API, Runtime } from "./Runtime";
import { MAX_RENDER_TARGETS } from "./types";
import { IColor } from "./math/Color";

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

export class GraphicsCommandList extends LuminoObject {
    private _owner: GraphicsContext;

    /**
     * Creates a new GraphicsCommandList.
     */
    public constructor(owner: GraphicsContext) {
        super();
        this._owner = owner;
    }

    public beginRenderPass(descriptor: BeginRederPassDescriptor, viewPoint: GraphicsViewPoint): SceneRenderPass {
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
        
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNCommandList_BeginSceneRenderPass(this.handle, desc, viewPoint.handle, r));
        const renderPass = new SceneRenderPass(this);
        renderPass._setHandle(handle, false);
        return renderPass;
    }
}

