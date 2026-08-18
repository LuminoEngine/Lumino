#include <LuminoC/lumino.h>

class Test_Logger : public ::testing::Test {
protected:
    // 他のテストへ影響しないよう既定レベルへ戻す。
    void TearDown() override { LNLogger_SetLevel(LN_LOG_LEVEL_INFO); }
};

// 有効なレベルはランタイム未初期化でも LN_OK を返すこと。
TEST_F(Test_Logger, SetLevelAcceptsAllLevels) {
    for (int level = LN_LOG_LEVEL_TRACE; level <= LN_LOG_LEVEL_DISABLE; ++level) {
        ASSERT_EQ(LN_OK, LNLogger_SetLevel(static_cast<LNLogLevel>(level)))
            << "level=" << level;
    }
}

// 範囲外の値は LN_ERROR_INVALID_ARGUMENT を返すこと。
TEST_F(Test_Logger, SetLevelRejectsOutOfRange) {
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT, LNLogger_SetLevel(static_cast<LNLogLevel>(-1)));
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT,
        LNLogger_SetLevel(static_cast<LNLogLevel>(LN_LOG_LEVEL_DISABLE + 1)));
}
