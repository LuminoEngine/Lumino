#pragma once
#include <LuminoEngine/Testing/GraphicsTestHelper.hpp>

//#define LN_UNIT_TEST_EXPERIMENTAL

class TestEnv {
public:
    static void initialize();
    static void setup(GraphicsAPI graphicsAPI);
    static void teardown();
    static void updateFrame();
    static void resetScene();

    static GraphicsCommandList* graphicsContext();
    static GraphicsContext* mainWindowSwapChain();
    static void resetGraphicsContext(GraphicsCommandList* context);
    static GraphicsCommandList* beginFrame();
    static void endFrame();
    static RenderPass* renderPass();
    static bool checkCurrentBackbufferScreenShot(const Path& filePath, int passRate = 95, bool save = false);

    static bool isCI;
    static String LuminoCLI;
    static Ref<DepthBuffer> depthBuffer;
    static RenderTargetTexture* lastBackBuffer;
};

#define LN_TEST_CLEAN_SCENE TestEnv::resetScene();

#define ASSERT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkCurrentBackbufferScreenShot(filePath))
#define ASSERT_SCREEN_S(filePath) ASSERT_TRUE(TestEnv::checkCurrentBackbufferScreenShot(filePath, 95, true))
#define ASSERT_CURRENT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkCurrentBackbufferScreenShot(filePath))
