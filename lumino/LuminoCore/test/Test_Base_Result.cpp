﻿#include "Common.hpp"
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
    if (len > 10) return Err(TestErr1{TestErr1::Kind::TooLong, "error."});
    if (len == 2) return "aaa"; 
    return Ok(std::string("123"));
}

static Result<void, int> testFunc2(int len) {
    if (len > 10) return Err(1);
    return Ok();
}

static Result<void> testFunc3(int len) {
    if (len > 10) return Err();
    return Ok();
}

static std::string g_str1 = "v1";
static std::string g_str12 = "aaa";
static Result<std::string&, TestErr1> testFunc4(int len) {
    if (len > 10) return Err(TestErr1{TestErr1::Kind::TooLong, "error."});
    if (len == 2) return g_str12; 
    return Ok<std::string&>(g_str1);
}

static std::string g_str2 = "v1";
static std::string g_str22 = "aaa";
static Result<const std::string&, TestErr1> testFunc5(int len) {
    if (len > 10) return Err(TestErr1{TestErr1::Kind::TooLong, "error."});
    if (len == 2) return g_str22; 
    return Ok<const std::string&>(g_str2);
}

TEST_F(Test_Base_Result, Basic) {
    auto r1 = testFunc1(1);
    ASSERT_EQ(true, r1.isOk());
    ASSERT_EQ(false, r1.isErr());
    ASSERT_EQ("123", r1.unwrap());

    auto r2 = testFunc1(100);
    ASSERT_EQ(false, r2.isOk());
    ASSERT_EQ(true, r2.isErr());
    ASSERT_EQ("error.", r2.unwrapErr().text);

    auto r3 = testFunc1(2);
    ASSERT_EQ(true, r3.isOk());
    ASSERT_EQ(false, r3.isErr());
    ASSERT_EQ("aaa", r3.unwrap());
}

TEST_F(Test_Base_Result, VoidOk) {
    auto r1 = testFunc2(1);
    ASSERT_EQ(true, r1.isOk());
    ASSERT_EQ(false, r1.isErr());

    auto r2 = testFunc2(100);
    ASSERT_EQ(false, r2.isOk());
    ASSERT_EQ(true, r2.isErr());
}

TEST_F(Test_Base_Result, Void2) {
    auto r1 = testFunc3(1);
    ASSERT_EQ(true, r1.isOk());
    ASSERT_EQ(false, r1.isErr());

    auto r2 = testFunc3(100);
    ASSERT_EQ(false, r2.isOk());
    ASSERT_EQ(true, r2.isErr());
}

TEST_F(Test_Base_Result, Reference1) {
    auto r1 = testFunc4(1);
    ASSERT_EQ(true, r1.isOk());
    ASSERT_EQ(false, r1.isErr());
    ASSERT_EQ("v1", r1.unwrap());
    r1.unwrap() = "v2";

    auto r4 = testFunc4(1);
    ASSERT_EQ("v2", r4.unwrap());

    auto r2 = testFunc4(100);
    ASSERT_EQ(false, r2.isOk());
    ASSERT_EQ(true, r2.isErr());

    auto r3 = testFunc4(2);
    ASSERT_EQ(true, r3.isOk());
    ASSERT_EQ(false, r3.isErr());
    ASSERT_EQ("aaa", r3.unwrap());
}

TEST_F(Test_Base_Result, Reference2) {
    auto r1 = testFunc5(1);
    ASSERT_EQ(true, r1.isOk());
    ASSERT_EQ(false, r1.isErr());
    ASSERT_EQ("v1", r1.unwrap());

    auto r2 = testFunc5(100);
    ASSERT_EQ(false, r2.isOk());
    ASSERT_EQ(true, r2.isErr());

    auto r3 = testFunc5(2);
    ASSERT_EQ(true, r3.isOk());
    ASSERT_EQ(false, r3.isErr());
    ASSERT_EQ("aaa", r3.unwrap());
}
