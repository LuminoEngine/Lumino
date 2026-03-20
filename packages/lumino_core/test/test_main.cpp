// ---------------------------------------------------------------------------
// Environment: 初期化・終了処理
// ---------------------------------------------------------------------------
class LuminoCoreEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // TODO: lumino_core の初期化処理をここに追加する
    }

    void TearDown() override {
        // TODO: lumino_core の終了処理をここに追加する
    }
};

// ---------------------------------------------------------------------------
// ダミーテストケース
// ---------------------------------------------------------------------------
TEST(LuminoCoreTest, Placeholder) {
    SUCCEED();
}

// ---------------------------------------------------------------------------
// エントリポイント
// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoCoreEnvironment());
    return RUN_ALL_TESTS();
}
