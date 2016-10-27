#include <TestConfig.h>

//==============================================================================
class Test_Graphics_Color : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Constructor)
{
	Color32 c1;
	ASSERT_EQ(Color32(0, 0, 0, 0), c1);
	Color32 c2(1, 2, 3);
	ASSERT_EQ(Color32(1, 2, 3, 255), c2);
	Color32 c3(1, 2, 3, 4);
	ASSERT_EQ(Color32(1, 2, 3, 4), c3);
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Operators)
{
	Color32 c1(1, 2, 3, 4);
	Color32 c2(1, 2, 3, 4);
	Color32 c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}

//==============================================================================
class Test_Graphics_Viewport : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Viewport, Basic)
{
	const SizeI& size = Viewport::GetMainViewport()->GetSize();
	ASSERT_EQ(160, size.width);
	ASSERT_EQ(120, size.height);
}
