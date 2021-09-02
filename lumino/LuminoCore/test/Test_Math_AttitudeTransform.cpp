#include "Test_Math_Common.hpp"

class Test_AttitudeTransform : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_AttitudeTransform, Basic)
{
	// コンストラクタ
	{
		AttitudeTransform t1;
		ASSERT_VEC3_NEAR(1, 1, 1, t1.scale);
		ASSERT_QUA_NEAR(0, 0, 0, 1, t1.rotation);
		ASSERT_VEC3_NEAR(0, 0, 0, t1.translation);
	}
	// コンストラクタ
	{
		AttitudeTransform t1(Vector3(1, 2, 3), Quaternion(4, 5, 6, 10), Vector3(7, 8, 9));
		ASSERT_VEC3_NEAR(1, 2, 3, t1.scale);
		ASSERT_QUA_NEAR(4, 5, 6, 10, t1.rotation);
		ASSERT_VEC3_NEAR(7, 8, 9, t1.translation);
	}
}
