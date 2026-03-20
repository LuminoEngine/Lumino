// ---------------------------------------------------------------------------
// Environment: 初期化・終了処理
// ---------------------------------------------------------------------------
class LuminoBaseEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // TODO: lumino_base の初期化処理をここに追加する
    }

    void TearDown() override {
        // TODO: lumino_base の終了処理をここに追加する
    }
};

// ---------------------------------------------------------------------------
// ダミーテストケース
// ---------------------------------------------------------------------------
TEST(LuminoBaseTest, Placeholder) {
    SUCCEED();
}

// ---------------------------------------------------------------------------
// エントリポイント
// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoBaseEnvironment());
    return RUN_ALL_TESTS();
}
