
#include <gtest/gtest.h>
#include <fstream>
#include <lumino_shader/UnifiedShader.hpp>
#include <lumino_shader/UnifiedShaderSerializer.hpp>

using namespace ln;
using namespace ln::shader;

TEST(UnifiedShaderSerializer, RoundTrip) {
    // Build a simple UnifiedShader manually
    Ref<UnifiedShader> shader = Ref<UnifiedShader>::adopt(new UnifiedShader());

    // Create a blob
    Blob* blob = shader->createBlob();
    blob->data = {0x01, 0x02, 0x03, 0x04};

    // Create a target entry point
    TargetEntryPoint* ep = shader->createEntryPoint();
    ep->target = ShaderTarget_SPIRV;
    ep->name = "vsMain";
    ep->codeBlobId = blob->id;
    VertexInputAttribute attr;
    attr.usage = AttributeUsage_Position;
    attr.index = 0;
    attr.layoutLocation = 0;
    ep->inputAttributes.push_back(attr);

    // Create global resource layout entry
    auto result = shader->getOrCreateInputResourceWithVerify("$Global", RegisterCategory_ConstantBuffer, 64, 0);
    ASSERT_TRUE(result.has_value());

    // Create a global constant buffer member
    auto memberResult = shader->getOrCreateGlobalMemberWithVerify(
        "color",
        ShaderGlobalMemberType_Float,
        ShaderGlobalMemberKind_Vector,
        0,
        4,
        0,
        0);
    ASSERT_TRUE(memberResult.has_value());

    // Create a target shader pass
    TargetShaderPass* tsp = shader->createTargetShaderPass();
    tsp->vertEntryPointId = ep->id;

    // Create a global shader pass
    GlobalShaderPass* gsp = shader->createGlobalShaderPass();
    gsp->name = "Forward";
    gsp->vertexEntryPoint = "vsMain";
    gsp->targetShaderPassIds[ShaderTarget_SPIRV - 1] = tsp->id;

    // Save to temporary file
    std::filesystem::path tempPath = std::filesystem::temp_directory_path() / "test_shader.lcsh";
    auto saveResult = UnifiedShaderSerializer::saveToFile(shader.get(), tempPath);
    ASSERT_TRUE(saveResult.has_value()) << saveResult.error().message;

    // Read back
    std::ifstream ifs(tempPath, std::ios::binary);
    ASSERT_TRUE(ifs.good());
    std::vector<uint8_t> fileData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    auto loadResult = UnifiedShaderSerializer::loadFromData(fileData.data(), fileData.size());
    ASSERT_TRUE(loadResult.has_value()) << loadResult.error().message;

    Ref<UnifiedShader> loaded = loadResult.value();

    // Verify blob
    ASSERT_EQ(loaded->blob(0)->data.size(), 4u);
    EXPECT_EQ(loaded->blob(0)->data[0], 0x01);

    // Verify entry point
    ASSERT_EQ(loaded->targetEntryPoints().size(), 1u);
    EXPECT_EQ(loaded->targetEntryPoint(0)->name, "vsMain");
    EXPECT_EQ(loaded->targetEntryPoint(0)->target, ShaderTarget_SPIRV);
    EXPECT_EQ(loaded->targetEntryPoint(0)->inputAttributes.size(), 1u);
    EXPECT_EQ(loaded->targetEntryPoint(0)->inputAttributes[0].usage, AttributeUsage_Position);

    // Verify global members
    ASSERT_EQ(loaded->globalConstantBufferMembers().size(), 1u);
    EXPECT_EQ(loaded->globalConstantBufferMembers()[0]->name, "color");
    EXPECT_EQ(loaded->globalConstantBufferMembers()[0]->type, ShaderGlobalMemberType_Float);
    EXPECT_EQ(loaded->globalConstantBufferMembers()[0]->kind, ShaderGlobalMemberKind_Vector);

    // Verify global shader pass
    ASSERT_EQ(loaded->globalShaderPasses().size(), 1u);
    EXPECT_EQ(loaded->globalShaderPasses()[0]->name, "Forward");

    // Verify global resource layout
    EXPECT_EQ(loaded->globalResourceLayout()->buffers.size(), 1u);
    EXPECT_EQ(loaded->globalResourceLayout()->buffers[0].name, "$Global");

    // Cleanup
    std::filesystem::remove(tempPath);
}
