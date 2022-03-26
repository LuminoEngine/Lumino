
#pragma once
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "../ShaderHelper.hpp"

namespace ln {
namespace kokage {
class ShaderRenderState;
class UnifiedShader;
class UnifiedShaderPass;

class UnifiedShaderVariantSet {
public:
    std::vector<std::string> values;
};

// 0 is invalid value.
using CodeContainerId = uint32_t;
using UnifiedShaderTechniqueId = uint32_t;
using UnifiedShadePassId = uint32_t;

struct USCodeInfo : public URefObject {
public:
    UnifiedShaderTriple triple;
    std::vector<byte_t> code;
};

struct USCodeContainerInfo : public URefObject {
public:
    USCodeContainerInfo(CodeContainerId id);

    ShaderStage2 stage;
    std::string entryPointName;
    ln::Array<URef<USCodeInfo>> codes;

    CodeContainerId id() const { return m_id; }
    void setCode(const UnifiedShaderTriple& triple, const std::vector<byte_t>& code);
    const USCodeInfo* findCode(const UnifiedShaderTriple& triple) const;

private:
    CodeContainerId m_id;
};

class UnifiedShaderTechnique : public URefObject {
public:
    UnifiedShaderVariantSet variantSet;

    std::string name;
    ShaderTechniqueClass techniqueClass;
    Array<UnifiedShadePassId> passes;

    UnifiedShaderTechnique(UnifiedShader* shader, UnifiedShaderTechniqueId id);
    UnifiedShaderTechniqueId id() const { return m_id; }
    bool hasVariant() const { return !variantSet.values.empty(); }

private:
    UnifiedShader* m_shader;
    UnifiedShaderTechniqueId m_id;
};

class UnifiedShaderPass : public URefObject {
public:
    UnifiedShadePassId id;
    std::string name;
    CodeContainerId vertexShader;
    CodeContainerId pixelShader;
    CodeContainerId computeShader;
    Ref<ShaderRenderState> renderState;
    DescriptorLayout descriptorLayout;
    std::vector<VertexInputAttribute> attributes; // used by vertexShader

    // Working variables.
    ShaderCodeTranspiler* vsTranspiler = nullptr;
    ShaderCodeTranspiler* psTranspiler = nullptr;

    UnifiedShaderPass()
        : vertexShader(0)
        , pixelShader(0)
        , computeShader(0) {}
};

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
class UnifiedShader : public RefObject {
public:
    // 0 is invalid value.
    using TechniqueId = uint32_t;
    using PassId = uint32_t;

    //static const int FileVersion = 1;
    enum FileVersion {
        FileVersion_1 = 1, // 0.7.0
        FileVersion_2,     // 0.8.0
        FileVersion_3,     // 0.9.0
        FileVersion_4,     // 0.10.0
        FileVersion_5,     // 0.11.0
        FileVersion_6,     // 0.11.0
        FileVersion_Last,
        FileVersion_Current = FileVersion_Last - 1,
    };
    static const String FileExt;

    UnifiedShader(DiagnosticsManager* diag);
    virtual ~UnifiedShader();

    bool save(const Path& filePath);
    bool load(Stream* stream);

    const Array<URef<USCodeContainerInfo>>& codeContainers() const { return m_codeContainers; }
    USCodeContainerInfo* codeContainer(CodeContainerId id) const { return m_codeContainers[idToIndex(id)]; }
    USCodeContainerInfo* addCodeContainer(ShaderStage2 stage, const std::string& entryPointName);
    void makeGlobalDescriptorLayout();

    UnifiedShaderPass* addPass(TechniqueId parentTech, const std::string& name);
    const Array<URef<UnifiedShaderPass>>& passes() const { return m_passes; }
    UnifiedShaderPass* pass(PassId id) const { return m_passes[idToIndex(id)]; }
    void addMergeDescriptorLayoutItem(UnifiedShaderPass* pass, const DescriptorLayout& layout);

    UnifiedShaderTechnique* addTechnique(const std::string& name, const ShaderTechniqueClass& techniqueClass);
    const Array<URef<UnifiedShaderTechnique>>& techniques() const { return m_techniques; }
    UnifiedShaderTechnique* technique(UnifiedShaderTechniqueId id) const { return m_techniques[idToIndex(id)]; }

    UnifiedShaderTechnique* addVariantTechnique(const std::string& name, const UnifiedShaderVariantSet& variantSet);

    // DescriptorLayout.uniformBufferRegister のみ有効。
    // それ以外は、値は入っているが binding が正しくないので使用できない。
    const DescriptorLayout& globalDescriptorLayout() const { return m_globalDescriptorLayout; }

    void saveCodes(const StringView& perfix) const;

private:
    int idToIndex(uint32_t id) const { return id - 1; }
    uint32_t indexToId(int index) const { return index + 1; }
    int findTechniqueInfoIndex(const std::string& name) const;
    int findPassInfoIndex(TechniqueId tech, const std::string& name) const;

    static void writeString(BinaryWriter* w, const std::string& str);
    static void writeByteArray(BinaryWriter* w, const std::vector<byte_t>& data);
    static std::string readString(BinaryReader* r);
    static std::vector<byte_t> readByteArray(BinaryReader* r);
    static bool checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag);

    DiagnosticsManager* m_diag;
    Array<URef<USCodeContainerInfo>> m_codeContainers;
    Array<URef<UnifiedShaderTechnique>> m_techniques;
    Array<URef<UnifiedShaderPass>> m_passes;
    DescriptorLayout m_globalDescriptorLayout; // Result of merging all pass layouts
};

} // namespace kokage
} // namespace ln
