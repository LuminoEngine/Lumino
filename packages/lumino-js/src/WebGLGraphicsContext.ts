import { GraphicsContext } from "./GraphicsContext";
import { API, Runtime } from "./Runtime";

export class WebGLGraphicsContext extends GraphicsContext {

    public constructor(webglContext: WebGLRenderingContext) {
        super();

        

        Runtime.webglContextHandle = Runtime.module.GL.registerContext(webglContext, {
            majorVersion: 2,
            minorVersion: 0,
        });
    
        // Lumino API を呼び出す前に、このコンテキストをカレントにする必要がある。
        Runtime.module.GL.makeContextCurrent(Runtime.webglContextHandle);


        this._handle = Runtime.safeCallWithReturnHandle((r) => API.LNGraphicsContext_CreateFromCurrentGL(webglContext.canvas.width, webglContext.canvas.height, r));

        console.log("LNGraphicsContext_CreateFromCurrentGL", webglContext.canvas.width, webglContext.canvas.height);
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
