/**
 * RenderTargets
 *
 * 2 つのレンダーターゲットテクスチャにそれぞれ異なる背景色で三角形を描画し、
 * それらをバックバッファに並べて表示するデモ。
 */
#include <string>
#include <LuminoC/lumino.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    /* 1. Runtime initialize */
    LNInstanceInitializeSettings settings = {};
    settings.enableValidation = LN_TRUE;
    LNInstance_Initialize(&settings);

    /* 2. Window + GraphicsContext */
    const uint32_t windowWidth = 1280;
    const uint32_t windowHeight = 720;
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-RenderTargets", windowWidth, windowHeight, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    /* 3. Create two render target textures (512x512 each) */
    const uint32_t rtWidth = 512;
    const uint32_t rtHeight = 512;
    LNHandle rt1 = LN_NULL_HANDLE;
    LNHandle rt2 = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTarget(graphicsContext, rtWidth, rtHeight, &rt1);
    LNTexture2D_CreateRenderTarget(graphicsContext, rtWidth, rtHeight, &rt2);

    /* 4. Create a triangle mesh to draw into each render target */
    LNVertex triVertices[3] = {
        /* posX   posY   posZ   nX nY nZ  u    v    r    g    b    a    tX tY tZ tW */
        {  0.0f,  0.5f,  0.0f,  0,0,1,  0.5f,0.0f,  1,1,1,1,  1,0,0,0 },
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f,1.0f,  1,1,1,1,  1,0,0,0 },
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f,1.0f,  1,1,1,1,  1,0,0,0 },
    };
    uint32_t triIndices[3] = { 0, 2, 1 };
    LNSubMesh triSub = { 0, 3, 0 };

    /* Material for RT1 (red tinted triangle) */
    LNHandle matRed = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRed);
    LNMaterial_SetColor(matRed, 1.0f, 0.3f, 0.3f, 1.0f);

    /* Material for RT2 (green tinted triangle) */
    LNHandle matGreen = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matGreen);
    LNMaterial_SetColor(matGreen, 0.3f, 1.0f, 0.3f, 1.0f);

    /* Triangle mesh for RT1 */
    LNHandle triMesh1 = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, triVertices, 3, triIndices, 3, &triSub, 1, &triMesh1);
    LNMesh_SetMaterial(triMesh1, 0, matRed);

    /* Triangle mesh for RT2 */
    LNHandle triMesh2 = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, triVertices, 3, triIndices, 3, &triSub, 1, &triMesh2);
    LNMesh_SetMaterial(triMesh2, 0, matGreen);

    /* 5. Camera for rendering into RTs (perspective) */
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        (float)rtWidth / (float)rtHeight,
        0.1f, 100.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 2.0f,   /* eye */
        0.0f, 0.0f, 0.0f,   /* target */
        0.0f, 1.0f, 0.0f);  /* up */

    /* 6. Two screen quads to display RT textures side-by-side on backbuffer */
    /*    Left quad: [-1, -1] to [0, 1]   Right quad: [0, -1] to [1, 1] */
    LNVertex leftQuadVerts[4] = {
        { -1.0f,  1.0f, 0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        {  0.0f,  1.0f, 0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        { -1.0f, -1.0f, 0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        {  0.0f, -1.0f, 0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
    };
    LNVertex rightQuadVerts[4] = {
        { 0.0f,  1.0f, 0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        { 1.0f,  1.0f, 0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        { 0.0f, -1.0f, 0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        { 1.0f, -1.0f, 0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
    };
    uint32_t quadIndices[6] = { 0, 2, 1,  1, 2, 3 };
    LNSubMesh quadSub = { 0, 6, 0 };

    /* Material for left quad (uses RT1 as texture) */
    LNHandle matRT1 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRT1);
    LNMaterial_SetMainTexture(matRT1, rt1);

    /* Material for right quad (uses RT2 as texture) */
    LNHandle matRT2 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRT2);
    LNMaterial_SetMainTexture(matRT2, rt2);

    /* Left quad mesh */
    LNHandle leftQuadMesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, leftQuadVerts, 4, quadIndices, 6, &quadSub, 1, &leftQuadMesh);
    LNMesh_SetMaterial(leftQuadMesh, 0, matRT1);

    /* Right quad mesh */
    LNHandle rightQuadMesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, rightQuadVerts, 4, quadIndices, 6, &quadSub, 1, &rightQuadMesh);
    LNMesh_SetMaterial(rightQuadMesh, 0, matRT2);

    /* 7. Main loop */
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNGraphicsProfiler profiler = {};
    int continueLoop = 1;
    float angle = 0.0f;
    while (LNWindow_ProcessEvents(window, &continueLoop) == LN_OK && continueLoop) {
        angle += 0.02f;

        /* Rotate the triangle */
        float sinA = sinf(angle);
        float cosA = cosf(angle);
        /* Quaternion for rotation around Z axis: (0, 0, sin(a/2), cos(a/2)) */
        LNTransform triTransform = {
            0, 0, 0,                                       /* position */
            0, 0, sinf(angle * 0.5f), cosf(angle * 0.5f), /* rotation (Z axis) */
            1, 1, 1                                        /* scale */
        };

        LNHandle renderer;
        LNGraphicsContext_BeginFrame(graphicsContext, &renderer);

        /* --- Pass 1: Render red triangle into RT1 --- */
        LNRenderer_BeginRenderPassToTexture(renderer, rt1, camera, 0.2f, 0.0f, 0.0f, 1.0f);
        LNRenderer_DrawMesh(renderer, triMesh1, &triTransform);
        LNRenderer_EndRenderPass(renderer);

        /* --- Pass 2: Render green triangle into RT2 --- */
        /* Rotate the opposite direction for visual difference */
        LNTransform triTransform2 = {
            0, 0, 0,
            0, 0, sinf(-angle * 0.5f), cosf(-angle * 0.5f),
            1, 1, 1
        };
        LNRenderer_BeginRenderPassToTexture(renderer, rt2, camera, 0.0f, 0.0f, 0.2f, 1.0f);
        LNRenderer_DrawMesh(renderer, triMesh2, &triTransform2);
        LNRenderer_EndRenderPass(renderer);

        /* --- Pass 3: Draw both RT textures side-by-side on backbuffer --- */
        LNRenderer_BeginRenderPass(renderer, graphicsContext, LN_NULL_HANDLE, 0.1f, 0.1f, 0.1f, 1.0f);
        LNRenderer_DrawMesh(renderer, leftQuadMesh, &identity);
        LNRenderer_DrawMesh(renderer, rightQuadMesh, &identity);
        LNRenderer_EndRenderPass(renderer);

        /* Profiler overlay */
        LNDebug_GetGraphicsProfiler(graphicsContext, &profiler);
        LNDebug_Print(graphicsContext, (std::string("FPS: ") + std::to_string(profiler.fps)).c_str());
        LNDebug_Print(graphicsContext, (std::string("DrawCall: ") + std::to_string(profiler.drawCallCount)).c_str());

        LNGraphicsContext_EndFrame(graphicsContext);
    }

    /* 8. Cleanup */
    LNObject_Release(rightQuadMesh);
    LNObject_Release(leftQuadMesh);
    LNObject_Release(matRT2);
    LNObject_Release(matRT1);
    LNObject_Release(triMesh2);
    LNObject_Release(triMesh1);
    LNObject_Release(matGreen);
    LNObject_Release(matRed);
    LNObject_Release(camera);
    LNObject_Release(rt2);
    LNObject_Release(rt1);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done.\n");
    return 0;
}
