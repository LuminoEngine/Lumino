#include <fstream>
#include <gtest/gtest.h>
#include <LuminoShader/ShaderCompiler2.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include <LuminoShader/WgslValidator.hpp>

using namespace ln;
using namespace ln::shader;

namespace {

std::filesystem::path testData(const char* fileName) {
    return std::filesystem::path(TEST_DATA_DIR) / fileName;
}

/** lumino.slang (import lumino;) を解決できるコンパイラを作る。 */
std::unique_ptr<ShaderCompiler2> createCompiler() {
    auto compiler = *ShaderCompiler2::create();
    compiler->addSearchPath(
        std::filesystem::path(LN_REPO_ROOT_DIR) / "packages" / "LuminoShader" / "shaders");
    return compiler;
}

} // anonymous namespace

//------------------------------------------------------------------------------
// Slang では通るが WGSL では不正になるシェーダを、コンパイル時に検出できることを確認する。
//
// NonUniformSample.slang は早期 return の後でテクスチャをサンプルしているため、
// WGSL の "textureSample must only be called from uniform control flow" に違反する。
// 検証が無いと、この不具合はブラウザで実行して画面が真っ黒になるまで気づけない。
TEST(Test_WgslValidation, NonUniformTextureSampleIsRejected) {
    if (!WgslValidator::available()) {
        GTEST_SKIP() << "WGSL validation is not available in this build.";
    }

    auto compiler = createCompiler();
    ASSERT_TRUE(compiler->isWgslValidationEnabled());

    auto result = compiler->build(testData("NonUniformSample.slang"));
    ASSERT_FALSE(result.has_value()) << "NonUniformSample.slang should fail WGSL validation.";

    const std::string& message = result.error().message;
    // 「どのシェーダの、どのエントリポイントが、なぜ落ちたか」がメッセージに載っていること。
    EXPECT_NE(message.find("WGSL validation failed"), std::string::npos) << message;
    EXPECT_NE(message.find("NonUniformSample.slang"), std::string::npos) << message;
    EXPECT_NE(message.find("fsMain"), std::string::npos) << message;
    EXPECT_NE(message.find("uniform control flow"), std::string::npos) << message;
    // 直し方のヒントが添えられていること。
    EXPECT_NE(message.find("hint"), std::string::npos) << message;
}

//------------------------------------------------------------------------------
// 検証を明示的に無効化すれば、これまで通りコンパイルが通ること。
TEST(Test_WgslValidation, ValidationCanBeDisabled) {
    auto compiler = createCompiler();
    compiler->setWgslValidationEnabled(false);

    auto result = compiler->build(testData("NonUniformSample.slang"));
    EXPECT_TRUE(result.has_value())
        << (result ? std::string() : result.error().message);
}

//------------------------------------------------------------------------------
// 正常なシェーダは検証を通ること (誤検出しないこと)。
TEST(Test_WgslValidation, ValidShaderPassesValidation) {
    auto compiler = createCompiler();
    auto result = compiler->build(testData("ParameterBlock1.slang"));
    EXPECT_TRUE(result.has_value())
        << (result ? std::string() : result.error().message);
}

//------------------------------------------------------------------------------
// 実行時のエラーメッセージ用に、ソースファイル名が .lcsh へ保存され復元されること。
TEST(Test_WgslValidation, SourceNameRoundTrip) {
    auto compiler = createCompiler();
    auto buildResult = compiler->build(testData("Test1.slang"));
    ASSERT_TRUE(buildResult.has_value()) << buildResult.error().message;

    UnifiedShader2* shader = compiler->shader();
    EXPECT_EQ(shader->sourceName(), "Test1.slang");

    // シリアライズしてから読み直しても保持されること。
    const auto filePath = std::filesystem::temp_directory_path() / "LuminoShader_test_SourceName.lcsh";
    auto saveResult = UnifiedShaderSerializer2::saveToFile(shader, filePath);
    ASSERT_TRUE(saveResult.has_value()) << saveResult.error().message;

    std::ifstream stream(filePath, std::ios::binary);
    ASSERT_TRUE(stream.good());
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    stream.close();
    std::filesystem::remove(filePath);

    auto loadResult = UnifiedShaderSerializer2::loadFromData(data.data(), data.size());
    ASSERT_TRUE(loadResult.has_value()) << loadResult.error().message;
    EXPECT_EQ((*loadResult)->sourceName(), "Test1.slang");
}
