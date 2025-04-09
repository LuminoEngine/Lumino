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
    , m_nativeCompShaderModule(nullptr)
    , m_vertEntryPointName()
    , m_fragEntryPointName()
    , m_compEntryPointName()
    , m_pipelineLayout(nullptr)
    , m_bindGroupLayout(nullptr) {
}

WebGPUShaderPass::~WebGPUShaderPass() {
}

MaybeResult_deprecated WebGPUShaderPass::init(
    WebGPUDevice* wgpuDevice, const ShaderPassCreateInfo2& createInfo) {
    m_wgpuDevice = wgpuDevice;
    m_attributes = *createInfo.attributes;
    m_targetBindingLayoutInfo = *createInfo.descriptorLayout;

    if (createInfo.vsCode) {
        m_vertEntryPointName = createInfo.vsEntryPointName;
        m_nativeVertShaderModule = createShaderModule(
            createInfo.vsCode,
            createInfo.vsCodeLen,
            createInfo.vsEntryPointName);
        if (!m_nativeVertShaderModule) {
            return LN_MAKE_ERROR_deprecated("Failed to create vertex shader module");
        }
    }

    if (createInfo.psCode) {
        m_fragEntryPointName = createInfo.psEntryPointName;
        m_nativeFragShaderModule = createShaderModule(
            createInfo.psCode,
            createInfo.psCodeLen,
            createInfo.psEntryPointName);
        if (!m_nativeFragShaderModule) {
            return LN_MAKE_ERROR_deprecated("Failed to create fragment shader module");
        }
    }

    if (createInfo.csCode) {
        m_compEntryPointName = createInfo.csEntryPointName;
        m_nativeCompShaderModule = createShaderModule(
            createInfo.csCode,
            createInfo.csCodeLen,
            createInfo.csEntryPointName);
        if (!m_nativeCompShaderModule) {
            return LN_MAKE_ERROR_deprecated("Failed to create compute shader module");
        }
    }

    auto result = createPipelineLayout(createInfo);
    if (!result) return result;
    
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
    if (m_pipelineLayout) {
        wgpuPipelineLayoutRelease(m_pipelineLayout);
        m_pipelineLayout = nullptr;
    }
    if (m_bindGroupLayout) {
        wgpuBindGroupLayoutRelease(m_bindGroupLayout);
        m_bindGroupLayout = nullptr;
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
        LN_MAKE_ERROR_deprecated("Failed wgpuDeviceCreateShaderModule");
        return nullptr;
    }
    return shaderModule;
}

MaybeResult_deprecated WebGPUShaderPass::createPipelineLayout(const ShaderPassCreateInfo2& createInfo) {
    WGPUDevice nativeDevice = m_wgpuDevice->wgpuDevice();

    // Build WGPUBindGroupLayoutEntry (b, t, s, u の各値をセットするスロットの定義)
    std::vector<WGPUBindGroupLayoutEntry> entries;
    for (int i = 0; i < createInfo.descriptorLayout->bindings.size(); i++) {
        const kokage::TargetBindingInfo& binding = createInfo.descriptorLayout->bindings[i];
        if (binding.space != 0) {
            return LN_MAKE_ERROR_deprecated("(Not Implemented) Binding space must be 0");
        }

        WGPUBindGroupLayoutEntry entry = WGPU_BIND_GROUP_LAYOUT_ENTRY_INIT;
        setupLayoutEntryDefault(&entry);
        entry.binding = binding.index;
        entry.visibility = WGPUShaderStage_None;
        if (binding.used & kokage::ShaderStageFlags_Vertex) {
            entry.visibility |= WGPUShaderStage_Vertex;
        }
        if (binding.used & kokage::ShaderStageFlags_Pixel) {
            entry.visibility |= WGPUShaderStage_Fragment;
        }
        if (binding.used & kokage::ShaderStageFlags_Compute) {
            entry.visibility |= WGPUShaderStage_Compute;
        }

        switch (binding.category) {
            case kokage::BindingResourceCategory_ConstantBuffer:
                entry.buffer.type = WGPUBufferBindingType_Uniform;
                entry.buffer.minBindingSize = binding.size;
                break;
            case kokage::BindingResourceCategory_TextureOrCombinedSampler:
                entry.texture.sampleType = WGPUTextureSampleType_Float;
                entry.texture.viewDimension = WGPUTextureViewDimension_2D;
                entry.texture.multisampled = false;
                break;
            case kokage::BindingResourceCategory_SamplerState:
                entry.sampler.type = WGPUSamplerBindingType_Filtering;
                break;
            case kokage::BindingResourceCategory_UnorderdAccess:
                return LN_MAKE_ERROR_NOT_IMPLEMENTED_deprecated();
             default:
                return LN_MAKE_ERROR_UNREACHABLE_deprecated();
        }

        entries.push_back(entry);
    }

    // Create a bind group layout
    WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
    bindGroupLayoutDesc.nextInChain = nullptr;
    bindGroupLayoutDesc.entryCount = entries.size();
    bindGroupLayoutDesc.entries = entries.data();
    m_bindGroupLayout = wgpuDeviceCreateBindGroupLayout(nativeDevice, &bindGroupLayoutDesc);
    if (!m_bindGroupLayout) {
        return LN_MAKE_ERROR_deprecated("Failed wgpuDeviceCreateBindGroupLayout");
    }

    // Create the pipeline layout
    WGPUPipelineLayoutDescriptor layoutDesc{};
    layoutDesc.nextInChain = nullptr;
    layoutDesc.bindGroupLayoutCount = 1;
    layoutDesc.bindGroupLayouts = &m_bindGroupLayout;
    m_pipelineLayout = wgpuDeviceCreatePipelineLayout(nativeDevice, &layoutDesc);
    if (!m_pipelineLayout) {
        return LN_MAKE_ERROR_deprecated("Failed wgpuDeviceCreatePipelineLayout");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUShaderPass::setupLayoutEntryDefault(WGPUBindGroupLayoutEntry* entry) {
    entry->buffer.nextInChain = nullptr;
    entry->buffer.type = WGPUBufferBindingType_BindingNotUsed;
    entry->buffer.hasDynamicOffset = false;

    entry->sampler.nextInChain = nullptr;
    entry->sampler.type = WGPUSamplerBindingType_BindingNotUsed;

    entry->storageTexture.nextInChain = nullptr;
    entry->storageTexture.access = WGPUStorageTextureAccess_BindingNotUsed;
    entry->storageTexture.format = WGPUTextureFormat_Undefined;
    entry->storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

    entry->texture.nextInChain = nullptr;
    entry->texture.multisampled = false;
    entry->texture.sampleType = WGPUTextureSampleType_BindingNotUsed;
}

} // namespace detail
} // namespace ln
