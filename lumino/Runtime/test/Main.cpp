
int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	setlocale(LC_ALL, "");

	if (argc == 1) {
		char* testArgs[] = {
			argv[0],
			"--gtest_break_on_failure",
			"--gtest_filter=Test_Base_Promise.*"
		};
		argc = sizeof(testArgs) / sizeof(char*);
		argv = testArgs;
	}

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	return result;
}
