#include <lumino_api/lumino.h>
#include <gtest/gtest.h>

//------------------------------------------------------------------------------
// Environment: 初期化・終了処理
//------------------------------------------------------------------------------
class LuminoApiEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        ASSERT_EQ(LNInstance_Initialize(), LN_OK);
    }

    void TearDown() override {
        LNInstance_Terminate();
    }
};

//------------------------------------------------------------------------------
// Texture2D C-API テスト
//------------------------------------------------------------------------------
TEST(LuminoApiTest, Texture2DCreateAndRelease) {
    LNHandle handle = LN_NULL_HANDLE;
    EXPECT_EQ(LNTexture2D_Create(256, 256, 0, &handle), LN_OK);
    EXPECT_NE(handle, LN_NULL_HANDLE);
    EXPECT_EQ(LNObject_Release(handle), LN_OK);
}

TEST(LuminoApiTest, ReleaseInvalidHandle) {
    EXPECT_EQ(LNObject_Release(LN_NULL_HANDLE), LN_ERROR_INVALID_HANDLE);
    EXPECT_EQ(LNObject_Release(0xDEADBEEF), LN_ERROR_INVALID_HANDLE);
}

TEST(LuminoApiTest, DoubleRelease) {
    LNHandle handle = LN_NULL_HANDLE;
    EXPECT_EQ(LNTexture2D_Create(64, 64, 0, &handle), LN_OK);
    EXPECT_EQ(LNObject_Release(handle), LN_OK);
    EXPECT_EQ(LNObject_Release(handle), LN_ERROR_INVALID_HANDLE);
}

TEST(LuminoApiTest, CreateNullOutHandle) {
    EXPECT_EQ(LNTexture2D_Create(64, 64, 0, nullptr), LN_ERROR_INVALID_ARGUMENT);
}

TEST(LuminoApiTest, CreateMultipleTextures) {
    LNHandle h1 = LN_NULL_HANDLE, h2 = LN_NULL_HANDLE;
    EXPECT_EQ(LNTexture2D_Create(128, 128, 0, &h1), LN_OK);
    EXPECT_EQ(LNTexture2D_Create(256, 256, 0, &h2), LN_OK);
    EXPECT_NE(h1, h2);
    EXPECT_EQ(LNObject_Release(h1), LN_OK);
    EXPECT_EQ(LNObject_Release(h2), LN_OK);
}

//------------------------------------------------------------------------------
// エントリポイント
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
