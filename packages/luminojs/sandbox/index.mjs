// Phase 6: ClearScreen サンドボックス
//
// luminojs の公開 API を使い、requestAnimationFrame ループで
// canvas を薄い緑色にクリアする。

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

    // 4. 描画ループ
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
