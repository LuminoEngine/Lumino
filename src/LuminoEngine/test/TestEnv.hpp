#pragma once

class TestEnv
{
public:
	static void setup();
	static void teardown();

	static void saveScreenShot(const Char* filePath);
	static bool equalsScreenShot(const Char* filePath, int passRate);
	static bool equalsBitmapFile(Bitmap2D* bmp1, const Char* filePath, int passRate);
	static bool checkScreenShot(const Char* filePath, int passRate = 99, bool save = false);	// 基本的に 99% 一致していれば良い。グラボによって、色成分+-1 くらいの誤差がある
	static void waitRendering();
};

#define ASSERT_SCREEN(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath)) 
#define ASSERT_SCREEN_S(filePath) ASSERT_TRUE(TestEnv::checkScreenShot(filePath, 99, true)) 
