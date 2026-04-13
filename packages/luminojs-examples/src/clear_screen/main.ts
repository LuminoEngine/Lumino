import {
  Runtime,
  Instance,
  Window,
  GraphicsBackend,
  LoadOp,
} from "luminojs";

const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
canvas.width = canvas.clientWidth;
canvas.height = canvas.clientHeight;

// 1. Load WASM module
await Runtime.initialize({
  wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
  print: (t: string) => console.log("[stdout]", t),
  printErr: (t: string) => console.warn("[stderr]", t),
});

// 2. Initialize Lumino instance (creates WebGPU device)
await Instance.initialize({ preferredBackend: GraphicsBackend.WebGPU });

// 3. Create Window from canvas
const win = await Window.createFromCanvas("#my_canvas", canvas.width, canvas.height);
const ctx = win.getGraphicsContext();

// 4. Render loop
function frame() {
  ctx.beginFrame().then(({ renderer }) => {
    renderer.beginRenderPass(ctx, {
      colorAttachments: [
        {
          clearColor: [0.6, 0.85, 0.6, 1.0],
          loadOp: LoadOp.Clear,
        },
      ],
    });
    renderer.endRenderPass();
    ctx.endFrame();
    requestAnimationFrame(frame);
  });
}
requestAnimationFrame(frame);
