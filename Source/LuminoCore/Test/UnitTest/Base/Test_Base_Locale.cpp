#include <TestConfig.h>
#include <Lumino/Base/Locale.h>

class Test_Base_Locale : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//---------------------------------------------------------------------
TEST_F(Test_Base_Locale, Constructor)
{
	Locale locale;
}
