
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Shader/Common.hpp>
#include "../Grammar/Token.hpp"

namespace glslang {
class TShader;
class TProgram;
}

namespace ln {
class Token;

namespace detail {
class ShaderManager;
class GraphicsManager;
class ShaderRenderState;

enum class ShaderCodeStage
{
    Vertex,
    Fragment,
};

// シェーダコード１つ分。
// HLSL 入力可能だが、technique 構文はあらかじめ取り除いておく必要がある。
class ShaderCodeTranspiler
{
public:
    DescriptorLayout descriptorLayout;

    static void initializeGlobals();
    static void finalizeGlobals();

    ShaderCodeTranspiler(ShaderManager* manager);
    ~ShaderCodeTranspiler();

    bool compileAndLinkFromHlsl(
        ShaderCodeStage stage,
        const char* code,
        size_t length,
        const std::string& entryPoint,
        const List<Path>& includeDir,
        const List<String>* definitions,
        DiagnosticsManager* diag);

    bool mapIOAndGenerateSpirv();

    ShaderCodeStage stage() const { return m_stage; }
	const std::vector<VertexInputAttribute>& attributes() const { return m_attributes; }
	const Ref<UnifiedShaderRefrectionInfo>& refrection() const { return m_refrection; }
	std::vector<byte_t> spirvCode() const;
    std::vector<byte_t> generateGlsl(uint32_t version, bool es);

private:
    ShaderManager* m_manager;
    ShaderCodeStage m_stage;
    std::unique_ptr<glslang::TShader> m_shader;
    std::unique_ptr<glslang::TProgram> m_program;
	std::vector<VertexInputAttribute> m_attributes;
	Ref<UnifiedShaderRefrectionInfo> m_refrection;
    std::vector<uint32_t> m_spirvCode;

};

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
