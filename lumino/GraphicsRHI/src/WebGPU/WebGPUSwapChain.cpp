#include <LuminoGraphicsRHI/WebGPU/WebGPUHelper.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPUSwapChain.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUSwapChain

WebGPUSwapChain::WebGPUSwapChain()
    : m_device(nullptr)
    , m_wgpuSurface(nullptr)
    , m_wgpuSwapChain(nullptr)
    , m_format(TextureFormat::Unknown)
    , m_width(0)
    , m_height(0)
    , m_imageIndex(0)
    , m_currentRenderTargets{} {
}

Result<> WebGPUSwapChain::init(WebGPUDevice* device, PlatformWindow* window, const SizeI& backbufferSize) {
    m_device = device;
    m_wgpuSurface = m_device->getWGPUSurface(window);
    m_width = backbufferSize.width;
    m_height = backbufferSize.height;

    WGPUSwapChainDescriptor swapChainDesc = {};
    swapChainDesc.width = backbufferSize.width;
    swapChainDesc.height = backbufferSize.height;
    swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;

    WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(m_wgpuSurface, m_device->wgpuAdapter());
    swapChainDesc.format = swapChainFormat;
    m_format = WebGPUHelper::WGPUTextureFormatToTextureFormat(swapChainFormat);
	
    swapChainDesc.presentMode = WGPUPresentMode_Fifo;
    m_wgpuSwapChain = wgpuDeviceCreateSwapChain(m_device->wgpuDevice(), m_wgpuSurface, &swapChainDesc);
    if (!m_wgpuSwapChain) {
        LN_LOG_ERROR("wgpuDeviceCreateSwapChain failed.");
        return err();
    }


    for (int i = 0; i < BackbufferCount; i++) {
        auto renderTarget = makeRef<WebGPURenderTarget>();
        if (!renderTarget->initForSwapChainWrapper(m_device, m_width, m_height, m_format)) {
            return err();
        }
        m_currentRenderTargets[i] = renderTarget;
    }
	
	//

    // https://hackmd.io/@webgpu/HJdib9rOD#GPUFence--gt-GPUQueueonSubmittedWorkDone
	
    return ok();
}

void WebGPUSwapChain::onDestroy() {
    for (auto& i : m_currentRenderTargets) {
        i->destroy();
    }
    m_currentRenderTargets = {};
    m_wgpuSwapChain = nullptr;
    m_wgpuSurface = nullptr;
}

uint32_t WebGPUSwapChain::getBackbufferCount() {
    return BackbufferCount;
}

void WebGPUSwapChain::acquireNextImage(int* outImageIndex) {
    *outImageIndex = m_imageIndex;

    WebGPURenderTarget* renderTarget = m_currentRenderTargets[m_imageIndex];
    renderTarget->wrapTextureView(wgpuSwapChainGetCurrentTextureView(m_wgpuSwapChain));
}

RHIResource* WebGPUSwapChain::getRenderTarget(int imageIndex) const {
    return m_currentRenderTargets[imageIndex];
}

Result<> WebGPUSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    LN_NOTIMPLEMENTED();
    return ok();
}

void WebGPUSwapChain::present() {
    wgpuSwapChainPresent(m_wgpuSwapChain);
    m_imageIndex = (m_imageIndex + 1) % BackbufferCount;
}

} // namespace detail
} // namespace ln
