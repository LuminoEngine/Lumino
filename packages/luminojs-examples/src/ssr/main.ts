/**
 * SSR (Screen Space Reflection)
 *
 * 地面の平面と回転する三角形を G-Buffer に描画し、
 * スクリーンスペース反射と合成する。
 *
 * レンダリングパイプライン:
 *   パス 1: G-Buffer (アルベド、ワールド法線 + 深度、メタリック/ラフネス/AO)
 *   パス 2: SSR + 合成 (フルスクリーンのポストプロセス)
 */
import {
    Runtime,
    GraphicsContext,
    LoadOp,
    TextureFormat,
    Texture,
    Material,
    Mesh,
    Camera,
    Matrix4x4,
    CullMode,
} from "luminojs";
import type { Transform } from "luminojs";

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    const W = canvas.width;
    const H = canvas.height;

    // WASM モジュールをロードし、Lumino インスタンスを初期化する (WebGPU デバイスが作成される)
    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });

    // canvas から GraphicsContext を作成する
    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // レンダーターゲット
    const gbufferA = Texture.createRenderTargetEx(context, W, H, TextureFormat.RGBA8_UNORM);
    const gbufferB = Texture.createRenderTargetEx(context, W, H, TextureFormat.RGBA32_FLOAT);
    const gbufferC = Texture.createRenderTargetEx(context, W, H, TextureFormat.RGBA8_UNORM);
    const sceneDepth = Texture.createDepthStencil(context, W, H);

    // マテリアル
    const gridTexture = await Texture.loadFromURL(
        new URL("../../public/CheckerGridGray1.png", import.meta.url).href,
    );

    // シーン用マテリアル (Unlit)
    const groundMaterial = Material.createUnlit();
    groundMaterial.setColor(0.4, 0.4, 0.4, 1.0);
    //groundMaterial.setMainTexture(gridTexture);

    const triangleMaterial = Material.createUnlit();
    triangleMaterial.setCullMode(CullMode.None);  // 両面描画
    triangleMaterial.setColor(1.0, 1.0, 1.0, 1.0);

    // SSR 用マテリアル (コンパイル済みシェーダから作成)
    const ssrResp = await fetch(new URL("../../public/SSR.lcsh", import.meta.url).href);
    const ssrData = new Uint8Array(await ssrResp.arrayBuffer());
    const matSSR = Material.createFromCompiledShader(ssrData);
    matSSR.setFloat4("ssrSettings", [10.0, 0.05, 0.3, 128.0]);
    matSSR.setNamedTexture("u_gbufferA", gbufferA);
    matSSR.setNamedTexture("u_gbufferB", gbufferB);
    matSSR.setNamedTexture("u_gbufferC", gbufferC);

    // 地面の平面: Y=0 に 4 頂点
    const groundMesh = Mesh.create(
        [
            { position: [-2, 0, -2], normal: [0, 1, 0], uv: [0, 0], color: [0.5, 0.5, 0.5, 1], tangent: [1, 0, 0, 0] },
            { position: [ 2, 0, -2], normal: [0, 1, 0], uv: [1, 0], color: [0.5, 0.5, 0.5, 1], tangent: [1, 0, 0, 0] },
            { position: [-2, 0,  2], normal: [0, 1, 0], uv: [0, 1], color: [0.5, 0.5, 0.5, 1], tangent: [1, 0, 0, 0] },
            { position: [ 2, 0,  2], normal: [0, 1, 0], uv: [1, 1], color: [0.5, 0.5, 0.5, 1], tangent: [1, 0, 0, 0] },
        ],
        new Uint32Array([0, 2, 1, 1, 2, 3]),
        [{ indexOffset: 0, indexCount: 6, materialIndex: 0 }],
    );
    groundMesh.setMaterial(0, groundMaterial);

    // 回転する三角形: 地面の上に浮かせる
    const triMesh = Mesh.create(
        [
            { position: [ 0, 1, 0], normal: [0, 0, 1], uv: [0.5, 0], color: [1, 0, 0, 1], tangent: [1, 0, 0, 0] },
            { position: [ 1, 0, 0], normal: [0, 0, 1], uv: [1, 1],   color: [0, 1, 0, 1], tangent: [1, 0, 0, 0] },
            { position: [-1, 0, 0], normal: [0, 0, 1], uv: [0, 1],   color: [0, 0, 1, 1], tangent: [1, 0, 0, 0] },
        ],
        new Uint32Array([0, 2, 1]),
        [{ indexOffset: 0, indexCount: 3, materialIndex: 0 }],
    );
    triMesh.setMaterial(0, triangleMaterial);

    const camera = Camera.create();
    const viewMatrix = Matrix4x4.makeLookAt(
        { x: 0, y: 2.5, z: 5 }, { x: 0, y: 0.5, z: 0 }, { x: 0, y: 1, z: 0 });
    const projMatrix = Matrix4x4.makePerspective(
        (45 * Math.PI) / 180, W / H, 0.1, 10.0);
    camera.setMatrices(viewMatrix, projMatrix);

    const groundTransform: Transform = {
        position: [0, 0, 0],
        rotation: [0, 0, 0, 1],
        scale: [1, 1, 1],
    };

    const statsEl = document.getElementById("stats")!;
    let lastStatsTime = performance.now();
    let statsFrameCount = 0;
    let fps = 0;
    let renderMs = 0;

    let frameTime = 0;
    let triangleAngle = 0;

    function render() {
        const t0 = performance.now();
        frameTime += 0.02;
        triangleAngle += 0.05;
        const cameraAngle = Math.sin(frameTime);

        Matrix4x4.makeLookAt(
            { x: 0 + cameraAngle, y: 2.5, z: 5 },
            { x: 0, y: 0.5, z: 0 },
            { x: 0, y: 1, z: 0 },
            viewMatrix
        );
        camera.setMatrices(viewMatrix, projMatrix);

        const triTransform: Transform = {
            position: [0, 1, -1],
            rotation: [
                0,
                Math.sin(triangleAngle * 0.5),
                Math.sin(triangleAngle * 0.5) * 0.4,
                Math.cos(triangleAngle * 0.5),
            ],
            scale: [1, 1, 1],
        };

        const frameInfo = context.beginFrame();

        if (!frameInfo) {

            // デバイスロスト復旧待ち。このフレームはスキップする

            requestAnimationFrame(render);

            return;

        }

        const { renderer, colorBuffer, depthBuffer } = frameInfo;

        // -- G-Buffer パス --
        renderer.beginRenderPass(
            context,
            {
                colorAttachments: [
                    { renderTarget: gbufferA.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
                    { renderTarget: gbufferB.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
                    { renderTarget: gbufferC.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
                ],
                depthStencil: { depthBuffer: sceneDepth.handle },
                shaderPassName: "GBuffer",
            },
            camera,
        );
        renderer.drawMesh(groundMesh, groundTransform, 0);
        renderer.drawMesh(triMesh, triTransform, 0);
        renderer.endRenderPass();

        // -- SSR + 合成パス --
        renderer.beginRenderPass(
            context,
            {
            colorAttachments: [
                { renderTarget: colorBuffer, clearColor: [0, 0, 0, 1], loadOp: LoadOp.Clear },
            ],
            depthStencil: { depthBuffer },
            },
            camera,
        );
        renderer.drawScreenRect(matSSR);
        renderer.endRenderPass();

        context.endFrame();

        const t1 = performance.now();
        renderMs = t1 - t0;
        statsFrameCount++;
        if (t1 - lastStatsTime >= 1000) {
            fps = statsFrameCount;
            statsFrameCount = 0;
            lastStatsTime = t1;
        }
        statsEl.textContent = `FPS: ${fps}\nrender: ${renderMs.toFixed(2)} ms`;

        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

main();
