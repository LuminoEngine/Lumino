
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";
import { DepthBuffer } from "./DepthBuffer";
import { GraphicsContext } from "./GraphicsContext";
export * from "./math/Matrix";
export * from "./graphics/Material";
export * from "./GraphicsViewPoint";
export * from "./Texture2D";
import { RenderTargetTexture } from "./RenderTargetTexture";

import { Runtime, Handle, API } from "./Runtime";
console.log("aaa", LuminoFFIModule());

export * from "./Runtime";

// const canvas = document.getElementById("maincanvas") as HTMLCanvasElement;
// var gl = canvas.getContext("webgl");


export class WebGLGraphicsContext extends GraphicsContext {
    private _currentColorBuffer: RenderTargetTexture;
    private _currentDepthBuffer: DepthBuffer;

    public override get currentColorBuffer(): RenderTargetTexture {
        API.LNGraphicsContext_GetCurrentColorBuffer(this._handle, Runtime.returnPointerView.byteOffset);
        this._currentColorBuffer._setHandle(Runtime.returnPointerView[0], false);
        return this._currentColorBuffer;
    }

    public override get currentDepthBuffer(): DepthBuffer {
        API.LNGraphicsContext_GetCurrentDepthBuffer(this._handle, Runtime.returnPointerView.byteOffset);
        this._currentDepthBuffer._setHandle(Runtime.returnPointerView[0], false);
        return this._currentDepthBuffer
    }

    public constructor(webglContext: WebGLRenderingContext) {
        super();
        this._currentColorBuffer = new RenderTargetTexture(this);
        this._currentDepthBuffer = new DepthBuffer(this);

        Runtime.webglContextHandle = Runtime.module.GL.registerContext(webglContext, {
            majorVersion: 1,
            minorVersion: 0,
        });
    
        // Lumino API を呼び出す前に、このコンテキストをカレントにする必要がある。
        Runtime.module.GL.makeContextCurrent(Runtime.webglContextHandle);


        // console.log("module.HEAPU8.buffer", module.HEAPU8.buffer);
        // const dataHeap = new Uint8Array(module.HEAPU8.buffer, 0, 4);
        API.LNGLGraphicsContext_CreateFromCurrentGL(800, 600, Runtime.returnPointerView.byteOffset);

        // console.log("dataHeap", dataHeap);
        // const handle = new Uint32Array(dataHeap.buffer, dataHeap.byteOffset, 1)[0];
        this._handle = Runtime.returnPointerView[0];
        console.log("handle3", this._handle, Runtime.returnPointerView.byteOffset);
    }
}

// export class RenderingContext {
//     public constructor(graphicsContext: GraphicsContext) {

//         API.LNRenderingContext_Create(graphicsContext._handle, Runtime.returnPointerView.byteOffset);
//         console.log("handle4", Runtime.returnPointerView[0], Runtime.returnPointerView.byteOffset);
        
        
//     }
// }
