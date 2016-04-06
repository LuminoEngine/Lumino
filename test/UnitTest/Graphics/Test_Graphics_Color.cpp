#include <TestConfig.h>

class UnitTest_Graphics_Color : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//-----------------------------------------------------------------------------
TEST_F(UnitTest_Graphics_Color, Constructor)
{
	Color c1;
	ASSERT_EQ(Color(0, 0, 0, 0), c1);
	Color c2(1, 2, 3);
	ASSERT_EQ(Color(1, 2, 3, 255), c2);
	Color c3(1, 2, 3, 4);
	ASSERT_EQ(Color(1, 2, 3, 4), c3);
}

//-----------------------------------------------------------------------------
TEST_F(UnitTest_Graphics_Color, Operators)
{
	Color c1(1, 2, 3, 4);
	Color c2(1, 2, 3, 4);
	Color c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}
