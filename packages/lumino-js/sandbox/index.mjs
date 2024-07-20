import * as Lumino from "./../dist/lumino.mjs";

//const image = new Image();
const imagePath = "./icon256.png";


let graphcisContext = undefined;
let commandList = undefined;
let viewPoint = undefined;
let texture = undefined;
let material = undefined;

Lumino.Runtime.initialize().then(() => {

    console.log("Lumino initialized2");
    Lumino.API.LNRuntime_Initialize();

    const canvas = document.getElementById("maincanvas");
    var gl = canvas.getContext("webgl2");

    console.log("UNIFORM_BUFFER_OFFSET_ALIGNMENT", gl.getParameter(gl.UNIFORM_BUFFER_OFFSET_ALIGNMENT));

    graphcisContext = new Lumino.WebGLGraphicsContext(gl);
    commandList = graphcisContext.createCommandList();
    viewPoint = new Lumino.GraphicsViewPoint();

    console.log("=== initialized ===");

    {
        const xhr = new XMLHttpRequest();
        xhr.open("GET", imagePath);
        xhr.responseType = "arraybuffer";
        xhr.onload = () => {
            console.log("xhr.onload", xhr.response);
            texture = new Lumino.Texture2D(xhr.response);
            material = new Lumino.Material();
            material.setMainTexture(texture);
        }
        xhr.onerror = (e) => {
            console.error(`HttpRequestError: ${xhr.statusText ?? "No error infomatino."} (${imagePath})`);
        };
        xhr.send();
        
    }
    


    //Lumino.API.LNRuntime_Terminate();

function isReady() {
    return texture !== undefined;
}

function render() {

    if (isReady()) {
        console.log("=== Begin Frame ===");
        viewPoint.setupPerspective2D(800, 600);
    
        commandList.reset(viewPoint);
        {
            const renderPass = commandList.beginRenderPass();
            const spriteRenderer = Lumino.SpriteRenderer.get();
            spriteRenderer.beginBatch(commandList, material, new Lumino.Matrix());
            spriteRenderer.drawSprite(
                null,
                { width: 256, height: 256 },
                { x: 0, y: 0 },
                {x: 0, y: 0, width: 1, height: 1},
                {r: 1, g: 1, b: 1, a: 1},
                Lumino.SpriteBaseDirection.Basic2D,
                Lumino.BillboardType.None);
            spriteRenderer.endBatch();
            renderPass.end();
        }
    
        graphcisContext.submitCommandList(commandList);
        console.log("=== End Frame ===");
    }

    window.requestAnimationFrame(render);
}

render();



    // console.log("canvas", canvas);
    // console.log("Lumino.Runtime.module.GL", Lumino.Runtime.module.GL);
    // const emGLContextHandle = Lumino.Runtime.module.GL.registerContext(gl, {
    //     // option は Effekseer を参考にしています。
    //     // Emscripte の生成するコードではコンテキスト情報として保持するものの参照はされていないようなので、仮の値を設定しておきます。
    //     // ※設定しておかないと ReferenceError が発生します。
    //     majorVersion: 1,
    //     minorVersion: 0,
    //     enableExtensionsByDefault: true,
    // });
    // console.log("emGLContextHandle", emGLContextHandle);

    
    // gl.clearColor(0.0, 0.0, 1.0, 1.0);
    // gl.clear(gl.COLOR_BUFFER_BIT);

    /*
    if (!canvas.getContextSafariWebGL2Fixed) {
        canvas.getContextSafariWebGL2Fixed = canvas.getContext;
        function fixedGetContext(ver, attrs) {
            var gl = canvas.getContextSafariWebGL2Fixed(ver, attrs);
            return ((ver == 'webgl') == (gl instanceof WebGLRenderingContext)) ? gl : null;
        }
        canvas.getContext = fixedGetContext;
    }

      var ctx =
        (canvas.getContext("webgl", webGLContextAttributes)
          // https://caniuse.com/#feat=webgl
          );

      if (!ctx) return 0;
      */
});


// const promise = LuminoFFI();
// promise.then((module) => {
//     console.log("loaded 1", module);


//     Lumino.API = {
//         LNRuntime_Initialize: module.cwrap("LNRuntime_Initialize", "number", []),
//     };


//     const result = Lumino.API.LNRuntime_Initialize();
//     console.log("Lumino initialized", result);
// });

