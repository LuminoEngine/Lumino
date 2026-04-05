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
 * - viewData  (ParameterBlock) → Set N+1
 * - sceneData (ParameterBlock) → Set N+2
 * - objectData(ParameterBlock) → Set N+3
 */
class ShaderPass : public RefObject {
public:
    /**
     * 事前構築済みのPipelineLayoutDescからShaderPassを生成する。
     */
    static Result<Ref<ShaderPass>> create(
        Ref<rhi::ShaderModule> vertShader,
        Ref<rhi::ShaderModule> fragShader,
        std::string vertEntry,
        std::string fragEntry,
        rhi::PipelineLayoutDesc pipelineLayoutDesc,
        u64 materialParamBufferSize,
        int16_t materialSetIndex,
        std::vector<MaterialMemberInfo> materialMembers,
        rhi::Device* device);

    /**
     * .lcshバイナリからシェーダモジュールとPipelineLayoutを一括構築する。
     * view/scene/objectのBindGroupLayoutDescは共有のものを引数で受け取り、
     * リフレクション情報に基づくセットインデックスに配置する。
     * $Material レイアウトはリフレクション情報から自動構築される。
     */
    static Result<Ref<ShaderPass>> createFromCompiledShader(
        const void* data, size_t size,
        const rhi::BindGroupLayoutDesc& viewLayoutDesc,
        const rhi::BindGroupLayoutDesc& sceneLayoutDesc,
        const rhi::BindGroupLayoutDesc& objectLayoutDesc,
        int16_t viewSetIndex,
        int16_t sceneSetIndex,
        int16_t objectSetIndex,
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
};

} // namespace ln
