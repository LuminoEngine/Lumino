import { GraphicsContext } from "./GraphicsContext";
import { API, Handle, Result, Runtime } from "./Runtime";

export class WebGPUGraphicsContext extends GraphicsContext {
    public static async create(selector: string): Promise<WebGPUGraphicsContext> {
        // const canvas = document.querySelector(selector) as HTMLCanvasElement;
        // if (!canvas) {
        //     throw new Error(`Canvas element not found for selector: ${selector}`);
        // }

        // const context = canvas.getContext("webgpu");
        // if (!context) {
        //     throw new Error("WebGPU context not available.");
        // }

        const ptr = Runtime.module._malloc(4) as number;
        const result = await API.LNGraphicsContext_CreateFromWebGPUCanvas(0, ptr);
        if (result != Result.LN_OK) {
            throw new Error(`Failed to call function. Result: ${result}`);
        }

        const buffer = new Uint32Array(Runtime.module.HEAPU8.buffer, ptr, 1);
        const handle = buffer[0];
        console.log("WebGPUGraphicsContext handle:", buffer[0]);
        Runtime.module._free(ptr);

        return new WebGPUGraphicsContext(handle);
    }


    private constructor(handle: Handle) {
        super();
        this._handle = handle

        
        //this._handle = Runtime.safeCallWithReturnHandle((r) => API.LNGraphicsContext_CreateFromWebGPUCanvas(0, r));
    }
}
