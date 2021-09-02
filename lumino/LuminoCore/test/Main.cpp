
#include "Common.hpp"

#ifdef _WIN32
#include <crtdbg.h>
#endif

#include <LuminoCore.hpp>

bool testProcess(int argc, char** argv, int* outExitCode)
{
	if (argc >= 2)
	{
		if (strcmp(argv[1], "proctest1") == 0) {
			*outExitCode = 2;
			return true;
		}
		else if (strcmp(argv[1], "proctest2") == 0) {
			char str[256];
			scanf("%s", &str);
			printf("[%s]", str);			// [ ] をつけて出力
			fprintf(stderr, "err:%s", str);	// err:をつけて出力
			*outExitCode = 0;
			return true;
		}
		else if (strcmp(argv[1], "proctest3") == 0)
		{
			unsigned char str[4] = { 0xE3, 0x81, 0x82, 0x00 };	// UTF8 'あ'
			printf((char*)str);
			*outExitCode = 0;
			return true;
		}
	}
	return false;
}

#include <filesystem>

int main(int argc, char** argv)
{

#if 0
	{
		ln::ElapsedTimer t1;
		t1.start();
		for (int i = 0; i < 1000000; i++) {
			//ln::detail::GenericStaticallyLocalPath<wchar_t> path("D:/Proj/Lumino/src/Core/test/Main.cpp", strlen("D:/Proj/Lumino/src/Core/test/Main.cpp"));
			//ln::detail::GenericStaticallyLocalPath<wchar_t> path("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", strlen("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
			ln::detail::GenericStaticallyLocalPath<wchar_t> path("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", strlen("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
		}
		std::cout << t1.elapsedNanoseconds() << "[ns]" << std::endl;
	}
	{
		ln::ElapsedTimer t1;
		t1.start();
		for (int i = 0; i < 1000000; i++) {
			//std::filesystem::path path("D:/Proj/Lumino/src/Core/test/Main.cpp");
			//std::filesystem::path path("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
			std::filesystem::path path("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
		}
		std::cout << t1.elapsedNanoseconds() << "[ns]" << std::endl;
	}

	return 0;
	{
		int exitCode;
		if (testProcess(argc, argv, &exitCode)) {
			return exitCode;
		}
	}
#endif

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

#ifdef __EMSCRIPTEN__
	{

		emscripten_push_main_loop_blocker(pre1, (void*)123);


		emscripten_set_main_loop(ems_loop, 60, true);
	}
#endif

	//TestHelper::setAssetsDirPath(LN_LOCALFILE("TestData"));
	//TestHelper::setTempDirPath(_T("TestTemp"));
	//Logger::addStdErrAdapter();
	//LN_LOG_INFO << "Running test.";
 //   LN_LOG_INFO << "SystemMultiByteEncoding: " << TextEncoding::systemMultiByteEncoding()->name();
 //   LN_LOG_INFO << "WideCharEncoding: " << TextEncoding::wideCharEncoding()->name();

	if (argc == 1) {
		char* testArgs[] =
		{
			argv[0],
			"--gtest_break_on_failure",
			//"--gtest_filter=Test_Base_String.insert",
		};
		argc = sizeof(testArgs) / sizeof(char*);
		argv = testArgs;
	}

	//testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
	return 0;
}

