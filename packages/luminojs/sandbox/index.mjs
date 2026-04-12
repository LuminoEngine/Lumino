// Phase 6: ClearScreen sandbox
//
// Uses the luminojs public API to clear the canvas with light green
// via a requestAnimationFrame loop.

import {
    Runtime,
    Instance,
    Window,
    GraphicsBackend,
    LoadOp,
} from "../lib/luminojs.mjs";

const logEl = document.getElementById("log");
function appendLog(line) {
    logEl.textContent += line + "\n";
    logEl.scrollTop = logEl.scrollHeight;
    console.log(line);
}

try {
    // 1. Load WASM module
    await Runtime.initialize({
        wasmPath: "../lib/LuminoC.wasm",
        print:    (t) => appendLog("[stdout] " + t),
        printErr: (t) => appendLog("[stderr] " + t),
    });
    appendLog("Runtime initialized.");

    // 2. Initialize Lumino instance (creates WebGPU device)
    await Instance.initialize({ preferredBackend: GraphicsBackend.WebGPU });
    appendLog("Instance initialized.");

    // 3. Create Window from canvas
    const win = await Window.createFromCanvas("#my_canvas", 1280, 720);
    const ctx = win.getGraphicsContext();
    appendLog("Window + GraphicsContext created.");

    // 4. Render loop
    function frame() {
        ctx.beginFrame().then(({ renderer }) => {
            renderer.beginRenderPass(ctx, {
                colorAttachments: [{
                    clearColor: [0.6, 0.85, 0.6, 1.0],
                    loadOp: LoadOp.Clear,
                }],
            });
            renderer.endRenderPass();
            ctx.endFrame();
            requestAnimationFrame(frame);
        });
    }
    requestAnimationFrame(frame);
    appendLog("Render loop started.");
} catch (e) {
    appendLog("ERROR: " + e);
    console.error(e);
}
