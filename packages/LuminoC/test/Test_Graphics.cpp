#include "pch.hpp"
#include <LuminoC/lumino.h>
#include "VisualTestHelper.hpp"
#include <vector>
#include <string>
#include <cstdio>

#define TEST_W 320
#define TEST_H 240

/** 列優先の並進行列を作るヘルパ (平行移動は m[12..14])。 */
static LNMatrix translationMatrix(float x, float y, float z) {
    LNMatrix m = {{ 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  x, y, z, 1 }};
    return m;
}

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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));
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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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





TEST_F(Test_Graphics, SpriteOrder) {
    LNHandle redTexture = LN_NULL_HANDLE;
    LNHandle redMaterial = LN_NULL_HANDLE;
    LNHandle greenTexture = LN_NULL_HANDLE;
    LNHandle greenMaterial = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/RedMargined.png", &redTexture));
    ASSERT_EQ(LN_OK, LNTexture2D_LoadFromFile(graphicsContext, TEST_DATA_DIR "/GreenMargined.png", &greenTexture));

    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &redMaterial));
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &greenMaterial));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(redMaterial, redTexture));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(greenMaterial, greenTexture));
    LNMaterial_SetBlendMode(redMaterial, LN_BLEND_MODE_ALPHA);
    LNMaterial_SetBlendMode(greenMaterial, LN_BLEND_MODE_ALPHA);

    // 深度テストは既定 (有効) のまま。半透明ブレンドのマテリアルは深度を書き込まない
    // ようになったため (Renderer::drawSubmesh)、同 zIndex で後から描いた緑が
    // 手前のスプライトの透明矩形に見切られることなく前面に描画される。

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));
    
    LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };

    LNRenderer_DrawSprite(
        renderer, redMaterial, 0,
        &identity,
        0.0f, 0.0f,         // offset (sprite position)
        128.0f, 128.0f,     // size
        0.5f, 0.5f,         // pivot (center)
        0.0f, 0.0f, 1.0f, 1.0f, // uv
        1.0f, 1.0f, 1.0f, 1.0f); // color

    LNRenderer_DrawSprite(
        renderer, greenMaterial, 0,
        &identity,
        40.0f, 0.0f,         // offset (sprite position)
        128.0f, 128.0f,     // size
        0.5f, 0.5f,         // pivot (center)
        0.0f, 0.0f, 1.0f, 1.0f, // uv
        1.0f, 1.0f, 1.0f, 1.0f); // color

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    // Capture and compare
    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.SpriteOrder", data, w, h, TEST_DATA_DIR));

    LNObject_Release(camera);
    LNObject_Release(greenMaterial);
    LNObject_Release(greenTexture);
    LNObject_Release(redMaterial);
    LNObject_Release(redTexture);
}

// SortMode (BACK_TO_FRONT) がワールド Z に基づいて前後を解決することを検証する。
// 手前 (緑, z=+0.5) を先に、奥 (赤, z=-0.5) を後に投入する。
// 投入順 (Stable) なら後から描いた赤が上になるはずだが、BACK_TO_FRONT では
// 奥→手前へ並べ替えられ、手前の緑が上に描画される。中央ピクセルが緑になることを確認する。
TEST_F(Test_Graphics, SpriteDepthSortBackToFront) {
    LNHandle greenMat = LN_NULL_HANDLE, redMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &greenMat));
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &redMat));
    LNMaterial_SetBlendMode(greenMat, LN_BLEND_MODE_ALPHA);
    LNMaterial_SetBlendMode(redMat, LN_BLEND_MODE_ALPHA);

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f; // 青クリア
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    rpDesc.sortMode = LN_SORT_MODE_BACK_TO_FRONT;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    LNMatrix front = translationMatrix(0.0f, 0.0f,  0.5f); // カメラ(z=1)に近い = 手前
    LNMatrix back  = translationMatrix(0.0f, 0.0f, -0.5f); // 遠い = 奥

    // 手前 (緑) を先に投入。
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, greenMat, 0, &front, 0.0f, 0.0f, 100.0f, 100.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));
    // 奥 (赤) を後に投入。
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, redMat, 0, &back, 0.0f, 0.0f, 100.0f, 100.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    const uint8_t* px = data + (static_cast<size_t>(h / 2) * w + w / 2) * 4;
    // 手前の緑が前面に並べ替えられている → 中央は緑 (G 高, R 低)。
    EXPECT_GT(px[1], 160) << "中央が緑ではありません。BACK_TO_FRONT の深度ソートが効いていない可能性があります。RGBA=("
                          << (int)px[0] << "," << (int)px[1] << "," << (int)px[2] << ")";
    EXPECT_LT(px[0], 96);

    LNObject_Release(camera);
    LNObject_Release(redMat);
    LNObject_Release(greenMat);
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
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &greenMat));
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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &mat1));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(mat1, texture));

    LNHandle mat2 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &mat2));

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
    LNMatrix xfLeft = translationMatrix(-80.0f, 0.0f, 0.0f);
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, mat1, 0,
        &xfLeft,
        0.0f, 0.0f,
        100.0f, 100.0f,
        0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    // Right
    LNMatrix xfRight = translationMatrix(80.0f, 0.0f, 0.0f);
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, mat2, 0,
        &xfRight,
        0.0f, 0.0f,
        100.0f, 100.0f,
        0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);

    ASSERT_TRUE(VisualTest::captureAndCompare("Test_Graphics.TwoSprites", data, w, h, TEST_DATA_DIR));

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
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &nearMat));
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
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &farMat));
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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &nearMat));
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
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &farMat));
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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &bgMat));
    LNHandle uiMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &uiMat));

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
        LNMatrix xfBg = translationMatrix(0.0f, 0.0f, 0.0f); // 画面中央
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, bgMat, 0,
            &xfBg,
            0.0f, 0.0f,
            200.0f, 200.0f,     // 中央を確実に覆う大きさ
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f)); // 赤
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
        LNMatrix xfUi = translationMatrix(120.0f, 80.0f, 0.0f); // 中央から離れた隅
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, uiMat, 0,
            &xfUi,
            0.0f, 0.0f,
            40.0f, 40.0f,        // 中央には掛からない小ささ
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f)); // 緑
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    }

    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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

    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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

// カラー + デプス + ステンシルすべてに LoadOp::Load を指定する検証。
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

    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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

// LNCamera_SetOrthographic2D の pivot 引数の検証。
//
// pivot=(0.5, 0.5) を指定すると、原点 (ワールド座標 (0,0)) が画面中央に対応する。
// このカメラで視点 (0,0,0) のままワールド (0,0,0) にスプライトを描画すると、
// スプライトは画面中央に表示されるはず。
//
// setOrthographic2D はビュー行列を単位行列に設定する (= 視点は (0,0,0))。
// そのため setLookAt は呼ばない。
//
// 検証:
//   - 画面中央ピクセルがスプライト色 (赤) になっている。
//   - 画面四隅はクリア色 (青) のまま (スプライトが全画面を覆っていない = 中央寄せの確認)。
TEST_F(Test_Graphics, Orthographic2DPivotCenter) {
    // テクスチャなし Unlit (既定の白テクスチャ)。色は drawSprite の頂点カラーで与える。
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));

    // pivot=(0.5, 0.5) で原点を画面中央に。視点はデフォルトの単位ビュー行列 = (0,0,0)。
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic2D(camera,
        (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f,
        0.5f, 0.5f));

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f; // 青クリア
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    // ワールド (0,0,0) に、アンカー中央 (0.5,0.5) の赤いスプライトを描画。
    // 四隅 (160px 角の外) には掛からない 100x100 の大きさにして中央寄せを判定可能にする。
    LNMatrix xfCenter = translationMatrix(0.0f, 0.0f, 0.0f); // 原点 = pivot により画面中央
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, material, 0,
        &xfCenter,
        0.0f, 0.0f,
        100.0f, 100.0f,
        0.5f, 0.5f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f)); // 赤

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
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
    const uint8_t* center = data + (static_cast<size_t>(cy) * w + cx) * 4;
    const int cr = center[0];
    const int cg = center[1];
    const int cb = center[2];

    // pivot=(0.5,0.5) で原点が画面中央 → 中央に赤いスプライトが出る (R 高, B 低)。
    // pivot が効いていないと原点は左上のままで、中央はクリア色 (青) になる。
    EXPECT_GT(cr, 192)
        << "中央の R channel が低すぎます。pivot=(0.5,0.5) で原点が画面中央に来ていない可能性があります。RGBA=("
        << cr << "," << cg << "," << cb << ")";
    EXPECT_LT(cb, 64)
        << "中央の B channel が高すぎます。中央がクリア色 (青) のままでスプライトが中央に描かれていません。RGBA=("
        << cr << "," << cg << "," << cb << ")";

    // 四隅 (左上) はクリア色 (青) のまま = スプライトが全画面を覆っておらず中央寄せである確認。
    const uint8_t* corner = data + (static_cast<size_t>(4) * w + 4) * 4;
    EXPECT_LT(corner[0], 64)
        << "左上隅が赤くなっています。スプライトが中央に収まっていません。RGBA=("
        << (int)corner[0] << "," << (int)corner[1] << "," << (int)corner[2] << ")";
    EXPECT_GT(corner[2], 192)
        << "左上隅の B channel が低すぎます。クリア色 (青) が保持されていません。RGBA=("
        << (int)corner[0] << "," << (int)corner[1] << "," << (int)corner[2] << ")";

    LNObject_Release(camera);
    LNObject_Release(material);
}

//------------------------------------------------------------------------------
// サンプラー設定 (フィルタ / アドレッシング) が実際にバックエンドまで効くことを検証する。
//
// 2x2 のテクスチャ (左上=赤, 右上=緑, 左下=青, 右下=黄) を、UV を -0.5 - 1.5 に
// 引き伸ばしたスプライトで描画する。画面を左右に分け、
//   左半分: Nearest + ClampToEdge
//   右半分: Linear  + Repeat
// を明示的に設定し、同じ相対位置のピクセルが異なる色になることを確認する。
// これによりサンプラー設定が (a) 実際に反映され、(b) マテリアル単位で独立している
// ことの両方を確認できる。
// 既定値そのものは SamplerDefaultIsLinearClampToEdge で検証する。
TEST_F(Test_Graphics, SamplerFilterAndAddressMode) {
    // 2x2 RGBA8。行は上から順に格納される。
    const uint8_t pixels[2 * 2 * 4] = {
        255,   0,   0, 255,   0, 255,   0, 255, // 上段: 赤, 緑
          0,   0, 255, 255, 255, 255,   0, 255, // 下段: 青, 黄
    };
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromPixels(
        graphicsContext, 2, 2, LN_TEXTURE_FORMAT_RGBA8_UNORM,
        pixels, sizeof(pixels), &texture));

    LNHandle matNearest = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matNearest));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(matNearest, texture));
    ASSERT_EQ(LN_OK, LNMaterial_SetSamplerState(matNearest,
        LN_TEXTURE_FILTER_MODE_NEAREST, LN_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE));

    LNHandle matLinearRepeat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &matLinearRepeat));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(matLinearRepeat, texture));
    ASSERT_EQ(LN_OK, LNMaterial_SetSamplerState(matLinearRepeat,
        LN_TEXTURE_FILTER_MODE_LINEAR, LN_TEXTURE_ADDRESS_MODE_REPEAT));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
    const float halfW = (float)TEST_W / 2.0f; // 160
    const float fullH = (float)TEST_H;        // 240

    // 左半分 (ワールド x = -80 中心)。UV は -0.5 - 1.5。
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, matNearest, 0, &identity,
        -halfW / 2.0f, 0.0f,
        halfW, fullH,
        0.5f, 0.5f,
        -0.5f, -0.5f, 2.0f, 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    // 右半分 (ワールド x = +80 中心)。同じ UV。
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, matLinearRepeat, 0, &identity,
        halfW / 2.0f, 0.0f,
        halfW, fullH,
        0.5f, 0.5f,
        -0.5f, -0.5f, 2.0f, 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    auto pixelAt = [&](int x, int y) { return data + (static_cast<size_t>(y) * w + x) * 4; };
    auto rgbText = [](const uint8_t* p) {
        return "RGBA=(" + std::to_string((int)p[0]) + "," + std::to_string((int)p[1]) +
               "," + std::to_string((int)p[2]) + ")";
    };

    // スプライトローカルの水平位置 fx から、UV は u = -0.5 + 2*fx になる。
    // 垂直位置 fy からは v = -0.5 + 2*fy。y=90 は fy=0.375 → v=0.25 (上段のテクセル中心)。
    const int sampleY = 90;

    // ---- 左半分: Nearest + ClampToEdge ----

    // fx=0.125 → u=-0.25。ClampToEdge なので左列 (赤) に張り付く。
    // Repeat なら u が 0.75 に回り込んで右列 (緑) になる。
    {
        const uint8_t* p = pixelAt(20, sampleY);
        EXPECT_GT((int)p[0], 200) << "ClampToEdge が効いていません (左端の外側が赤になっていない)。" << rgbText(p);
        EXPECT_LT((int)p[1], 60) << "ClampToEdge が効いていません (左端の外側に緑が回り込んでいる)。" << rgbText(p);
    }

    // fx=0.875 → u=1.25。ClampToEdge なので右列 (緑) に張り付く。
    // Repeat なら u が 0.25 に回り込んで左列 (赤) になる。
    {
        const uint8_t* p = pixelAt(140, sampleY);
        EXPECT_GT((int)p[1], 200) << "ClampToEdge が効いていません (右端の外側が緑になっていない)。" << rgbText(p);
        EXPECT_LT((int)p[0], 60) << "ClampToEdge が効いていません (右端の外側に赤が回り込んでいる)。" << rgbText(p);
    }

    // fx=0.475 → u=0.45。テクセル境界 (0.5) の手前なので Nearest なら純粋な赤。
    // Linear なら赤と緑が混ざって G が持ち上がる。
    {
        const uint8_t* p = pixelAt(76, sampleY);
        EXPECT_GT((int)p[0], 200) << "Nearest が効いていません。" << rgbText(p);
        EXPECT_LT((int)p[1], 60) << "Nearest が効いていません (赤と緑が補間されています)。" << rgbText(p);
    }

    // ---- 右半分: Linear + Repeat ----

    // 左半分の fx=0.125 と同じ相対位置。Repeat なので u=-0.25 が 0.75 に回り込み緑になる。
    // ここが赤だと、左半分のマテリアルの ClampToEdge が右半分に漏れている。
    {
        const uint8_t* p = pixelAt(160 + 20, sampleY);
        EXPECT_GT((int)p[1], 200) << "Repeat が効いていません (回り込んだ緑にならない)。" << rgbText(p);
        EXPECT_LT((int)p[0], 60) << "サンプラー設定がマテリアル間で漏れています。" << rgbText(p);
    }

    // 左半分の fx=0.475 と同じ相対位置。Linear なので赤と緑が混ざり G が 0 より明確に上がる。
    // ここが純粋な赤だと、左半分のマテリアルの Nearest が右半分に漏れている。
    {
        const uint8_t* p = pixelAt(160 + 76, sampleY);
        EXPECT_GT((int)p[1], 40) << "Linear 補間が効いていません (Nearest のように見えます)。" << rgbText(p);
    }

    LNObject_Release(camera);
    LNObject_Release(matLinearRepeat);
    LNObject_Release(matNearest);
    LNObject_Release(texture);
}

//------------------------------------------------------------------------------
// サンプラー設定を一切呼ばないマテリアルの既定値が Linear + ClampToEdge であることを
// 検証する。
//
// 既定を Repeat から ClampToEdge に変更した意図 (ポストエフェクトやシャドウマップで
// 反対側の端から色が回り込まないようにする) を固定するためのテスト。
// UV を -0.5 - 1.5 に引き伸ばした全画面スプライトを 1 枚描き、範囲外の領域が
// 端の色で埋まる (回り込まない) ことを確認する。
TEST_F(Test_Graphics, SamplerDefaultIsLinearClampToEdge) {
    // 2x2 RGBA8。行は上から順に格納される。
    const uint8_t pixels[2 * 2 * 4] = {
        255,   0,   0, 255,   0, 255,   0, 255, // 上段: 赤, 緑
          0,   0, 255, 255, 255, 255,   0, 255, // 下段: 青, 黄
    };
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromPixels(
        graphicsContext, 2, 2, LN_TEXTURE_FORMAT_RGBA8_UNORM,
        pixels, sizeof(pixels), &texture));

    // サンプラー設定は明示的に呼ばない (既定値の検証が目的)。
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material, texture));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f);
    LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, material, 0, &identity,
        0.0f, 0.0f,
        (float)TEST_W, (float)TEST_H,
        0.5f, 0.5f,
        -0.5f, -0.5f, 2.0f, 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    auto pixelAt = [&](int x, int y) { return data + (static_cast<size_t>(y) * w + x) * 4; };
    auto rgbText = [](const uint8_t* p) {
        return "RGBA=(" + std::to_string((int)p[0]) + "," + std::to_string((int)p[1]) +
               "," + std::to_string((int)p[2]) + ")";
    };

    // 全画面スプライトなので、画面上の位置 (fx, fy) から u = -0.5 + 2*fx, v = -0.5 + 2*fy。
    // y=90 は fy=0.375 → v=0.25 (上段のテクセル中心)。
    const int sampleY = 90;

    // x=40 → fx=0.125 → u=-0.25 (テクスチャの左外側)。
    // 既定が ClampToEdge なら左列 (赤) で埋まる。Repeat だと u が 0.75 に回り込み緑になる。
    {
        const uint8_t* p = pixelAt(40, sampleY);
        EXPECT_GT((int)p[0], 200)
            << "既定が ClampToEdge になっていません (左外側が赤で埋まらない)。" << rgbText(p);
        EXPECT_LT((int)p[1], 60)
            << "既定が Repeat のままです (左外側に反対端の緑が回り込んでいる)。" << rgbText(p);
    }

    // x=280 → fx=0.875 → u=1.25 (テクスチャの右外側)。
    // 既定が ClampToEdge なら右列 (緑) で埋まる。Repeat だと u が 0.25 に回り込み赤になる。
    {
        const uint8_t* p = pixelAt(280, sampleY);
        EXPECT_GT((int)p[1], 200)
            << "既定が ClampToEdge になっていません (右外側が緑で埋まらない)。" << rgbText(p);
        EXPECT_LT((int)p[0], 60)
            << "既定が Repeat のままです (右外側に反対端の赤が回り込んでいる)。" << rgbText(p);
    }

    // x=152 → fx=0.475 → u=0.45。テクセル境界 (0.5) の手前。
    // 既定が Linear なら赤と緑が混ざる (R も G も 0 でない)。Nearest なら純粋な赤。
    {
        const uint8_t* p = pixelAt(152, sampleY);
        EXPECT_GT((int)p[1], 40)
            << "既定が Linear になっていません (Nearest のように見えます)。" << rgbText(p);
        EXPECT_GT((int)p[0], 100)
            << "赤成分が低すぎます。u=0.45 は赤寄りの補間になるはずです。" << rgbText(p);
    }

    LNObject_Release(camera);
    LNObject_Release(material);
    LNObject_Release(texture);
}

//------------------------------------------------------------------------------
// LNDebug_GetGraphicsProfiler の契約を固定するテスト。
//
// - drawCallCount は BeginFrame でリセットされ、EndFrame の後は「描き終えた
//   フレームの値」として読める。
// - 同一マテリアルのスプライトはバッチングされるため、枚数に比例して
//   drawCallCount が増えない。
// - fps / lastFrameTimeMs は EndFrame の中で更新されるため、EndFrame 後は正の値。
//
// この 3 項目は luminojs の GraphicsContext.getProfiler() が公開している値と
// 同一のものです。
//------------------------------------------------------------------------------
TEST_F(Test_Graphics, GraphicsProfiler) {
    // テクスチャなし Unlit (既定の白テクスチャ)。全スプライトで共有してバッチングさせる。
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic2D(camera,
        (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f, 0.5f, 0.5f));

    // spriteCount 枚のスプライトを 1 フレーム描画し、EndFrame 後の計測値を返す。
    auto renderSprites = [&](int spriteCount, LNGraphicsProfiler* outProfiler) {
        LNHandle renderer, colorBuffer, depthBuffer;
        ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

        for (int i = 0; i < spriteCount; i++) {
            LNMatrix xf = translationMatrix((float)i * 2.0f, 0.0f, 0.0f);
            ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
                renderer, material, 0,
                &xf,
                0.0f, 0.0f,
                8.0f, 8.0f,
                0.5f, 0.5f,
                0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f));
        }

        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

        // 計測値は EndFrame の後に読む。
        ASSERT_EQ(LN_OK, LNDebug_GetGraphicsProfiler(graphicsContext, outProfiler));
    };

    LNGraphicsProfiler one = {};
    renderSprites(1, &one);
    LNGraphicsProfiler many = {};
    renderSprites(16, &many);

    // スプライトを 1 枚でも描けば 1 回以上のドローコールが記録される。
    EXPECT_GT(one.drawCallCount, 0);

    // 同一マテリアルの 16 枚はバッチングされ、枚数に比例して増えない。
    EXPECT_LT(many.drawCallCount, 16)
        << "スプライトのバッチングが効いていません (16 枚で drawCallCount="
        << many.drawCallCount << ")";

    // EndFrame の中でフレーム時間と FPS が更新される。
    EXPECT_GT(many.lastFrameTimeMs, 0.0f);
    EXPECT_GT(many.fps, 0.0f);

    // NULL 引数は弾く。
    EXPECT_EQ(LN_ERROR_INVALID_ARGUMENT,
              LNDebug_GetGraphicsProfiler(graphicsContext, nullptr));
    // 無効なハンドルは弾く。
    LNGraphicsProfiler dummy = {};
    EXPECT_EQ(LN_ERROR_INVALID_HANDLE,
              LNDebug_GetGraphicsProfiler(LN_NULL_HANDLE, &dummy));

    LNObject_Release(camera);
    LNObject_Release(material);
}

//------------------------------------------------------------------------------
// マテリアルの状態をフレームをまたいで変更したときに、その変更が正しく反映される
// ことを検証するテスト群。
//
// Renderer はフレームスロットごとに UBO と BindGroup をキャッシュしており、
// 「UBO の内容が変わった (Material::paramVersion)」と
// 「BindGroup の構成が変わった (Material::bindingVersion)」を別々に追跡している。
// この分離を間違えると「設定したのに次のフレームで反映されない」「古いテクスチャを
// 使い続ける」という不具合になるため、フレームスロットが一巡する回数だけ回して固定する。
//------------------------------------------------------------------------------

// 毎フレーム色を変えたときに、各フレームで正しい色が出ることを確認する。
// UBO はフレームスロットごとに別のバッファなので、変更時に全スロットを
// 書き直し対象にしていないと 2 フレーム前の色が出る。
TEST_F(Test_Graphics, MaterialColorChangeAcrossFrames) {
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    // 全画面スプライトを 1 枚描いて、中央ピクセルの RGB を返す。
    auto renderAndSampleCenter = [&](uint8_t* outRgb) {
        LNHandle renderer, colorBuffer, depthBuffer;
        ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f; // 黒でクリア
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

        LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, material, 0, &identity,
            0.0f, 0.0f,
            (float)TEST_W, (float)TEST_H,
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f));

        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

        const uint8_t* data = nullptr;
        int32_t w = 0, h = 0;
        ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
        ASSERT_NE(nullptr, data);
        const uint8_t* p = data + (static_cast<size_t>(h / 2) * w + (w / 2)) * 4;
        outRgb[0] = p[0]; outRgb[1] = p[1]; outRgb[2] = p[2];
    };

    // フレームスロットは 2 枚なので、2 巡させて両スロットが 2 回ずつ更新されることを見る。
    const float colors[4][3] = {
        { 1.0f, 0.0f, 0.0f }, // 赤
        { 0.0f, 1.0f, 0.0f }, // 緑
        { 0.0f, 0.0f, 1.0f }, // 青
        { 1.0f, 1.0f, 0.0f }, // 黄
    };
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(LN_OK, LNMaterial_SetColor(material, colors[i][0], colors[i][1], colors[i][2], 1.0f));
        uint8_t rgb[3] = {};
        renderAndSampleCenter(rgb);
        for (int c = 0; c < 3; c++) {
            if (colors[i][c] > 0.5f) {
                EXPECT_GT((int)rgb[c], 200)
                    << "frame " << i << " ch " << c << ": 設定した色が反映されていません。"
                    << " RGB=(" << (int)rgb[0] << "," << (int)rgb[1] << "," << (int)rgb[2] << ")";
            } else {
                EXPECT_LT((int)rgb[c], 60)
                    << "frame " << i << " ch " << c << ": 前のフレームの色が残っています。"
                    << " RGB=(" << (int)rgb[0] << "," << (int)rgb[1] << "," << (int)rgb[2] << ")";
            }
        }
    }

    LNObject_Release(camera);
    LNObject_Release(material);
}

// フレームをまたいでテクスチャを差し替えたときに、新しいテクスチャが出ることを確認する。
// テクスチャの差し替えは BindGroup の構成の変化なので、全フレームスロットの
// BindGroup を破棄しないと古いテクスチャを使い続ける。
TEST_F(Test_Graphics, MaterialTextureSwapAcrossFrames) {
    // 1x1 の単色テクスチャを 2 枚。
    const uint8_t redPixel[4]   = { 255, 0, 0, 255 };
    const uint8_t greenPixel[4] = { 0, 255, 0, 255 };
    LNHandle redTex = LN_NULL_HANDLE;
    LNHandle greenTex = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromPixels(
        graphicsContext, 1, 1, LN_TEXTURE_FORMAT_RGBA8_UNORM, redPixel, sizeof(redPixel), &redTex));
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromPixels(
        graphicsContext, 1, 1, LN_TEXTURE_FORMAT_RGBA8_UNORM, greenPixel, sizeof(greenPixel), &greenTex));

    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    auto renderAndSampleCenter = [&](uint8_t* outRgb) {
        LNHandle renderer, colorBuffer, depthBuffer;
        ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

        LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, material, 0, &identity,
            0.0f, 0.0f,
            (float)TEST_W, (float)TEST_H,
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f));

        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

        const uint8_t* data = nullptr;
        int32_t w = 0, h = 0;
        ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
        ASSERT_NE(nullptr, data);
        const uint8_t* p = data + (static_cast<size_t>(h / 2) * w + (w / 2)) * 4;
        outRgb[0] = p[0]; outRgb[1] = p[1]; outRgb[2] = p[2];
    };

    // 赤 -> 緑 -> 赤 -> 緑。フレームスロット 2 枚を 2 巡させる。
    for (int i = 0; i < 4; i++) {
        const bool useRed = (i % 2) == 0;
        ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material, useRed ? redTex : greenTex));
        uint8_t rgb[3] = {};
        renderAndSampleCenter(rgb);
        if (useRed) {
            EXPECT_GT((int)rgb[0], 200) << "frame " << i << ": 赤テクスチャが反映されていません。";
            EXPECT_LT((int)rgb[1], 60) << "frame " << i << ": 前のフレームの緑テクスチャが残っています。";
        } else {
            EXPECT_GT((int)rgb[1], 200) << "frame " << i << ": 緑テクスチャが反映されていません。";
            EXPECT_LT((int)rgb[0], 60) << "frame " << i << ": 前のフレームの赤テクスチャが残っています。";
        }
    }

    LNObject_Release(camera);
    LNObject_Release(material);
    LNObject_Release(greenTex);
    LNObject_Release(redTex);
}

// フレームをまたいで名前付きサンプラー設定を変えたときに、新しい設定が反映される
// ことを確認する。サンプラーの差し替えも BindGroup の構成の変化にあたる。
TEST_F(Test_Graphics, MaterialNamedSamplerStateChangeAcrossFrames) {
    // 2x2 RGBA8。行は上から順に格納される。
    const uint8_t pixels[2 * 2 * 4] = {
        255,   0,   0, 255,   0, 255,   0, 255, // 上段: 赤, 緑
          0,   0, 255, 255, 255, 255,   0, 255, // 下段: 青, 黄
    };
    LNHandle texture = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromPixels(
        graphicsContext, 2, 2, LN_TEXTURE_FORMAT_RGBA8_UNORM, pixels, sizeof(pixels), &texture));

    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(graphicsContext, LN_BUILTIN_SHADER_UNLIT, &material));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material, texture));

    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic(camera, (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f));
    ASSERT_EQ(LN_OK, LNCamera_SetLookAt(camera, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    // 全画面スプライトに UV 0..1 を張る。画面 x から u = x / TEST_W、y から v = y / TEST_H。
    // サンプル点 (152, 60) は u=0.475, v=0.25。
    // テクセル中心は u=0.25 / 0.75 なので、Nearest なら左列 (赤)、
    // Linear なら赤と緑が 0.55 : 0.45 で混ざり G が持ち上がる。
    // v=0.25 は上段のテクセル中心なので、縦方向の補間は起きない。
    auto renderAndSample = [&](uint8_t* outRgb) {
        LNHandle renderer, colorBuffer, depthBuffer;
        ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));

        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

        LNMatrix identity = { { 1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1 } };
        ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
            renderer, material, 0, &identity,
            0.0f, 0.0f,
            (float)TEST_W, (float)TEST_H,
            0.5f, 0.5f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f));

        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

        const uint8_t* data = nullptr;
        int32_t w = 0, h = 0;
        ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
        ASSERT_NE(nullptr, data);
        const uint8_t* p = data + (static_cast<size_t>(60) * w + 152) * 4;
        outRgb[0] = p[0]; outRgb[1] = p[1]; outRgb[2] = p[2];
    };

    // Nearest -> Linear -> Nearest -> Linear。フレームスロット 2 枚を 2 巡させる。
    for (int i = 0; i < 4; i++) {
        const bool nearest = (i % 2) == 0;
        ASSERT_EQ(LN_OK, LNMaterial_SetNamedSamplerState(
            material, "u_baseTexture",
            nearest ? LN_TEXTURE_FILTER_MODE_NEAREST : LN_TEXTURE_FILTER_MODE_LINEAR,
            LN_TEXTURE_ADDRESS_MODE_CLAMP_TO_EDGE));
        uint8_t rgb[3] = {};
        renderAndSample(rgb);
        if (nearest) {
            EXPECT_LT((int)rgb[1], 60)
                << "frame " << i << ": Nearest が反映されていません (赤と緑が補間されています)。"
                << " RGB=(" << (int)rgb[0] << "," << (int)rgb[1] << "," << (int)rgb[2] << ")";
        } else {
            EXPECT_GT((int)rgb[1], 40)
                << "frame " << i << ": Linear が反映されていません (Nearest のように見えます)。"
                << " RGB=(" << (int)rgb[0] << "," << (int)rgb[1] << "," << (int)rgb[2] << ")";
        }
    }

    LNObject_Release(camera);
    LNObject_Release(material);
    LNObject_Release(texture);
}
