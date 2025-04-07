#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUUniformBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUShaderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUBindGroupCache.hpp>
#include <LuminoEngine/Base/MixHash.hpp>

namespace ln {
namespace detail {

WebGPUBindGroupCache::WebGPUBindGroupCache(WebGPUDevice* device)
    : m_device(device) {
}

WebGPUBindGroupCache::~WebGPUBindGroupCache() {
    dispose();
}

void WebGPUBindGroupCache::dispose() {
    for (auto& entry : m_cache) {
        if (entry.second->bindGroup) {
            wgpuBindGroupRelease(entry.second->bindGroup);
        }
    }
    m_cache.clear();
}

MaybeResult WebGPUBindGroupCache::getOrCreate(
    WebGPUShaderPass* shaderPass,
    const detail::ShaderDescriptorTableUpdateInfo& updateInfo,
    WGPUBindGroup* outBindGroup) {

    // Make hash key & find in cache.
    MixHash hash;
    for (const auto& item : updateInfo.uniforms) {
        hash.add(&item, sizeof(item));
    }
    for (const auto& item : updateInfo.resources) {
        hash.add(&item, sizeof(item));
    }
    for (const auto& item : updateInfo.samplers) {
        hash.add(&item, sizeof(item));
    }
    for (const auto& item : updateInfo.storages) {
        hash.add(&item, sizeof(item));
    }
    uint32_t key = hash.value();
    auto itr = m_cache.find(key);
    if (itr != m_cache.end()) {
        *outBindGroup = itr->second->bindGroup;
        return LN_MAKE_SUCCESS();
    }



    const kokage::TargetBindingLayoutInfo& targetBindingLayoutInfo = shaderPass->targetBindingLayoutInfo();

    
    std::vector<WGPUBindGroupEntry> entries;
    for (const auto& info : targetBindingLayoutInfo.bindings) {
        if (info.space != 0) {
            LN_NOTIMPLEMENTED();
            return LN_MAKE_ERROR_NOT_IMPLEMENTED();
        }

        WGPUBindGroupEntry entry = WGPU_BIND_GROUP_ENTRY_INIT;
        entry.binding = info.index;
        switch (info.descriptorEntryCategory) {
            case kokage::BindingResourceCategory_UniformBuffer: {
                const auto& item = updateInfo.uniforms[info.descriptorEntryIndex];
                const WebGPUUniformBuffer* buffer = static_cast<WebGPUUniformBuffer*>(item.object);
                entry.offset = item.offset;
                entry.size = buffer->memorySize();
                break;
            }
            case kokage::BindingResourceCategory_TextureOrCombinedSampler: {
                LN_NOTIMPLEMENTED();
                break;
            }
            case kokage::BindingResourceCategory_SamplerState: {
                LN_NOTIMPLEMENTED();
                break;
            }
            case kokage::BindingResourceCategory_UnorderdAccess: {
                LN_NOTIMPLEMENTED();
                break;
            }
            default:
                break;
        }

        entries.push_back(entry);
    }

    WGPUBindGroupDescriptor bindGroupDesc = WGPU_BIND_GROUP_DESCRIPTOR_INIT;
    bindGroupDesc.nextInChain = nullptr;
    bindGroupDesc.layout = shaderPass->bindGroupLayout();
    // There must be as many bindings as declared in the layout!
    bindGroupDesc.entryCount = entries.size();
    bindGroupDesc.entries = entries.data();
    WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(m_device->wgpuDevice(), &bindGroupDesc);
    if (!bindGroup) {
        return LN_MAKE_ERROR("wgpuDeviceCreateBindGroup Failed.");
    }

    auto cacheEntry = std::make_unique<CacheEntry>();
    cacheEntry->shaderPass = shaderPass;
    cacheEntry->bindGroup = bindGroup;
    m_cache.insert({ key, std::move(cacheEntry) });

    *outBindGroup = bindGroup;
    return LN_MAKE_SUCCESS();
}

} // namespace detail
} // namespace ln
