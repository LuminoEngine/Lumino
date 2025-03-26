#include "TestEnv.hpp"

class Test_BasicRendering : public ::testing::Test {};

TEST_F(Test_BasicRendering, Clear1) {

    LNHandle surfaceContext = TestEnv::surfaceContext;

    LNHandle renderingCommandList = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCommandList_Get(surfaceContext, &renderingCommandList));

    // Rendering loop.
    {
        LNHandle backbuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentColorBuffer(surfaceContext, &backbuffer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_GetCurrentDepthBuffer(surfaceContext, &depthBuffer));

        ASSERT_EQ(LN_OK, LNCommandList_Reset(renderingCommandList));

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

        ASSERT_EQ(LN_OK, LNRenderPass_End(renderingPass));

        ASSERT_EQ(LN_OK, LNGraphicsContext_SubmitCommandList(surfaceContext, renderingCommandList));

        TestEnv::present();
    }

    ASSERT_SCREENSHOT(U"Test_BasicRendering.Clear1.png");
}
