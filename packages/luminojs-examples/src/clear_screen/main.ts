import {
    Runtime,
    GraphicsContext,
    LoadOp,
    Camera,
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

    const camera = Camera.create();

    // Render loop
    function frame() {
        const frameInfo = context.beginFrame();
        if (!frameInfo) {
            // デバイスロスト復旧待ち。このフレームはスキップする
            requestAnimationFrame(frame);
            return;
        }
        const { renderer } = frameInfo;
        renderer.beginRenderPass(context, {
            colorAttachments: [
                {
                    clearColor: [0.6, 0.85, 0.6, 1.0],
                    loadOp: LoadOp.Clear,
                },
            ],
        }, camera);
        renderer.endRenderPass();
        context.endFrame();
        requestAnimationFrame(frame);
    }
    requestAnimationFrame(frame);
}

main();
