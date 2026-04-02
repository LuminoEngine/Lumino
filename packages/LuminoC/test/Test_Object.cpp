#include "pch.hpp"
#include <LuminoC/lumino.h>
#include "VisualTestHelper.hpp"

class Test_Object : public ::testing::Test {};

TEST_F(Test_Object, Texture2DCreateAndRelease) {
    LNHandle handle = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_Create(256, 256, 0, &handle));
    ASSERT_NE(LN_NULL_HANDLE, handle);
    ASSERT_EQ(LN_OK, LNObject_Release(handle));
}

TEST_F(Test_Object, ReleaseInvalidHandle) {
    ASSERT_EQ(LN_ERROR_INVALID_HANDLE, LNObject_Release(LN_NULL_HANDLE));
    ASSERT_EQ(LN_ERROR_INVALID_HANDLE, LNObject_Release(0xDEADBEEF));
}

TEST_F(Test_Object, DoubleRelease) {
    LNHandle handle = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_Create(64, 64, 0, &handle));
    ASSERT_EQ(LN_OK, LNObject_Release(handle));
    ASSERT_EQ(LN_ERROR_INVALID_HANDLE, LNObject_Release(handle));
}

TEST_F(Test_Object, CreateNullOutHandle) {
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT, LNTexture2D_Create(64, 64, 0, nullptr));
}

TEST_F(Test_Object, CreateMultipleTextures) {
    LNHandle h1 = LN_NULL_HANDLE, h2 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_Create(128, 128, 0, &h1));
    ASSERT_EQ(LN_OK, LNTexture2D_Create(256, 256, 0, &h2));
    ASSERT_NE(h1, h2);
    ASSERT_EQ(LN_OK, LNObject_Release(h1));
    ASSERT_EQ(LN_OK, LNObject_Release(h2));
}
