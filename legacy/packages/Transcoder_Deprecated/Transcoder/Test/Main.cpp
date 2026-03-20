
GTEST_API_ int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

	char* testArgs[] = {
		argv[0],
		"--gtest_break_on_failure",
		//"--gtest_filter=Test_Asset_AssetObject.*",
	};
	argc = sizeof(testArgs) / sizeof(char*);
	testing::InitGoogleTest(&argc, (char**)testArgs);

	int result = RUN_ALL_TESTS();
	return result;
}
