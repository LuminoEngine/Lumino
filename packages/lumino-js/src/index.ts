
// @ts-ignore
//import LuminoFFIModule from "../dist/LuminoFFI.js";
import { DepthBuffer } from "./DepthBuffer";
import { GraphicsContext } from "./GraphicsContext";
export * from "./math/Types";
export * from "./math/Matrix";
export * from ".//GraphicsContext";
export * from "./GraphicsCommandList";
export * from "./GraphicsViewPoint";
export * from "./graphics/Material";
export * from "./Texture2D";
export * from "./graphics/SpriteRenderer.js";
import { RenderTexture } from "./RenderTargetTexture";

import { Runtime, API } from "./Runtime";
//console.log("aaa", LuminoFFIModule());

export * from "./Runtime";

// const canvas = document.getElementById("maincanvas") as HTMLCanvasElement;
// var gl = canvas.getContext("webgl");


export class WebGLGraphicsContext extends GraphicsContext {
    private _currentColorBuffer: RenderTexture;
    private _currentDepthBuffer: DepthBuffer;

    public override get currentColorBuffer(): RenderTexture {
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNGraphicsContext_GetCurrentColorBuffer(this._handle, r));
        this._currentColorBuffer._setHandle(handle, false);
        return this._currentColorBuffer;
    }

    public override get currentDepthBuffer(): DepthBuffer {
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNGraphicsContext_GetCurrentDepthBuffer(this._handle, r));
        this._currentDepthBuffer._setHandle(handle, false);
        return this._currentDepthBuffer
    }

    public constructor(webglContext: WebGLRenderingContext) {
        super();
        this._currentColorBuffer = new RenderTexture(this);
        this._currentDepthBuffer = new DepthBuffer(this);

        

        Runtime.webglContextHandle = Runtime.module.GL.registerContext(webglContext, {
            majorVersion: 2,
            minorVersion: 0,
        });
    
        // Lumino API を呼び出す前に、このコンテキストをカレントにする必要がある。
        Runtime.module.GL.makeContextCurrent(Runtime.webglContextHandle);


        this._handle = Runtime.safeCallWithReturnHandle((r) => API.LNGLGraphicsContext_CreateFromCurrentGL(webglContext.canvas.width, webglContext.canvas.height, r));

        console.log("LNGLGraphicsContext_CreateFromCurrentGL", webglContext.canvas.width, webglContext.canvas.height);
        // this._handle = Runtime.returnPointerView[0];
        // console.log("handle3", this._handle, Runtime.returnPointerView.byteOffset);
    }

    public makeCurrentContext(): void {
        Runtime.module.GL.makeContextCurrent(Runtime.webglContextHandle);
    }
}

// export class RenderingContext {
//     public constructor(graphicsContext: GraphicsContext) {

//         API.LNRenderingContext_Create(graphicsContext._handle, Runtime.returnPointerView.byteOffset);
//         console.log("handle4", Runtime.returnPointerView[0], Runtime.returnPointerView.byteOffset);
        
        
//     }
// }
