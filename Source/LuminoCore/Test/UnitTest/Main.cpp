
#include <stdio.h>
#include "gtest/gtest.h"
#include "TestConfig.h"
#include <Lumino/IO/FileSystem.h>

PathName Test_GetTempFilePath(const TCHAR* fileName)
{
	PathName base(__FILE__);
	PathName tempDir(base.GetParent(), _T("../../"));
	tempDir.Append(_T("tmp"));
	PathName path(tempDir, fileName);
	FileSystem::CreateDirectory(path.GetParent());
	return PathName(path.c_str());
}

GTEST_API_ int main(int argc, char **argv)
{
	// Process クラスのテストスタブ
	if (strcmp(argv[argc - 1], "--proctest1") == 0)
	{
		printf("stdout");
		return 5;
	}
	else if (strcmp(argv[argc - 1], "--proctest2") == 0)
	{
		char str[256];
		scanf("%s", &str);
		return strlen(str);		// 文字数を返す
	}
	else if (strcmp(argv[argc - 1], "--proctest3") == 0)
	{
		char str[256];
		scanf("%s", &str);
		printf("[%s]", str);	// [ ] をつけて出力
		return 0;
	}
	else if (strcmp(argv[argc - 1], "--proctest4_utf8") == 0)
	{
        unsigned char str[4] = { 0xE3, 0x81, 0x82, 0x00 };	// UTF8 'あ'
		printf((char*)str);
		return 0;
	}

	printf("Running main()\n");
	
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

#if 1	// 部分的にテストを実行したりする
	char* testArgs[] = {
		argv[0],
		"--gtest_filter=Test_Base_Exception.*"
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);
#else
	testing::InitGoogleTest(&argc, argv);
#endif
	return RUN_ALL_TESTS();
}
