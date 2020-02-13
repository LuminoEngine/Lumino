
#include <stdio.h>
#include "Common.hpp"
#include "TestEnv.hpp"
#include <LuminoEngine/Platform/PlatformSupport.hpp>

GTEST_API_ int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

    // CI 環境でテストをスキップする
    if (!ln::detail::checkGraphicsSupport()) {
        printf("Info: The driver does not appear to support Graphics. Skip LuminoEngine unit tests.\n");
        return 0;
    }

	char* testArgs[] = {
		argv[0],
		"--gtest_break_on_failure",
		//"--gtest_filter=Test_Base_Event.*"
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);

	//GlobalLogger::setLevel(LogLevel::Verbose);

	TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
	TestHelper::setTempDirPath(Environment::specialFolderPath(SpecialFolder::Temporary));
	TestEnv::setup();
	int result = RUN_ALL_TESTS();
	TestEnv::teardown();

	return result;
}
