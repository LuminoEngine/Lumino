/**
 * SSR (Screen Space Reflection)
 *
 * スクリーンスペースリフレクションのデモ。
 * 地面 (四角形メッシュ) と、その上で Y 軸回転する三角形メッシュの
 * シンプルなシーンを描画し、地面に SSR で反射を表示します。
 *
 * レンダリングパイプライン (3パス):
 *   Pass 1: シーンカラー描画 (Unlit)
 *   Pass 2: ビュー空間 深度+法線 描画 (DepthNormal シェーダ)
 *   Pass 3: SSR + コンポジット (SSR シェーダ, フルスクリーン)
 */
#include "../Utils.h"

int main() {
    InitializeInstance();

    // Window + GraphicsContext
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-SSR", WINDOW_W, WINDOW_H, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    //--------------------------------------------------------------
    // Render targets
    //--------------------------------------------------------------

    // GBufferA: Albedo.x, Albedo.y, Albedo.z, 0
    LNHandle gbuffersA = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTargetEx(
        graphicsContext,
        WINDOW_W,
        WINDOW_H,
        LN_TEXTURE_FORMAT_RGBA8_UNORM,
        &gbuffersA);

    // GBufferB: WorldNormal.x, WorldNormal.y, WorldNormal.z, Depth
    LNHandle gbuffersB = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTargetEx(
        graphicsContext,
        WINDOW_W,
        WINDOW_H,
        LN_TEXTURE_FORMAT_RGBA32_FLOAT,
        &gbuffersB);

    LNHandle gbuffersC = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTargetEx(
        graphicsContext,
        WINDOW_W,
        WINDOW_H,
        LN_TEXTURE_FORMAT_RGBA8_UNORM,
        &gbuffersC);

    LNHandle debugRenderTarget = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTargetEx(
        graphicsContext,
        WINDOW_W,
        WINDOW_H,
        LN_TEXTURE_FORMAT_RGBA32_FLOAT,
        &debugRenderTarget);

    // Shared depth buffer
    LNHandle sceneDepth = LN_NULL_HANDLE;
    LNTexture2D_CreateDepthStencil(graphicsContext, WINDOW_W, WINDOW_H, &sceneDepth);

    //--------------------------------------------------------------
    // Materials
    //--------------------------------------------------------------

    LNHandle gridTexture = LN_NULL_HANDLE;
    LNTexture2D_LoadFromFile(graphicsContext, ASSETS_DIR "/CheckerGridGray1.png", &gridTexture);

    // Scene materials (Unlit)
    LNHandle groundMaterial = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &groundMaterial);
    LNMaterial_SetColor(groundMaterial, 0.8f, 0.3f, 0.3f, 1.0f); // dark gray ground
    LNMaterial_SetMainTexture(groundMaterial, gridTexture);

    LNHandle triangleMaterial = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &triangleMaterial);
    LNMaterial_SetColor(triangleMaterial, 1.0f, 0.4f, 0.2f, 1.0f); // orange

    // SSR material
    LNHandle matSSR = LN_NULL_HANDLE;
    LNMaterial_CreateFromShaderSourceFile(
        graphicsContext,
        ASSETS_DIR "/SSR.slang",
        LN_REPO_ROOT_DIR "/packages/LuminoShader/shaders",
        &matSSR);

    // SSR parameters: maxDistance, stepSize, thickness, maxSteps
    const float ssrSettings[4] = {10.0f, 0.05f, 0.3f, 128.0f};
    LNMaterial_SetFloat4(matSSR, "ssrSettings", ssrSettings);

    // Bind render target textures to SSR material
    LNMaterial_SetNamedTexture(matSSR, "u_gbufferA", gbuffersA);
    LNMaterial_SetNamedTexture(matSSR, "u_gbufferB", gbuffersB);
    LNMaterial_SetNamedTexture(matSSR, "u_gbufferC", gbuffersC);

    //--------------------------------------------------------------
    // Scene geometry
    //--------------------------------------------------------------

    // clang-format off
    // Ground plane: 4 vertices at Y=0, extends from (-3,-3) to (3,3)
    LNVertex groundVertices[4] = {
        // posX   posY   posZ   nX nY nZ   u    v      r g b a   tX tY tZ tW
        { -2.0f,  0.0f, -2.0f,  0, 1, 0,  0.0f, 0.0f,  0.5f,0.5f,0.5f,1,  1,0,0,0 },
        {  2.0f,  0.0f, -2.0f,  0, 1, 0,  1.0f, 0.0f,  0.5f,0.5f,0.5f,1,  1,0,0,0 },
        { -2.0f,  0.0f,  2.0f,  0, 1, 0,  0.0f, 1.0f,  0.5f,0.5f,0.5f,1,  1,0,0,0 },
        {  2.0f,  0.0f,  2.0f,  0, 1, 0,  1.0f, 1.0f,  0.5f,0.5f,0.5f,1,  1,0,0,0 },
    };
    uint32_t groundIndices[6] = { 0, 2, 1,  1, 2, 3 };
    LNSubMesh groundSub = { 0, 6, 0 };
    LNHandle groundMesh = LN_NULL_HANDLE;
    LNMesh_Create(
        graphicsContext,
        groundVertices, 4,
        groundIndices, 6,
        &groundSub, 1,
        &groundMesh);
    LNMesh_SetMaterial(groundMesh, 0, groundMaterial);

    // Rotating triangle: hovers above ground at Y=0.8
    LNVertex triVertices[3] = {
        {  0.0f,  1.0f,  0.0f,  0, 0, 1,  0.5f, 0.0f,  1,0,0,1,  1,0,0,0 },
        {  1.0f,  0.0f,  0.0f,  0, 0, 1,  1.0f, 1.0f,  0,1,0,1,  1,0,0,0 },
        { -1.0f,  0.0f,  0.0f,  0, 0, 1,  0.0f, 1.0f,  0,0,1,1,  1,0,0,0 },
    };
    uint32_t triIndices[3] = { 0, 2, 1 };
    LNSubMesh triSub = { 0, 3, 0 };
    LNHandle triMesh = LN_NULL_HANDLE;
    LNMesh_Create(
        graphicsContext,
        triVertices, 3,
        triIndices, 3,
        &triSub, 1,
        &triMesh);
    LNMesh_SetMaterial(triMesh, 0, triangleMaterial);
    
    //--------------------------------------------------------------
    // Camera
    //--------------------------------------------------------------

    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    //LNCamera_SetPerspective(camera,
    //    45.0f * 3.14159f / 180.0f,
    //    (float)WINDOW_W / (float)WINDOW_H,
    //    0.1f, 10.0f);
    LNCamera_SetOrthographic(
        camera,
        6.0f, 6.0f,   // width, height
        0.1f, 10.0f
    );
    LNCamera_SetLookAt(camera,
        0.0f, 2.5f, 5.0f,   // eye: slightly above and behind
        0.0f, 0.5f, 0.0f,   // target: slightly above ground
        0.0f, 1.0f, 0.0f);  // up

    // Main loop
    LNTransform groundTransform = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNGraphicsProfiler profiler = {};
    LNBool quit = LN_FALSE;
    float cameraAngle = 0.0f;
    float triangleAngle = 0.0f;
    float frameTime = 0.0f;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        frameTime += 0.02f;
        cameraAngle = sinf(frameTime);
        triangleAngle += 0.05f;

        LNCamera_SetLookAt(camera,
            0.0f + cameraAngle, 2.5f, 5.0f,   // eye: slightly above and behind
            0.0f, 0.5f, 0.0f,   // target: slightly above ground
            0.0f, 1.0f, 0.0f);  // up

        // Triangle transform: rotate around Y axis, hover above ground
        LNTransform triTransform = {
            0.0f, 1.0f, -1.0f,                                     // position
            0.0f, sinf(triangleAngle * 0.5f), sinf(triangleAngle * 0.5f)*0.4f, cosf(triangleAngle * 0.5f),  // rotation (Y axis)
            1.0f, 1.0f, 1.0f                                      // scale
        };

        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);
        
#if 0   // test
        {
            LNRenderPassDesc rpDesc;
            LNRenderPassDesc_Init(&rpDesc);
            rpDesc.colorAttachmentCount = 1;
            rpDesc.colorAttachments[0].renderTarget = colorBuffer;
            rpDesc.colorAttachments[0].clearColor[0] = 0.05f;
            rpDesc.colorAttachments[0].clearColor[1] = 0.05f;
            rpDesc.colorAttachments[0].clearColor[2] = 0.05f;
            rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
            rpDesc.depthStencil.depthBuffer = depthBuffer;

            LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
            LNRenderer_DrawMesh(renderer, groundMesh, &groundTransform, 0);
            LNRenderer_DrawMesh(renderer, triMesh, &triTransform, 0);
            LNRenderer_EndRenderPass(renderer);
        }
#endif
        
        //--------------------------------------------------------------
        // G-Buffer
        //--------------------------------------------------------------
        {
            LNRenderPassDesc rpDesc;
            LNRenderPassDesc_Init(&rpDesc);
            rpDesc.colorAttachmentCount = 3;
            rpDesc.colorAttachments[0].renderTarget = gbuffersA;
            rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[2] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[3] = 0.0f;
            rpDesc.colorAttachments[1].renderTarget = gbuffersB;
            rpDesc.colorAttachments[1].clearColor[0] = 0.0f;
            rpDesc.colorAttachments[1].clearColor[1] = 0.0f;
            rpDesc.colorAttachments[1].clearColor[2] = 0.0f;
            rpDesc.colorAttachments[1].clearColor[3] = 0.0f;
            rpDesc.colorAttachments[2].renderTarget = gbuffersC;
            rpDesc.colorAttachments[2].clearColor[0] = 0.0f;
            rpDesc.colorAttachments[2].clearColor[1] = 0.0f;
            rpDesc.colorAttachments[2].clearColor[2] = 0.0f;
            rpDesc.colorAttachments[2].clearColor[3] = 0.0f;
            rpDesc.depthStencil.depthBuffer = sceneDepth;
            rpDesc.shaderPassName = "GBuffer";

            LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
            // 各 Mesh の Material から "GBuffer" パスが自動選択される (無い Material はスキップ)
            LNRenderer_DrawMesh(renderer, groundMesh, &groundTransform, 0);
            LNRenderer_DrawMesh(renderer, triMesh, &triTransform, 0);
            LNRenderer_EndRenderPass(renderer);
        }

        //--------------------------------------------------------------
        // SSR + Composite (fullscreen)
        //--------------------------------------------------------------
        {
            LNRenderPassDesc rpDesc;
            LNRenderPassDesc_Init(&rpDesc);
            rpDesc.colorAttachmentCount = 1;
            rpDesc.colorAttachments[0].renderTarget = colorBuffer;
            //rpDesc.colorAttachments[0].renderTarget = debugRenderTarget;
            rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[2] = 0.0f;
            rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
            rpDesc.depthStencil.depthBuffer = depthBuffer;

            LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera);
            LNRenderer_DrawScreenRect(renderer, matSSR);
            LNRenderer_EndRenderPass(renderer);
        }

        printGraphicsProfilering(graphicsContext);
        LNGraphicsContext_EndFrame(graphicsContext);

        _sleep(16); // ~60 FPS
    }

    LNObject_Release(triMesh);
    LNObject_Release(groundMesh);
    LNObject_Release(matSSR);
    LNObject_Release(triangleMaterial);
    LNObject_Release(groundMaterial);
    LNObject_Release(camera);
    LNObject_Release(sceneDepth);
    LNObject_Release(debugRenderTarget);
    LNObject_Release(gbuffersC);
    LNObject_Release(gbuffersB);
    LNObject_Release(gbuffersA);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}
