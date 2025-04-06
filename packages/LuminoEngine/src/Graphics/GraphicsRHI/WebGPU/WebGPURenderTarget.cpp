#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU//WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderTarget

WebGPURenderTarget::WebGPURenderTarget()
    : m_rhiDevice(nullptr)
    , m_wgpuTextureView(nullptr)
    , m_nativeFormat(WGPUTextureFormat_Undefined) {
}

Result<> WebGPURenderTarget::initForSwapChainWrapper(
    WebGPUDevice* rhiDevice,
    int width,
    int height,
    TextureFormat format, WGPUTextureFormat nativeFormat) {
    if (!RHIResource::initAsRenderTarget(width, height, format, false, false)) return err();
    m_rhiDevice = rhiDevice;
    m_nativeFormat = nativeFormat;
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
