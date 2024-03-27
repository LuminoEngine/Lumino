#include "Test_Math_Common.hpp"
#include <LuminoCore/Runtime/Archive.hpp>
#include <LuminoCore/Math/MathSerialization.hpp>

class Test_Math : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Math, Basic)
{
	// DegreesToRadians
	{
		ASSERT_FLOAT_EQ(Math::PI, Math::degreesToRadians(180.0f));
	}
	// RadiansToDegrees
	{
		ASSERT_FLOAT_EQ(180.0f, Math::radiansToDegrees(Math::PI));
	}
	// clamp
	{
		ASSERT_FLOAT_EQ(0.0, Math::clamp(0.0, 0.0, 1.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp(1.0, 0.0, 1.0));
		ASSERT_FLOAT_EQ(0.0, Math::clamp(0.0 - FLT_EPSILON, 0.0, 1.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp(1.0 + FLT_EPSILON, 0.0, 1.0));
	}
	// clamp01
	{
		ASSERT_FLOAT_EQ(0.0, Math::clamp01(0.0));
		ASSERT_FLOAT_EQ(1.0, Math::clamp01(1.0));
		ASSERT_FLOAT_EQ(0.0, Math::clamp01(0.0 - FLT_EPSILON));
		ASSERT_FLOAT_EQ(1.0, Math::clamp01(1.0 + FLT_EPSILON));
	}
	// nextPow2
	{
		ASSERT_EQ(2, Math::nextPow2(0));
		ASSERT_EQ(128, Math::nextPow2(127));
		ASSERT_EQ(128, Math::nextPow2(128));
		ASSERT_EQ(256, Math::nextPow2(129));
		ASSERT_EQ(1024, Math::nextPow2(900));
	}
	// NearEqual
	{
		ASSERT_EQ(true, Math::nearEqual(1.0, 1.0));
		ASSERT_EQ(false, Math::nearEqual(1.0, 1.0 + FLT_EPSILON));
		ASSERT_EQ(false, Math::nearEqual(1.0, 1.0 - FLT_EPSILON));
	}
	// isnan/isinf (float)
	{
		volatile float Float_Zero = 0.0f;
		float a = 0.0f / Float_Zero;	// NaN
		float b = 1.0f / Float_Zero;	// Inf

		ASSERT_EQ(true, Math::isNaN(a));
		ASSERT_EQ(false, Math::isNaN(b));
		ASSERT_EQ(false, Math::isInf(a));
		ASSERT_EQ(true, Math::isInf(b));
	}
	// isnan/isinf (double)
	{
		volatile double Float_Zero = 0.0;
		double a = 0.0 / Float_Zero;	// NaN
		double b = 1.0 / Float_Zero;	// Inf

		ASSERT_EQ(true, Math::isNaN(a));
		ASSERT_EQ(false, Math::isNaN(b));
		ASSERT_EQ(false, Math::isInf(a));
		ASSERT_EQ(true, Math::isInf(b));
	}
	// QuadAccel
	{
		ASSERT_FLOAT_EQ(0.0f, Math::quadAccel(0, 20.0, -9.8f, 0.0));
		ASSERT_FLOAT_EQ(20.4f, Math::quadAccel(0, 20.0, -9.8f, 2.0));
		ASSERT_FLOAT_EQ(1.599998f, Math::quadAccel(0, 20.0, -9.8f, 4.0));
		ASSERT_FLOAT_EQ(-56.400009f, Math::quadAccel(0, 20.0, -9.8f, 6.0));
		ASSERT_FLOAT_EQ(-153.600006f, Math::quadAccel(0, 20.0, -9.8f, 8.0));
	}
}
