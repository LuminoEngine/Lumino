#include <LuminoEngine/GraphicsRHI/WebGPU//WebGPUHelper.hpp>
#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderTarget

WebGPURenderTarget::WebGPURenderTarget()
    : m_rhiDevice(nullptr)
    , m_wgpuTextureView(nullptr) {
}

Result<> WebGPURenderTarget::initForSwapChainWrapper(WebGPUDevice* rhiDevice, int width, int height, TextureFormat format) {
    if (!RHIResource::initAsRenderTarget(width, height, format, false, false)) return err();
    m_rhiDevice = rhiDevice;
    return ok();
}

void WebGPURenderTarget::onDestroy() {
    m_wgpuTextureView = nullptr;
}

void WebGPURenderTarget::wrapTextureView(WGPUTextureView view) {
    m_wgpuTextureView = view;
}

} // namespace detail
} // namespace ln
