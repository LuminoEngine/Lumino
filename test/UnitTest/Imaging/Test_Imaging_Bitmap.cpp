#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Imaging_Bitmap : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Imaging_Bitmap, PngFile)
{
	Bitmap bmp1(LOCALFILE("TestData/img1_BYTE_R8G8B8A8.png"));

	// png のデフォルトの読み取り
	ASSERT_EQ(PixelFormat_BYTE_R8G8B8A8, bmp1.GetPixelFormat());

	ASSERT_EQ(32, bmp1.GetSize().Width);
	ASSERT_EQ(32, bmp1.GetSize().Height);

	byte_t* buf = bmp1.GetBitmapBuffer()->GetData();
	//printf("%X %X %X %X\n", buf[0], buf[1], buf[2], buf[3]);
	ASSERT_EQ(0xFF, buf[0]);	// R
	ASSERT_EQ(0x7F, buf[1]);	// G
	ASSERT_EQ(0x3F, buf[2]);	// B
	ASSERT_EQ(0xFF, buf[3]);	// A

	int rowSize = bmp1.GetSize().Width * 4;

	byte_t* rt = &buf[rowSize - 4];	// 右上px
	ASSERT_EQ(0x00, rt[0]);		// R
	ASSERT_EQ(0xFF, rt[1]);		// G
	ASSERT_EQ(0x00, rt[2]);		// B
	ASSERT_EQ(0xFF, rt[3]);		// A

	byte_t* lb = &buf[rowSize * 31];// 左下px
	ASSERT_EQ(0x00, lb[0]);		// R
	ASSERT_EQ(0x7F, lb[1]);		// G
	ASSERT_EQ(0xFF, lb[2]);		// B
	ASSERT_EQ(0xFF, lb[3]);		// A

	byte_t* rb = &lb[rowSize - 4];	// 右下px
	ASSERT_EQ(0xFF, rb[0]);		// R
	ASSERT_EQ(0x00, rb[1]);		// G
	ASSERT_EQ(0xFF, rb[2]);		// B
	ASSERT_EQ(0x7F, rb[3]);		// A
}

//-----------------------------------------------------------------------------
TEST_F(Test_Imaging_Bitmap, Save)
{
	Bitmap bmp1(LOCALFILE("TestData/img1_BYTE_R8G8B8A8.png"));
	bmp1.Save(LOCALFILE("TestData/img1_BYTE_R8G8B8A8_cpy.png"));
}

//-----------------------------------------------------------------------------
TEST_F(Test_Imaging_Bitmap, BitBlt)
{
	Bitmap bmp1(Size(32, 32), PixelFormat_BYTE_R8G8B8A8);
	Bitmap bmp2(LOCALFILE("TestData/img2_BYTE_R8G8B8A8.png"));
	bmp1.BitBlt(Rect(0, 0, 32, 32), &bmp2, Rect(0, 0, 32, 32), Color::White, false);
	ASSERT_TRUE(TestEnv::EqualsBitmapFile(&bmp1, LOCALFILE("TestData/img2_BYTE_R8G8B8A8.png")));

}
