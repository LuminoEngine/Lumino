#include "TestEnv.hpp"

class Test_BasicRendering : public ::testing::Test {};

TEST_F(Test_BasicRendering, Clear1) {

    LNHandle graphicsContext = TestEnv::graphicsContext;

    LNHandle renderingCommandList = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNGraphicsCommandList_Create(graphicsContext, &renderingCommandList));

    // Rendering loop.
    {
        LNHandle backbuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentColorBuffer(graphicsContext, &backbuffer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentDepthBuffer(graphicsContext, &depthBuffer));

        ASSERT_EQ(LN_OK, LNGraphicsCommandList_Reset(renderingCommandList, TestEnv::viewPoint));

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
        ASSERT_EQ(LN_OK, LNGraphicsCommandList_BeginRenderPass(renderingCommandList, descriptor, &renderingPass));

        ASSERT_EQ(LN_OK, LNRenderPass_End(renderingPass));

        ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(graphicsContext, renderingCommandList));

        TestEnv::present();
    }

    ASSERT_SCREENSHOT(U"Test_BasicRendering.Clear1.png");

    LNObject_Release(renderingCommandList);
}
