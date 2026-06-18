#include "pch.hpp"
#include <LuminoC/lumino.h>

#define TEST_W 320
#define TEST_H 240

// lumino.slang (import lumino;) を含むディレクトリ。
#define LUMINO_SHADER_DIR LN_REPO_ROOT_DIR "/packages/LuminoShader/shaders"

class Test_Shader : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LN_OK, LNWindow_Create("ShaderTest", TEST_W, TEST_H, &window));
        ASSERT_EQ(LN_OK, LNWindow_GetGraphicsContext(window, &graphicsContext));
    }

    void TearDown() override {
        if (window != LN_NULL_HANDLE) LNObject_Release(window);
    }
};

// マテリアル定数バッファ ($Material CB) を持たないシェーダ
// (フルスクリーン blit: テクスチャ + サンプラのみ) を読み込めることを検証する。
//
// 以前は ShaderPass::buildFromUnifiedShader が $Material 内に定数バッファが
// 無いと "No constant buffer found in '$Material'" エラーで失敗していた。
TEST_F(Test_Shader, CreateFromShaderSourceWithoutConstantBuffer) {
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShaderSourceFile(
        graphicsContext,
        TEST_DATA_DIR "/Data/Composite.slang",
        LUMINO_SHADER_DIR,
        &material));
    ASSERT_NE(LN_NULL_HANDLE, material);

    LNObject_Release(material);
}
