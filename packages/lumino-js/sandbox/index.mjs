import * as Lumino from "../lib/lumino.mjs";

//const image = new Image();
const imagePath = "./icon256.png";

const USE_WEBGPU = true;

let graphcisContext = undefined;
let commandList = undefined;
let viewPoint = undefined;
let texture = undefined;
let material = undefined;
let frameCount = 0;

const options = {
    wasmPath: "../lib/LuminoFFI.wasm",
}

Lumino.Runtime.initialize(options).then(async () => {

    console.log("Lumino initialized2");
    Lumino.API.LNInstance_Initialize();

    if (USE_WEBGPU) {
        graphcisContext = await Lumino.WebGPUGraphicsContext.create("#my_canvas");
    }
    else {
        const canvas = document.getElementById("my_canvas");
        var gl = canvas.getContext("webgl2");
        console.log("UNIFORM_BUFFER_OFFSET_ALIGNMENT", gl.getParameter(gl.UNIFORM_BUFFER_OFFSET_ALIGNMENT));
        graphcisContext = new Lumino.WebGLGraphicsContext(gl);
    }

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
    


    //Lumino.API.LNInstance_Terminate();

    function isReady() {
        return texture !== undefined;
    }

    function render() {

        if (isReady()) {
            console.log("=== Begin Frame ===");
            viewPoint.setupPerspective2D(800, 600);

            graphcisContext.prepareFrame(800, 600);

            const commandList = graphcisContext.commandList();
        
            {
                const desc = {
                    renderTargets: [
                        {
                            renderTarget: graphcisContext.currentColorBuffer,
                            clearColor: { r: 0, g: 1, b: 1, a: 1 }
                        }
                    ],
                    depthBuffer: {
                        depthBuffer: graphcisContext.currentDepthBuffer,
                        clearDepth: 1.0,
                        clearStencil: 0,
                    }
                };

                const renderPass = commandList.beginRenderPass(desc, viewPoint);
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

        //if (frameCount < 2) {
            window.requestAnimationFrame(render);
        //}
        frameCount++;
    }

    render();
});
