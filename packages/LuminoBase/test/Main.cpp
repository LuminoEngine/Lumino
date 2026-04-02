//------------------------------------------------------------------------------
// Environment: 初期化・終了処理
//------------------------------------------------------------------------------
class LuminoBaseEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

//------------------------------------------------------------------------------
// エントリポイント
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
#if 0 // Debug Utility.
    if (argc == 1) {
        char* testArgs[] = {
            argv[0],
            (char*)"--gtest_break_on_failure",
            (char*)"--gtest_filter=Test_Matrix4x4.ViewProjectionRH",
        };
        argc = sizeof(testArgs) / sizeof(char*);
        argv = testArgs;
    }
#endif
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoBaseEnvironment());
    return RUN_ALL_TESTS();
}
