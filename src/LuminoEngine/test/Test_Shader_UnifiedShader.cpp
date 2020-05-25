#include "Common.hpp"
#define LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "../src/Shader/UnifiedShader.hpp"
#include "../src/Shader/UnifiedShaderCompiler.hpp"

class Test_Shader_UnifiedShader : public LuminoSceneTest {};

TEST_F(Test_Shader_UnifiedShader, Basic)
{
    {
        auto diag = makeObject<DiagnosticsManager>();
        detail::UnifiedShaderCompiler compiler(detail::EngineDomain::shaderManager(), diag);

        ByteBuffer buffer = FileSystem::readAllBytes(LN_ASSETFILE("Shader/UnusedUniform1.fx"));

        List<Path> includeDirs;
        List<String> definitions;
        compiler.compile(reinterpret_cast<char*>(buffer.data()), buffer.size(), includeDirs, definitions);

        compiler.link();

        detail::UnifiedShader* shader = compiler.unifiedShader();

        shader->save(LN_TEMPFILE("UnifiedShader.lcfx"));
        //shader->saveCodes(String(LN_TEMPFILE("")) + u"/");
    }


    auto shader = Shader::create(LN_TEMPFILE("UnifiedShader.lcfx"));

    ASSERT_EQ(1, shader->techniques()->size());
    auto tech = shader->techniques()->front();
    ASSERT_EQ(2, tech->passes()->size());

    // Pass0
    {
        auto pass = tech->passes()[0];

        const auto& passDL = pass->descriptorLayout();
        ASSERT_EQ(1, passDL.m_buffers.size());
        ASSERT_EQ(0, passDL.m_buffers[0].dataIndex);
        ASSERT_EQ(0, passDL.m_buffers[0].bindingIndex);
        ASSERT_EQ(detail::ShaderStageFlags_Pixel, passDL.m_buffers[0].stageFlags);

        ASSERT_EQ(2, passDL.m_textures.size());
        ASSERT_EQ(0, passDL.m_textures[0].bindingIndex);
        ASSERT_EQ(0, passDL.m_textures[0].dataIndex);
        ASSERT_EQ(detail::ShaderStageFlags_Pixel, passDL.m_textures[0].stageFlags);
        ASSERT_EQ(1, passDL.m_textures[1].bindingIndex);
        ASSERT_EQ(1, passDL.m_textures[1].dataIndex);
        ASSERT_EQ(detail::ShaderStageFlags_Pixel, passDL.m_textures[1].stageFlags);

        ASSERT_EQ(0, passDL.m_samplers.size());
    }

    // Pass1
    {
        auto pass = tech->passes()[1];

        const auto& passDL = pass->descriptorLayout();
        ASSERT_EQ(1, passDL.m_buffers.size());
        ASSERT_EQ(0, passDL.m_buffers[0].dataIndex);
        ASSERT_EQ(0, passDL.m_buffers[0].bindingIndex);
        ASSERT_EQ(detail::ShaderStageFlags_Vertex | detail::ShaderStageFlags_Pixel, passDL.m_buffers[0].stageFlags);

        ASSERT_EQ(1, passDL.m_textures.size());
        ASSERT_EQ(1, passDL.m_textures[0].dataIndex);    // _Texture1 は使わず、_Texture2 を使うので、1
        ASSERT_EQ(0, passDL.m_textures[0].bindingIndex);
        ASSERT_EQ(detail::ShaderStageFlags_Pixel, passDL.m_textures[0].stageFlags);
    }
}
