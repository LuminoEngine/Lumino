
#pragma once
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "ShaderTranspiler.hpp"

namespace ln {
namespace detail {
class ShaderRenderState;

/*
 * データ構造
 * --------
 * ### CodeContainer
 * ターゲットごとのシェーダコードが入っている。
 * 例えば、GLSL, HLSL, SPIR-V など、これらをまとめるものが CodeContainer。
 * Pass から参照される。
 *
 * ### Pass
 * HLSL の Pass 相当。
 * - VertexShader (CodeContainer)
 * - PixelShader (CodeContainer)
 * - RenderState
 * などを持っている。
 *
 * ### Technique
 * HLSL の Technique 相当。
 * Pass をグループ化する。
 *
 * Triple
 * --------
 * <target>-<version>-<option>
 * - hlsl-5-.
 * - glsl-430-.
 * - glsl-300-es
 */
class UnifiedShader
    : public RefObject
{
public:
    struct CodeInfo
    {
        UnifiedShaderTriple triple;
        std::vector<byte_t> code;
        Ref<UnifiedShaderRefrectionInfo> refrection;
    };

    // 0 is invalid value.
    using CodeContainerId = uint32_t;
    using TechniqueId = uint32_t;
    using PassId = uint32_t;

    //static const int FileVersion = 1;
	enum FileVersion {
		FileVersion_1 = 1,
		FileVersion_Last,
		FileVersion_Current = FileVersion_Last - 1,
	};
    static const String FileExt;

    UnifiedShader(DiagnosticsManager* diag);
    virtual ~UnifiedShader();

    bool save(const Path& filePath);
    bool load(Stream* stream);

    bool addCodeContainer(const std::string& entryPointName, CodeContainerId* outId);
    void setCode(CodeContainerId container, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code, UnifiedShaderRefrectionInfo* refrection);
    void setCode(const std::string& entryPointName, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code, UnifiedShaderRefrectionInfo* refrection);
    bool hasCode(const std::string& entryPointName, const UnifiedShaderTriple& triple) const;
    bool findCodeContainer(const std::string& entryPointName, CodeContainerId* outId) const;
    const CodeInfo* findCode(CodeContainerId conteinreId, const UnifiedShaderTriple& triple) const;

    bool addTechnique(const std::string& name, TechniqueId* outTech);
    int techniqueCount() const { return m_techniques.size(); }
    TechniqueId techniqueId(int index) const { return indexToId(index); }
    const std::string& techniqueName(TechniqueId techId) { return m_techniques[idToIndex(techId)].name; }

    bool addPass(TechniqueId parentTech, const std::string& name, PassId* outPass);
    int getPassCountInTechnique(TechniqueId parentTech) const;
    PassId getPassIdInTechnique(TechniqueId parentTech, int index) const;
    int passCount() const { return m_passes.size(); }
    PassId passId(int index) const { return indexToId(index); }
    const std::string& passName(PassId passId) { return m_passes[idToIndex(passId)].name; }
    void setVertexShader(PassId pass, CodeContainerId code);
    void setPixelShader(PassId pass, CodeContainerId code);
    void setRenderState(PassId pass, ShaderRenderState* state);
	void setAttributeSemantics(PassId pass, const std::vector<VertexInputAttribute>& semantics);
	//void setRefrection(PassId pass, UnifiedShaderRefrectionInfo* value);
    CodeContainerId vertexShader(PassId pass) const;
    CodeContainerId pixelShader(PassId pass) const;
    ShaderRenderState* renderState(PassId pass) const;
	const std::vector<VertexInputAttribute>& attributeSemantics(PassId pass) const;
    //UnifiedShaderRefrectionInfo* refrection(PassId pass) const;

    void saveCodes(const StringRef& perfix) const;

private:
    int idToIndex(uint32_t id) const { return id - 1; }
    uint32_t indexToId(int index) const { return index + 1; }
    int findCodeContainerInfoIndex(const std::string& entryPointName) const;
    int findTechniqueInfoIndex(const std::string& name) const;
    int findPassInfoIndex(TechniqueId tech, const std::string& name) const;

    static void writeString(BinaryWriter* w, const std::string& str);
	static void writeByteArray(BinaryWriter* w, const std::vector<byte_t>& data);
    static std::string readString(BinaryReader* r);
	static std::vector<byte_t> readByteArray(BinaryReader* r);
    static bool checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag);

    struct CodeContainerInfo
    {
        std::string entryPointName;
        std::vector<CodeInfo> codes;
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
		std::vector<VertexInputAttribute> attributeSemantics;
    };

    DiagnosticsManager* m_diag;
    List<CodeContainerInfo> m_codeContainers;
    List<TechniqueInfo> m_techniques;
    List<PassInfo> m_passes;
};

} // namespace detail
} // namespace ln
