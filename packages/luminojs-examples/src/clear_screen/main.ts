import {
    Runtime,
    GraphicsContext,
    LoadOp,
} from "luminojs";

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;

    // Load WASM module and initialize Lumino instance (creates WebGPU device)
    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });

    // Create GraphicsContext from canvas
    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // Render loop
    function frame() {
        const { renderer } = context.beginFrame();
        renderer.beginRenderPass(context, {
            colorAttachments: [
                {
                    clearColor: [0.6, 0.85, 0.6, 1.0],
                    loadOp: LoadOp.Clear,
                },
            ],
        });
        renderer.endRenderPass();
        context.endFrame();
        requestAnimationFrame(frame);
    }
    requestAnimationFrame(frame);
}

main();
