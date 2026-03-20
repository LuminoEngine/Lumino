#include "TestEnv.hpp"

int main(int argc, char** argv) {
#if 0
    if (argc == 1) {
        char* testArgs[] = {
            argv[0],
            "--gtest_break_on_failure",
            "--gtest_filter=Test_BasicRendering.*"
        };
        argc = sizeof(testArgs) / sizeof(char*);
        argv = testArgs;
    }
#endif

#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    setlocale(LC_ALL, "");

    testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnv);
    return RUN_ALL_TESTS();
}
