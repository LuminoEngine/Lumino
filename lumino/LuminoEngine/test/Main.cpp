
#include <stdio.h>
#include <LuminoPlatform/PlatformSupport.hpp>
#include "Common.hpp"
#include "TestEnv.hpp"

int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

    //// CI 環境でテストをスキップする
    //if (!ln::detail::checkGraphicsSupport()) {
    //    printf("Info: The driver does not appear to support Graphics. Skip LuminoEngine unit tests.\n");
    //    return 0;
    //}

	if (argc == 1) {
		char* testArgs[] = {
			argv[0],
			"--gtest_break_on_failure",
			"--gtest_filter=Test_Base_Serializer.*"
		};
		argc = sizeof(testArgs) / sizeof(char*);
		argv = testArgs;
	}

	testing::InitGoogleTest(&argc, argv);

	Logger::setLevel(LogLevel::Debug);
	Logger::addStdErrAdapter();

	auto tempPath = Path(Environment::specialFolderPath(SpecialFolder::Temporary), _TT("Lumino"));

	TestHelper::setAssetsDirPath(LN_LOCALFILE("Assets"));
	TestHelper::setTempDirPath(tempPath);
	TestEnv::setup();
	int result = RUN_ALL_TESTS();
	TestEnv::teardown();

	return result;
}
