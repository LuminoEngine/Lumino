#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUBindGroupCache final {
    // NOTE: WGPUBindGroup は固定的で、 Vulkan のように後から書き換えることができない。
    //   Babylon.js や pixi.js の実装ではバインドする全てのリソースをキーとしてキャッシュしていたので、それを参考にする。
public:
    WebGPUBindGroupCache(WebGPUDevice* device);
    ~WebGPUBindGroupCache();
    void dispose();
    MaybeResult_deprecated getOrCreate(
        WebGPUShaderPass* shaderPass,
        const detail::ShaderDescriptorTableUpdateInfo& updateInfo,
        WGPUBindGroup* outBindGroup); 

private:
    class CacheEntry {
    public:
        WebGPUShaderPass* shaderPass;
        WGPUBindGroup bindGroup;
    };
    WebGPUDevice* m_device;
    std::unordered_map<uint32_t, std::unique_ptr<CacheEntry>> m_cache;
};

} // namespace detail
} // namespace ln
