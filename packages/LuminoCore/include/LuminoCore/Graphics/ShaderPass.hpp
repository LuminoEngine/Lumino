#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <string>

namespace ln {

/**
 * ShaderPass: シェーダモジュールとPipelineLayout（4 Set分）をまとめて保持する。
 *
 * 4セット規約:
 * - Set 0: View (camera) data
 * - Set 1: Scene (lighting) data
 * - Set 2: Material data (per-shader)
 * - Set 3: Object (per-draw) data
 *
 * PipelineLayoutは内部でBindGroupLayoutを所有するため、
 * クライアントはBindGroupLayoutを直接触る必要がない。
 */
class ShaderPass : public RefObject {
public:
    /**
     * 事前構築済みのPipelineLayoutDescからShaderPassを生成する。
     * @param pipelineLayoutDesc 4 Set分のBindGroupLayoutDescを含むレイアウト記述
     */
    static Result<Ref<ShaderPass>> create(
        Ref<rhi::ShaderModule> vertShader,
        Ref<rhi::ShaderModule> fragShader,
        std::string vertEntry,
        std::string fragEntry,
        rhi::PipelineLayoutDesc pipelineLayoutDesc,
        u64 materialParamBufferSize,
        rhi::Device* device);

    /**
     * .lcshバイナリからシェーダモジュールとPipelineLayoutを一括構築する。
     * Set0/1/3のBindGroupLayoutDescは共有のものを引数で受け取る。
     * Set2（materialData）はリフレクション情報から自動構築される。
     */
    static Result<Ref<ShaderPass>> createFromCompiledShader(
        const void* data, size_t size,
        const rhi::BindGroupLayoutDesc& viewLayoutDesc,
        const rhi::BindGroupLayoutDesc& sceneLayoutDesc,
        const rhi::BindGroupLayoutDesc& objectLayoutDesc,
        rhi::Device* device);

    // Accessors
    rhi::ShaderModule* vertexShader() const { return m_vertShader.get(); }
    rhi::ShaderModule* fragmentShader() const { return m_fragShader.get(); }
    const std::string& vertexEntry() const { return m_vertEntry; }
    const std::string& fragmentEntry() const { return m_fragEntry; }
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }
    u64 materialParamBufferSize() const { return m_materialParamBufferSize; }

private:
    ShaderPass() = default;

    Ref<rhi::ShaderModule> m_vertShader;
    Ref<rhi::ShaderModule> m_fragShader;
    std::string m_vertEntry;
    std::string m_fragEntry;
    Ref<rhi::PipelineLayout> m_pipelineLayout;
    u64 m_materialParamBufferSize = 0;
};

} // namespace ln
