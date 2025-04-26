import { DepthBuffer } from "./graphics/DepthBuffer";
import { GraphicsCommandList } from "./GraphicsCommandList";
import { RenderTexture } from "./RenderTargetTexture";
import { API, Handle, Runtime } from "./Runtime";

export abstract class GraphicsContext {
    public _handle: Handle;
    private _currentColorBuffer: RenderTexture;
    private _currentDepthBuffer: DepthBuffer;
    private _commandList: GraphicsCommandList;

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
        this._commandList = new GraphicsCommandList(this);
    }

    /**
     */
    public commandList(): GraphicsCommandList {
        return this._commandList;
    }

    /**
     * 
     * @param width 
     * @param height 
     * 
     * width and height are texel sizes, not pixel sizes.
     */
    public prepareFrame(width: number, height: number): void {
        const handle1 = Runtime.module._malloc(4);
        const handle2 = Runtime.module._malloc(4);
        const handle3 = Runtime.module._malloc(4);
        Runtime.safeCall(() => API.LNGraphicsContext_BeginFrame(this._handle, width, height, handle1, handle2, handle3));
        console.log("prepareFrame malloc", handle1, handle2, handle3);
        const value1 = Runtime.module.getValue(handle1, "i32");
        const value2 = Runtime.module.getValue(handle2, "i32");
        const value3 = Runtime.module.getValue(handle3, "i32");
        console.log("prepareFrame value", value1, value2, value3);
        this._currentColorBuffer._setHandle(value1, false);
        this._currentDepthBuffer._setHandle(value2, false);
        this._commandList._setHandle(value3, false);
        Runtime.module._free(handle1);
        Runtime.module._free(handle2);
        Runtime.module._free(handle3);
    }

    /**
     * Sends the command list recorded in GraphicsCommandList to the GPU.
     */
    public submitCommandList(commandList: GraphicsCommandList): void {
        API.LNGraphicsContext_EndFrame(this.handle);
    }
}

