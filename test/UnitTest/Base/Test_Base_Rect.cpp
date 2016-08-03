#include <TestConfig.h>

class Test_Base_Rect : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define ASSERT_EQ_RECT(x_, y_, w_, h_, rc) \
	ASSERT_EQ(x_, rc.x); \
	ASSERT_EQ(y_, rc.y); \
	ASSERT_EQ(w_, rc.width); \
	ASSERT_EQ(h_, rc.height);

//------------------------------------------------------------------------------
TEST_F(Test_Base_Rect, Basic)
{
	Rect rc1;
	ASSERT_EQ_RECT(0, 0, 0, 0, rc1);
	Rect rc2(1, 2, 3, 4);
	ASSERT_EQ_RECT(1, 2, 3, 4, rc2);
	Rect rc3(Point(5, 6), SizeI(7, 8));
	ASSERT_EQ_RECT(5, 6, 7, 8, rc3);
	Rect rc4(rc3);
	ASSERT_EQ_RECT(5, 6, 7, 8, rc4);

	ASSERT_EQ(5, rc4.GetLeft());
	ASSERT_EQ(6, rc4.GetTop());
	ASSERT_EQ(12, rc4.GetRight());
	ASSERT_EQ(14, rc4.GetBottom());

	ASSERT_EQ(Point(5, 6), rc4.GetTopLeft());
	ASSERT_EQ(Point(12, 6), rc4.GetTopRight());
	ASSERT_EQ(Point(5, 14), rc4.GetBottomLeft());
	ASSERT_EQ(Point(12, 14), rc4.GetBottomRight());

	rc3.SetLocation(Point(10, 20));
	ASSERT_EQ(Point(10, 20), rc3.GetLocation());

	rc3.SetSize(SizeI(30, 40));
	ASSERT_EQ(SizeI(30, 40), rc3.GetSize());

	Rect rc5(0, 0, -1, -2);
	ASSERT_TRUE(rc5.IsEmpty());
	ASSERT_FALSE(rc1.IsEmpty());
	ASSERT_FALSE(rc3.IsEmpty());

	ASSERT_TRUE(rc1.IsZero());
	ASSERT_FALSE(rc5.IsZero());


	ASSERT_TRUE(Rect(0, 1, 2, 3) == Rect(0, 1, 2, 3));
	ASSERT_FALSE(Rect(0, 1, 2, 3) == Rect(5, 6, 7, 8));
	ASSERT_FALSE(Rect(0, 1, 2, 3) != Rect(0, 1, 2, 3));
	ASSERT_TRUE(Rect(0, 1, 2, 3) != Rect(5, 6, 7, 8));
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Rect, Contains)
{
	Rect rc1(10, 20, 30, 40);
	ASSERT_TRUE(rc1.Contains(10, 20));
	ASSERT_TRUE(rc1.Contains(Point(30, 40)));

	// 同じサイズ → true
	Rect rc2(10, 20, 30, 40);
	ASSERT_TRUE(rc1.Contains(rc2));

	// 内部 → true
	Rect rc3(15, 25, 5, 6);
	ASSERT_TRUE(rc1.Contains(rc3));

	// 1px 大きい → false
	Rect rc4(10, 20, 30, 40 + 1);
	ASSERT_FALSE(rc1.Contains(rc4));

	// 1px ずれ → false
	Rect rc5(10, 20 - 1, 30, 40);
	ASSERT_FALSE(rc1.Contains(rc5));

	// rc1 全体を囲む → false
	Rect rc6(0, 1, 100, 200);
	ASSERT_FALSE(rc1.Contains(rc6));

}
