#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <string>
#include <vector>

namespace ln {


/** Info about a single member within a material constant buffer (from shader reflection). */
struct MaterialMemberInfo {
    std::string name;       // e.g. "u_time"
    int16_t offset;         // byte offset within the CB
    int16_t size;           // byte size
};

/**
 * ShaderPass: シェーダモジュールとPipelineLayoutをまとめて保持する。
 *
 * セットインデックスはシェーダのリフレクション情報から動的に決定される:
 * - $Material (bare uniform) → Set N (通常 0)
 * - viewData  (ParameterBlock) → Set ?N
 * - sceneData (ParameterBlock) → Set ?N
 * - objectData(ParameterBlock) → Set ?N
 */
class ShaderPass : public RefObject {
public:
    inline static const char* kViewDataParameterBlockName = "viewData";

    /**
     * .lcshバイナリからシェーダモジュールとPipelineLayoutを一括構築する。
     * view/scene/objectのBindGroupLayoutDescは共有のものを引数で受け取り、
     * リフレクション情報に基づくセットインデックスに配置する。
     * $Material レイアウトはリフレクション情報から自動構築される。
     */
    static Result<Ref<ShaderPass>> createFromCompiledShader(
        const void* data, size_t size,
        rhi::Device* device);

    // Accessors
    rhi::ShaderModule* vertexShader() const { return m_vertShader.get(); }
    rhi::ShaderModule* fragmentShader() const { return m_fragShader.get(); }
    const std::string& vertexEntry() const { return m_vertEntry; }
    const std::string& fragmentEntry() const { return m_fragEntry; }
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }
    u64 materialParamBufferSize() const { return m_materialParamBufferSize; }

    /** Descriptor set index for the material ($Material) BindGroup. */
    int16_t materialSetIndex() const { return m_materialSetIndex; }

    /** $Global CB member layout for name-based parameter setting. */
    const std::vector<MaterialMemberInfo>& materialMembers() const { return m_materialMembers; }

    /** Descriptor set index for view data (from shader reflection). */
    int16_t viewSetIndex() const { return m_viewSetIndex; }

    /** Descriptor set index for scene data (from shader reflection). */
    int16_t sceneSetIndex() const { return m_sceneSetIndex; }

    /** Descriptor set index for object data (from shader reflection). */
    int16_t objectSetIndex() const { return m_objectSetIndex; }

    /** Shared BindGroupLayoutDesc for view/camera data. */
    const rhi::BindGroupLayoutDesc& viewLayoutDesc() const { return m_viewLayoutDesc; }

    /** Shared BindGroupLayoutDesc for scene/lighting data. */
    const rhi::BindGroupLayoutDesc& sceneLayoutDesc() const { return m_sceneLayoutDesc; }

    /** Shared BindGroupLayoutDesc for per-object data. */
    const rhi::BindGroupLayoutDesc& objectLayoutDesc() const { return m_objectLayoutDesc; }

    /** Per-object UBO size from shader reflection. */
    u64 objectUBOSize() const { return m_objectUBOSize; }

private:
    ShaderPass() = default;

    Ref<rhi::ShaderModule> m_vertShader;
    Ref<rhi::ShaderModule> m_fragShader;
    std::string m_vertEntry;
    std::string m_fragEntry;
    Ref<rhi::PipelineLayout> m_pipelineLayout;
    u64 m_materialParamBufferSize = 0;
    int16_t m_materialSetIndex = -1;
    std::vector<MaterialMemberInfo> m_materialMembers;

    // Descriptor set indices from shader reflection
    int16_t m_viewSetIndex = -1;
    int16_t m_sceneSetIndex = -1;
    int16_t m_objectSetIndex = -1;

    // Shared BindGroupLayoutDescs (value types, no GPU objects)
    rhi::BindGroupLayoutDesc m_viewLayoutDesc;
    rhi::BindGroupLayoutDesc m_sceneLayoutDesc;
    rhi::BindGroupLayoutDesc m_objectLayoutDesc;
    u64 m_objectUBOSize = 0;
};

} // namespace ln
