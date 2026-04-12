#include "WebGPUShaderModule.hpp"
#include "WebGPUDevice.hpp"

namespace ln::rhi::webgpu {

WebGPUShaderModule::WebGPUShaderModule() = default;

VoidResult WebGPUShaderModule::init(WebGPUDevice* device, const ShaderModuleDesc& desc) {
    m_device = device;

    if (desc.format != ShaderCodeFormat::WGSL) {
        return LN_MAKE_ERROR("WebGPUShaderModule only accepts WGSL format.");
    }
    if (!desc.code || desc.codeSizeBytes == 0) {
        return LN_MAKE_ERROR("WebGPUShaderModule: empty shader code.");
    }

    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.next = nullptr;
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code.data = reinterpret_cast<const char*>(desc.code);
    wgslSource.code.length = desc.codeSizeBytes;

    WGPUShaderModuleDescriptor moduleDesc = {};
    moduleDesc.nextInChain = &wgslSource.chain;
    moduleDesc.label = {"LuminoShaderModule", WGPU_STRLEN};

    m_module = wgpuDeviceCreateShaderModule(m_device->wgpuDevice(), &moduleDesc);
    if (!m_module) {
        return LN_MAKE_ERROR("wgpuDeviceCreateShaderModule failed.");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUShaderModule::finalize() {
    if (m_module) {
        wgpuShaderModuleRelease(m_module);
        m_module = nullptr;
    }
    ShaderModule::finalize();
}

} // namespace ln::rhi::webgpu
