#include <fstream>
#include <gtest/gtest.h>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/ShaderCompiler2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>

using namespace ln;
using namespace ln::shader;

static std::unique_ptr<ShaderCompiler2> compileParameterBlock1() {
    auto compiler = *ShaderCompiler2::create();
    //compiler->setDumpEnabled(true);
    auto inputFilePath = std::filesystem::path(TEST_DATA_DIR) / "ParameterBlock1.slang";
    auto buildResult = compiler->build(inputFilePath);
    EXPECT_TRUE(buildResult.has_value()) << buildResult.error().message;
    return compiler;
}

// .lcsh に保存し、そのバイト列を読み戻す。一時ファイルは削除する。
static std::vector<uint8_t> saveAndRead(const UnifiedShader2* shader, const char* fileName) {
    auto tempPath = std::filesystem::temp_directory_path() / fileName;
    auto saveResult = UnifiedShaderSerializer2::saveToFile(shader, tempPath);
    EXPECT_TRUE(saveResult.has_value()) << saveResult.error().message;

    std::vector<uint8_t> fileData;
    std::ifstream ifs(tempPath, std::ios::binary | std::ios::ate);
    EXPECT_TRUE(ifs.is_open());
    size_t fileSize = ifs.tellg();
    fileData.resize(fileSize);
    ifs.seekg(0);
    ifs.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    ifs.close();
    std::filesystem::remove(tempPath);
    return fileData;
}

TEST(Test_ParameterBlock2, ReflectParameterBlockLayout) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    const auto& blocks = shader->parameterBlocks();
    ASSERT_EQ(blocks.size(), 3);

    // ブロック 0: viewData (ビュー単位) - 暗黙の CB
    {
        const auto& block = blocks[0];
        EXPECT_EQ(block.name, "viewData");
        EXPECT_EQ(block.setIndex, 0);
        EXPECT_TRUE(block.hasImplicitConstantBuffer);
        ASSERT_EQ(block.elements.size(), 1);
        EXPECT_EQ(block.elements[0].kind, ParameterBlockElementKind_ConstantBuffer);
        EXPECT_EQ(block.elements[0].name, "");
        EXPECT_GT(block.elements[0].constantBufferSize, 0);
    }

    // ブロック 1: materialData (マテリアル単位) - 明示的な CB + Texture + Sampler
    {
        const auto& block = blocks[1];
        EXPECT_EQ(block.name, "materialData");
        EXPECT_EQ(block.setIndex, 1);
        EXPECT_FALSE(block.hasImplicitConstantBuffer);
        ASSERT_EQ(block.elements.size(), 3);

        EXPECT_EQ(block.elements[0].name, "params");
        EXPECT_EQ(block.elements[0].kind, ParameterBlockElementKind_ConstantBuffer);
        EXPECT_GT(block.elements[0].constantBufferSize, 0);

        EXPECT_EQ(block.elements[1].name, "baseTexture");
        EXPECT_EQ(block.elements[1].kind, ParameterBlockElementKind_Texture);

        EXPECT_EQ(block.elements[2].name, "baseSampler");
        EXPECT_EQ(block.elements[2].kind, ParameterBlockElementKind_SamplerState);
    }

    // ブロック 2: objectData (オブジェクト単位) - 暗黙の CB
    {
        const auto& block = blocks[2];
        EXPECT_EQ(block.name, "objectData");
        EXPECT_EQ(block.setIndex, 2);
        EXPECT_TRUE(block.hasImplicitConstantBuffer);
        ASSERT_EQ(block.elements.size(), 1);
        EXPECT_EQ(block.elements[0].kind, ParameterBlockElementKind_ConstantBuffer);
        EXPECT_GT(block.elements[0].constantBufferSize, 0);
    }
}

TEST(Test_ParameterBlock2, ShaderPassLinking) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    // グローバルパスは "Forward" の 1 つだけのはず
    const auto& passes = shader->globalShaderPasses();
    ASSERT_EQ(passes.size(), 1);
    EXPECT_EQ(passes[0]->name, "Forward");
    EXPECT_EQ(passes[0]->vertexEntryPoint, "vsMain");
    EXPECT_EQ(passes[0]->fragmentEntryPoint, "fsMain");

    // 各ターゲットにリンク済みの TargetShaderPass があるはず
    for (int t = ShaderTarget_SPIRV; t <= ShaderTarget_METAL; t++) {
        TargetShaderPassId2 tpId = passes[0]->getTargetShaderPassId(static_cast<ShaderTarget>(t));
        EXPECT_GE(tpId, 0);

        TargetShaderPass2* tp = shader->targetShaderPass(tpId);
        EXPECT_GE(tp->vertEntryPointId, 0);
        EXPECT_GE(tp->fragEntryPointId, 0);

        // マージ後のバインディングレイアウトには頂点とフラグメント両方のバインディングが含まれるはず
        EXPECT_FALSE(tp->bindingLayout.bindings.empty());
    }
}

TEST(Test_ParameterBlock2, VertexAttributes) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    // SPIRV の頂点エントリポイントを取得する
    auto epResult = shader->getTargetEntryPoint(ShaderTarget_SPIRV, "vsMain");
    ASSERT_TRUE(epResult.has_value()) << epResult.error().message;
    TargetEntryPoint2* ep = epResult.value();

    // 頂点入力属性は POSITION, NORMAL, TEXCOORD, COLOR, TANGENT の 5 つのはず
    ASSERT_EQ(ep->inputAttributes.size(), 5);

    // 期待する usage がすべて含まれることを確認する
    bool hasPosition = false, hasNormal = false, hasTexCoord = false, hasColor = false, hasTangent = false;
    for (const auto& attr : ep->inputAttributes) {
        if (attr.usage == AttributeUsage_Position) hasPosition = true;
        if (attr.usage == AttributeUsage_Normal) hasNormal = true;
        if (attr.usage == AttributeUsage_TexCoord) hasTexCoord = true;
        if (attr.usage == AttributeUsage_Color) hasColor = true;
        if (attr.usage == AttributeUsage_Tangent) hasTangent = true;
    }
    EXPECT_TRUE(hasPosition);
    EXPECT_TRUE(hasNormal);
    EXPECT_TRUE(hasTexCoord);
    EXPECT_TRUE(hasColor);
    EXPECT_TRUE(hasTangent);
}

TEST(Test_ParameterBlock2, SerializeRoundTrip) {
    auto compiler = compileParameterBlock1();
    UnifiedShader2* shader = compiler->shader();

    auto fileData = saveAndRead(shader, "test_paramblock2.lcsh");
    auto loadResult = UnifiedShaderSerializer2::loadFromData(fileData.data(), fileData.size());
    ASSERT_TRUE(loadResult.has_value()) << loadResult.error().message;
    UnifiedShader2* loaded = loadResult.value().get();

    // パラメータブロックを検証する
    const auto& origBlocks = shader->parameterBlocks();
    const auto& loadBlocks = loaded->parameterBlocks();
    ASSERT_EQ(origBlocks.size(), loadBlocks.size());
    for (size_t i = 0; i < origBlocks.size(); i++) {
        EXPECT_EQ(origBlocks[i].name, loadBlocks[i].name);
        EXPECT_EQ(origBlocks[i].setIndex, loadBlocks[i].setIndex);
        EXPECT_EQ(origBlocks[i].hasImplicitConstantBuffer, loadBlocks[i].hasImplicitConstantBuffer);
        ASSERT_EQ(origBlocks[i].elements.size(), loadBlocks[i].elements.size());
        for (size_t j = 0; j < origBlocks[i].elements.size(); j++) {
            EXPECT_EQ(origBlocks[i].elements[j].name, loadBlocks[i].elements[j].name);
            EXPECT_EQ(origBlocks[i].elements[j].kind, loadBlocks[i].elements[j].kind);
            EXPECT_EQ(origBlocks[i].elements[j].constantBufferSize, loadBlocks[i].elements[j].constantBufferSize);
        }
    }

    // グローバルシェーダパスを検証する
    const auto& origPasses = shader->globalShaderPasses();
    const auto& loadPasses = loaded->globalShaderPasses();
    ASSERT_EQ(origPasses.size(), loadPasses.size());
    for (size_t i = 0; i < origPasses.size(); i++) {
        EXPECT_EQ(origPasses[i]->name, loadPasses[i]->name);
        EXPECT_EQ(origPasses[i]->vertexEntryPoint, loadPasses[i]->vertexEntryPoint);
        EXPECT_EQ(origPasses[i]->fragmentEntryPoint, loadPasses[i]->fragmentEntryPoint);
        EXPECT_EQ(origPasses[i]->computeEntryPoint, loadPasses[i]->computeEntryPoint);
    }

    // ターゲットエントリポイントの数を検証する
    EXPECT_EQ(shader->targetEntryPoints().size(), loaded->targetEntryPoints().size());

    // ターゲットシェーダパスの数を検証する
    EXPECT_EQ(shader->targetShaderPasses().size(), loaded->targetShaderPasses().size());
}

// 指定ターゲット以外のコード blob を読み飛ばすことを確認する。
TEST(Test_ParameterBlock2, LoadWithTargetFilter) {
    auto compiler = compileParameterBlock1();
    auto fileData = saveAndRead(compiler->shader(), "test_target_filter.lcsh");

    auto loadResult = UnifiedShaderSerializer2::loadFromData(
        fileData.data(), fileData.size(), ShaderTarget_SPIRV);
    ASSERT_TRUE(loadResult.has_value()) << loadResult.error().message;
    UnifiedShader2* spirvOnly = loadResult.value().get();

    // SPIRV のエントリポイントの blob だけが実体化され、他は空のまま。
    // メタ情報は全ターゲット分あるため、skippedCount は 0 にならない。
    size_t loadedCount = 0;
    size_t skippedCount = 0;
    for (const auto& ep : spirvOnly->targetEntryPoints()) {
        const size_t size = spirvOnly->blob(ep->codeBlobId)->data.size();
        if (ep->target == ShaderTarget_SPIRV) {
            EXPECT_GT(size, 0u);
            loadedCount++;
        }
        else {
            EXPECT_EQ(size, 0u) << "blob of target " << ep->target << " should be skipped";
            skippedCount++;
        }
    }
    EXPECT_GT(loadedCount, 0u);
    EXPECT_GT(skippedCount, 0u);

    // 未知のターゲットを引いても範囲外アクセスにならない。
    auto* pass = spirvOnly->globalShaderPasses()[0].get();
    EXPECT_EQ(pass->getTargetShaderPassId(static_cast<ShaderTarget>(ShaderTarget_Last + 1)), -1);
    EXPECT_EQ(pass->getTargetShaderPassId(ShaderTarget_UNKNOWN), -1);
}
