#pragma once
#include <LuminoShaderCompiler/detail/UnifiedShader.hpp>
#include "HLSLMetadataParser.hpp"

namespace ln {
namespace detail {

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

class UnifiedShaderCompiler
{
public:
	UnifiedShaderCompiler(ShaderManager* manager, DiagnosticsManager* diag);

	// ※ inputCode は非 const。中身が書き換わる。
	bool compile(
		char* inputCode, size_t inputCodeLength,
		const List<Path>& includeDirectories, const List<String>& definitions);

	bool compileCompute(
		const char* code, size_t len, const std::string& entryPoint,
		const List<Path>& includeDirectories, const List<String>& definitions);

	bool compileSingleCodes(
		const char* vsData, size_t vsLen, const std::string& vsEntryPoint,
		const char* psData, size_t psLen, const std::string& psEntryPoint,
		const List<Path>& includeDirectories, const List<String>& definitions);

	bool link();

	const Ref<UnifiedShader>& unifiedShader() const { return m_unifiedShader; }

private:
    bool createTechPassCodeContainer();
	static std::string makeKey(ShaderStage2 stage, const std::string& entryPoint);
    static std::string makeKey2(const std::string& techName, const std::string& passName, ShaderStage2 stage, const std::string& entryPoint);

	ShaderManager* m_manager;
	Ref<UnifiedShader> m_unifiedShader;
	DiagnosticsManager* m_diag;
	std::vector<HLSLTechnique> m_metadataTechniques;
	//HLSLMetadataParser m_metadataParser;
	std::unordered_map<std::string, std::shared_ptr<ShaderCodeTranspiler>> m_transpilerMap;	// kei is "stage:entryPoint"
	// TODO: ↑ unordered_map やめたい。順序付けされなくなるので。今は makeKey() で 1 とか 2 とか prefix つけることで対策している。
};

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

} // namespace detail
} // namespace ln
