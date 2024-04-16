
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";
console.log("aaa", LuminoFFIModule());

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
            API.LNRuntime_Initialize = module.cwrap("LNRuntime_Initialize", "number", []);
            API.LNRuntime_Terminate = module.cwrap("LNRuntime_Terminate", "void", []);
            API.LNGraphicsContext_CreateFromOpenGL = module.cwrap("LNGraphicsContext_CreateFromOpenGL", "number", ["number", "number", "number"]);
            API.LNRenderingContext_Create = module.cwrap("LNRenderingContext_Create", "number", ["number", "number"]);
            API.LNObject_Release = module.cwrap("LNObject_Release", "number", ["number"]);
            API.LNObject_Retain = module.cwrap("LNObject_Retain", "number", ["number"]);




            // NOTE: cwrap でポインタを渡す方法のひとつに "array" があるが、これは JS->C の方向にしか使えない。 const uint8_t* みたいな感じ。
        });
    }
}

export enum Result {
    LN_OK = 0,
    LN_ERROR_UNKNOWN = -1,
}

type Handle = number;

export class API {
    public static LNRuntime_Initialize: () => Result;
    public static LNRuntime_Terminate: () => void;
    public static LNGraphicsContext_CreateFromOpenGL: (width: number, height: number, outReturn: number) => Result;
    public static LNRenderingContext_Create: (graphicsContext: Handle, outReturn: number) => Result;
    public static LNObject_Release: (obj: Handle) => Result;
    public static LNObject_Retain: (obj: Handle) => Result;
}

// const canvas = document.getElementById("maincanvas") as HTMLCanvasElement;
// var gl = canvas.getContext("webgl");


export class GraphicsContext {
    public _handle: Handle;
}

export class WebGLGraphicsContext extends GraphicsContext {

    public constructor(webglContext: WebGLRenderingContext) {
        super();

        Runtime.webglContextHandle = Runtime.module.GL.registerContext(webglContext, {
            majorVersion: 1,
            minorVersion: 0,
        });
        console.log("Runtime.webglContextHandle", Runtime.webglContextHandle);
    
        // Lumino API を呼び出す前に、このコンテキストをカレントにする必要がある。
        Runtime.module.GL.makeContextCurrent(Runtime.webglContextHandle);


        // console.log("module.HEAPU8.buffer", module.HEAPU8.buffer);
        // const dataHeap = new Uint8Array(module.HEAPU8.buffer, 0, 4);
        API.LNGraphicsContext_CreateFromOpenGL(800, 600, Runtime.returnPointerView.byteOffset);

        // console.log("dataHeap", dataHeap);
        // const handle = new Uint32Array(dataHeap.buffer, dataHeap.byteOffset, 1)[0];
        this._handle = Runtime.returnPointerView[0];
        console.log("handle3", this._handle, Runtime.returnPointerView.byteOffset);
    }
}

export class RenderingContext {
    public constructor(graphicsContext: GraphicsContext) {

        API.LNRenderingContext_Create(graphicsContext._handle, Runtime.returnPointerView.byteOffset);
        console.log("handle4", Runtime.returnPointerView[0], Runtime.returnPointerView.byteOffset);
        
        
    }
}
