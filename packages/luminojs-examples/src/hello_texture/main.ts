import {
    Runtime,
    GraphicsContext,
    LoadOp,
    Texture,
    Material,
    Mesh,
    Camera,
    Matrix4x4,
    Transform,
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

    // Load texture
    const texture = await Texture.loadFromURL(new URL("./assets/picture1.png", import.meta.url).href);

    // Create Unlit material with texture
    const material = Material.createUnlit();
    material.setMainTexture(texture);

    // Create quad mesh
    const mesh = Mesh.create(
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
    const viewMatrix = Matrix4x4.makeLookAt(
        { x: 0, y: 0, z: 3 }, { x: 0, y: 0, z: 0 }, { x: 0, y: 1, z: 0 });
    const projMatrix = Matrix4x4.makePerspective(
        (60 * Math.PI) / 180, canvas.width / canvas.height, 0.1, 100);
    camera.setMatrices(viewMatrix, projMatrix);

    // 8. Render loop
    const identity: Transform = { position: [0, 0, 0], rotation: [0, 0, 0, 1], scale: [1, 1, 1] };

    function frame() {
        const frameInfo = context.beginFrame();
        if (!frameInfo) {
            // デバイスロスト復旧待ち。このフレームはスキップする
            requestAnimationFrame(frame);
            return;
        }
        const { renderer } = frameInfo;
        renderer.beginRenderPass(
            context,
            {
                colorAttachments: [
                    {
                        clearColor: [0.6, 0.85, 0.6, 1.0],
                        loadOp: LoadOp.Clear,
                    },
                ],
            },
            camera,
        );
        renderer.drawMesh(mesh, identity);
        renderer.endRenderPass();
        context.endFrame();
        requestAnimationFrame(frame);
    }
    requestAnimationFrame(frame);
}

main();
