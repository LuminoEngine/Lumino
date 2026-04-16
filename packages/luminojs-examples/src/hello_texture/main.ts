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
    Transform,
} from "luminojs";

async function main() {
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

    // 4. Load texture
    const texture = await Texture.loadFromURL(ctx, new URL("./assets/picture1.png", import.meta.url).href);

    // 5. Create Unlit material with texture
    const material = Material.createUnlit(ctx);
    material.setMainTexture(texture);

    // 6. Create quad mesh
    const mesh = Mesh.create(
        ctx,
        [
        { position: [-0.5, 0.5, 0], normal: [0, 0, 1], uv: [0, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [0.5, 0.5, 0], normal: [0, 0, 1], uv: [1, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [-0.5, -0.5, 0], normal: [0, 0, 1], uv: [0, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        { position: [0.5, -0.5, 0], normal: [0, 0, 1], uv: [1, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        ],
        new Uint32Array([0, 2, 1, 1, 2, 3]),
        [{ indexOffset: 0, indexCount: 6, materialIndex: 0 }],
    );
    mesh.setMaterial(0, material);

    // 7. Create perspective camera
    const camera = Camera.create();
    camera.setPerspective((60 * Math.PI) / 180, canvas.width / canvas.height, 0.1, 100);
    camera.setLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    // 8. Render loop
    const identity: Transform = { position: [0, 0, 0], rotation: [0, 0, 0, 1], scale: [1, 1, 1] };

    function frame() {
        const { renderer } = ctx.beginFrame();
        renderer.beginRenderPass(
            ctx,
            {
            colorAttachments: [
                {
                clearColor: [0.6, 0.85, 0.6, 1.0],
                loadOp: LoadOp.Clear,
                },
            ],
            },
            camera.handle,
        );
        renderer.drawMesh(mesh, identity);
        renderer.endRenderPass();
        ctx.endFrame();
        requestAnimationFrame(frame);
    }
    requestAnimationFrame(frame);
}

main();
