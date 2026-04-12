// Web-only Device::create factory.
// On desktop, Device::create lives in VulkanDevice.cpp and handles both Vulkan
// and WebGPU backends. On Emscripten, Vulkan is unavailable so we provide a
// separate translation unit that only instantiates the WebGPU backend.

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

} // namespace ln::rhi

#endif // __EMSCRIPTEN__
