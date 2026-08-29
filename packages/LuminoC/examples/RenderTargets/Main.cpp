/**
 * RenderTargets
 *
 * 2 つのレンダーターゲットテクスチャにそれぞれ異なる背景色で三角形を描画し、
 * それらをバックバッファに並べて表示するデモ。
 */
#include "../Utils.h"

int main(void) {
    InitializeInstance();

    // 2. Window と GraphicsContext
    LNHandle window = LN_NULL_HANDLE;
    LNWindow_Create("LuminoC-RenderTargets", WINDOW_W, WINDOW_H, &window);
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNWindow_GetGraphicsContext(window, &graphicsContext);

    // 3. レンダーターゲットテクスチャを 2 つ作成 (各 512x512)
    const uint32_t rtWidth = 512;
    const uint32_t rtHeight = 512;
    LNHandle rt1 = LN_NULL_HANDLE;
    LNHandle rt2 = LN_NULL_HANDLE;
    LNTexture2D_CreateRenderTarget(graphicsContext, rtWidth, rtHeight, &rt1);
    LNTexture2D_CreateRenderTarget(graphicsContext, rtWidth, rtHeight, &rt2);

    // RT 描画用の共有デプスバッファ (両 RT は同じサイズ)
    LNHandle rtDepth = LN_NULL_HANDLE;
    LNTexture2D_CreateDepthStencil(graphicsContext, rtWidth, rtHeight, &rtDepth);

    // 4. 各レンダーターゲットに描画する三角形メッシュを作成
    LNVertex triVertices[3] = {
        // posX   posY   posZ   nX nY nZ  u    v    r    g    b    a    tX tY tZ tW
        {  0.0f,  0.5f,  0.0f,  0,0,1,  0.5f,0.0f,  1,1,1,1,  1,0,0,0 },
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f,1.0f,  1,1,1,1,  1,0,0,0 },
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f,1.0f,  1,1,1,1,  1,0,0,0 },
    };
    uint32_t triIndices[3] = { 0, 2, 1 };
    LNSubMesh triSub = { 0, 3, 0 };

    // RT1 用マテリアル (赤い三角形)
    LNHandle matRed = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRed);
    LNMaterial_SetColor(matRed, 1.0f, 0.3f, 0.3f, 1.0f);

    // RT2 用マテリアル (緑の三角形)
    LNHandle matGreen = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matGreen);
    LNMaterial_SetColor(matGreen, 0.3f, 1.0f, 0.3f, 1.0f);

    // RT1 用の三角形メッシュ
    LNHandle triMesh1 = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, triVertices, 3, triIndices, 3, &triSub, 1, &triMesh1);
    LNMesh_SetMaterial(triMesh1, 0, matRed);

    // RT2 用の三角形メッシュ
    LNHandle triMesh2 = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, triVertices, 3, triIndices, 3, &triSub, 1, &triMesh2);
    LNMesh_SetMaterial(triMesh2, 0, matGreen);

    // 5. RT へ描画するためのカメラ (透視投影)
    LNHandle camera = LN_NULL_HANDLE;
    LNCamera_Create(&camera);
    LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        (float)rtWidth / (float)rtHeight,
        0.1f, 100.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 2.0f,   // 視点
        0.0f, 0.0f, 0.0f,   // 注視点
        0.0f, 1.0f, 0.0f);  // 上方向

    // 6. RT テクスチャをバックバッファに並べて表示するための画面四角形 2 つ
    //    左の四角形: [-1, -1] から [0, 1]   右の四角形: [0, -1] から [1, 1]
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

    // 左の四角形用マテリアル (RT1 をテクスチャとして使用)
    LNHandle matRT1 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRT1);
    LNMaterial_SetMainTexture(matRT1, rt1);

    // 右の四角形用マテリアル (RT2 をテクスチャとして使用)
    LNHandle matRT2 = LN_NULL_HANDLE;
    LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matRT2);
    LNMaterial_SetMainTexture(matRT2, rt2);

    // 左の四角形メッシュ
    LNHandle leftQuadMesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, leftQuadVerts, 4, quadIndices, 6, &quadSub, 1, &leftQuadMesh);
    LNMesh_SetMaterial(leftQuadMesh, 0, matRT1);

    // 右の四角形メッシュ
    LNHandle rightQuadMesh = LN_NULL_HANDLE;
    LNMesh_Create(graphicsContext, rightQuadVerts, 4, quadIndices, 6, &quadSub, 1, &rightQuadMesh);
    LNMesh_SetMaterial(rightQuadMesh, 0, matRT2);

    // 7. メインループ
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNGraphicsProfiler profiler = {};
    LNBool quit = LN_FALSE;
    float angle = 0.0f;
    while (LNWindow_ProcessEvents(window, &quit) == LN_OK && !quit) {
        angle += 0.02f;

        // 三角形を回転させる
        float sinA = sinf(angle);
        float cosA = cosf(angle);
        // Z 軸回りの回転のクォータニオン: (0, 0, sin(a/2), cos(a/2))
        LNTransform triTransform = {
            0, 0, 0,                                       // 位置
            0, 0, sinf(angle * 0.5f), cosf(angle * 0.5f), // 回転 (Z 軸)
            1, 1, 1                                        // スケール
        };

        LNHandle renderer, colorBuffer, depthBuffer;
        LNGraphicsContext_BeginFrame(graphicsContext, WINDOW_W, WINDOW_H, &renderer, &colorBuffer, &depthBuffer);

        // --- Pass 1: 赤い三角形を RT1 へ描画 ---
        LNRenderPassDesc rt1Desc;
        LNRenderPassDesc_Init(&rt1Desc);
        rt1Desc.colorAttachmentCount = 1;
        rt1Desc.colorAttachments[0].renderTarget = rt1;
        rt1Desc.depthStencil.depthBuffer = rtDepth;
        rt1Desc.colorAttachments[0].clearColor[0] = 0.2f;
        rt1Desc.colorAttachments[0].clearColor[1] = 0.0f;
        rt1Desc.colorAttachments[0].clearColor[2] = 0.0f;
        rt1Desc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rt1Desc, camera);
        LNRenderer_DrawMesh(renderer, triMesh1, &triTransform, 0);
        LNRenderer_EndRenderPass(renderer);

        // --- Pass 2: 緑の三角形を RT2 へ描画 ---
        // 見た目の違いを出すため逆方向に回転させる
        LNTransform triTransform2 = {
            0, 0, 0,
            0, 0, sinf(-angle * 0.5f), cosf(-angle * 0.5f),
            1, 1, 1
        };
        LNRenderPassDesc rt2Desc;
        LNRenderPassDesc_Init(&rt2Desc);
        rt2Desc.colorAttachmentCount = 1;
        rt2Desc.colorAttachments[0].renderTarget = rt2;
        rt2Desc.depthStencil.depthBuffer = rtDepth;
        rt2Desc.colorAttachments[0].clearColor[0] = 0.0f;
        rt2Desc.colorAttachments[0].clearColor[1] = 0.0f;
        rt2Desc.colorAttachments[0].clearColor[2] = 0.2f;
        rt2Desc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &rt2Desc, camera);
        LNRenderer_DrawMesh(renderer, triMesh2, &triTransform2, 0);
        LNRenderer_EndRenderPass(renderer);

        // --- Pass 3: 両方の RT テクスチャをバックバッファに並べて描画 ---
        LNRenderPassDesc bbDesc;
        LNRenderPassDesc_Init(&bbDesc);
        bbDesc.colorAttachments[0].clearColor[0] = 0.1f;
        bbDesc.colorAttachments[0].clearColor[1] = 0.1f;
        bbDesc.colorAttachments[0].clearColor[2] = 0.1f;
        bbDesc.colorAttachments[0].clearColor[3] = 1.0f;
        LNRenderer_BeginRenderPass(renderer, graphicsContext, &bbDesc, LN_NULL_HANDLE);
        LNRenderer_DrawMesh(renderer, leftQuadMesh, &identity, 0);
        LNRenderer_DrawMesh(renderer, rightQuadMesh, &identity, 0);
        LNRenderer_EndRenderPass(renderer);

        // プロファイラのオーバーレイ
        LNDebug_GetGraphicsProfiler(graphicsContext, &profiler);
        LNDebug_Print(graphicsContext, (std::string("FPS: ") + std::to_string(profiler.fps)).c_str());
        LNDebug_Print(graphicsContext, (std::string("DrawCall: ") + std::to_string(profiler.drawCallCount)).c_str());

        LNGraphicsContext_EndFrame(graphicsContext);
    }

    // 8. 解放
    LNObject_Release(rightQuadMesh);
    LNObject_Release(leftQuadMesh);
    LNObject_Release(matRT2);
    LNObject_Release(matRT1);
    LNObject_Release(triMesh2);
    LNObject_Release(triMesh1);
    LNObject_Release(matGreen);
    LNObject_Release(matRed);
    LNObject_Release(camera);
    LNObject_Release(rtDepth);
    LNObject_Release(rt2);
    LNObject_Release(rt1);
    LNObject_Release(window);
    LNInstance_Terminate();

    printf("Done.\n");
    return 0;
}
