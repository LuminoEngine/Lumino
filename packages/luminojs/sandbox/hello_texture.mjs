// HelloTexture サンドボックス
//
// PNG テクスチャをロードし、Unlit マテリアルで四角形メッシュに描画する。
// packages/LuminoC/examples/HelloTexture/Main.cpp と同等。

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
    // 1. WASM モジュールをロードする
    await Runtime.initialize({
        wasmPath: "../lib/LuminoC.wasm",
        print:    (t) => appendLog("[stdout] " + t),
        printErr: (t) => appendLog("[stderr] " + t),
    });
    appendLog("Runtime initialized.");
    appendLog("Build timestamp: " + Runtime.getBuildTimestamp());

    // 2. Lumino インスタンスを初期化する (WebGPU デバイスが作成される)
    await Instance.initialize({ preferredBackend: GraphicsBackend.WebGPU });
    appendLog("Instance initialized.");

    // 3. canvas から Window を作成する
    const win = await Window.createFromCanvas("#my_canvas", 1280, 720);
    const ctx = win.getGraphicsContext();
    appendLog("Window + GraphicsContext created.");

    // 4. テクスチャをロードする
    const texture = await Texture.loadFromURL(ctx, "./assets/picture1.png");
    appendLog("Texture loaded.");

    // 5. テクスチャ付きの Unlit マテリアルを作成する
    const material = Material.createUnlit(ctx);
    material.setMainTexture(texture);
    appendLog("Material created.");

    // 6. 四角形メッシュを作成する (頂点 4 つ、インデックス 6 つ、CCW ワインディング)
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

    // 7. 透視投影カメラを作成する
    const camera = Camera.create();
    camera.setPerspective(60 * Math.PI / 180, 1280 / 720, 0.1, 100);
    camera.setLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
    appendLog("Camera created.");

    // 8. 描画ループ
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
