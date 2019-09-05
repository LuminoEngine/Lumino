#pragma once

//#define LN_UNIT_TEST_EXPERIMENTAL

class TestEnv
{
public:
	static void setup();
	static void teardown();
    static void updateFrame();

	static void resetGraphicsContext(GraphicsContext* context);
	static Ref<Bitmap2D> capture();
	static void saveScreenShot(const Char* filePath);
	static bool equalsScreenShot(const Char* filePath, int passRate);
	static bool equalsBitmapFile(Bitmap2D* bmp1, const Char* filePath, int passRate);
	static bool checkScreenShot(const Char* filePath, int passRate = 95, bool save = false);	// 基本的に 95% くらい一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある
	static void waitRendering();

	static String LuminoCLI;
	static Ref<DepthBuffer> depthBuffer;
	static RenderTargetTexture* lastBackBuffer;
};

#define LN_TEST_CLEAN_SCENE \
    Engine::mainWorld()->removeAllObjects(); \
    Engine::mainUIRoot()->removeAllChildren(); \
	Engine::mainCamera()->setPosition(0, 0, -10); \
	Engine::mainCamera()->lookAt(Vector3(0, 0, 0)); \

#define ASSERT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath)) 
#define ASSERT_SCREEN_S(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, 95, true)) 
