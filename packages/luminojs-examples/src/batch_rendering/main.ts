/**
 * BatchRendering サンプル
 *
 * Renderer.drawSprite と Renderer.drawMesh による透過的なバッチ描画のデモ。
 * 2 種類のマテリアルを交互に使う 1024 枚のスプライトが自動的にバッチ化され、
 * 最小限のドローコールで描画される。
 * 同じ Renderer でメッシュも描画する。
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
import type { Transform } from "luminojs";

const WINDOW_W = 1280;
const WINDOW_H = 720;
const SPRITE_COUNT = 1024;

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = WINDOW_W;
    canvas.height = WINDOW_H;

    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });

    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // テクスチャ
    const texture0 = await Texture.loadFromURL(
        new URL("../../public/picture1.png", import.meta.url).href);
    const texture1 = await Texture.loadFromURL(
        new URL("../../public/picture1.png", import.meta.url).href);

    // マテリアル - Unlit マテリアルを 2 つ
    const material0 = Material.createUnlit();
    material0.setMainTexture(texture0);
    material0.setColor(1.0, 0.8, 0.8, 1.0);

    const material1 = Material.createUnlit();
    material1.setMainTexture(texture1);
    material1.setColor(0.8, 0.8, 1.0, 1.0);

    // カメラ (正射影、原点は画面中央)
    const camera = Camera.create();
    const viewMatrix = Matrix4x4.makeLookAt(
        { x: 0, y: 0, z: 1 },
        { x: 0, y: 0, z: 0 },
        { x: 0, y: 1, z: 0 },
    );
    const projMatrix = Matrix4x4.makeOrthographic(WINDOW_W, WINDOW_H, -1000, 1000);
    camera.setMatrices(viewMatrix, projMatrix);

    // スプライトとメッシュの混在描画を示すための簡単な四角形メッシュ
    const mesh = Mesh.create(
        [
            { position: [-50, 50, 0], normal: [0, 0, 1], uv: [0, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
            { position: [50, 50, 0], normal: [0, 0, 1], uv: [1, 0], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
            { position: [-50, -50, 0], normal: [0, 0, 1], uv: [0, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
            { position: [50, -50, 0], normal: [0, 0, 1], uv: [1, 1], color: [1, 1, 1, 1], tangent: [1, 0, 0, 0] },
        ],
        new Uint32Array([0, 2, 1, 1, 2, 3]),
        [{ indexOffset: 0, indexCount: 6, materialIndex: 0 }],
    );
    mesh.setMaterial(0, material0);

    // 統計情報のオーバーレイ
    const statsEl = document.getElementById("stats")!;
    let lastTime = performance.now();
    let frameCount = 0;
    let fps = 0;
    let renderMs = 0;

    // 描画ループ
    let frame = 0;

    function render() {
        const t = frame * 0.02;
        const t0 = performance.now();

        const frameInfo = context.beginFrame();

        if (!frameInfo) {

            // デバイスロスト復旧待ち。このフレームはスキップする

            requestAnimationFrame(render);

            return;

        }

        const { renderer } = frameInfo;
        renderer.beginRenderPass(
            context,
            {
                colorAttachments: [
                    {
                        clearColor: [0.15, 0.15, 0.2, 1.0],
                        loadOp: LoadOp.Clear,
                    },
                ],
            },
            camera,
        );

        // スプライトを追加する
        const cols = 32;
        const spacing = 18.0;
        const offsetX = -(cols * spacing) * 0.5;
        const offsetY = -((SPRITE_COUNT / cols) * spacing) * 0.5;

        // ノード全体で 1 つの行列を共有し、スプライトごとには offset (x, y) だけ渡す。
        // 行列インスタンスはこの 1 個だけで済む (タイルマップ等の典型パターン)。
        const nodeXf = new Matrix4x4(); // 単位行列
        for (let i = 0; i < SPRITE_COUNT; i++) {
            const col = i % cols;
            const row = Math.floor(i / cols);
            const x = offsetX + col * spacing;
            const y = offsetY + row * spacing;
            const zIndex = Math.floor(Math.sin(t + i * 0.1) * 10.0);
            const mat = (i % 2 === 0) ? material0 : material1;

            renderer.drawSprite(
                mat, zIndex,
                nodeXf,
                x, y,
                16.0, 16.0,
                0.5, 0.5,
                0.0, 0.0, 1.0, 1.0,
                1.0, 1.0, 1.0, 1.0,
            );
        }

        // メッシュを追加する
        const meshTransform: Transform = {
            position: [200, 200, 0],
            rotation: [0, 0, 0, 1],
            scale: [1, 1, 1],
        };
        renderer.drawMesh(mesh, meshTransform, 0);

        renderer.endRenderPass();
        context.endFrame();

        // 描画時間と FPS を計測する
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
