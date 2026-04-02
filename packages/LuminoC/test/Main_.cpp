#include <LuminoC/lumino.h>
#include <gtest/gtest.h>

class LuminoApiEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        ASSERT_EQ(LN_OK, LNInstance_Initialize());
    }

    void TearDown() override {
        LNInstance_Terminate();
    }
};

int main(int argc, char** argv) {
#if 0 // Debug Utility.
        if (argc == 1) {
            char* testArgs[] = {
                argv[0],
                (char*)"--gtest_break_on_failure",
                (char*)"--gtest_filter=Test_Object.Texture2DCreateAndRelease",
            };
            argc = sizeof(testArgs) / sizeof(char*);
            argv = testArgs;
        }
#endif
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
