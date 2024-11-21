#include "TestEnv.hpp"

class Test_SpriteRenderer : public ::testing::Test {};

TEST_F(Test_SpriteRenderer, Basic1) {

    LNHandle graphicsContext = TestEnv::graphicsContext;


    // Load texture and create material.
    auto imageData = ln::FileSystem::readAllBytes(TestEnv::getTestDataPath(U"Rendering/Sprite.png")).unwrap();
    LNHandle texture1 = LN_NULL_HANDLE;
    LNHandle material1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateFromImageFileData(imageData.data(), imageData.size(), &texture1));
    ASSERT_EQ(LN_OK, LNMaterial_Create(&material1));
    ASSERT_EQ(LN_OK, LNMaterial_SetMainTexture(material1, texture1));

    LNHandle renderingCommandList = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNGraphicsCommandList_Create(graphicsContext, &renderingCommandList));

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNBatchRenderer_Get(&spriteRenderer));

    // Rendering loop.
    {
        // Begin frame.
        LNHandle backbuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentColorBuffer(graphicsContext, &backbuffer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentDepthBuffer(graphicsContext, &depthBuffer));
        ASSERT_EQ(LN_OK, LNGraphicsCommandList_Reset(renderingCommandList));

        // Rendering pass.
        {
            LNHandle renderingPass = LN_NULL_HANDLE;
            LNRenderPassDescriptor descriptor;
            descriptor.renderTargets[0].renderTarget = backbuffer;
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
            ASSERT_EQ(LN_OK, LNGraphicsCommandList_BeginRenderPass(renderingCommandList, descriptor, TestEnv::viewPoint, &renderingPass));

            // Draw Sprite.
            {
                LNMatrix transform;
                LNMatrix_SetIdentity(&transform);
                transform.m41 = 10;
                transform.m42 = 20;
                LNBatchRenderer_BeginBatch(spriteRenderer, renderingCommandList, material1, &transform);
                LNBatchRenderer_DrawSprite(
                    spriteRenderer, NULL,
                    32, 32,
                    0, 0,
                    0, 0, 1, 1,
                    1, 1, 1, 1,
                    LN_SPRITE_BASE_DIRECTION_BASIC2D,
                    LN_BILLBOARD_TYPE_NONE);
                LNBatchRenderer_EndBatch(spriteRenderer);
            }

            ASSERT_EQ(LN_OK, LNRenderPass_End(renderingPass));
        }

        ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(graphicsContext, renderingCommandList));
        TestEnv::present();
    }

    ASSERT_SCREENSHOT(U"Rendering/Expects/Test_SpriteRenderer.Basic1.png");

    LNObject_Release(renderingCommandList);
    LNObject_Release(material1);
    LNObject_Release(texture1);
}
