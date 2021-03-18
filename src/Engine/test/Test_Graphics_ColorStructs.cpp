#include "Common.hpp"

//==============================================================================
class Test_Graphics_Color : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Constructor)
{
	ColorI c1;
	ASSERT_EQ(ColorI(0, 0, 0, 0), c1);
	ColorI c2(1, 2, 3);
	ASSERT_EQ(ColorI(1, 2, 3, 255), c2);
	ColorI c3(1, 2, 3, 4);
	ASSERT_EQ(ColorI(1, 2, 3, 4), c3);
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Operators)
{
	ColorI c1(1, 2, 3, 4);
	ColorI c2(1, 2, 3, 4);
	ColorI c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, parse)
{
	auto c1 = Color::parse(u"#FF0000");
	ASSERT_EQ(true, c1.r >= 1.0f);
	ASSERT_EQ(true, c1.g == 0.0f);
	ASSERT_EQ(true, c1.b == 0.0f);
	ASSERT_EQ(true, c1.a >= 1.0f);

	auto c2 = Color::parse(u"#FF0000FF");
	ASSERT_EQ(true, c2.r == 0.0f);
	ASSERT_EQ(true, c2.g == 0.0f);
	ASSERT_EQ(true, c2.b >= 1.0f);
	ASSERT_EQ(true, c2.a >= 1.0f);

	auto c3 = Color::parse(u"#0F0");
	ASSERT_EQ(true, c3.r == 0.0f);
	ASSERT_EQ(true, c3.g >= 1.0f);
	ASSERT_EQ(true, c3.b == 0.0f);
	ASSERT_EQ(true, c3.a >= 1.0f);

	auto c4 = Color::parse(u"#0F00");
	ASSERT_EQ(true, c4.r >= 1.0f);
	ASSERT_EQ(true, c4.g == 0.0f);
	ASSERT_EQ(true, c4.b == 0.0f);
	ASSERT_EQ(true, c4.a == 0.0f);
}

