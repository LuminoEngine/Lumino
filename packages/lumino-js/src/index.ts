
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";

import { Runtime, Handle, API } from "./Runtime";
console.log("aaa", LuminoFFIModule());

export * from "./Runtime";

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
