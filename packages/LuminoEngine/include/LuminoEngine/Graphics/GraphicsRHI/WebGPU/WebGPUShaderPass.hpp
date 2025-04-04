#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUShaderPass : public IShaderPass {
public:
    WebGPUShaderPass();
    ~WebGPUShaderPass() override;
    MaybeResult init(WebGPUDevice* m_wgpuDevice, const ShaderPassCreateInfo2& createInfo);
    void onDestroy() override;

    WGPUShaderModule nativeVertShaderModule() const { return m_nativeVertShaderModule; }
    WGPUShaderModule nativeFragShaderModule() const { return m_nativeFragShaderModule; }
    WGPUShaderModule nativeCompShaderModule() const { return m_nativeCompShaderModule; }
    const std::string& vertEntryPointName() const { return m_vertEntryPointName; }
    const std::string& fragEntryPointName() const { return m_fragEntryPointName; }
    const std::string& compEntryPointName() const { return m_compEntryPointName; }

private:
    WGPUShaderModule createShaderModule(
        const byte_t* source,
        size_t sourceSize,
        const char* entryPointName);

    WebGPUDevice* m_wgpuDevice;
    WGPUShaderModule m_nativeVertShaderModule;
    WGPUShaderModule m_nativeFragShaderModule;
    WGPUShaderModule m_nativeCompShaderModule;
    std::string m_vertEntryPointName;
    std::string m_fragEntryPointName;
    std::string m_compEntryPointName;
};

} // namespace detail
} // namespace ln
