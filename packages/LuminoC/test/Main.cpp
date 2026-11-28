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
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
