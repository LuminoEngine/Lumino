#include "Common.hpp"
#include <LuminoCore/Base/Result.hpp>

//==============================================================================

class Test_Base_Result : public ::testing::Test {};

struct TestErr1 {
    enum class Kind {
        Invalid,
        TooLong,
    };

    Kind kind;
    std::string text;
};

static Result<std::string, TestErr1> testFunc1(int len) {
    if (len > 10) return Err(TestErr1{TestErr1::Kind::TooLong, "len error."});
    return Ok(std::string("123"));
}

TEST_F(Test_Base_Result, Basic) {
    auto r1 = testFunc1(1);
    auto r2 = testFunc1(100);
}
