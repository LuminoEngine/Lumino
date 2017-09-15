#include <TestConfig.h>
#include <Lumino/Base/Diff.h>

class Test_Base_Environment : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Environment, GetEnvironmentVariable)
{
	String value;
	ASSERT_EQ(true, Environment::tryGetEnvironmentVariable(_LT("PATH"), &value));
	ASSERT_EQ(false, value.isEmpty());	// 何か入っているはず
	ASSERT_EQ(false, Environment::tryGetEnvironmentVariable(_LT("ASDFGHJKL"), &value));
}

