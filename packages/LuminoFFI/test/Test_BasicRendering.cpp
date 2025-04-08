#include "TestEnv.hpp"

class Test_BasicRendering : public ::testing::Test {};

TEST_F(Test_BasicRendering, Clear1) {

    LNHandle surfaceContext = TestEnv::surfaceContext;

    // Rendering loop.
    {
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        ASSERT_EQ(
            LN_OK, LNGraphicsContext_PrepareFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer, &commandList));

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
        ASSERT_EQ(LN_OK, LNCommandList_BeginRenderPass(commandList, descriptor, TestEnv::viewPoint, &renderingPass));

        ASSERT_EQ(LN_OK, LNRenderPass_End(renderingPass));

        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(surfaceContext, commandList));

        ASSERT_SCREENSHOT(U"Test_BasicRendering.Clear1.png");

        TestEnv::present();
    }

}
