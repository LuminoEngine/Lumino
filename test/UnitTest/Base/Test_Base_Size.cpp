#include <TestConfig.h>

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

//-----------------------------------------------------------------------------
TEST_F(Test_Base_Size, Constants)
{
	ASSERT_EQ_SIZE(0, 0, Size::Zero);

	ASSERT_EQ_SIZE(0, 0, SizeF::Zero);
}

//-----------------------------------------------------------------------------
TEST_F(Test_Base_Size, Basic)
{
	Size s1;
	ASSERT_EQ_SIZE(0, 0, s1);
	Size s2(1, 2);
	ASSERT_EQ_SIZE(1, 2, s2);

	s2.Set(3, 4);
	ASSERT_EQ_SIZE(3, 4, s2);

	ASSERT_TRUE(s1.IsZero());
	ASSERT_FALSE(s2.IsZero());

	Size s3(3, 4);
	ASSERT_TRUE(s2 == s3);
	ASSERT_FALSE(s1 == s2);
	ASSERT_FALSE(s2 != s3);
	ASSERT_TRUE(s1 != s2);

	ASSERT_TRUE(SizeF(0, 0).IsAnyZero());
	ASSERT_TRUE(SizeF(1, 0).IsAnyZero());
	ASSERT_TRUE(SizeF(0, 1).IsAnyZero());
	ASSERT_FALSE(SizeF(1, 1).IsAnyZero());
}

//-----------------------------------------------------------------------------
TEST_F(Test_Base_Size, Float)
{
	SizeF s1;
	ASSERT_EQ_SIZE_FLOAT(0, 0, s1);
	SizeF s2(0.5f, 0.75f);
	ASSERT_EQ_SIZE_FLOAT(0.5f, 0.75f, s2);

	s2.Set(3.5f, 4.25f);
	ASSERT_EQ_SIZE_FLOAT(3.5f, 4.25f, s2);

	ASSERT_TRUE(s1.IsZero());
	ASSERT_FALSE(s2.IsZero());

	SizeF s3(3.5f, 4.25f);
	ASSERT_TRUE(s2 == s3);
	ASSERT_FALSE(s1 == s2);
	ASSERT_FALSE(s2 != s3);
	ASSERT_TRUE(s1 != s2);

	ASSERT_TRUE(SizeF(0.0f, 0.0f).IsAnyZero());
	ASSERT_TRUE(SizeF(1.0f, 0.0f).IsAnyZero());
	ASSERT_TRUE(SizeF(0.0f, 1.0f).IsAnyZero());
	ASSERT_FALSE(SizeF(1.0f, 1.0f).IsAnyZero());
}
