#include "Common.hpp"

class Test_Graphics_Bitmap : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Bitmap, SimpleLoadSave)
{
	//* [ ] downflow でピクセルデータを取得できること。
	{
		auto bmp1 = makeObject_deprecated<Bitmap2D>();
		bmp1->load(LN_ASSETFILE("Bitmap1.png"));
		ASSERT_EQ(ColorI(255, 0, 0, 255), bmp1->getPixel32(0, 0));
		ASSERT_EQ(ColorI(0, 255, 0, 255), bmp1->getPixel32(3, 0));
		ASSERT_EQ(ColorI(0, 0, 255, 255), bmp1->getPixel32(0, 3));
		ASSERT_EQ(ColorI(255, 255, 255, 0), bmp1->getPixel32(3, 3));
	}
	//* [ ] downflow でピクセルデータを保存できること。
	{
		{
			auto bmp0 = makeObject_deprecated<Bitmap2D>(4, 4, PixelFormat::RGBA8);
			bmp0->setPixel32(0, 0, ColorI(255, 0, 0, 255));
			bmp0->setPixel32(3, 0, ColorI(0, 255, 0, 255));
			bmp0->setPixel32(0, 3, ColorI(0, 0, 255, 255));
			bmp0->setPixel32(3, 3, ColorI(255, 255, 255, 0));
			bmp0->save(LN_TEMPFILE("bmp0.png"));
		}
		{
			auto bmp1 = makeObject_deprecated<Bitmap2D>();
			bmp1->load(LN_TEMPFILE("bmp0.png"));
			ASSERT_EQ(ColorI(255, 0, 0, 255), bmp1->getPixel32(0, 0));
			ASSERT_EQ(ColorI(0, 255, 0, 255), bmp1->getPixel32(3, 0));
			ASSERT_EQ(ColorI(0, 0, 255, 255), bmp1->getPixel32(0, 3));
			ASSERT_EQ(ColorI(255, 255, 255, 0), bmp1->getPixel32(3, 3));
		}
	}
}
