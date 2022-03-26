#pragma once
#include <LuminoGraphics/ShaderCompiler/detail/UnifiedShader.hpp>
#include "HLSLMetadataParser.hpp"

namespace ln {
namespace kokage {

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

class UnifiedShaderCompiler {
public:
    UnifiedShaderCompiler(detail::ShaderManager* manager, DiagnosticsManager* diag);

    // ※ inputCode は非 const。中身が書き換わる。
    bool compile(
        char* inputCode,
        size_t inputCodeLength,
        const List<Path>& includeDirectories,
        const List<String>& definitions);

    bool compileCompute(
        const char* code,
        size_t len,
        const std::string& entryPoint,
        const List<Path>& includeDirectories,
        const List<String>& definitions);

    bool compileSingleCodes(
        const char* vsData,
        size_t vsLen,
        const std::string& vsEntryPoint,
        const char* psData,
        size_t psLen,
        const std::string& psEntryPoint,
        const List<Path>& includeDirectories,
        const List<String>& definitions);

    bool link();

    const Ref<UnifiedShader>& unifiedShader() const { return m_unifiedShader; }

private:
    UnifiedShaderPass* createVSPSPass(
        UnifiedShaderTechnique* parentTech,
        const HLSLPass& metaPass,
        const char* inputCode,
        size_t inputCodeLength,
        const List<Path>& includeDirectories,
        const List<String>& definitions);

    detail::ShaderManager* m_manager;
    Ref<UnifiedShader> m_unifiedShader;
    DiagnosticsManager* m_diag;
    std::vector<HLSLTechnique> m_metadataTechniques;
    std::vector<URef<ShaderCodeTranspiler>> m_transpilers;
};

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

} // namespace kokage
} // namespace ln
