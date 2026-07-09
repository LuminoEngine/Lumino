#include "pch.hpp"
#include <cstdint>
#include <LuminoC/lumino.h>

//------------------------------------------------------------------------------
// LNDebug_GetStructSize の基本テスト。
//
// 構造体サイズはプラットフォーム (ポインタ幅) により異なるため、デスクトップ
// ビルドでは具体的なサイズ値は表明しない。wasm32 の期待値は LuminoAPI.cpp の
// static_assert と、TS 側 Runtime.initialize() の実行時照合で検証される。
//------------------------------------------------------------------------------

class Test_Debug : public ::testing::Test {};

// 既知の構造体名はすべて LN_OK を返し、サイズは 0 より大きいこと。
TEST_F(Test_Debug, GetStructSizeReturnsPositiveForKnownNames) {
    const char* names[] = {
        "LNInstanceInitializeSettings",
        "LNColorAttachmentDesc",
        "LNDepthStencilAttachmentDesc",
        "LNRenderPassDesc",
        "LNVertex",
        "LNSubMesh",
        "LNTransform",
        "LNMatrix",
        "LNGraphicsProfiler",
    };
    for (const char* name : names) {
        uint32_t size = 0;
        ASSERT_EQ(LN_OK, LNDebug_GetStructSize(name, &size)) << "name=" << name;
        ASSERT_GT(size, 0u) << "name=" << name;
    }
}

// 未知の構造体名は LN_ERROR_INVALID_ARGUMENT を返すこと。
TEST_F(Test_Debug, GetStructSizeRejectsUnknownName) {
    uint32_t size = 0xFFFFFFFFu;
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT,
              LNDebug_GetStructSize("LNDoesNotExist", &size));
}

// NULL 引数は LN_ERROR_INVALID_ARGUMENT を返すこと。
TEST_F(Test_Debug, GetStructSizeRejectsNullArguments) {
    uint32_t size = 0;
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT,
              LNDebug_GetStructSize(nullptr, &size));
    ASSERT_EQ(LN_ERROR_INVALID_ARGUMENT,
              LNDebug_GetStructSize("LNRenderPassDesc", nullptr));
}
