#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUShaderPass.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUShaderPass

WebGPUShaderPass::WebGPUShaderPass()
    : m_wgpuDevice(nullptr)
    , m_nativeVertShaderModule(nullptr)
    , m_nativeFragShaderModule(nullptr)
    , m_nativeCompShaderModule(nullptr) {
}

WebGPUShaderPass::~WebGPUShaderPass() {
}

MaybeResult WebGPUShaderPass::init(
    WebGPUDevice* wgpuDevice, const ShaderPassCreateInfo2& createInfo) {
    m_wgpuDevice = wgpuDevice;

    if (createInfo.vsCode) {
        m_nativeVertShaderModule = createShaderModule(
            createInfo.vsCode,
            createInfo.vsCodeLen,
            createInfo.vsEntryPointName);
        if (!m_nativeVertShaderModule) {
            return LN_MAKE_ERROR("Failed to create vertex shader module");
        }
        m_vertEntryPointName = createInfo.vsEntryPointName;
    }

    if (createInfo.psCode) {
        m_nativeFragShaderModule = createShaderModule(
            createInfo.psCode,
            createInfo.psCodeLen,
            createInfo.psEntryPointName);
        if (!m_nativeFragShaderModule) {
            return LN_MAKE_ERROR("Failed to create fragment shader module");
        }
        m_fragEntryPointName = createInfo.psEntryPointName;
    }

    if (createInfo.csCode) {
        m_nativeCompShaderModule = createShaderModule(
            createInfo.csCode,
            createInfo.csCodeLen,
            createInfo.csEntryPointName);
        if (!m_nativeCompShaderModule) {
            return LN_MAKE_ERROR("Failed to create compute shader module");
        }
        m_compEntryPointName = createInfo.csEntryPointName;
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUShaderPass::onDestroy() {
    if (m_nativeVertShaderModule) {
        wgpuShaderModuleRelease(m_nativeVertShaderModule);
        m_nativeVertShaderModule = nullptr;
    }
    if (m_nativeFragShaderModule) {
        wgpuShaderModuleRelease(m_nativeFragShaderModule);
        m_nativeFragShaderModule = nullptr;
    }
    if (m_nativeCompShaderModule) {
        wgpuShaderModuleRelease(m_nativeCompShaderModule);
        m_nativeCompShaderModule = nullptr;
    }
    IShaderPass::onDestroy();
}

WGPUShaderModule WebGPUShaderPass::createShaderModule(
    const byte_t* source, size_t sourceSize, const char* entryPointName) {
    WGPUDevice nativeDevice = m_wgpuDevice->wgpuDevice();

    WGPUShaderModuleDescriptor shaderDesc = WGPU_SHADER_MODULE_DESCRIPTOR_INIT;
#ifdef WEBGPU_BACKEND_WGPU
    shaderDesc.hintCount = 0;
    shaderDesc.hints = nullptr;
#endif
    WGPUShaderSourceWGSL shaderCodeDesc = WGPU_SHADER_SOURCE_WGSL_INIT;
    // Set the chained struct's header
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = WGPUSType_ShaderSourceWGSL;
    // Connect the chain
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
    shaderCodeDesc.code.data = reinterpret_cast<const char*>(source);
    shaderCodeDesc.code.length = sourceSize;
    WGPUShaderModule shaderModule = wgpuDeviceCreateShaderModule(nativeDevice, &shaderDesc);
    if (!shaderModule) {
        LN_LOG_ERROR("Failed wgpuDeviceCreateShaderModule");
        return nullptr;
    }
    return shaderModule;
}

} // namespace detail
} // namespace ln
