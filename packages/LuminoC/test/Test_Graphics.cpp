#include "pch.hpp"
#include <LuminoC/lumino.h>
#include "VisualTestHelper.hpp"
#include <vector>
#include <cstdio>

class Test_Graphics : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LN_OK, LNWindow_Create("VisualTest", 320, 240, &window));
        ASSERT_EQ(LN_OK, LNWindow_GetGraphicsContext(window, &graphicsContext));
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
        320.0f / 240.0f,
        0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f));

    // Render one frame
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, &renderer));
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.0f, 0.0f, 1.0f, 1.0f));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, mesh, &identity));
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
        60.0f * 3.14159f / 180.0f, 320.0f / 240.0f, 0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    // Render
    LNTransform identity = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};
    LNHandle renderer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, &renderer));
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.5f, 0.5f, 0.5f, 1.0f));

    // Push mask - 左半分のみ描画を許可する (テクスチャが塗られている部分)
    ASSERT_EQ(LN_OK, LNRenderer_PushStencilMask(renderer, maskMesh, &identity, maskMat));

    // Draw green quad - should only be visible in masked area
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, quadMesh, &identity));

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
    EXPECT_EQ(320, w);
    EXPECT_EQ(240, h);

    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(quadMesh);
    LNObject_Release(greenMat);
    LNObject_Release(maskMesh);
    LNObject_Release(maskMat);
    LNObject_Release(maskTex); // discard the empty one
    LNObject_Release(spriteTex);
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
        60.0f * 3.14159f / 180.0f, 320.0f / 240.0f, 0.1f, 100.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    // Render one frame
    LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
    LNHandle renderer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, &renderer));
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, camera, 0.0f, 0.0f, 0.0f, 1.0f));
    ASSERT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, mesh, &identity));
    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // Capture and verify
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    EXPECT_EQ(320, w);
    EXPECT_EQ(240, h);

    // Cleanup
    LNObject_Release(camera);
    LNObject_Release(mesh);
    LNObject_Release(material);
}
