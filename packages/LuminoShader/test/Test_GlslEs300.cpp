#include "TestHelper.hpp"
#include <set>
#include <sstream>

using namespace ln;
using namespace ln::shader;

namespace {

std::string entryPointSource(const UnifiedShader2* shader, const char* name) {
    auto ep = shader->getTargetEntryPoint(ShaderTarget_GLSL_ES300, name);
    EXPECT_TRUE(ep.has_value()) << ep.error().message;
    if (!ep) return std::string();
    const auto& data = shader->blob(ep.value()->codeBlobId)->data;
    return std::string(data.begin(), data.end());
}

// "in "/"out " で始まる宣言行から変数名 (末尾のトークン) を集める。
std::set<std::string> declaredNames(const std::string& source, const std::string& qualifier) {
    std::set<std::string> names;
    std::istringstream iss(source);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.rfind(qualifier, 0) != 0) continue;
        const size_t semicolon = line.find(';');
        if (semicolon == std::string::npos) continue;
        const size_t begin = line.find_last_of(' ', semicolon) + 1;
        names.insert(line.substr(begin, semicolon - begin));
    }
    return names;
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

    // テクスチャとサンプラーは sampler2D に結合されている。
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

// ステージ間の varying の名前が両ステージで一致していること。
//
// GLSL ES 300 の varying に layout(location = N) は無く、リンク時の対応付けは名前で行われる。
// SPIRV-Cross はステージごとに独立した名前を付けるため、ここが崩れると
// "does not match any VERTEX varying" でプログラムのリンクが失敗する。
TEST(Test_GlslEs300, VaryingNamesMatchAcrossStages) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    const std::string vs = entryPointSource(shader, "vsMain");
    const std::string fs = entryPointSource(shader, "fsMain");

    // 頂点出力とフラグメント入力が varying。頂点入力とフラグメント出力は
    // layout(location = N) が付くため "in "/"out " 始まりの行には現れない。
    const auto vsOut = declaredNames(vs, "out ");
    const auto fsIn = declaredNames(fs, "in ");

    ASSERT_FALSE(fsIn.empty());
    EXPECT_EQ(vsOut, fsIn);
    for (const auto& name : fsIn) {
        EXPECT_EQ(name.rfind("_ln_vary_", 0), 0u) << name;
    }
}

// ユニフォームブロック名が (set, binding) から決まる規則的な名前へ書き換わっていること。
//
// ESSL 300 には layout(binding = N) が無く、実行時は glGetUniformBlockIndex に
// ブロック名を渡してブロックを特定する。ここが崩れると WebGL2 バックエンドは
// ユニフォームバッファを 1 つも結び付けられなくなる。
TEST(Test_GlslEs300, UniformBlockNaming) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    const std::string vs = entryPointSource(shader, "vsMain");
    const std::string fs = entryPointSource(shader, "fsMain");

    auto passId = shader->globalShaderPasses()[0]->getTargetShaderPassId(ShaderTarget_GLSL_ES300);
    ASSERT_GE(passId, 0);
    const auto& bindings = shader->targetShaderPass(passId)->bindingLayout.bindings;

    size_t checked = 0;
    for (const auto& b : bindings) {
        if (b.kind != ParameterBlockElementKind_ConstantBuffer) continue;
        const std::string decl = "uniform " + glslUniformBlockName(b.setIndex, b.bindingIndex);
        EXPECT_TRUE(vs.find(decl) != std::string::npos || fs.find(decl) != std::string::npos)
            << "not found: " << decl;
        ++checked;
    }
    EXPECT_GT(checked, 0u);
}

// .lcsh へ保存して読み戻しても対応表が保たれること。
TEST(Test_GlslEs300, SerializeCombinedSampler) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    auto fileData = saveAndRead(shader, "test_glsl_es300.lcsh");
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
