// HelloTexture sandbox
//
// Loads a PNG texture and renders it on a quad mesh using an Unlit material.
// Equivalent to packages/LuminoC/examples/HelloTexture/Main.cpp.

import {
    Runtime,
    Instance,
    Window,
    GraphicsBackend,
    LoadOp,
    Texture,
    Material,
    Mesh,
    Camera,
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
    appendLog("Build timestamp: " + Runtime.getBuildTimestamp());

    // 2. Initialize Lumino instance (creates WebGPU device)
    await Instance.initialize({ preferredBackend: GraphicsBackend.WebGPU });
    appendLog("Instance initialized.");

    // 3. Create Window from canvas
    const win = await Window.createFromCanvas("#my_canvas", 1280, 720);
    const ctx = win.getGraphicsContext();
    appendLog("Window + GraphicsContext created.");

    // 4. Load texture
    const texture = await Texture.loadFromURL(ctx, "./assets/picture1.png");
    appendLog("Texture loaded.");

    // 5. Create Unlit material with texture
    const material = Material.createUnlit(ctx);
    material.setMainTexture(texture);
    appendLog("Material created.");

    // 6. Create quad mesh (4 vertices, 6 indices, CCW winding)
    //   v0(-0.5, 0.5) --- v1(0.5, 0.5)
    //      |           /      |
    //   v2(-0.5,-0.5) --- v3(0.5,-0.5)
    const mesh = Mesh.create(ctx, [
        { position: [-0.5,  0.5, 0], normal: [0, 0, 1], uv: [0, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [ 0.5,  0.5, 0], normal: [0, 0, 1], uv: [1, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [-0.5, -0.5, 0], normal: [0, 0, 1], uv: [0, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [ 0.5, -0.5, 0], normal: [0, 0, 1], uv: [1, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
    ], new Uint32Array([0, 2, 1, 1, 2, 3]),
       [{ indexOffset: 0, indexCount: 6, materialIndex: 0 }]);
    mesh.setMaterial(0, material);
    appendLog("Mesh created.");

    // 7. Create perspective camera
    const camera = Camera.create();
    camera.setPerspective(60 * Math.PI / 180, 1280 / 720, 0.1, 100);
    camera.setLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
    appendLog("Camera created.");

    // 8. Render loop
    const identity = { position: [0, 0, 0], rotation: [0, 0, 0, 1], scale: [1, 1, 1] };

    function frame() {
        ctx.beginFrame().then(({ renderer }) => {
            renderer.beginRenderPass(ctx, {
                colorAttachments: [{
                    clearColor: [0.6, 0.85, 0.6, 1.0],
                    loadOp: LoadOp.Clear,
                }],
            }, camera.handle);
            renderer.drawMesh(mesh, identity);
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
