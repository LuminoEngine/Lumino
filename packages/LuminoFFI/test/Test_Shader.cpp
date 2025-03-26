#include "TestEnv.hpp"

class Test_Shader : public ::testing::Test {};

TEST_F(Test_Shader, Basic1) {
    LNHandle surfaceContext = TestEnv::surfaceContext;

    // Create Shader.
    const auto code = TestEnv::compileShader(TestEnv::getTestDataPath(U"Test_Shader.Basic1/Test_Shader.Basic1.fx"));
    LNHandle shader1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNShader_Create(code.data(), code.size(), &shader1));

    // Create Material.
    LNHandle material1 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_Create(&material1));
    ASSERT_EQ(LN_OK, LNMaterial_SetShader(material1, shader1));

    LNHandle renderingCommandList = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCommandList_Get(surfaceContext, &renderingCommandList));

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNBatchRenderer_Get(&spriteRenderer));

    // Rendering loop.
    {
        // Begin frame.
        LNHandle backbuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentColorBuffer(surfaceContext, &backbuffer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentDepthBuffer(surfaceContext, &depthBuffer));
        ASSERT_EQ(LN_OK, LNCommandList_Reset(renderingCommandList));

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
            ASSERT_EQ(LN_OK, LNCommandList_BeginRenderPass(renderingCommandList, descriptor, TestEnv::viewPoint, &renderingPass));

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

        ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(surfaceContext, renderingCommandList));
        TestEnv::present();
    }

    ASSERT_SCREENSHOT(U"Test_Shader.Basic1/Expects.png");

    LNObject_Release(material1);
    LNObject_Release(shader1);
}
