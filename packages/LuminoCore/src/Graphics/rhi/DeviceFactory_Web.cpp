// Web 専用の Device::create ファクトリ。
// デスクトップでは Device::create は VulkanDevice.cpp にあり、Vulkan と WebGPU の
// 両バックエンドを扱う。Emscripten では Vulkan が使えないため、Web で使えるバックエンド
// (WebGL2 / WebGPU) だけを生成する別の翻訳単位を用意する。

#if defined(__EMSCRIPTEN__)

#include "webgl2/WebGL2Device.hpp"
#include "webgpu/WebGPUDevice.hpp"

namespace ln::rhi {

namespace {

/** WebGL2 デバイスを生成する。WebGL のコンテキスト生成は同期的に完了する。 */
Result<Ref<Device>> createWebGL2Device(const DeviceDesc& desc) {
    auto dev = Ref<webgl2::WebGL2Device>::adopt(new webgl2::WebGL2Device());
    auto r = dev->init(desc);
    if (!r) {
        return LN_FORWARD_ERROR(r);
    }
    return Ref<Device>(dev);
}

} // namespace

Result<Ref<Device>> Device::create(const DeviceDesc& desc) {
    if (desc.backend == Backend::WebGL2) {
        return createWebGL2Device(desc);
    }
    if (desc.backend == Backend::WebGPU) {
        auto dev = Ref<webgpu::WebGPUDevice>::adopt(new webgpu::WebGPUDevice());
        if (!dev->init(desc)) {
            return LN_MAKE_ERROR("Failed to initialize WebGPU device");
        }
        return Ref<Device>(dev);
    }
    return LN_MAKE_ERROR("Only WebGL2 and WebGPU backends are supported on the web.");
}

Result<Ref<Device>> Device::beginCreateAsync(const DeviceDesc& desc) {
    // WebGL2 は同期的に初期化できるため、既定の pumpAsyncInit() が Ready を返す。
    if (desc.backend == Backend::WebGL2) {
        return createWebGL2Device(desc);
    }
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
    return LN_MAKE_ERROR("Only WebGL2 and WebGPU backends are supported on the web.");
}

} // namespace ln::rhi

#endif // __EMSCRIPTEN__
