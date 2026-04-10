#include "WebGPUPipelineLayout.hpp"
#include "WebGPUBuffer.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUTextureView.hpp"

namespace ln::rhi::webgpu {

namespace {

WGPUShaderStage toWGPUShaderStage(ShaderStage stage) {
    WGPUShaderStage result = WGPUShaderStage_None;
    const u32 bits = static_cast<u32>(stage);
    if (bits & static_cast<u32>(ShaderStage::Vertex))   result |= WGPUShaderStage_Vertex;
    if (bits & static_cast<u32>(ShaderStage::Fragment)) result |= WGPUShaderStage_Fragment;
    if (bits & static_cast<u32>(ShaderStage::Compute))  result |= WGPUShaderStage_Compute;
    return result;
}

} // namespace

// ------ WebGPUBindGroupLayout -------------------------------------------------------------------------------------------------

WebGPUBindGroupLayout::WebGPUBindGroupLayout() = default;

VoidResult WebGPUBindGroupLayout::init(WebGPUDevice* device, const BindGroupLayoutDesc& desc) {
    m_device = device;

    std::vector<WGPUBindGroupLayoutEntry> entries;
    entries.reserve(desc.entries.size());
    for (const auto& e : desc.entries) {
        // WGPU_BIND_GROUP_LAYOUT_ENTRY_INIT zero-initializes all four resource sub-structs
        // (buffer/sampler/texture/storageTexture). Their *_BindingNotUsed enum values are 0,
        // so the unset sub-structs are correctly inert; only the active one needs to be filled.
        WGPUBindGroupLayoutEntry entry = WGPU_BIND_GROUP_LAYOUT_ENTRY_INIT;
        entry.binding = e.binding;
        entry.visibility = toWGPUShaderStage(e.visibility);

        switch (e.type) {
            case BindingType::UniformBuffer:
                entry.buffer.type = WGPUBufferBindingType_Uniform;
                entry.buffer.hasDynamicOffset = e.hasDynamicOffset ? WGPU_TRUE : WGPU_FALSE;
                break;
            case BindingType::StorageBuffer:
                entry.buffer.type = WGPUBufferBindingType_Storage;
                entry.buffer.hasDynamicOffset = e.hasDynamicOffset ? WGPU_TRUE : WGPU_FALSE;
                break;
            case BindingType::SampledTexture:
                entry.texture.sampleType = WGPUTextureSampleType_Float;
                entry.texture.viewDimension = WGPUTextureViewDimension_2D;
                entry.texture.multisampled = WGPU_FALSE;
                break;
            case BindingType::Sampler:
                entry.sampler.type = WGPUSamplerBindingType_Filtering;
                break;
            case BindingType::StorageTexture:
                return LN_MAKE_ERROR("WebGPUBindGroupLayout: StorageTexture binding not yet implemented.");
            default:
                return LN_MAKE_ERROR("WebGPUBindGroupLayout: unknown BindingType.");
        }
        entries.push_back(entry);
    }

    WGPUBindGroupLayoutDescriptor layoutDesc = WGPU_BIND_GROUP_LAYOUT_DESCRIPTOR_INIT;
    layoutDesc.label = {"LuminoBindGroupLayout", WGPU_STRLEN};
    layoutDesc.entryCount = entries.size();
    layoutDesc.entries = entries.empty() ? nullptr : entries.data();

    m_layout = wgpuDeviceCreateBindGroupLayout(m_device->wgpuDevice(), &layoutDesc);
    if (!m_layout) {
        return LN_MAKE_ERROR("wgpuDeviceCreateBindGroupLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUBindGroupLayout::finalize() {
    if (m_layout) {
        wgpuBindGroupLayoutRelease(m_layout);
        m_layout = nullptr;
    }
    BindGroupLayout::finalize();
}

// ------ WebGPUBindGroup -------------------------------------------------------------------------------------------------------

WebGPUBindGroup::WebGPUBindGroup() = default;

VoidResult WebGPUBindGroup::init(WebGPUDevice* device, WebGPUBindGroupLayout* layout,
                                  const std::vector<BindGroupEntry>& entries) {
    m_device = device;

    std::vector<WGPUBindGroupEntry> wgpuEntries;
    wgpuEntries.reserve(entries.size());
    for (const auto& e : entries) {
        WGPUBindGroupEntry entry = WGPU_BIND_GROUP_ENTRY_INIT;
        entry.binding = e.binding;
        if (e.buffer) {
            entry.buffer = static_cast<WebGPUBuffer*>(e.buffer)->handle();
            entry.offset = e.offset;
            entry.size   = (e.size > 0) ? e.size : WGPU_WHOLE_SIZE;
        } else if (e.textureView) {
            entry.textureView = static_cast<WebGPUTextureView*>(e.textureView)->handle();
        } else if (e.sampler) {
            return LN_MAKE_ERROR("WebGPUBindGroup: Sampler binding not yet implemented.");
        } else {
            return LN_MAKE_ERROR("WebGPUBindGroup: BindGroupEntry has no resource set.");
        }
        wgpuEntries.push_back(entry);
    }

    WGPUBindGroupDescriptor desc = WGPU_BIND_GROUP_DESCRIPTOR_INIT;
    desc.label      = {"LuminoBindGroup", WGPU_STRLEN};
    desc.layout     = layout->handle();
    desc.entryCount = wgpuEntries.size();
    desc.entries    = wgpuEntries.empty() ? nullptr : wgpuEntries.data();

    m_bindGroup = wgpuDeviceCreateBindGroup(m_device->wgpuDevice(), &desc);
    if (!m_bindGroup) {
        return LN_MAKE_ERROR("wgpuDeviceCreateBindGroup failed.");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUBindGroup::finalize() {
    if (m_bindGroup) {
        wgpuBindGroupRelease(m_bindGroup);
        m_bindGroup = nullptr;
    }
    BindGroup::finalize();
}

// ------ WebGPUPipelineLayout --------------------------------------------------------------------------------------------------

WebGPUPipelineLayout::WebGPUPipelineLayout() = default;

VoidResult WebGPUPipelineLayout::init(WebGPUDevice* device, const PipelineLayoutDesc& desc) {
    m_device = device;

    // Create one WebGPUBindGroupLayout per descriptor set. Sparse/empty entries are still
    // materialized so that set indices in the resulting pipeline layout match shader expectations.
    m_bindGroupLayouts.reserve(desc.setLayouts.size());
    std::vector<WGPUBindGroupLayout> wgpuLayouts;
    wgpuLayouts.reserve(desc.setLayouts.size());

    for (const auto& setLayout : desc.setLayouts) {
        auto bgl = Ref<WebGPUBindGroupLayout>::adopt(new WebGPUBindGroupLayout());
        auto result = bgl->init(m_device, setLayout);
        if (!result) {
            return tl::unexpected(result.error());
        }
        wgpuLayouts.push_back(bgl->handle());
        m_bindGroupLayouts.push_back(std::move(bgl));
    }

    WGPUPipelineLayoutDescriptor layoutDesc = WGPU_PIPELINE_LAYOUT_DESCRIPTOR_INIT;
    layoutDesc.label = {"LuminoPipelineLayout", WGPU_STRLEN};
    layoutDesc.bindGroupLayoutCount = wgpuLayouts.size();
    layoutDesc.bindGroupLayouts = wgpuLayouts.empty() ? nullptr : wgpuLayouts.data();

    m_layout = wgpuDeviceCreatePipelineLayout(m_device->wgpuDevice(), &layoutDesc);
    if (!m_layout) {
        return LN_MAKE_ERROR("wgpuDeviceCreatePipelineLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

Result<Ref<BindGroup>> WebGPUPipelineLayout::createBindGroup(
    u32 setIndex, const std::vector<BindGroupEntry>& entries) {
    if (setIndex >= m_bindGroupLayouts.size()) {
        return LN_MAKE_ERROR("WebGPUPipelineLayout::createBindGroup: setIndex out of range.");
    }
    auto bg = Ref<WebGPUBindGroup>::adopt(new WebGPUBindGroup());
    auto r  = bg->init(m_device, m_bindGroupLayouts[setIndex].get(), entries);
    if (!r) return tl::unexpected(r.error());
    return Ref<BindGroup>(bg);
}

void WebGPUPipelineLayout::finalize() {
    if (m_layout) {
        wgpuPipelineLayoutRelease(m_layout);
        m_layout = nullptr;
    }
    m_bindGroupLayouts.clear();
    PipelineLayout::finalize();
}

} // namespace ln::rhi::webgpu
