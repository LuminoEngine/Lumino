#include <gtest/gtest.h>

int main(int argc, char** argv) {
#if 1 // Debug Utility.
        if (argc == 1) {
            char* testArgs[] = {
                argv[0],
                (char*)"--gtest_break_on_failure",
                (char*)"--gtest_filter=Test_ParameterBlock2.ReflectParameterBlockLayout",
            };
            argc = sizeof(testArgs) / sizeof(char*);
            argv = testArgs;
        }
#endif
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
