
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";

export class Runtime {
    public static module: any;
    
    //public static returnDataHeap: Uint8Array;
    public static returnPointerView: Uint32Array;

    public static webglContextHandle: number = 0;

    public static initialize(): Promise<void> {


        //console.log("initialize...");
        return LuminoFFIModule().then((module: any) => {
            //console.log("module.HEAPU8.buffer", module.HEAPU8.buffer);
            //this.returnDataHeap = new Uint8Array(module.HEAPU8.buffer, 0, 4);
            //this.returnPointerArray = new Uint32Array(this.returnDataHeap.buffer, this.returnDataHeap.byteOffset, 1);
            this.returnPointerView = new Uint32Array(module.HEAPU8.buffer, 0, 1);

            this.module = module;
            
            API.LNRenderPassDescriptor_Get = module.cwrap("LNRenderPassDescriptor_Get", "number", []);
            API.LNRenderPassDescriptor_SetRenderTarget = module.cwrap("LNRenderPassDescriptor_SetRenderTarget", "number", ["number", "number", "number", "number", "number", "number", "number", "number"]);
            API.LNRenderPassDescriptor_SetDepthBuffer = module.cwrap("LNRenderPassDescriptor_SetDepthBuffer", "number", ["number", "number", "number", "number", "number", "number"]);

            API.LNRuntime_Initialize = module.cwrap("LNRuntime_Initialize", "number", []);
            API.LNRuntime_Terminate = module.cwrap("LNRuntime_Terminate", "void", []);
            API.LNGLGraphicsContext_CreateFromCurrentGL = module.cwrap("LNGLGraphicsContext_CreateFromCurrentGL", "number", ["number", "number", "number"]);

            API.LNGraphicsContext_GetCurrentColorBuffer = module.cwrap("LNGraphicsContext_GetCurrentColorBuffer", "number", ["number", "number"]);
            API.LNGraphicsContext_GetCurrentDepthBuffer = module.cwrap("LNGraphicsContext_GetCurrentDepthBuffer", "number", ["number", "number"]);
            API.LNGraphicsContext_SubmitCommandList = module.cwrap("LNGraphicsContext_SubmitCommandList", "number", ["number", "number"]);
            
            API.LNGraphicsCommandList_Create = module.cwrap("LNGraphicsCommandList_Create", "number", ["number", "number"]);
            API.LNGraphicsCommandList_Reset = module.cwrap("LNGraphicsCommandList_Reset", "number", ["number"]);
            API.LNGraphicsCommandList_BeginRenderPass = module.cwrap("LNGraphicsCommandList_BeginRenderPass", "number", ["number", "number", "number"]);
            
            API.LNGraphicsViewPoint_Create = module.cwrap("LNGraphicsViewPoint_Create", "number", ["number"]);
            API.LNGraphicsViewPoint_SetupPerspective2D = module.cwrap("LNGraphicsViewPoint_SetupPerspective2D", "number", ["number", "number", "number", "number", "number", "number", "number", "number"]);

            API.LNRenderPass_End = module.cwrap("LNRenderPass_End", "number", ["number"]);

            //API.LNRenderingContext_Create = module.cwrap("LNRenderingContext_Create", "number", ["number", "number"]);
            API.LNObject_Release = module.cwrap("LNObject_Release", "number", ["number"]);
            API.LNObject_Retain = module.cwrap("LNObject_Retain", "number", ["number"]);




            // NOTE: cwrap でポインタを渡す方法のひとつに "array" があるが、これは JS->C の方向にしか使えない。 const uint8_t* みたいな感じ。
        });
    }
}


export type Handle = number;

export enum Result {
    LN_OK = 0,
    LN_ERROR_UNKNOWN = -1,
}

export class API {
    // NOTE: パラメータの受け渡し
    // https://twiddlingbits.dev/docsite/gettingstarted/parameters/
    // int32_t, float, double は、 number が自動的に変換される。
    // int64_t はされない。int32_t 扱いされ、アドレスがずれる。

    public static LNRenderPassDescriptor_Get: () => Handle;
    public static LNRenderPassDescriptor_SetRenderTarget: (descriptor: Handle, index: number, renderTarget: Handle, clearR: number, clearG: number, clearB: number, clearA: number, clearEnable: number) => Result;
    public static LNRenderPassDescriptor_SetDepthBuffer: (descriptor: Handle, depthBuffer: Handle, clearDepth: number, clearStencil: number, clearDepthEnable: number, clearStencilEnable: number) => Result;


    public static LNRuntime_Initialize: () => Result;
    public static LNRuntime_Terminate: () => void;

    public static LNGLGraphicsContext_CreateFromCurrentGL: (width: number, height: number, outGraphicsContext: number) => Result;
    public static LNGraphicsContext_GetCurrentColorBuffer: (graphicsContext: Handle, outRenderTarget: number) => Result;
    public static LNGraphicsContext_GetCurrentDepthBuffer: (graphicsContext: Handle, outDepthBuffer: number) => Result;
    public static LNGraphicsContext_SubmitCommandList: (graphicsContext: Handle, graphicsCommandList: number) => Result;

    public static LNGraphicsCommandList_Create: (graphicsContext: Handle, outGraphicsCommandList: number) => Result;
    public static LNGraphicsCommandList_Reset: (graphicsCommandList: Handle, viewPoint: number) => Result;
    public static LNGraphicsCommandList_BeginRenderPass: (graphicsCommandList: Handle, descriptor: Handle, ouRenderPass: number) => Result;


    public static LNGraphicsViewPoint_Create: (outGraphicsViewPoint: number) => Result;
    public static LNGraphicsViewPoint_SetupPerspective2D: (graphicsViewPoint: Handle, x: number, y: number, z: number, width: number, height: number, nearZ: number, farZ: number) => Result;

    public static LNRenderPass_End: (renderPass: Handle) => Result;
    
    //public static LNRenderingContext_Create: (graphicsContext: Handle, outReturn: number) => Result;
    public static LNObject_Release: (obj: Handle) => Result;
    public static LNObject_Retain: (obj: Handle) => Result;
}
