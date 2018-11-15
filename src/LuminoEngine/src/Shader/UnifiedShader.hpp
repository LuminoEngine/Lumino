
#pragma once
#include <LuminoEngine/Engine/Diagnostics.hpp>

namespace ln {
namespace detail {
class ShaderRenderState;

enum class CodeKind
{
	Glsl = 0,
};

class UnifiedShader
	: public RefObject
{
public:
	// 0 is invalid value.
	using CodeContainerId = uint32_t;
	using TechniqueId = uint32_t;
	using PassId = uint32_t;

	static const int FileVersion = 1;
	static const String FileExt;

	UnifiedShader(DiagnosticsManager* diag);
	virtual ~UnifiedShader();

	bool save(const Path& filePath);
	bool load(Stream* stream);

	bool addCodeContainer(const std::string& entryPointName, CodeContainerId* outId);
	void setCode(CodeContainerId container, CodeKind kind, const std::string& code);
	void setCode(const std::string& entryPointName, CodeKind kind, const std::string& code);
	bool hasCode(const std::string& entryPointName, CodeKind kind) const;
	bool findCodeContainer(const std::string& entryPointName, CodeContainerId* outId) const;
	const std::string& getCode(CodeContainerId conteinreId, CodeKind kind) const;
	 
	bool addTechnique(const std::string& name, TechniqueId* outTech);
	int techniqueCount() const { return m_techniques.size(); }
	TechniqueId techniqueId(int index) const { return indexToId(index); }
	const std::string& techniqueName(TechniqueId techId) { return m_techniques[idToIndex(techId)].name; }

	bool addPass(TechniqueId parentTech, const std::string& name, PassId* outPass);
	int passCount() const { return m_passes.size(); }
	PassId passId(int index) const { return indexToId(index); }
	const std::string& passName(PassId passId) { return m_passes[idToIndex(passId)].name; }
	void setVertexShader(PassId pass, CodeContainerId code);
	void setPixelShader(PassId pass, CodeContainerId code);
	void setRenderState(PassId pass, ShaderRenderState* state);
	CodeContainerId vertexShader(PassId pass) const;
	CodeContainerId pixelShader(PassId pass) const;
	ShaderRenderState* renderState(PassId pass) const;

private:
	int idToIndex(uint32_t id) const { return id - 1; }
	uint32_t indexToId(int index) const { return index + 1; }
	int findCodeContainerInfoIndex(const std::string& entryPointName) const;
	int findTechniqueInfoIndex(const std::string& name) const;
	int findPassInfoIndex(const std::string& name) const;

	static void writeString(BinaryWriter* w, const std::string& str);
	static std::string readString(BinaryReader* r);
	static bool checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag);

	struct CodeContainerInfo
	{
		std::string entryPointName;
		std::array<std::string, 1> codes;
	};

	struct TechniqueInfo
	{
		std::string name;
		List<PassId> passes;
	};

	struct PassInfo
	{
		std::string name;
		CodeContainerId vertexShader;
		CodeContainerId pixelShader;
		Ref<ShaderRenderState> renderState;
	};

	DiagnosticsManager* m_diag;
	List<CodeContainerInfo> m_codeContainers;
	List<TechniqueInfo> m_techniques;
	List<PassInfo> m_passes;
};


#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER


} // namespace detail
} // namespace ln

