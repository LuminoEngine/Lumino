#include "TestEnv.hpp"

class Test_BatchRendering : public ::testing::Test {};

// 測定環境:
// - Processor: 11th Gen Intel(R) Core(TM) i9-11900 @ 2.50GHz (16 CPUs), ~2.5GHz
// - Memory: 32768MB RAM
// - Card name: NVIDIA GeForce RTX 3070
// - OpenGL
// 測定方法:
// A. 1回描画 (ドローコール 1)
// B. Batch 使わないで 1000 回描画 (ドローコール 1000)
// C. Batch 使って 1000 回描画 (ドローコール 1)
// 結果:
// A. 12.7[ms]
// B. 13.9[ms]
// C. 12.7[ms]
// 考察:
// 2024/11/21 時点ではそもそもの描画が重いが、1000回を1回にまとめることで描画速度が向上している。
TEST_F(Test_BatchRendering, Basic1) {
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
        ASSERT_EQ(
            LN_OK, LNGraphicsContext_BeginFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer));

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
                LN_OK,
                LNGraphicsContext_BeginSceneRenderPass(surfaceContext, descriptor, TestEnv::viewPoint, &renderingPass));

            // Draw Sprite.
            {
                LNMatrix transform;
                LNMatrix_SetIdentity(&transform);

                // 5回描いてみる。
                for (int i = 0; i < 5; i++) {
                    transform.m41 = i * 32;
                    LNDrawSpriteParams params = {};
                    params.worldTransform = transform;
                    params.material = material1;
                    params.size = {32, 32};
                    params.anchorRatio = {0, 0};
                    params.uvRect = {0, 0, 1, 1};
                    params.color = {1, 1, 1, 1};
                    params.baseDirection = LN_SPRITE_BASE_DIRECTION_BASIC2D;
                    params.billboardType = LN_BILLBOARD_TYPE_NONE;
                    LNSceneRenderPass_DrawSprite(renderingPass, &params);
                }
            }

            ASSERT_EQ(LN_OK, LNGraphicsContext_EndSceneRenderPass(surfaceContext, renderingPass));
        }

        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(surfaceContext));
        ASSERT_SCREENSHOT(U"Test_BatchRendering.Basic1/Expects.png");

        // ドローコールは1回だけ。
        LNGraphicsProfilerng profilerng;
        ASSERT_EQ(LN_OK, LNDebug_GetGraphicsProfilerng(surfaceContext, &profilerng));
        ASSERT_EQ(1, profilerng.drawCallCount);

        TestEnv::present();
    }

    LNObject_Release(material1);
    LNObject_Release(texture1);
}

TEST_F(Test_BatchRendering, TooMany10000) {
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
        ASSERT_EQ(
            LN_OK, LNGraphicsContext_BeginFrame(surfaceContext, 320, 240, &colorBuffer, &depthBuffer));

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
                LN_OK,
                LNGraphicsContext_BeginSceneRenderPass(surfaceContext, descriptor, TestEnv::viewPoint, &renderingPass));

            // Draw Sprite.
            {
                LNMatrix transform;
                LNMatrix_SetIdentity(&transform);

                // たくさん描いてみる。
                for (int i = 0; i < 10000; i++) {
                    transform.m41 = i * 32;
                    LNDrawSpriteParams params = {};
                    params.worldTransform = transform;
                    params.material = material1;
                    params.size = {32, 32};
                    params.anchorRatio = {0, 0};
                    params.uvRect = {0, 0, 1, 1};
                    params.color = {1, 1, 1, 1};
                    params.baseDirection = LN_SPRITE_BASE_DIRECTION_BASIC2D;
                    params.billboardType = LN_BILLBOARD_TYPE_NONE;
                    LNSceneRenderPass_DrawSprite(renderingPass, &params);
                }
            }

            ASSERT_EQ(LN_OK, LNGraphicsContext_EndSceneRenderPass(surfaceContext, renderingPass));
        }

        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(surfaceContext));
        
        // ドローコールは1回だけ。
        LNGraphicsProfilerng profilerng;
        ASSERT_EQ(LN_OK, LNDebug_GetGraphicsProfilerng(surfaceContext, &profilerng));
        ASSERT_EQ(2, profilerng.drawCallCount);
        // NOTE: 現在インデックスの総数 50000 個くらいになっていて、10000個描画すると 60000 使うことになる。
        // なので 50000+10000 の2回に分けられる。

        TestEnv::present();
    }

    LNObject_Release(material1);
    LNObject_Release(texture1);
}
