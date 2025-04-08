#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUShaderPass : public IShaderPass {
public:
    WebGPUShaderPass();
    ~WebGPUShaderPass() override;
    MaybeResult_deprecated init(
        WebGPUDevice* m_wgpuDevice,
        const ShaderPassCreateInfo2& createInfo);
    void onDestroy() override;

    const std::vector<kokage::VertexInputAttribute>& attributes() const { return m_attributes; }
    const kokage::TargetBindingLayoutInfo& targetBindingLayoutInfo() const { return m_targetBindingLayoutInfo; }
    WGPUShaderModule nativeVertShaderModule() const { return m_nativeVertShaderModule; }
    WGPUShaderModule nativeFragShaderModule() const { return m_nativeFragShaderModule; }
    WGPUShaderModule nativeCompShaderModule() const { return m_nativeCompShaderModule; }
    const std::string& vertEntryPointName() const { return m_vertEntryPointName; }
    const std::string& fragEntryPointName() const { return m_fragEntryPointName; }
    const std::string& compEntryPointName() const { return m_compEntryPointName; }
    WGPUPipelineLayout pipelineLayout() const { return m_pipelineLayout; }
    WGPUBindGroupLayout bindGroupLayout() const { return m_bindGroupLayout; }

private:
    WGPUShaderModule createShaderModule(
        const byte_t* source,
        size_t sourceSize,
        const char* entryPointName);
    MaybeResult_deprecated createPipelineLayout(const ShaderPassCreateInfo2& createInfo);
    static void setupLayoutEntryDefault(WGPUBindGroupLayoutEntry* entry);

    WebGPUDevice* m_wgpuDevice;
    std::vector<kokage::VertexInputAttribute> m_attributes;   // TODO: コピーで保持すると無駄が多いかも。
    kokage::TargetBindingLayoutInfo m_targetBindingLayoutInfo; // TODO: コピーで保持すると無駄が多いかも。
    WGPUShaderModule m_nativeVertShaderModule;
    WGPUShaderModule m_nativeFragShaderModule;
    WGPUShaderModule m_nativeCompShaderModule;
    std::string m_vertEntryPointName;
    std::string m_fragEntryPointName;
    std::string m_compEntryPointName;
    WGPUPipelineLayout m_pipelineLayout;
    WGPUBindGroupLayout m_bindGroupLayout;
};

} // namespace detail
} // namespace ln
