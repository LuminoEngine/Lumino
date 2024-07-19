import { DepthBuffer } from "./DepthBuffer";
import { RenderTargetTexture } from "./RenderTargetTexture";
import { Handle } from "./Runtime";

export abstract class GraphicsContext {
    public _handle: Handle;

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
}

