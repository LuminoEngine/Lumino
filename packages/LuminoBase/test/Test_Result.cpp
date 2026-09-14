#include <LuminoBase/Result.hpp>
using namespace ln;

class Test_Result : public ::testing::Test {};

// LN_MAKE_ERROR / LN_MAKE_ERROR_WITH_CODE は末尾にセミコロンを含まないため、
// 三項演算子や波括弧なしの if/else の中でも式として使用できること。
// (コンパイルが通ること自体が検証内容)
static VoidResult makeErrorInExpression(bool useCode) {
    return useCode
        ? LN_MAKE_ERROR_WITH_CODE(ErrorCode::DeviceLost, "device lost")
        : LN_MAKE_ERROR("runtime error");
}

TEST_F(Test_Result, MakeErrorIsUsableAsExpression) {
    VoidResult r1 = makeErrorInExpression(true);
    ASSERT_FALSE(r1.has_value());
    EXPECT_EQ(ErrorCode::DeviceLost, r1.error().code);

    VoidResult r2 = makeErrorInExpression(false);
    ASSERT_FALSE(r2.has_value());
    EXPECT_EQ(ErrorCode::RuntimeError, r2.error().code);
}

// LN_FORWARD_ERROR は引数を括弧で囲んでいるため、単純な識別子以外も渡せること。
TEST_F(Test_Result, ForwardErrorAcceptsNonIdentifierArgument) {
    VoidResult source = LN_MAKE_ERROR_WITH_CODE(ErrorCode::IOError, "io error");
    const VoidResult* ptr = &source;

    VoidResult forwarded = LN_FORWARD_ERROR(*ptr);
    ASSERT_FALSE(forwarded.has_value());
    EXPECT_EQ(ErrorCode::IOError, forwarded.error().code);
}

// エラーメッセージに '%' が含まれていても、書式指定子として再展開されないこと。
TEST_F(Test_Result, MessageWithPercentIsNotReexpanded) {
    VoidResult r = LN_MAKE_ERROR("%s", "C:/path/with%20space and %d %n");
    ASSERT_FALSE(r.has_value());
    EXPECT_NE(std::string::npos, r.error().message.find("with%20space and %d %n"));
}
