
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include <LuminoGraphics/ShaderCompiler/detail/UnifiedShader.hpp>
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "../src/ShaderCompiler/UnifiedShaderCompiler.hpp"

class Test_Shader_ShaderCompiler : public ::testing::Test {};

TEST_F(Test_Shader_ShaderCompiler, ShaderVariant) {
    {
        auto diag = makeObject<DiagnosticsManager>();
        detail::UnifiedShaderCompiler compiler(detail::ShaderManager::instance(), diag);

        ByteBuffer buffer = FileSystem::readAllBytes(LN_ASSETFILE("ShaderVariant.hlsl"));

        List<Path> includeDirs;
        List<String> definitions;
        compiler.compile(reinterpret_cast<char*>(buffer.data()), buffer.size(), includeDirs, definitions);

        compiler.link();

        detail::UnifiedShader* shader = compiler.unifiedShader();

        shader->save(LN_TEMPFILE("UnifiedShader.lcfx"));
        //shader->saveCodes(String(LN_TEMPFILE("")) + u"/");
    }
}
#endif
