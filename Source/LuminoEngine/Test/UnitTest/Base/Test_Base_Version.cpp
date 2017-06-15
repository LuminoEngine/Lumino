#include <TestConfig.h>

class Test_Base_Version : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Base_Version, Basic)
{
	// マクロや関数が呼べること。コンパイルエラーにならなければOK。
	{
		int major = LUMINO_ENGINE_VERSION_MAJOR;
		int minor = LUMINO_ENGINE_VERSION_MINOR;
		int revison = LUMINO_ENGINE_VERSION_REVISION;
		int build = LUMINO_ENGINE_VERSION_BUILD;
		const TCHAR* str = LUMINO_ENGINE_VERSION_STRING;
        major; minor; revison; build; str;  // ignore warning
	}
	{
		int major = Version::Engine::GetMajor();
		int minor = Version::Engine::GetMinor();
		int revison = Version::Engine::getRevision();
		int build = Version::Engine::GetBuild();
		const TCHAR* str = Version::Engine::getString();
	}
}
