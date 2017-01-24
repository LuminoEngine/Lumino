#include <TestConfig.h>

class Test_Base_Resource : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Resource, Integrate)
{
	//Resources::RegisterResource(LN_LOCALFILE("TestData/ResourceTest1.txt"));
	//ASSERT_EQ(_T("value"), Resources::GetString(_T("test")));
}
