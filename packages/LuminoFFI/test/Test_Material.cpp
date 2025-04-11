#include "TestEnv.hpp"

class Test_Material : public ::testing::Test {};

TEST_F(Test_Material, Basic1) {
    LNHandle surfaceContext = TestEnv::surfaceContext;

    LNHandle material1 = LN_NULL_HANDLE;
    auto filePath = TestEnv::getTestDataPath(U"Test_Material.Basic1/test.slang");
    LNMaterial_CreateFromSourceFile(filePath.str().toUtf8().c_str(), &material1);

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    LNBatchRenderer_Get(&spriteRenderer);

    // Rendering loop.
    for (int i = 0; i < 5; i++)
    {
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        LNGraphicsContext_PrepareFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer, &commandList);

        int index = -1;
        LNMaterial_FindParameterIndex(material1, "u_Value1", &index);
        LNMaterial_SetFloat(material1, index, 1.0f);

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
        LNCommandList_BeginRenderPass(commandList, descriptor, TestEnv::viewPoint, &renderingPass);

        LNMatrix transform;
        LNMatrix_SetIdentity(&transform);
        transform.m41 = 0;
        transform.m42 = 0;
        LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material1, &transform);
        LNBatchRenderer_DrawSprite(
            spriteRenderer,
            NULL,
            320,
            240,
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            1,
            1,
            LN_SPRITE_BASE_DIRECTION_BASIC2D,
            LN_BILLBOARD_TYPE_NONE);
        LNBatchRenderer_EndBatch(spriteRenderer);

        LNRenderPass_End(renderingPass);

        LNGraphicsContext_EndFrame(surfaceContext, commandList);
        ASSERT_SCREENSHOT(U"Test_Material.Basic1/Expects.png");
        TestEnv::present();
    };

    LNObject_Release(material1);
}
