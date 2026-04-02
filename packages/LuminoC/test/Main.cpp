#include <LuminoC/lumino.h>
#include <gtest/gtest.h>

class LuminoApiEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        LNInstanceInitializeSettings settings = {};
        settings.enableValidation = LN_TRUE;
        ASSERT_EQ(LN_OK, LNInstance_Initialize(&settings));
    }

    void TearDown() override {
        LNInstance_Terminate();
    }
};

int main(int argc, char** argv) {
#if 1 // Debug Utility.
        if (argc == 1) {
            char* testArgs[] = {
                argv[0],
                (char*)"--gtest_break_on_failure",
                (char*)"--gtest_filter=Test_Graphics.StencilMask1",
            };
            argc = sizeof(testArgs) / sizeof(char*);
            argv = testArgs;
        }
#endif
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
