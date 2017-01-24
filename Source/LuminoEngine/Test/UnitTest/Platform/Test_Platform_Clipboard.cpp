#include <TestConfig.h>

class Test_Platform_Clipboard : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Platform_Clipboard, SetText_GetText)
{
#if 0
	Platform::Clipboard::SetText(TestEnv::Application->GetPlatformManager()->GetMainWindow(), _T("test"));
	String str = Platform::Clipboard::GetText(TestEnv::Application->GetPlatformManager()->GetMainWindow());
	ASSERT_STREQ(_T("test"), str);
#endif
}
