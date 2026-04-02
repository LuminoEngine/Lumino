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
