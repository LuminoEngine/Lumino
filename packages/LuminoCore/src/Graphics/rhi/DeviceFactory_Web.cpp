// Web 専用の Device::create ファクトリ。
// デスクトップでは Device::create は VulkanDevice.cpp にあり、Vulkan と WebGPU の
// 両バックエンドを扱う。Emscripten では Vulkan が使えないため、WebGPU バックエンドだけを
// 生成する別の翻訳単位を用意する。

#if defined(__EMSCRIPTEN__)

#include "webgpu/WebGPUDevice.hpp"

namespace ln::rhi {

Result<Ref<Device>> Device::create(const DeviceDesc& desc) {
    if (desc.backend == Backend::WebGPU) {
        auto dev = Ref<webgpu::WebGPUDevice>::adopt(new webgpu::WebGPUDevice());
        if (!dev->init(desc)) {
            return LN_MAKE_ERROR("Failed to initialize WebGPU device");
        }
        return Ref<Device>(dev);
    }
    return LN_MAKE_ERROR("Only WebGPU backend is supported on the web.");
}

Result<Ref<Device>> Device::beginCreateAsync(const DeviceDesc& desc) {
    // ブラウザではアダプタ/デバイス要求がイベントループ経由で解決されるため、
    // ブロッキングせずに要求だけ発行し、pumpAsyncInit() で完了を確認する。
    if (desc.backend == Backend::WebGPU) {
        auto dev = Ref<webgpu::WebGPUDevice>::adopt(new webgpu::WebGPUDevice());
        auto r = dev->initAsyncBegin(desc);
        if (!r) {
            return LN_FORWARD_ERROR(r);
        }
        return Ref<Device>(dev);
    }
    return LN_MAKE_ERROR("Only WebGPU backend is supported on the web.");
}

} // namespace ln::rhi

#endif // __EMSCRIPTEN__
