#include "Common.hpp"
#define LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "../src/Shader/UnifiedShader.hpp"
#include "../src/Shader/UnifiedShaderCompiler.hpp"

class Test_Shader_UnifiedShader : public LuminoSceneTest {};

TEST_F(Test_Shader_UnifiedShader, Basic)
{
  
    auto diag = makeObject<DiagnosticsManager>();
    detail::UnifiedShaderCompiler compiler(detail::EngineDomain::shaderManager(), diag);

    ByteBuffer buffer = FileSystem::readAllBytes(LN_ASSETFILE("Shader/UnusedUniform1.fx"));

    List<Path> includeDirs;
    List<String> definitions;
    compiler.compile(reinterpret_cast<char*>(buffer.data()), buffer.size(), includeDirs, definitions);

    compiler.link();

    detail::UnifiedShader* shader = compiler.unifiedShader();

    printf("");



}
