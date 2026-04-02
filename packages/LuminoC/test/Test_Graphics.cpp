#include "pch.hpp"
#include <LuminoC/lumino.h>
#include "VisualTestHelper.hpp"

class Test_Graphics : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LNWindow_Create("VisualTest", 320, 240, &window), LN_OK);
        ASSERT_EQ(LNWindow_GetGraphicsContext(window, &graphicsContext), LN_OK);
    }

    void TearDown() override {
        if (graphicsContext != LN_NULL_HANDLE) LNObject_Release(graphicsContext);
        if (window != LN_NULL_HANDLE) LNObject_Release(window);
    }
};

TEST_F(Test_Graphics, ClearScreen) {
    LNHandle renderer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, &renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginRenderPass(graphicsContext, 0.0f, 0.0f, 1.0f, 1.0f));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndRenderPass(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(320, w);
    ASSERT_EQ(240, h);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.ClearScreen", data, w, h, TEST_DATA_DIR));
}

TEST_F(Test_Graphics, HelloTexture) {
    // Load texture (Sprite.png)
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Sprite.png", &texture), LN_OK);

    // Create Unlit material with the texture
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LNMaterial_CreateUnlit(graphicsContext, &material), LN_OK);
    ASSERT_EQ(LNMaterial_SetMainTexture(material, texture), LN_OK);

    // Create quad mesh
    LNVertex vertices[4] = {
        /* posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW */
        { -0.5f,  0.5f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        {  0.5f,  0.5f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
    };
    uint32_t indices[6] = { 0, 2, 1,  1, 2, 3 };
    LNSubMesh sub = { 0, 6, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    ASSERT_EQ(LNMesh_Create(graphicsContext, vertices, 4, indices, 6, &sub, 1, &mesh), LN_OK);
    ASSERT_EQ(LNMesh_SetMaterial(mesh, 0, material), LN_OK);

    // Perspective camera
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LNCamera_Create(&camera), LN_OK);
    ASSERT_EQ(LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        320.0f / 240.0f,
        0.1f, 100.0f), LN_OK);
    ASSERT_EQ(LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f), LN_OK);

    // Render one frame
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer;
    ASSERT_EQ(LNGraphicsContext_BeginFrame(graphicsContext, &renderer), LN_OK);
    ASSERT_EQ(LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.0f, 0.0f, 1.0f, 1.0f), LN_OK);
    ASSERT_EQ(LNRenderer_DrawMesh(renderer, mesh, &identity), LN_OK);
    ASSERT_EQ(LNRenderer_EndRenderPass(renderer), LN_OK);
    ASSERT_EQ(LNGraphicsContext_EndFrame(graphicsContext), LN_OK);

    // Capture and compare
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h), LN_OK);
    ASSERT_NE(data, nullptr);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.HelloTexture", data, w, h, TEST_DATA_DIR));

    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(mesh);
    LNObject_Release(material);
    LNObject_Release(texture);
}

// ステンシルマスクがスプライトに適用できることを検証します。
// スプライトの左半分は表示され、右半分はマスクされて透明になるはずです。
TEST_F(Test_Graphics, StencilMask1) {
    // Create a 64x64 mask texture: left half white (alpha=1), right half transparent (alpha=0).
    //const uint32_t maskW = 64, maskH = 64;
    //std::vector<uint8_t> maskPixels(maskW * maskH * 4, 0);
    //for (uint32_t y = 0; y < maskH; ++y) {
    //    for (uint32_t x = 0; x < maskW; ++x) {
    //        uint8_t* p = &maskPixels[(y * maskW + x) * 4];
    //        if (x < maskW / 2) {
    //            p[0] = 255; p[1] = 255; p[2] = 255; p[3] = 255; // opaque
    //        } else {
    //            p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; // transparent
    //        }
    //    }
    //}
    //VisualTest::savePng("test.png", maskPixels.data(), 64, 64);
    LNHandle maskTex = LN_NULL_HANDLE;
    //ASSERT_EQ(LNTexture2D_Create(maskW, maskH, 0 /* RGBA8 */, &maskTex), LN_OK);
    ASSERT_EQ(LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Data/Mask1.png", &maskTex), LN_OK);
    // マスクピクセルをアップロードします。今のところは、一時ファイルを使って LoadFromFile を使用します。
    // 実際には、LNTexture2D_Create でプログラム的にテクスチャを作成し、デフォルトの白色を使用します。
    // LoadFromFile なしでは C API 経由でデータをアップロードできないため、スプライトテクスチャをマスクとして使用します。
    // Sprite.png はアルファ値がゼロではないため、パイプラインの流れをテストします。
    LNHandle spriteTex = LN_NULL_HANDLE;
    ASSERT_EQ(LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Sprite.png", &spriteTex), LN_OK);
    



    // Create mask material
    LNHandle maskMat = LN_NULL_HANDLE;
    ASSERT_EQ(LNMaterial_CreateUnlit(graphicsContext, &maskMat), LN_OK);
    ASSERT_EQ(LNMaterial_SetMainTexture(maskMat, maskTex), LN_OK);

    // Create mask quad mesh (covers left half of screen in NDC-like coords)
    LNVertex maskVerts[4] = {
        { -1.5f,  0.5f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-left
        {  0.5f,  0.5f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-right
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right

        
        //{ -0.8f,  0.8f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-left
        //{  0.8f,  0.8f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-right
        //{ -0.8f, -0.8f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        //{  0.8f, -0.8f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right
    };
    uint32_t maskIndices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh maskSub = { 0, 6, 0 };
    LNHandle maskMesh = LN_NULL_HANDLE;
    ASSERT_EQ(LNMesh_Create(graphicsContext, maskVerts, 4, maskIndices, 6, &maskSub, 1, &maskMesh), LN_OK);
    ASSERT_EQ(LNMesh_SetMaterial(maskMesh, 0, maskMat), LN_OK);

    // Create a fullscreen green quad to be masked
    LNHandle greenMat = LN_NULL_HANDLE;
    ASSERT_EQ(LNMaterial_CreateUnlit(graphicsContext, &greenMat), LN_OK);
    ASSERT_EQ(LNMaterial_SetMainTexture(greenMat, spriteTex), LN_OK);
    //ASSERT_EQ(LNMaterial_SetColor(greenMat, 0.0f, 1.0f, 0.0f, 1.0f), LN_OK);

    LNVertex quadVerts[4] = {
        { -0.8f,  0.8f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-left
        {  0.8f,  0.8f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-right
        { -0.8f, -0.8f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        {  0.8f, -0.8f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right
    };
    uint32_t quadIndices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh quadSub = { 0, 6, 0 };
    LNHandle quadMesh = LN_NULL_HANDLE;
    ASSERT_EQ(LNMesh_Create(graphicsContext, quadVerts, 4, quadIndices, 6, &quadSub, 1, &quadMesh), LN_OK);
    ASSERT_EQ(LNMesh_SetMaterial(quadMesh, 0, greenMat), LN_OK);

    // Camera
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LNCamera_Create(&camera), LN_OK);
    ASSERT_EQ(LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f, 320.0f / 240.0f, 0.1f, 100.0f), LN_OK);
    ASSERT_EQ(LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), LN_OK);

   // int cont = 0;
   ////for (int i = 0; i < 10; i++) {
   // while (true) {
   //     LNHandle result = LNWindow_ProcessEvents(window, &cont);
   //     if (result != LN_OK || !cont) break;

        // Render
        LNTransform identity = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
        LNHandle renderer;
        ASSERT_EQ(LNGraphicsContext_BeginFrame(graphicsContext, &renderer), LN_OK);
        ASSERT_EQ(LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.5f, 0.5f, 0.5f, 1.0f), LN_OK);

        // Push mask - 左半分のみ描画を許可する (テクスチャが塗られている部分)
        ASSERT_EQ(LNRenderer_PushStencilMask(renderer, maskMesh, &identity, maskMat), LN_OK);

        // Draw green quad — should only be visible in masked area
        //ASSERT_EQ(LNRenderer_DrawMesh(renderer, maskMesh, &identity), LN_OK);
        ASSERT_EQ(LNRenderer_DrawMesh(renderer, quadMesh, &identity), LN_OK);

        // Pop mask
        ASSERT_EQ(LNRenderer_PopStencilMask(renderer), LN_OK);

        ASSERT_EQ(LNRenderer_EndRenderPass(renderer), LN_OK);
        ASSERT_EQ(LNGraphicsContext_EndFrame(graphicsContext), LN_OK);
    //}

#if 1
    // Capture and compare
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h), LN_OK);
    ASSERT_NE(data, nullptr);

    // Verify the center-right area is background color (blue), not green.
    // The mask covers x in [-0.5, 0.0] in world coords, so roughly the left-center area.
    // Just verify the test runs without crashing. Detailed visual comparison requires reference images.
    EXPECT_EQ(320, w);
    EXPECT_EQ(240, h);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.StencilMask1", data, w, h, TEST_DATA_DIR, true));
#endif
    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(quadMesh);
    LNObject_Release(greenMat);
    LNObject_Release(maskMesh);
    LNObject_Release(maskMat);
    LNObject_Release(maskTex); // discard the empty one
    LNObject_Release(spriteTex);
}
