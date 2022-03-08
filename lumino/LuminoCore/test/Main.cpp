
#include <stdio.h>
#include "Common.hpp"
#include <LuminoCore.hpp>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>


static volatile bool g_ioDione = false;
extern "C" void setIODone()
{
	printf("called setIODone()\n");
	g_ioDione = true;
}



void pre1(void *arg)
{
	EM_ASM(
		//create your directory where we keep our persistent data
		FS.mkdir('/persistent_data'); 
	
		//mount persistent directory as IDBFS
		FS.mount(IDBFS,{},'/persistent_data');
	
		Module.print("start file sync..");
		//flag to check when data are synchronized
		Module.syncdone = 0;
	
		//populate persistent_data directory with existing persistent source data 
		//stored with Indexed Db
		//first parameter = "true" mean synchronize from Indexed Db to 
		//Emscripten file system,
		// "false" mean synchronize from Emscripten file system to Indexed Db
		//second parameter = function called when data are synchronized
		FS.syncfs(true, function(err) {
						assert(!err);
						Module.print("end file sync..");
						Module.syncdone = 1;
						//Module.ccall('setIODone', "");
		});
	);

	printf("waiting\n");
	//while (!g_ioDione) {

	//}
	printf("wait end\n");
}

static void ems_loop()
{
	static int count = 0;

	if (count == 10)
	{

		{
			FILE* fp = fopen("/persistent_data/out.txt", "r");
			if (fp) {
				printf("open file.");
				char str[256];
				fgets(str, 256, fp);
				printf(str);
			}
			else {
				printf("no file.");
			}
		}


			
		FILE* fp = fopen("/persistent_data/out.txt", "w");
		if (!fp) {
			printf("failed fp.");
			return;
		}
		printf("fp:%p\n", fp);
		fprintf(fp, "test");
		fclose(fp);
		
		//persist Emscripten current data to Indexed Db
		EM_ASM(
				Module.print("Start File sync..");
				Module.syncdone = 0;
				FS.syncfs(false, function(err) {
								assert(!err);
								Module.print("End File sync..");
								Module.syncdone = 1;
								});
		);
	}


	if (count == 50)
	{

		{
			FILE* fp = fopen("/persistent_data/out.txt", "r");
			if (fp) {
				printf("open file.\n");
				char str[256];
				fgets(str, 256, fp);
				printf(str);
			}
			else {
				printf("no file.\n");
			}
		}

	}

	if (count == 60)
	{
		emscripten_cancel_main_loop();
	}

	printf("count:%d\n", count);

	count++;
}

static void main_loop()
{
	static int init = 0;
	if (!init)
	{
		init = 1;

		char* testArgs[] =
		{
			"",
			"--gtest_break_on_failure",
			"--gtest_filter=Test_IO_FileSystem.*"
		};
		int argc = sizeof(testArgs) / sizeof(char*);
		testing::InitGoogleTest(&argc, (char**)testArgs);
		RUN_ALL_TESTS();
	}
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");

	printf("Running test.\n");
	emscripten_set_main_loop(main_loop, 1, true);
	return 0;
}

#else

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


bool myErrorHandler(Exception& e) {
	ln::detail::printError(e);
	return true;
}

#include "../src/IO/Internal.hpp"
#include <filesystem>


int main(int argc, char** argv)
{
	ln::Exception::setNotificationHandler(myErrorHandler);

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
#endif
	{
		int exitCode;
		if (testProcess(argc, argv, &exitCode)) {
			return exitCode;
		}
	}

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

	TestHelper::setAssetsDirPath(LN_LOCALFILE("TestData"));
	TestHelper::setTempDirPath(_T("TestTemp"));
	Logger::addStdErrAdapter();
	LN_LOG_INFO(U"Running test.");
    LN_LOG_INFO(U"SystemMultiByteEncoding: {}", TextEncoding::systemMultiByteEncoding()->name());
    LN_LOG_INFO(U"WideCharEncoding: {}", TextEncoding::wideCharEncoding()->name());

	if (argc == 1) {
		char* testArgs[] =
		{
			argv[0],
			"--gtest_break_on_failure",
			//"--gtest_filter=Test_Base_Array.filter",
		};
		argc = sizeof(testArgs) / sizeof(char*);
		argv = testArgs;
	}

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif
