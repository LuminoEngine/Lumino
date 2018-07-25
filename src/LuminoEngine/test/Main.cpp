
#include <stdio.h>
#include "Common.hpp"
#include "TestEnv.hpp"

GTEST_API_ int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

	char* testArgs[] = {
		argv[0],
		"--gtest_break_on_failure",
		"--gtest_filter=Test_Graphics_LowLevelRendering.*"
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
