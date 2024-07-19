import { DepthBuffer } from "./DepthBuffer";
import { GraphicsCommandList } from "./GraphicsCommandList";
import { RenderTargetTexture } from "./RenderTargetTexture";
import { API, Handle } from "./Runtime";

export abstract class GraphicsContext {
    public _handle: Handle;

    public get handle(): Handle {
        return this._handle;
    }

    /**
     * Get the current color buffer.
     * 
     * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an RenderPass.
     */
    public abstract get currentColorBuffer(): RenderTargetTexture;

    /**
     * Get the current depth buffer.
     * 
     * This buffer is a special buffer that represents the default Framebuffer and can only be used to attach to an RenderPass.
     */
    public abstract get currentDepthBuffer(): DepthBuffer;

    /**
     * Creates a new GraphicsCommandList.
     */
    public createCommandList(): GraphicsCommandList {
        return new GraphicsCommandList(this);
    }

    /**
     * Sends the command list recorded in GraphicsCommandList to the GPU.
     */
    public submitCommandList(commandList: GraphicsCommandList): void {
        API.LNGraphicsContext_SubmitCommandList(this.handle, commandList.handle);
    }
}

