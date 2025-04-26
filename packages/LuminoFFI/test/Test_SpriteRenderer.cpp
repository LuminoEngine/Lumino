#include "TestEnv.hpp"

class Test_SpriteRenderer : public ::testing::Test {};

TEST_F(Test_SpriteRenderer, Basic1) {
    LNHandle surfaceContext = TestEnv::surfaceContext;

    // Load texture and create material.
    auto imageData = ln::FileSystem::readAllBytes(TestEnv::getTestDataPath(U"Rendering/Sprite.png")).unwrap();
    LNHandle texture1 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1));
    ASSERT_EQ(LN_OK, LNMaterial_Create(&material1));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material1, texture1));

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNBatchRenderer_Get(&spriteRenderer));

    // Rendering loop.
    {
        // Begin frame.
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        ASSERT_EQ(
            LN_OK, LNGraphicsContext_BeginFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer, &commandList));

        // Rendering pass.
        {
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
            ASSERT_EQ(
                LN_OK, LNCommandList_BeginSceneRenderPass(commandList, descriptor, TestEnv::viewPoint, &renderingPass));

            // Draw Sprite.
            {
                LNMatrix transform;
                LNMatrix_SetIdentity(&transform);
                transform.m41 = 10;
                transform.m42 = 20;

                LNDrawSpriteParams params = {};
                params.worldTransformOrNull = &transform;
                params.material = material1;
                params.size = {32, 32};
                params.anchorRatio = {0, 0};
                params.uvRect = {0, 0, 1, 1};
                params.color = {1, 1, 1, 1};
                params.baseDirection = LN_SPRITE_BASE_DIRECTION_BASIC2D;
                params.billboardType = LN_BILLBOARD_TYPE_NONE;
                LNSceneRenderPass_DrawSprite(renderingPass, &params);
            }

            ASSERT_EQ(LN_OK, LNCommandList_EndSceneRenderPass(commandList, renderingPass));
        }

        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(surfaceContext));
        ASSERT_SCREENSHOT(U"Rendering/Expects/Test_SpriteRenderer.Basic1.png");
        TestEnv::present();
    }

    LNObject_Release(material1);
    LNObject_Release(texture1);
}
