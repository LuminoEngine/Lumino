#include <LuminoBase/Logger.hpp>

namespace {

// setCallback は関数ポインタしか受け取れないため、捕捉結果はファイルスコープに置く。
std::vector<LNLogLevel> g_captured;

void captureLog(LNLogLevel level, const char* /*file*/, int /*line*/,
                const char* /*func*/, const char* /*message*/) {
    g_captured.push_back(level);
}

// プロセス起動直後の (どのテストも setLevel() を呼んでいない時点の) レベル。
// Logger.cpp の g_level は定数初期化されるため、この動的初期化の時点では既定値のままである。
const LNLogLevel g_initialLevel = ln::Logger::level();

} // anonymous namespace

class Test_Logger : public ::testing::Test {
protected:
    void SetUp() override {
        g_captured.clear();
        ln::Logger::setCallback(&captureLog);
    }
    void TearDown() override {
        ln::Logger::setCallback(nullptr);
        ln::Logger::setLevel(g_initialLevel);
        g_captured.clear();
    }
};

// 設定レベル未満のログは破棄され、以上のログは出力されること。
TEST_F(Test_Logger, LevelFiltersLowerSeverity) {
    ln::Logger::setLevel(LN_LOG_LEVEL_WARNING);

    LN_LOG_TRACE("trace");
    LN_LOG_INFO("info");
    LN_LOG_WARNING("warning");
    LN_LOG_ERROR("error");

    ASSERT_EQ(2u, g_captured.size());
    ASSERT_EQ(LN_LOG_LEVEL_WARNING, g_captured[0]);
    ASSERT_EQ(LN_LOG_LEVEL_ERROR, g_captured[1]);
}

// LN_LOG_LEVEL_DISABLE はすべてのログを破棄すること。
TEST_F(Test_Logger, DisableDiscardsEverything) {
    ln::Logger::setLevel(LN_LOG_LEVEL_DISABLE);

    LN_LOG_TRACE("trace");
    LN_LOG_ERROR("error");
    LN_LOG_FATAL("fatal");

    ASSERT_TRUE(g_captured.empty());
}

// 既定レベルは LN_LOG_LEVEL_INFO であること (WASM も含め全プラットフォーム共通)。
// setLevel() すると検証対象そのものを上書きするため、起動時に記録した値を見る。
TEST_F(Test_Logger, DefaultLevelIsInfo) {
    ASSERT_EQ(LN_LOG_LEVEL_INFO, g_initialLevel);
}
