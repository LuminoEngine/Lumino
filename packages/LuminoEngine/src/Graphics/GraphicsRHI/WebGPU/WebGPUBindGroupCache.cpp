#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUUniformBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUTextureBase.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSamplerState.hpp>
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

MaybeResult_deprecated WebGPUBindGroupCache::getOrCreate(
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
            return LN_MAKE_ERROR_NOT_IMPLEMENTED_deprecated();
        }

        WGPUBindGroupEntry entry = WGPU_BIND_GROUP_ENTRY_INIT;
        entry.binding = info.index;
        switch (info.category) {
            case kokage::BindingResourceCategory_ConstantBuffer: {
                const auto& item = updateInfo.uniforms[info.descriptorEntryIndex];
                const WebGPUUniformBuffer* buffer = static_cast<WebGPUUniformBuffer*>(item.object);
                entry.buffer = buffer->nativeBuffer();
                entry.offset = item.offset;
                entry.size = info.size;
                break;
            }
            case kokage::BindingResourceCategory_TextureOrCombinedSampler: {
                // WebGPU は CombinedSampler しかサポートしていないので、 Texture だけで OK.
                const auto& item = updateInfo.resources[info.descriptorEntryIndex];
                const WebGPUTextureBase* texture = static_cast<WebGPUTextureBase*>(item.object);
                entry.textureView = texture->nativeTextureView();
                break;
            }
            case kokage::BindingResourceCategory_SamplerState: {
                const ShaderDescriptorTableUpdateItem* item = nullptr;
                if (info.descriptorEntryCategory == kokage::RegisterCategory_TextureOrCombinedSampler) {
                    // 入力シェーダコード上、 CombinedSampler が使われている場合は Texture に付いている SamplerState を使う。
                    item = &updateInfo.resources[info.descriptorEntryIndex];
                }
                else {
                    item = &updateInfo.samplers[info.descriptorEntryIndex];
                }
                const auto* sampler = static_cast<WebGPUSamplerState*>(item->stamplerState);
                entry.sampler = sampler->nativeSampler();
                break;
            }
            case kokage::BindingResourceCategory_UnorderdAccess: {
                LN_NOTIMPLEMENTED();
                break;
            }
            default:
                LN_UNREACHABLE();
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
        return LN_MAKE_ERROR_deprecated("wgpuDeviceCreateBindGroup Failed.");
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
