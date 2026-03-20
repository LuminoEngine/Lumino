#include <lumino_api/lumino_api.h>
#include <gtest/gtest.h>

// ---------------------------------------------------------------------------
// Environment: 初期化・終了処理
// ---------------------------------------------------------------------------
class LuminoApiEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // TODO: lumino_api の初期化処理をここに追加する
    }

    void TearDown() override {
        // TODO: lumino_api の終了処理をここに追加する
    }
};

// ---------------------------------------------------------------------------
// ダミーテストケース
// ---------------------------------------------------------------------------
TEST(LuminoApiTest, Placeholder) {
    SUCCEED();
}

// ---------------------------------------------------------------------------
// エントリポイント
// ---------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
