#include <TestConfig.h>

//==============================================================================
class Test_Base_Point : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define ASSERT_EQ_POINT(x_, y_, v) \
	ASSERT_EQ(x_, v.x); \
	ASSERT_EQ(y_, v.y);

#define ASSERT_EQ_POINT_FLOAT(x_, y_, v) \
	ASSERT_FLOAT_EQ(x_, v.x); \
	ASSERT_FLOAT_EQ(y_, v.y);

//------------------------------------------------------------------------------
TEST_F(Test_Base_Point, Basic)
{
	PointI pt1;
	ASSERT_EQ_POINT(0, 0, pt1);
	PointI pt2(1, 2);
	ASSERT_EQ_POINT(1, 2, pt2);

	pt2.set(3, 4);
	ASSERT_EQ_POINT(3, 4, pt2);

	ASSERT_TRUE(pt1.IsZero());
	ASSERT_FALSE(pt2.IsZero());

	PointI pt3(3, 4);
	ASSERT_TRUE(pt2 == pt3);
	ASSERT_FALSE(pt1 == pt2);
	ASSERT_FALSE(pt2 != pt3);
	ASSERT_TRUE(pt1 != pt2);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Point, Float)
{
	PointF pt1;
	ASSERT_EQ_POINT_FLOAT(0, 0, pt1);
	PointF pt2(0.5f, 0.75f);
	ASSERT_EQ_POINT_FLOAT(0.5f, 0.75f, pt2);

	pt2.set(3.5f, 4.25f);
	ASSERT_EQ_POINT_FLOAT(3.5f, 4.25f, pt2);

	ASSERT_TRUE(pt1.IsZero());
	ASSERT_FALSE(pt2.IsZero());

	PointF pt3(3.5f, 4.25f);
	ASSERT_TRUE(pt2 == pt3);
	ASSERT_FALSE(pt1 == pt2);
	ASSERT_FALSE(pt2 != pt3);
	ASSERT_TRUE(pt1 != pt2);
}

//==============================================================================
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
//TEST_F(Test_Base_Rect, Basic)
//{
//	RectI rc1;
//	ASSERT_EQ_RECT(0, 0, 0, 0, rc1);
//	RectI rc2(1, 2, 3, 4);
//	ASSERT_EQ_RECT(1, 2, 3, 4, rc2);
//	RectI rc3(PointI(5, 6), SizeI(7, 8));
//	ASSERT_EQ_RECT(5, 6, 7, 8, rc3);
//	RectI rc4(rc3);
//	ASSERT_EQ_RECT(5, 6, 7, 8, rc4);
//
//	ASSERT_EQ(5, rc4.GetLeft());
//	ASSERT_EQ(6, rc4.GetTop());
//	ASSERT_EQ(12, rc4.getRight());
//	ASSERT_EQ(14, rc4.GetBottom());
//
//	ASSERT_EQ(PointI(5, 6), rc4.GetTopLeft());
//	ASSERT_EQ(PointI(12, 6), rc4.GetTopRight());
//	ASSERT_EQ(PointI(5, 14), rc4.GetBottomLeft());
//	ASSERT_EQ(PointI(12, 14), rc4.GetBottomRight());
//
//	rc3.SetLocation(PointI(10, 20));
//	ASSERT_EQ(PointI(10, 20), rc3.GetLocation());
//
//	rc3.SetSize(SizeI(30, 40));
//	ASSERT_EQ(SizeI(30, 40), rc3.GetSize());
//
//	RectI rc5(0, 0, -1, -2);
//	ASSERT_TRUE(rc5.IsEmpty());
//	ASSERT_FALSE(rc1.IsEmpty());
//	ASSERT_FALSE(rc3.IsEmpty());
//
//	ASSERT_TRUE(rc1.IsZero());
//	ASSERT_FALSE(rc5.IsZero());
//
//
//	ASSERT_TRUE(RectI(0, 1, 2, 3) == RectI(0, 1, 2, 3));
//	ASSERT_FALSE(RectI(0, 1, 2, 3) == RectI(5, 6, 7, 8));
//	ASSERT_FALSE(RectI(0, 1, 2, 3) != RectI(0, 1, 2, 3));
//	ASSERT_TRUE(RectI(0, 1, 2, 3) != RectI(5, 6, 7, 8));
//}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Rect, contains)
{
	Rect rc1(10, 20, 30, 40);
	ASSERT_TRUE(rc1.contains(10, 20));
	ASSERT_TRUE(rc1.contains(PointF(30, 40)));

	// 同じサイズ → true
	Rect rc2(10, 20, 30, 40);
	ASSERT_TRUE(rc1.contains(rc2));

	// 内部 → true
	Rect rc3(15, 25, 5, 6);
	ASSERT_TRUE(rc1.contains(rc3));

	// 1px 大きい → false
	Rect rc4(10, 20, 30, 40 + 1);
	ASSERT_FALSE(rc1.contains(rc4));

	// 1px ずれ → false
	Rect rc5(10, 20 - 1, 30, 40);
	ASSERT_FALSE(rc1.contains(rc5));

	// rc1 全体を囲む → false
	Rect rc6(0, 1, 100, 200);
	ASSERT_FALSE(rc1.contains(rc6));

}

//==============================================================================
class Test_Base_Size : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

#define ASSERT_EQ_SIZE(x, y, v) \
	ASSERT_EQ(x, v.width); \
	ASSERT_EQ(y, v.height);

#define ASSERT_EQ_SIZE_FLOAT(x, y, v) \
	ASSERT_FLOAT_EQ(x, v.width); \
	ASSERT_FLOAT_EQ(y, v.height);

//------------------------------------------------------------------------------
TEST_F(Test_Base_Size, Constants)
{
	ASSERT_EQ_SIZE(0, 0, SizeI::Zero);

	ASSERT_EQ_SIZE(0, 0, Size::Zero);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Size, Basic)
{
	SizeI s1;
	ASSERT_EQ_SIZE(0, 0, s1);
	SizeI s2(1, 2);
	ASSERT_EQ_SIZE(1, 2, s2);

	s2.set(3, 4);
	ASSERT_EQ_SIZE(3, 4, s2);

	ASSERT_TRUE(s1.IsZero());
	ASSERT_FALSE(s2.IsZero());

	SizeI s3(3, 4);
	ASSERT_TRUE(s2 == s3);
	ASSERT_FALSE(s1 == s2);
	ASSERT_FALSE(s2 != s3);
	ASSERT_TRUE(s1 != s2);

	ASSERT_TRUE(Size(0, 0).IsAnyZero());
	ASSERT_TRUE(Size(1, 0).IsAnyZero());
	ASSERT_TRUE(Size(0, 1).IsAnyZero());
	ASSERT_FALSE(Size(1, 1).IsAnyZero());
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_Size, Float)
{
	Size s1;
	ASSERT_EQ_SIZE_FLOAT(0, 0, s1);
	Size s2(0.5f, 0.75f);
	ASSERT_EQ_SIZE_FLOAT(0.5f, 0.75f, s2);

	s2.set(3.5f, 4.25f);
	ASSERT_EQ_SIZE_FLOAT(3.5f, 4.25f, s2);

	ASSERT_TRUE(s1.IsZero());
	ASSERT_FALSE(s2.IsZero());

	Size s3(3.5f, 4.25f);
	ASSERT_TRUE(s2 == s3);
	ASSERT_FALSE(s1 == s2);
	ASSERT_FALSE(s2 != s3);
	ASSERT_TRUE(s1 != s2);

	ASSERT_TRUE(Size(0.0f, 0.0f).IsAnyZero());
	ASSERT_TRUE(Size(1.0f, 0.0f).IsAnyZero());
	ASSERT_TRUE(Size(0.0f, 1.0f).IsAnyZero());
	ASSERT_FALSE(Size(1.0f, 1.0f).IsAnyZero());
}

