
// @ts-ignore
import LuminoFFIModule from "../lib/LuminoFFI.js";

console.log("LuminoFFIModule", LuminoFFIModule);

export interface RuntimeOptions {
    locateFile?: (path: string) => string;
}

export class Runtime {
    public static module: any;
    
    //public static returnDataHeap: Uint8Array;
    public static returnPointerPtr: number;
    public static returnPointerHEAPU8: Uint8Array;
    public static returnPointerView: Uint32Array | undefined;

    public static matrixMemoryCachePtr: number;
    //public static matrixMemoryCacheView: Float32Array;

    public static webglContextHandle: number = 0;

    public static initialize(options?: RuntimeOptions): Promise<void> {

        var moduleArg = {
            locateFile: options?.locateFile,
            LuminoGLSubmitCommandList2: (ptr: number) => {
                console.log("LuminoGLSubmitCommandList22", ptr);
                const size = 4 * 2;
                const p = ptr >> 2;
                const view = this.module.HEAP32.subarray(p, p+size);
                console.log("  buf", view, view[0], view[1]);
            },
        };

        //console.log("initialize...");
        return LuminoFFIModule(moduleArg).then((module: any) => {
            //console.log("module.HEAPU8.buffer", module.HEAPU8.buffer);
            //this.returnDataHeap = new Uint8Array(module.HEAPU8.buffer, 0, 4);
            //this.returnPointerArray = new Uint32Array(this.returnDataHeap.buffer, this.returnDataHeap.byteOffset, 1);

            

            this.module = module;
            console.log("module", module);

            this.returnPointerPtr = this.module._malloc(4);
            this.returnPointerHEAPU8 = module.HEAPU8.buffer;
            console.log("module.HEAPU8.buffer", module.HEAPU8.buffer);
            this.matrixMemoryCachePtr = this.module._malloc(4 * 16);
            
            //Runtime.module.HEAP8.set(new Uint8Array(buffer), ptr);

            API.LNMatrix_SetIdentity = module.cwrap("LNMatrix_SetIdentity", "void", ["number"]);
            
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
            API.LNGraphicsCommandList_BeginRenderPass = module.cwrap("LNGraphicsCommandList_BeginRenderPass", "number", ["number", "number", "number", "number"]);
            
            API.LNGraphicsViewPoint_Create = module.cwrap("LNGraphicsViewPoint_Create", "number", ["number"]);
            API.LNGraphicsViewPoint_SetupPerspective2D = module.cwrap("LNGraphicsViewPoint_SetupPerspective2D", "number", ["number", "number", "number", "number", "number", "number", "number", "number"]);

            API.LNRenderPass_End = module.cwrap("LNRenderPass_End", "number", ["number"]);

            API.LNTexture2D_CreateFromImageFileData = module.cwrap("LNTexture2D_CreateFromImageFileData", "number", ["number", "number", "number"]);

            API.LNMaterial_Create = module.cwrap("LNMaterial_Create", "number", ["number"]);
            API.LNMaterial_SetMainTexture = module.cwrap("LNMaterial_SetMainTexture", "number", ["number", "number"]);

            API.LNBatchRenderer_Get = module.cwrap("LNBatchRenderer_Get", "number", ["number"]);
            API.LNBatchRenderer_BeginBatch = module.cwrap("LNBatchRenderer_BeginBatch", "number", ["number", "number", "number", "number"]);
            API.LNBatchRenderer_EndBatch = module.cwrap("LNBatchRenderer_EndBatch", "number", ["number"]);
            API.LNBatchRenderer_DrawSprite = module.cwrap("LNBatchRenderer_DrawSprite", "number", ["number","number","number","number","number","number","number","number","number","number","number","number","number","number","number","number"]);

            //API.LNRenderingContext_Create = module.cwrap("LNRenderingContext_Create", "number", ["number", "number"]);
            API.LNObject_Release = module.cwrap("LNObject_Release", "number", ["number"]);
            API.LNObject_Retain = module.cwrap("LNObject_Retain", "number", ["number"]);




            // NOTE: cwrap でポインタを渡す方法のひとつに "array" があるが、これは JS->C の方向にしか使えない。 const uint8_t* みたいな感じ。
        });
    }

    public static getRenterPointerInfo(): [number, Uint32Array] {
        // メモリが足りなくなると wasmMemory.buffer が grow され、それに伴い module.HEAPU8 などの View も再作成される。
        // 古いものを使っていると C++ 側に情報を渡せなくなるので、変化があるならつくりなおす。
        // なお grow が発生すると次のようなログが出てくる。
        // Warning: Enlarging memory arrays, this is not fast! 391905280,47028633
        if (this.returnPointerView === undefined || this.returnPointerHEAPU8 != this.module.HEAPU8.buffer) {
            this.returnPointerView = new Uint32Array(this.module.HEAPU8.buffer, this.returnPointerPtr, 1);
        }
        return [this.returnPointerPtr, this.returnPointerView];
    }

    public static setMatrix(matrix: Float32Array): number {
        this.module.HEAPF32.set(matrix, this.matrixMemoryCachePtr >> 2);
        return this.matrixMemoryCachePtr;
    }

    /**
     * A helper for calling API functions.
     */
    public static safeCall(func: () => Result): void {
        const result = func();
        if (result != Result.LN_OK) {
            throw new Error(`Failed to call function. Result: ${result}`);
        }
    }

    /**
     * A helper for calling API functions that return a Handle with output arguments.
     */
    public static safeCallWithReturnHandle(func: (retptr: number) => Result): Handle {
        const [ptr, buffer] = this.getRenterPointerInfo();
        const result = func(ptr);
        if (result != Result.LN_OK) {
            throw new Error(`Failed to call function. Result: ${result}`);
        }
        return buffer[0];
    }
}


export type Handle = number;
export type StructHandle = number;

export enum Result {
    LN_OK = 0,
    LN_ERROR_UNKNOWN = -1,
}

export class API {
    // NOTE: パラメータの受け渡し
    // https://twiddlingbits.dev/docsite/gettingstarted/parameters/
    // int32_t, float, double は、 number が自動的に変換される。
    // int64_t はされない。int32_t 扱いされ、アドレスがずれる。

    public static LNMatrix_SetIdentity: (matrix: StructHandle) => void;

    public static LNRenderPassDescriptor_Get: () => StructHandle;
    public static LNRenderPassDescriptor_SetRenderTarget: (descriptor: StructHandle, index: number, renderTarget: Handle, clearR: number, clearG: number, clearB: number, clearA: number, clearEnable: number) => Result;
    public static LNRenderPassDescriptor_SetDepthBuffer: (descriptor: StructHandle, depthBuffer: Handle, clearDepth: number, clearStencil: number, clearDepthEnable: number, clearStencilEnable: number) => Result;


    public static LNRuntime_Initialize: () => Result;
    public static LNRuntime_Terminate: () => void;

    public static LNGLGraphicsContext_CreateFromCurrentGL: (width: number, height: number, outGraphicsContext: number) => Result;
    public static LNGraphicsContext_GetCurrentColorBuffer: (graphicsContext: Handle, outRenderTarget: number) => Result;
    public static LNGraphicsContext_GetCurrentDepthBuffer: (graphicsContext: Handle, outDepthBuffer: number) => Result;
    public static LNGraphicsContext_SubmitCommandList: (graphicsContext: Handle, graphicsCommandList: number) => Result;

    public static LNGraphicsCommandList_Create: (graphicsContext: Handle, outGraphicsCommandList: number) => Result;
    public static LNGraphicsCommandList_Reset: (graphicsCommandList: Handle) => Result;
    public static LNGraphicsCommandList_BeginRenderPass: (graphicsCommandList: Handle, descriptor: Handle, viewPoint: Handle, ouRenderPass: number) => Result;


    public static LNGraphicsViewPoint_Create: (outGraphicsViewPoint: number) => Result;
    public static LNGraphicsViewPoint_SetupPerspective2D: (graphicsViewPoint: Handle, x: number, y: number, z: number, width: number, height: number, nearZ: number, farZ: number) => Result;

    public static LNRenderPass_End: (renderPass: Handle) => Result;

    public static LNTexture2D_CreateFromImageFileData: (data: number, length: number, outTexture2D: number) => Result;

    public static LNMaterial_Create: (outMaterial: number) => Result;
    public static LNMaterial_SetMainTexture: (material: Handle, texture: Handle) => Result;

    public static LNBatchRenderer_Get: (outSpriteRenderer: number) => Result;
    public static LNBatchRenderer_BeginBatch: (spriteRenderer: Handle, graphicsCommandList: Handle, material: Handle, transform: number) => Result;
    public static LNBatchRenderer_EndBatch: (spriteRenderer: Handle) => Result;
    public static LNBatchRenderer_DrawSprite: (spriteRenderer: Handle, localTransformOrNull: number, width: number, height: number, anchorRatioX: number, anchorRatioY: number, uvRectX: number, uvRectY: number, uvRectW: number, uvRectH: number, r: number, g: number, b: number, a: number, baseDirection: number, billboardType: number) => Result;

    
    //public static LNRenderingContext_Create: (graphicsContext: Handle, outReturn: number) => Result;
    public static LNObject_Release: (obj: Handle) => Result;
    public static LNObject_Retain: (obj: Handle) => Result;
}
