#include "Common.hpp"
#include <LuminoCore/Base/Environment.hpp>

class Test_Base_Environment : public ::testing::Test {};

TEST_F(Test_Base_Environment, GetEnvironmentVariable)
{
	auto value = Environment::getEnvironmentVariable(_LT("PATH"));
	ASSERT_EQ(true, value.hasValue());
	ASSERT_EQ(false, (*value).isEmpty());

	value = Environment::getEnvironmentVariable(_LT("ASDFGHJKL"));
	ASSERT_EQ(false, value.hasValue());
}

