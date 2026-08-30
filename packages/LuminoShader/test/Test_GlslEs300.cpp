#include <fstream>
#include <gtest/gtest.h>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/ShaderCompiler2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>

using namespace ln;
using namespace ln::shader;

namespace {

std::unique_ptr<ShaderCompiler2> compileParameterBlock1() {
    auto compiler = *ShaderCompiler2::create();
    auto inputFilePath = std::filesystem::path(TEST_DATA_DIR) / "ParameterBlock1.slang";
    auto buildResult = compiler->build(inputFilePath);
    EXPECT_TRUE(buildResult.has_value()) << buildResult.error().message;
    return compiler;
}

std::string entryPointSource(const UnifiedShader2* shader, const char* name) {
    auto ep = shader->getTargetEntryPoint(ShaderTarget_GLSL_ES300, name);
    EXPECT_TRUE(ep.has_value()) << ep.error().message;
    if (!ep) return std::string();
    const auto& data = shader->blob(ep.value()->codeBlobId)->data;
    return std::string(data.begin(), data.end());
}

} // namespace

// SPIR-V から ESSL 300 が生成され、clipspace と Y の補正が入っていること。
TEST(Test_GlslEs300, GenerateSource) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    const std::string vs = entryPointSource(shader, "vsMain");
    const std::string fs = entryPointSource(shader, "fsMain");

    EXPECT_EQ(vs.rfind("#version 300 es", 0), 0u);
    EXPECT_EQ(fs.rfind("#version 300 es", 0), 0u);

    // ESSL 300 に sampler2D 以外のテクスチャ型は無い。
    EXPECT_EQ(fs.find("texture2D "), std::string::npos);
    EXPECT_NE(fs.find("sampler2D"), std::string::npos);

    // fixup_clipspace / flip_vert_y は gl_Position の後処理として現れる。
    EXPECT_NE(vs.find("gl_Position.z = 2.0 * gl_Position.z - gl_Position.w;"), std::string::npos);
    EXPECT_NE(vs.find("gl_Position.y = -gl_Position.y;"), std::string::npos);
}

// combined sampler の対応表が作られ、GLSL 中の変数名と一致すること。
TEST(Test_GlslEs300, CombinedSamplerTable) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    auto ep = shader->getTargetEntryPoint(ShaderTarget_GLSL_ES300, "fsMain");
    ASSERT_TRUE(ep.has_value()) << ep.error().message;

    // baseTexture と baseSampler の組が 1 つだけできる。
    const auto& combined = ep.value()->bindingLayout.combinedSamplers;
    ASSERT_EQ(combined.size(), 1u);
    EXPECT_FALSE(combined[0].name.empty());
    EXPECT_GE(combined[0].textureSetIndex, 0);
    EXPECT_GE(combined[0].textureBindingIndex, 0);
    EXPECT_GE(combined[0].samplerSetIndex, 0);
    EXPECT_GE(combined[0].samplerBindingIndex, 0);

    // 名前は glGetUniformLocation で引くため、生成されたソースに現れている必要がある。
    const std::string fs = entryPointSource(shader, "fsMain");
    EXPECT_NE(fs.find(combined[0].name), std::string::npos) << combined[0].name;

    // GLSL 以外のターゲットには載らない。
    auto spirvEp = shader->getTargetEntryPoint(ShaderTarget_SPIRV, "fsMain");
    ASSERT_TRUE(spirvEp.has_value()) << spirvEp.error().message;
    EXPECT_TRUE(spirvEp.value()->bindingLayout.combinedSamplers.empty());

    // パスへマージした結果にも残る。テクスチャユニット番号はこの添字。
    auto passId = shader->globalShaderPasses()[0]->getTargetShaderPassId(ShaderTarget_GLSL_ES300);
    ASSERT_GE(passId, 0);
    EXPECT_EQ(shader->targetShaderPass(passId)->bindingLayout.combinedSamplers.size(), 1u);
}

// .lcsh へ保存して読み戻しても対応表が保たれること。
TEST(Test_GlslEs300, SerializeCombinedSampler) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    auto tempPath = std::filesystem::temp_directory_path() / "test_glsl_es300.lcsh";
    auto saveResult = UnifiedShaderSerializer2::saveToFile(shader, tempPath);
    ASSERT_TRUE(saveResult.has_value()) << saveResult.error().message;

    std::vector<uint8_t> fileData;
    {
        std::ifstream ifs(tempPath, std::ios::binary | std::ios::ate);
        ASSERT_TRUE(ifs.is_open());
        fileData.resize(static_cast<size_t>(ifs.tellg()));
        ifs.seekg(0);
        ifs.read(reinterpret_cast<char*>(fileData.data()), fileData.size());
    }
    std::filesystem::remove(tempPath);

    auto loadResult = UnifiedShaderSerializer2::loadFromData(
        fileData.data(), fileData.size(), ShaderTarget_GLSL_ES300);
    ASSERT_TRUE(loadResult.has_value()) << loadResult.error().message;
    UnifiedShader2* loaded = loadResult.value().get();

    auto orig = shader->getTargetEntryPoint(ShaderTarget_GLSL_ES300, "fsMain");
    auto restored = loaded->getTargetEntryPoint(ShaderTarget_GLSL_ES300, "fsMain");
    ASSERT_TRUE(orig.has_value());
    ASSERT_TRUE(restored.has_value()) << restored.error().message;

    const auto& a = orig.value()->bindingLayout.combinedSamplers;
    const auto& b = restored.value()->bindingLayout.combinedSamplers;
    ASSERT_EQ(a.size(), b.size());
    for (size_t i = 0; i < a.size(); i++) {
        EXPECT_EQ(a[i].name, b[i].name);
        EXPECT_EQ(a[i].textureSetIndex, b[i].textureSetIndex);
        EXPECT_EQ(a[i].textureBindingIndex, b[i].textureBindingIndex);
        EXPECT_EQ(a[i].samplerSetIndex, b[i].samplerSetIndex);
        EXPECT_EQ(a[i].samplerBindingIndex, b[i].samplerBindingIndex);
    }

    // ターゲットフィルタで GLSL のコードが実体化されること。
    EXPECT_GT(loaded->blob(restored.value()->codeBlobId)->data.size(), 0u);
}
