/**
 * Stencil Mask example
 *
 * 中央の円形メッシュをステンシルマスクとしてプッシュし、
 * マスク領域内でだけ敷き詰めたスプライトが見えるデモ。
 */
import {
    Runtime,
    GraphicsContext,
    LoadOp,
    Texture,
    Material,
    Mesh,
    Camera,
    Matrix4x4,
} from "luminojs";
import type { Transform, Vertex } from "luminojs";

const WINDOW_W = 1280;
const WINDOW_H = 720;

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = WINDOW_W;
    canvas.height = WINDOW_H;

    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });
    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // --- Mask material (StencilMask compiled shader) ---
    // StencilMask シェーダは texture.a * color.a * vertex.a < 0.5 で discard するため、
    // デフォルト白テクスチャ + color α=1 + 頂点 α=1 で全域を有効マスクにする。
    const maskResp = await fetch(
        new URL("../../public/StencilMask.lcsh", import.meta.url).href);
    const maskData = new Uint8Array(await maskResp.arrayBuffer());
    const maskMat = Material.createFromCompiledShader(maskData);
    maskMat.setColor(1, 1, 1, 1);

    // --- Mask mesh: 中央の円 (三角形ファン) ---
    const segments = 64;
    const radius = 220;
    const maskVerts: Vertex[] = [
        {
            position: [0, 0, 0], normal: [0, 0, 1], uv: [0.5, 0.5],
            color: [1, 1, 1, 1], tangent: [1, 0, 0, 0],
        },
    ];
    const maskIdx: number[] = [];
    for (let i = 0; i < segments; i++) {
        const a = (i / segments) * Math.PI * 2;
        maskVerts.push({
            position: [Math.cos(a) * radius, Math.sin(a) * radius, 0],
            normal: [0, 0, 1],
            uv: [0.5 + Math.cos(a) * 0.5, 0.5 + Math.sin(a) * 0.5],
            color: [1, 1, 1, 1],
            tangent: [1, 0, 0, 0],
        });
        const next = (i + 1) % segments;
        maskIdx.push(0, 1 + i, 1 + next);
    }
    const maskMesh = Mesh.create(
        maskVerts,
        new Uint32Array(maskIdx),
        [{ indexOffset: 0, indexCount: maskIdx.length, materialIndex: 0 }],
    );
    maskMesh.setMaterial(0, maskMat);

    // --- Content: batched sprites ---
    const texture = await Texture.loadFromURL(
        new URL("../../public/picture1.png", import.meta.url).href);
    const spriteMat = Material.createUnlit();
    spriteMat.setMainTexture(texture);

    // --- Camera (orthographic, origin at center) ---
    const camera = Camera.create();
    const viewMatrix = Matrix4x4.makeLookAt(
        { x: 0, y: 0, z: 1 },
        { x: 0, y: 0, z: 0 },
        { x: 0, y: 1, z: 0 },
    );
    const projMatrix = Matrix4x4.makeOrthographic(WINDOW_W, WINDOW_H, -1000, 1000);
    camera.setMatrices(viewMatrix, projMatrix);

    const maskTransform: Transform = {
        position: [0, 0, 0],
        rotation: [0, 0, 0, 1],
        scale: [1, 1, 1],
    };

    // Stats overlay
    const statsEl = document.getElementById("stats")!;
    let lastTime = performance.now();
    let frameCount = 0;
    let fps = 0;
    let renderMs = 0;

    let frame = 0;

    function render() {
        const t0 = performance.now();
        const phase = frame * 0.02;

        const { renderer, depthBuffer } = context.beginFrame();

        renderer.beginRenderPass(
            context,
            {
                colorAttachments: [
                    { clearColor: [0.08, 0.08, 0.12, 1.0], loadOp: LoadOp.Clear },
                ],
                // ステンシルマスクはステンシルバッファを必要とするため必須。
                depthStencil: { depthBuffer },
            },
            camera,
        );

        // 1. 円形マスクをプッシュ
        renderer.pushStencilMask(maskMesh, maskTransform, maskMat);

        // 2. マスク内にスプライトを敷き詰める
        const cols = 40;
        const rows = 24;
        const spacing = 32;
        const offsetX = -(cols * spacing) * 0.5;
        const offsetY = -(rows * spacing) * 0.5;
        for (let i = 0; i < cols * rows; i++) {
            const col = i % cols;
            const row = Math.floor(i / cols);
            const x = offsetX + col * spacing;
            const y = offsetY + row * spacing;
            const wobble = Math.sin(phase + col * 0.2 + row * 0.15) * 3.0;
            renderer.drawSprite(
                spriteMat, 0,
                x, y + wobble, 0.0,
                28.0, 28.0,
                0.5, 0.5,
                0.0, 0.0, 1.0, 1.0,
                1.0, 1.0, 1.0, 1.0,
                0.0,
            );
        }

        // 3. マスクをポップ
        renderer.popStencilMask();

        renderer.endRenderPass();
        context.endFrame();

        const t1 = performance.now();
        renderMs = t1 - t0;
        frameCount++;
        if (t1 - lastTime >= 1000) {
            fps = frameCount;
            frameCount = 0;
            lastTime = t1;
        }
        statsEl.textContent = `FPS: ${fps}\nrender: ${renderMs.toFixed(2)} ms`;

        frame++;
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

main();
