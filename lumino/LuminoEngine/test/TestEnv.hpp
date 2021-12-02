#pragma once

//#define LN_UNIT_TEST_EXPERIMENTAL

class TestEnv
{
public:
	static void setup();
	static void teardown();
    static void updateFrame();
	static void resetScene();

	static GraphicsContext* graphicsContext();
	static SwapChain* mainWindowSwapChain();
	static void resetGraphicsContext(GraphicsContext* context);
	static GraphicsContext* beginFrame();
	static void endFrame();
	static RenderPass* renderPass();
	static Ref<Bitmap2D> capture(RenderTargetTexture* renderTarget = nullptr);
	static void saveScreenShot(const  Path& filePath, RenderTargetTexture* renderTarget = nullptr);
	static bool equalsScreenShot(const  Path& filePath, RenderTargetTexture* renderTarget, int passRate);
	static bool equalsBitmapFile(Bitmap2D* bmp1, const  Path& filePath, int passRate);
	static bool checkScreenShot(const Path& filePath, RenderTargetTexture* renderTarget, int passRate = 95, bool save = false);	// 基本的に 95% くらい一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある
	static void waitRendering();

	static String LuminoCLI;
	static Ref<DepthBuffer> depthBuffer;
	//static RenderTargetTexture* lastBackBuffer;
};

#define LN_TEST_CLEAN_SCENE TestEnv::resetScene();


#define ASSERT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, nullptr)) 
#define ASSERT_SCREEN_S(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, nullptr, 95, true))
#define ASSERT_CURRENT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, TestEnv::mainWindowSwapChain()->currentBackbuffer()))
#define ASSERT_RENDERTARGET(filePath, renderTarget) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, renderTarget)) 
#define ASSERT_RENDERTARGET_S(filePath, renderTarget) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, renderTarget, 95, true)) 

