
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include <LuminoGraphics/ShaderCompiler/detail/UnifiedShader.hpp>
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "../src/ShaderCompiler/UnifiedShaderCompiler.hpp"

class Test_Shader_ShaderCompiler : public ::testing::Test {};

TEST_F(Test_Shader_ShaderCompiler, Simple) {
    const auto file = LN_TEMPFILE("Test_Shader_ShaderCompiler.Simple");
    auto diag = makeObject<DiagnosticsManager>();

    {
        kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), diag);

        List<Path> includeDirs;
        List<String> definitions;
        ByteBuffer code = FileSystem::readAllBytes(LN_ASSETFILE("ShaderCompiler/Simple.hlsl"));
        compiler.compile(reinterpret_cast<char*>(code.data()), code.size(), includeDirs, definitions);
        compiler.link();

        compiler.unifiedShader()->save(file);
    }

    kokage::UnifiedShader shader(diag);
    shader.load(FileStream::create(file));

    const auto& codeContainers = shader.codeContainers();
    ASSERT_EQ(2, codeContainers.length());
    ASSERT_EQ("VSMain", codeContainers[0]->entryPointName);
    ASSERT_EQ(4, codeContainers[0]->codes.length());
    ASSERT_EQ("PSMain", codeContainers[1]->entryPointName);
    ASSERT_EQ(4, codeContainers[1]->codes.length());
}

TEST_F(Test_Shader_ShaderCompiler, ShaderVariant) {
    const auto file = LN_TEMPFILE("Test_Shader_ShaderCompiler.ShaderVariant");
    auto diag = makeObject<DiagnosticsManager>();
    diag->setOutputToStdErr(true);

    {
        kokage::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), diag);

        List<Path> includeDirs;
        List<String> definitions;
        ByteBuffer code = FileSystem::readAllBytes(LN_ASSETFILE("ShaderCompiler/ShaderVariant.hlsl"));
        compiler.compile(reinterpret_cast<char*>(code.data()), code.size(), includeDirs, definitions);
        compiler.link();

        compiler.unifiedShader()->save(file);
    }

    kokage::UnifiedShader shader(diag);
    shader.load(FileStream::create(file));

    const auto& codeContainers = shader.codeContainers();
    ASSERT_EQ(146, codeContainers.length());

    const auto& techniques = shader.techniques();
    ASSERT_TRUE(techniques[0]->hasVariant());

    //shader->saveCodes(String(LN_TEMPFILE("")) + U"/");
    //shader->save(LN_TEMPFILE("UnifiedShader.lcfx"));
}

#endif
