
#pragma once
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "HLSLMetadataParser.hpp"
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
    };

    // 0 is invalid value.
    using CodeContainerId = uint32_t;
    using TechniqueId = uint32_t;
    using PassId = uint32_t;

    //static const int FileVersion = 1;
	enum FileVersion {
		FileVersion_1 = 1,  // 0.7.0
        FileVersion_2,      // 0.8.0
		FileVersion_3,      // 0.9.0
        FileVersion_4,      // 0.10.0
        FileVersion_5,      // 0.11.0
		FileVersion_Last,
		FileVersion_Current = FileVersion_Last - 1,
	};
    static const String FileExt;

    UnifiedShader(DiagnosticsManager* diag);
    virtual ~UnifiedShader();

    bool save(const Path& filePath);
    bool load(Stream* stream);


    bool addCodeContainer(ShaderStage2 stage, const std::string& entryPointName, CodeContainerId* outId);
    void setCode(CodeContainerId container, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code);
    const CodeInfo* findCode(CodeContainerId conteinreId, const UnifiedShaderTriple& triple) const;
    const std::string& entryPointName(CodeContainerId conteinreId) const;
    void makeGlobalDescriptorLayout();

    bool addTechnique(const std::string& name, const ShaderTechniqueClass& techniqueClass, TechniqueId* outTech);
    int techniqueCount() const { return m_techniques.size(); }
    TechniqueId techniqueId(int index) const { return indexToId(index); }
    const std::string& techniqueName(TechniqueId techId) const { return m_techniques[idToIndex(techId)].name; }
    const ShaderTechniqueClass& techniqueClass(TechniqueId techId) const { return m_techniques[idToIndex(techId)].techniqueClass; }

    bool addPass(TechniqueId parentTech, const std::string& name, PassId* outPass);
    int getPassCountInTechnique(TechniqueId parentTech) const;
    PassId getPassIdInTechnique(TechniqueId parentTech, int index) const;
    int passCount() const { return m_passes.size(); }
    PassId passId(int index) const { return indexToId(index); }
    const std::string& passName(PassId passId) const { return m_passes[idToIndex(passId)].name; }
    void setVertexShader(PassId pass, CodeContainerId code);
    void setPixelShader(PassId pass, CodeContainerId code);
    void setComputeShader(PassId pass, CodeContainerId code);
    void setRenderState(PassId pass, ShaderRenderState* state);
	//void setRefrection(PassId pass, UnifiedShaderRefrectionInfo* value);
    void addMergeDescriptorLayoutItem(PassId pass, const DescriptorLayout& layout);
    CodeContainerId vertexShader(PassId pass) const;
    CodeContainerId pixelShader(PassId pass) const;
    CodeContainerId computeShader(PassId pass) const;
    ShaderRenderState* renderState(PassId pass) const;
	const DescriptorLayout& descriptorLayout(PassId pass) const;
    //UnifiedShaderRefrectionInfo* refrection(PassId pass) const;
    void setAttributes(PassId pass, const std::vector<VertexInputAttribute>& attrs);
	const std::vector<VertexInputAttribute>& attributes(PassId pass) const;

    // DescriptorLayout.uniformBufferRegister のみ有効。
    // それ以外は、値は入っているが binding が正しくないので使用できない。
    const DescriptorLayout& globalDescriptorLayout() const { return m_globalDescriptorLayout; }

    void saveCodes(const StringRef& perfix) const;

private:
    int idToIndex(uint32_t id) const { return id - 1; }
    uint32_t indexToId(int index) const { return index + 1; }
    //int findCodeContainerInfoIndex(ShaderStage2 stage, const std::string& entryPointName) const;
    int findTechniqueInfoIndex(const std::string& name) const;
    int findPassInfoIndex(TechniqueId tech, const std::string& name) const;

    static void writeString(BinaryWriter* w, const std::string& str);
	static void writeByteArray(BinaryWriter* w, const std::vector<byte_t>& data);
    static std::string readString(BinaryReader* r);
	static std::vector<byte_t> readByteArray(BinaryReader* r);
    static bool checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag);

    struct CodeContainerInfo
    {
		ShaderStage2 stage;
        std::string entryPointName;
        std::vector<CodeInfo> codes;
    };

    struct TechniqueInfo
    {
        std::string name;
        ShaderTechniqueClass techniqueClass;
        List<PassId> passes;
    };

    struct PassInfo
    {
        std::string name;
        CodeContainerId vertexShader;
        CodeContainerId pixelShader;
        CodeContainerId computeShader;
        Ref<ShaderRenderState> renderState;
		DescriptorLayout descriptorLayout;
		std::vector<VertexInputAttribute> attributes;	// used by vertexShader
    };

    DiagnosticsManager* m_diag;
    List<CodeContainerInfo> m_codeContainers;
    List<TechniqueInfo> m_techniques;
    List<PassInfo> m_passes;
    DescriptorLayout m_globalDescriptorLayout; // Result of merging all pass layouts
};

} // namespace detail
} // namespace ln
