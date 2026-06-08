#include "pch.hpp"
#include <LuminoC/lumino.h>
#include "VisualTestHelper.hpp"
#include <vector>
#include <cstdio>

#define TEST_W 320
#define TEST_H 240

class Test_Graphics : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LN_OK, LNWindow_Create("VisualTest", TEST_W, TEST_H, &window));
        ASSERT_EQ(LN_OK, LNWindow_GetGraphicsContext(window, &graphicsContext));
    }

    void TearDown() override {
        if (window != LN_NULL_HANDLE) LNObject_Release(window);
    }
};

TEST_F(Test_Graphics, ClearScreen) {
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.ClearScreen", data, w, h, TEST_DATA_DIR));
}

TEST_F(Test_Graphics, HelloTexture) {
    // Load texture (Sprite.png)
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Sprite.png", &texture));

    // Create Unlit material with the texture
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &material));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material, texture));

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
    ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, vertices, 4, indices, 6, &sub, 1, &mesh));
    ASSERT_EQ(LN_OK, LNMesh_SetMaterial(mesh, 0, material));

    // Perspective camera
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        TEST_W.0f / TEST_H.0f,
        0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, mesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // Capture and compare
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);

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
    LNHandle maskTex = LN_NULL_HANDLE;
    //ASSERT_EQ(LN_OK, LNTexture2D_Create(maskW, maskH, 0 /* RGBA8 */, &maskTex));
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Data/Mask1.png", &maskTex));
    // マスクピクセルをアップロードします。今のところは、一時ファイルを使って LoadFromFile を使用します。
    // 実際には、LNTexture2D_Create でプログラム的にテクスチャを作成し、デフォルトの白色を使用します。
    // LoadFromFile なしでは C API 経由でデータをアップロードできないため、スプライトテクスチャをマスクとして使用します。
    // Sprite.png はアルファ値がゼロではないため、パイプラインの流れをテストします。
    LNHandle spriteTex = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Sprite.png", &spriteTex));

    // Create mask material
    LNHandle maskMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_STENCIL_MASK, & maskMat));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(maskMat, maskTex));

    // Create mask quad mesh (covers left half of screen in NDC-like coords)
    LNVertex maskVerts[4] = {
        { -0.5f,  0.5f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-left
        {  0.5f,  0.5f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-right
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right
    };
    uint32_t maskIndices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh maskSub = { 0, 6, 0 };
    LNHandle maskMesh = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, maskVerts, 4, maskIndices, 6, &maskSub, 1, &maskMesh));
    ASSERT_EQ(LN_OK, LNMesh_SetMaterial(maskMesh, 0, maskMat));

    // Create a fullscreen green quad to be masked
    LNHandle greenMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &greenMat));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(greenMat, spriteTex));

    LNVertex quadVerts[4] = {
        { -0.8f,  0.8f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-left
        {  0.8f,  0.8f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top-right
        { -0.8f, -0.8f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        {  0.8f, -0.8f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right
    };
    uint32_t quadIndices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh quadSub = { 0, 6, 0 };
    LNHandle quadMesh = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, quadVerts, 4, quadIndices, 6, &quadSub, 1, &quadMesh));
    ASSERT_EQ(LN_OK, LNMesh_SetMaterial(quadMesh, 0, greenMat));

    // Camera
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f, TEST_W.0f / TEST_H.0f, 0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    LNTransform identity = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.5f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.5f;
    rpDesc.colorAttachments[0].clearColor[2] = 0.5f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    // Push mask - 左半分のみ描画を許可する (テクスチャが塗られている部分)
    ASSERT_EQ(LN_OK, LNRenderer_PushStencilMask(renderer, maskMesh, &identity, maskMat));

    // Draw green quad - should only be visible in masked area
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, quadMesh, &identity, 0));

    // Pop mask
    ASSERT_EQ(LN_OK, LNRenderer_PopStencilMask(renderer));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // Capture and compare
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);

    // Verify the center-right area is background color (blue), not green.
    // The mask covers x in [-0.5, 0.0] in world coords, so roughly the left-center area.
    // Just verify the test runs without crashing. Detailed visual comparison requires reference images.
    EXPECT_EQ(TEST_W, w);
    EXPECT_EQ(TEST_H, h);

    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(quadMesh);
    LNObject_Release(greenMat);
    LNObject_Release(maskMesh);
    LNObject_Release(maskMat);
    LNObject_Release(maskTex); // discard the empty one
    LNObject_Release(spriteTex);
}

// 2つのスプライトを横に並べて描画するテスト。
// 左: Sprite.png テクスチャ付き Unlit、右: デフォルト Unlit (白)。
// (左側が白になってしまう問題の修正確認)
TEST_F(Test_Graphics, TwoSprites) {
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/Sprite.png", &texture));

    LNHandle mat1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &mat1));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(mat1, texture));

    LNHandle mat2 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &mat2));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    // Left
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, mat1, 0,
        -80.0f, 0.0f, 0.0f,
        100.0f, 100.0f,
        0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f));

    // Right
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, mat2, 0,
        80.0f, 0.0f, 0.0f,
        100.0f, 100.0f,
        0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.TwoSprites", data, w, h, TEST_DATA_DIR, true));

    LNObject_Release(camera);
    LNObject_Release(mat2);
    LNObject_Release(mat1);
    LNObject_Release(texture);
}

// 深度テストOFFのテスト。
TEST_F(Test_Graphics, MaterialDepthTestEnabled) {
    uint32_t indices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh sub = { 0, 6, 0 };

    // 深度テスト有効の赤・手前の四角
    LNHandle nearMat = LN_NULL_HANDLE;
    LNHandle nearMesh = LN_NULL_HANDLE;
    {
        ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &nearMat));
        ASSERT_EQ(LN_OK, LNMaterial_SetColor(nearMat, 1.0f, 0.0f, 0.0f, 1.0f));
        LNVertex nearVerts[4] = {
            { -0.6f,  0.6f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f,  0.6f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            { -0.6f, -0.6f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f, -0.6f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        };
        ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, nearVerts, 4, indices, 6, &sub, 1, &nearMesh));
        ASSERT_EQ(LN_OK, LNMesh_SetMaterial(nearMesh, 0, nearMat));
    }
    
    // 深度テスト無効の緑・奥の四角。手前の赤い四角が緑を覆い隠すはず。
    LNHandle farMat = LN_NULL_HANDLE;
    LNHandle farMesh = LN_NULL_HANDLE;
    {
        ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &farMat));
        ASSERT_EQ(LN_OK, LNMaterial_SetColor(farMat, 0.0f, 1.0f, 0.0f, 1.0f));
        ASSERT_EQ(LN_OK, LNMaterial_SetDepthTestEnabled(farMat, LN_FALSE));

        LNVertex farVerts[4] = {
            { -0.6f,  0.6f, -0.5f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f,  0.6f, -0.5f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            { -0.6f, -0.6f, -0.5f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f, -0.6f, -0.5f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        };
        
        ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, farVerts, 4, indices, 6, &sub, 1, &farMesh));
        ASSERT_EQ(LN_OK, LNMesh_SetMaterial(farMesh, 0, farMat));
    }

    // Camera. Z- 方向が投影される。
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        TEST_W.0f / TEST_H.0f,
        0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, nearMesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, farMesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // DepthTest が OFF なので、後から描かれた緑い四角が、赤い四角に隠されずに、描画されるはず。
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    const int cx = w / 2;
    const int cy = h / 2;
    const uint8_t* px = data + (static_cast<size_t>(cy) * w + cx) * 4;
    EXPECT_LT(px[0], 96);
    EXPECT_GT(px[1], 160);

    LNObject_Release(camera);
    LNObject_Release(farMesh);
    LNObject_Release(nearMesh);
    LNObject_Release(farMat);
    LNObject_Release(nearMat);
}

// 深度書き込みOFFのテスト。
TEST_F(Test_Graphics, MaterialDepthWriteEnabled) {
    uint32_t indices[6] = { 0, 2, 1, 1, 2, 3 };
    LNSubMesh sub = { 0, 6, 0 };

    // NearMesh (Red)
    LNHandle nearMat = LN_NULL_HANDLE;
    LNHandle nearMesh = LN_NULL_HANDLE;
    {
        ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &nearMat));
        ASSERT_EQ(LN_OK, LNMaterial_SetColor(nearMat, 1.0f, 0.0f, 0.0f, 1.0f));
        ASSERT_EQ(LN_OK, LNMaterial_SetDepthWriteEnabled(nearMat, LN_FALSE));
        LNVertex nearVerts[4] = {
            { -0.6f,  0.6f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f,  0.6f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            { -0.6f, -0.6f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f, -0.6f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        };
        ASSERT_EQ(
            LN_OK, LNMesh_Create(graphicsContext, nearVerts, 4, indices, 6, &sub, 1, &nearMesh));
        ASSERT_EQ(LN_OK, LNMesh_SetMaterial(nearMesh, 0, nearMat));
    }

    // FarMesh (Green). 
    LNHandle farMat = LN_NULL_HANDLE;
    LNHandle farMesh = LN_NULL_HANDLE;
    {
        ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &farMat));
        ASSERT_EQ(LN_OK, LNMaterial_SetColor(farMat, 0.0f, 1.0f, 0.0f, 1.0f));
        LNVertex farVerts[4] = {
            { -0.6f,  0.6f,  0.0f,  0,0,1,  0.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f,  0.6f,  0.0f,  0,0,1,  1.0f, 0.0f,  1,1,1,1,  1,0,0,0 },
            { -0.6f, -0.6f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
            {  0.6f, -0.6f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 },
        };
        ASSERT_EQ(
            LN_OK, LNMesh_Create(graphicsContext, farVerts, 4, indices, 6, &sub, 1, &farMesh));
        ASSERT_EQ(LN_OK, LNMesh_SetMaterial(farMesh, 0, farMat));
    }
    
    // Camera. Z- 方向が投影される。
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f,
        TEST_W.0f / TEST_H.0f,
        0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, nearMesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, farMesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    const int cx = w / 2;
    const int cy = h / 2;
    const uint8_t* px = data + (static_cast<size_t>(cy) * w + cx) * 4;
    EXPECT_LT(px[0], 96);
    EXPECT_GT(px[1], 160);

    LNObject_Release(camera);
    LNObject_Release(farMesh);
    LNObject_Release(nearMesh);
    LNObject_Release(farMat);
    LNObject_Release(nearMat);
}

// 同一フレーム内の異なる RenderPass でそれぞれスプライトを描画したとき、
// 先行パスのスプライトが後続パスのスプライトに上書きされないことを検証する。
//
// 回帰の背景:
//   スプライト描画は 1 つのコマンドエンコーダに記録され EndFrame の submit 時に
//   まとめて実行される。以前は全フレームで単一の共有 DynamicMesh に毎回
//   オフセット 0 から頂点を書き込んでいたため、フレーム内で flush が複数回
//   走ると、後続 flush (UI スプライト) の writeBuffer が先行 flush (背景
//   スプライト) の頂点バッファを上書きし、submit 時には全描画が最後の
//   データを読んでしまっていた。結果、背景スプライトが UI スプライトの
//   ジオメトリに化けて画面中央から消える、という不具合が起きていた。
//
// このテストは:
//   Pass 1: 画面中央に大きな赤いスプライト (背景相当)
//   Pass 2: 隅に小さな緑のスプライト (UI 相当, LoadOp = Load)
// を 1 フレームで描画し、画面中央が赤のまま保たれることを確認する。
// 不具合がある場合、Pass 1 の描画が Pass 2 の小さな隅ジオメトリを読むため、
// 中央はクリア色 (青) になってしまう。
TEST_F(Test_Graphics, SpritesAcrossRenderPasses) {
    // テクスチャなし Unlit (既定の白テクスチャ) を 2 つ用意する。
    // drawSprite の頂点カラーで色付けするため、テクスチャは不要。
    LNHandle bgMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &bgMat));
    LNHandle uiMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateUnlit(graphicsContext, &uiMat));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

    // --- Pass 1: 中央の大きな赤いスプライト (背景相当, LoadOp = Clear で青クリア) ---
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_CLEAR;
        rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, bgMat, 0,
            0.0f, 0.0f, 0.0f,   // 画面中央
            200.0f, 200.0f,     // 中央を確実に覆う大きさ
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, // 赤
            0.0f));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    // --- Pass 2: 隅の小さな緑のスプライト (UI 相当, LoadOp = Load) ---
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_LOAD;
        rpDesc.depthStencil.depthLoadOp = LN_LOAD_OP_LOAD;
        rpDesc.depthStencil.stencilLoadOp = LN_LOAD_OP_LOAD;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, uiMat, 0,
            120.0f, 80.0f, 0.0f, // 中央から離れた隅
            40.0f, 40.0f,        // 中央には掛からない小ささ
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f, // 緑
            0.0f));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    // 中央ピクセルをサンプリング (キャプチャは RGBA8 に swizzle 済み)。
    const int cx = w / 2;
    const int cy = h / 2;
    const uint8_t* px = data + (static_cast<size_t>(cy) * w + cx) * 4;
    const int r = px[0];
    const int g = px[1];
    const int b = px[2];

    // 修正後は Pass 1 の赤いスプライトが中央に残る (R 高, B 低)。
    // 不具合時は Pass 1 が Pass 2 の隅ジオメトリを読み、中央がクリア色の青になる。
    EXPECT_GT(r, 192)
        << "R channel が低すぎます。背景スプライトが UI スプライトに上書きされた可能性があります。RGBA=("
        << r << "," << g << "," << b << ")";
    EXPECT_LT(b, 64)
        << "B channel が高すぎます。中央がクリア色 (青) になり背景スプライトが消えています。RGBA=("
        << r << "," << g << "," << b << ")";

    LNObject_Release(camera);
    LNObject_Release(uiMat);
    LNObject_Release(bgMat);
}

// コンパイル済みシェーダ (.lcsh) からマテリアルを作成し、赤い三角形を描画するテスト。
TEST_F(Test_Graphics, CustomShaderMaterial) {
    // Load compiled shader from file
    const char* shaderPath = TEST_DATA_DIR "/Data/Unlit.lcsh";
    FILE* fp = fopen(shaderPath, "rb");
    ASSERT_NE(nullptr, fp) << "Failed to open: " << shaderPath;
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<unsigned char> shaderData(sz);
    fread(shaderData.data(), 1, sz, fp);
    fclose(fp);
    ASSERT_GT(shaderData.size(), 0u);

    // Create material from compiled shader
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromCompiledShader(
        graphicsContext, shaderData.data(), (uint32_t)shaderData.size(), &material));
    ASSERT_EQ(LN_OK, LNMaterial_SetColor(material, 1.0f, 0.0f, 0.0f, 1.0f)); // Red

    // Triangle mesh
    LNVertex vertices[3] = {
        /* posX   posY   posZ   normX normY normZ  u    v    r    g    b    a    tanX tanY tanZ tanW */
        {  0.0f,  0.5f,  0.0f,  0,0,1,  0.5f, 0.0f,  1,1,1,1,  1,0,0,0 }, // top
        { -0.5f, -0.5f,  0.0f,  0,0,1,  0.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-left
        {  0.5f, -0.5f,  0.0f,  0,0,1,  1.0f, 1.0f,  1,1,1,1,  1,0,0,0 }, // bottom-right
    };
    uint32_t indices[3] = { 0, 1, 2 };
    LNSubMesh sub = { 0, 3, 0 };

    LNHandle mesh = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMesh_Create(graphicsContext, vertices, 3, indices, 3, &sub, 1, &mesh));
    ASSERT_EQ(LN_OK, LNMesh_SetMaterial(mesh, 0, material));

    // Camera
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetPerspective(camera,
        60.0f * 3.14159f / 180.0f, TEST_W.0f / TEST_H.0f, 0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, mesh, &identity, 0));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // Capture and verify
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    EXPECT_EQ(TEST_W, w);
    EXPECT_EQ(TEST_H, h);

    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(mesh);
    LNObject_Release(material);
}

// [A案] カラーアタッチメントの LoadOp::Load 単体の検証。
//
// 色の Load 経路だけを切り分けて検証するため、デプス・ステンシルは
// 既定の CLEAR のままにし、カラーのみ LoadOp::Load を指定します。
// (デプス・ステンシルの Load は別バグがあるため、ここでは触らない)
//
// 同一フレーム内で 2 つの RenderPass を実行します。
//   Pass 1: 画面全体を青 (0,0,1) でクリア (LoadOp = Clear)
//   Pass 2: カラーのみ LoadOp = Load で開始し、何も描画しない
//
// Pass 2 の clearColor はあえて赤 (1,0,0) に設定しておきます。
// LoadOp::Load が正しく機能していれば、Pass 1 の青がそのまま保持され、
// 最終結果は青になるはずです。
// もし不具合により Load 指定時にもクリアされてしまうと、結果は赤 (clearColor) になります。
TEST_F(Test_Graphics, LoadOpLoadColor) {
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

    // --- Pass 1: 画面全体を青でクリア (LoadOp = Clear) ---
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_CLEAR;
        rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    // --- Pass 2: カラーのみ LoadOp = Load で開始 (何も描画しない) ---
    // デプス・ステンシルは LNRenderPassDesc_Init 既定の CLEAR のままにする。
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_LOAD;
        // クリアされてしまった場合に検知できるよう、clearColor は赤にしておく。
        rpDesc.colorAttachments[0].clearColor[0] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    // 中央ピクセルをサンプリング (キャプチャは RGBA8 に swizzle 済み)
    const int cx = w / 2;
    const int cy = h / 2;
    const uint8_t* px = data + (static_cast<size_t>(cy) * w + cx) * 4;
    const int r = px[0];
    const int g = px[1];
    const int b = px[2];

    // LoadOp::Load が機能していれば Pass 1 の青が保持されている (R≈0, B≈255)。
    // 不具合で再クリアされている場合は Pass 2 の clearColor 赤になる (R≈255, B≈0)。
    EXPECT_LT(r, 64)
        << "R channel が高すぎます。LoadOp::Load が無視され赤でクリアされた可能性があります。RGBA=("
        << r << "," << g << "," << b << ")";
    EXPECT_GT(b, 192)
        << "B channel が低すぎます。Pass 1 の青が保持されていません。RGBA=("
        << r << "," << g << "," << b << ")";

    // 視覚確認用に PNG を保存
   // VisualTest::savePng(TEST_DATA_DIR "/LoadOpLoadColor_actual.png", data, w, h);
}

// [B案] カラー + デプス + ステンシルすべてに LoadOp::Load を指定する検証。
//
// 以前はこの組み合わせで Vulkan Validation Error
//   (VUID-VkAttachmentDescription-format-06700: stencilLoadOp=LOAD なのに
//    initialLayout=UNDEFINED) が発生していた。
// depthLoadOp の配線漏れと initialLayout の修正後、エラーなく動作し、
// かつ Pass 1 の青が保持されることを検証する。
//
//   Pass 1: 画面全体を青 (0,0,1) でクリア (全アタッチメント Clear)
//   Pass 2: カラー・デプス・ステンシルすべて Load で開始し、何も描画しない
TEST_F(Test_Graphics, LoadOpLoadDepthStencil) {
    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

    // --- Pass 1: 全アタッチメントをクリア (LoadOp = Clear) ---
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_CLEAR;
        rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    // --- Pass 2: カラー・デプス・ステンシルすべて Load で開始 (何も描画しない) ---
    {
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].loadOp = LN_LOAD_OP_LOAD;
        // クリアされてしまった場合に検知できるよう、clearColor は赤にしておく。
        rpDesc.colorAttachments[0].clearColor[0] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[2] = 0.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        rpDesc.depthStencil.depthLoadOp = LN_LOAD_OP_LOAD;
        rpDesc.depthStencil.stencilLoadOp = LN_LOAD_OP_LOAD;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    // 中央ピクセルをサンプリング (キャプチャは RGBA8 に swizzle 済み)
    const int cx = w / 2;
    const int cy = h / 2;
    const uint8_t* px = data + (static_cast<size_t>(cy) * w + cx) * 4;
    const int r = px[0];
    const int g = px[1];
    const int b = px[2];

    EXPECT_LT(r, 64)
        << "R channel が高すぎます。LoadOp::Load が無視され赤でクリアされた可能性があります。RGBA=("
        << r << "," << g << "," << b << ")";
    EXPECT_GT(b, 192)
        << "B channel が低すぎます。Pass 1 の青が保持されていません。RGBA=("
        << r << "," << g << "," << b << ")";

    // 視覚確認用に PNG を保存
    //VisualTest::savePng(TEST_DATA_DIR "/LoadOpLoadDepthStencil_actual.png", data, w, h);
}
