#include "TestEnv.hpp"

class Test_Mesh : public ::testing::Test {};

TEST_F(Test_Mesh, Basic1) {
    LNHandle surfaceContext = TestEnv::surfaceContext;

    // Load texture and create material.
    auto imageData = ln::FileSystem::readAllBytes(TestEnv::getTestDataPath(U"Rendering/Sprite.png")).unwrap();
    LNHandle texture1 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1));
    ASSERT_EQ(LN_OK, LNMaterial_Create(&material1));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material1, texture1));

    LNHandle mesh = LN_NULL_HANDLE;
    LNMesh_Create(&mesh);
    LNMesh_AddSprite2DSurface(mesh, material1, {32, 32}, {0, 0}, {0, 0, 1, 1}, {1, 1, 1, 1});

    LNHandle renderItem = LN_NULL_HANDLE;
    LNRenderItem_Create(&renderItem);
    LNRenderItem_SetMesh(renderItem, mesh);

    // Try drawing it a few times.
    for (int i = 0; i < 5; i++)
    {
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNGraphicsContext_BeginFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer);

        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = colorBuffer;
        descriptor.renderTargets[0].clearColor[0] = 0.0f;
        descriptor.renderTargets[0].clearColor[1] = 0.0f;
        descriptor.renderTargets[0].clearColor[2] = 1.0f;
        descriptor.renderTargets[0].clearColor[3] = 1.0f;
        descriptor.renderTargets[0].clearEnable = LN_TRUE;
        descriptor.depthBuffer.depthBuffer = depthBuffer;
        descriptor.depthBuffer.clearDepth = 1.0f;
        descriptor.depthBuffer.clearStencil = 0;
        descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
        descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
        LNGraphicsContext_BeginSceneRenderPass(surfaceContext, descriptor, TestEnv::viewPoint, &renderingPass);

        LNMatrix transform;
        LNMatrix_SetIdentity(&transform);
        transform.m41 = 0;
        transform.m42 = 0;
        LNRenderItem_SetTransform(renderItem, &transform);

        LNSceneRenderPass_DrawRenderItem(renderingPass, renderItem);

        LNGraphicsContext_EndSceneRenderPass(surfaceContext, renderingPass);

        LNGraphicsContext_EndFrame(surfaceContext);
        ASSERT_SCREENSHOT(U"Test_Mesh.Basic1/Expects.png");
        TestEnv::present();
    };

    LNObject_Release(renderItem);
    LNObject_Release(mesh);
    LNObject_Release(material1);
    LNObject_Release(texture1);
}
