#include <LuminoGraphicsRHI/WebGPU//WebGPUHelper.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPURenderTarget.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderTarget

WebGPURenderTarget::WebGPURenderTarget()
    : m_rhiDevice(nullptr)
    , m_wgpuTextureView(nullptr) {
}

Result WebGPURenderTarget::resetFromSwapChain(WebGPUDevice* rhiDevice, WGPUTextureView view, int width, int height, TextureFormat format) {
    if (!RHIResource::initAsRenderTarget(width, height, format, false, false)) return err();
    m_rhiDevice = rhiDevice;
    m_wgpuTextureView = view;
    return ok();
}

void WebGPURenderTarget::dispose() {
    m_wgpuTextureView = nullptr;
}

} // namespace detail
} // namespace ln
