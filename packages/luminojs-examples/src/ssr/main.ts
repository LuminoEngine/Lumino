/**
 * SSR (Screen Space Reflection)
 *
 * Ground plane + rotating triangle rendered to G-Buffers,
 * then composited with screen-space reflections.
 *
 * Rendering pipeline:
 *   Pass 1: G-Buffer (albedo, world normal + depth, metallic/roughness/AO)
 *   Pass 2: SSR + composite (full-screen post-process)
 */
import {
  Runtime,
  Instance,
  Window,
  GraphicsBackend,
  LoadOp,
  TextureFormat,
  Texture,
  Material,
  Mesh,
  Camera,
} from "luminojs";
import type { Transform } from "luminojs";

async function main() {
  const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
  canvas.width = canvas.clientWidth;
  canvas.height = canvas.clientHeight;
  const W = canvas.width;
  const H = canvas.height;

  // 1. Load WASM module
  await Runtime.initialize({
    wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    print: (t: string) => console.log("[stdout]", t),
    printErr: (t: string) => console.warn("[stderr]", t),
  });

  // 2. Initialize Lumino instance (creates WebGPU device)
  await Instance.initialize({ preferredBackend: GraphicsBackend.WebGPU });

  // 3. Create Window from canvas
  const win = await Window.createFromCanvas("#my_canvas", W, H);
  const ctx = win.getGraphicsContext();

  // --------------------------------------------------------------
  // Render targets
  // --------------------------------------------------------------
  const gbufferA = Texture.createRenderTargetEx(ctx, W, H, TextureFormat.RGBA8_UNORM);
  const gbufferB = Texture.createRenderTargetEx(ctx, W, H, TextureFormat.RGBA32_FLOAT);
  const gbufferC = Texture.createRenderTargetEx(ctx, W, H, TextureFormat.RGBA8_UNORM);
  const sceneDepth = Texture.createDepthStencil(ctx, W, H);

  // --------------------------------------------------------------
  // Materials
  // --------------------------------------------------------------
  const gridTexture = await Texture.loadFromURL(
    ctx,
    new URL("./assets/CheckerGridGray1.png", import.meta.url).href,
  );

  // Scene materials (Unlit)
  const groundMaterial = Material.createUnlit(ctx);
  groundMaterial.setColor(0.4, 0.4, 0.4, 1.0); // orange
  //groundMaterial.setMainTexture(gridTexture);

  const triangleMaterial = Material.createUnlit(ctx);
  triangleMaterial.setColor(1.0, 1.0, 1.0, 1.0);

  // SSR material (from pre-compiled shader)
  const ssrResp = await fetch(new URL("./assets/SSR.lcsh", import.meta.url).href);
  const ssrData = new Uint8Array(await ssrResp.arrayBuffer());
  const matSSR = await Material.createFromCompiledShader(ctx, ssrData);
  matSSR.setFloat4("ssrSettings", [10.0, 0.05, 0.3, 128.0]);
  matSSR.setNamedTexture("u_gbufferA", gbufferA);
  matSSR.setNamedTexture("u_gbufferB", gbufferB);
  matSSR.setNamedTexture("u_gbufferC", gbufferC);

  // --------------------------------------------------------------
  // Scene geometry
  // --------------------------------------------------------------

  // Ground plane: 4 vertices at Y=0
  const groundMesh = Mesh.create(
    ctx,
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

  // Rotating triangle: hovers above ground
  const triMesh = Mesh.create(
    ctx,
    [
      { position: [ 0, 1, 0], normal: [0, 0, 1], uv: [0.5, 0], color: [1, 0, 0, 1], tangent: [1, 0, 0, 0] },
      { position: [ 1, 0, 0], normal: [0, 0, 1], uv: [1, 1],   color: [0, 1, 0, 1], tangent: [1, 0, 0, 0] },
      { position: [-1, 0, 0], normal: [0, 0, 1], uv: [0, 1],   color: [0, 0, 1, 1], tangent: [1, 0, 0, 0] },
    ],
    new Uint32Array([0, 2, 1]),
    [{ indexOffset: 0, indexCount: 3, materialIndex: 0 }],
  );
  triMesh.setMaterial(0, triangleMaterial);

  // --------------------------------------------------------------
  // Camera
  // --------------------------------------------------------------
  const camera = Camera.create();
  camera.setPerspective(
    (45 * Math.PI) / 180,
    W / H,
    0.1,
    10.0,
  );
  camera.setLookAt(0, 2.5, 5, 0, 0.5, 0, 0, 1, 0);

  // --------------------------------------------------------------
  // Main loop
  // --------------------------------------------------------------
  const groundTransform: Transform = {
    position: [0, 0, 0],
    rotation: [0, 0, 0, 1],
    scale: [1, 1, 1],
  };

  let frameTime = 0;
  let triangleAngle = 0;

  function frame() {
    frameTime += 0.02;
    triangleAngle += 0.05;
    const cameraAngle = Math.sin(frameTime);

    camera.setLookAt(
      0 + cameraAngle, 2.5, 5, // eye
      0, 0.5, 0,               // target
      0, 1, 0,                  // up
    );

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

    ctx.beginFrame().then(({ renderer, colorBuffer, depthBuffer }) => {
      // -- G-Buffer pass --
      renderer.beginRenderPass(
        ctx,
        {
          colorAttachments: [
            { renderTarget: gbufferA.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
            { renderTarget: gbufferB.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
            { renderTarget: gbufferC.handle, clearColor: [0, 0, 0, 0], loadOp: LoadOp.Clear },
          ],
          depthStencil: { depthBuffer: sceneDepth.handle },
          shaderPassName: "GBuffer",
        },
        camera.handle,
      );
      renderer.drawMesh(groundMesh, groundTransform, 0);
      renderer.drawMesh(triMesh, triTransform, 0);
      renderer.endRenderPass();

      // -- SSR + Composite pass --
      renderer.beginRenderPass(
        ctx,
        {
          colorAttachments: [
            { renderTarget: colorBuffer, clearColor: [0, 0, 0, 1], loadOp: LoadOp.Clear },
          ],
          depthStencil: { depthBuffer },
        },
        camera.handle,
      );
      renderer.drawScreenRect(matSSR);
      renderer.endRenderPass();

      ctx.endFrame();
      requestAnimationFrame(frame);
    });
  }
  requestAnimationFrame(frame);
}

main();
