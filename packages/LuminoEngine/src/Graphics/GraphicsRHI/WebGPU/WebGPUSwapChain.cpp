#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSwapChain.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPUSwapChain

WebGPUSwapChain::WebGPUSwapChain()
    : m_device(nullptr)
    , m_wgpuSurface(nullptr)
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

    WGPUDevice wgpuDevice = m_device->wgpuDevice();
    WGPUAdapter wgpuAdapter = m_device->wgpuAdapter();

    // Querying Surface Capabilities
    // https://webgpu-native.github.io/webgpu-headers/Surfaces.html#Surface-Capabilities
    WGPUTextureFormat preferredFormat = WGPUTextureFormat_Undefined;
    bool supportsMailbox = false;
    {
        WGPUSurfaceCapabilities capabilities = WGPU_SURFACE_CAPABILITIES_INIT;
        WGPUStatus result = wgpuSurfaceGetCapabilities(m_wgpuSurface, wgpuAdapter, &capabilities);
        if (result != WGPUStatus_Success) {
            return LN_MAKE_ERROR("wgpuSurfaceGetCapabilities failed. %d", result);
        }
        preferredFormat = capabilities.formats[0];
        bool supportsMailbox = false;
        for (size_t i = 0; i < capabilities.presentModeCount; i++) {
            if (capabilities.presentModes[i] == WGPUPresentMode_Mailbox) supportsMailbox = true;
        }
        wgpuSurfaceCapabilitiesFreeMembers(capabilities);
    }

    // Configure the surface
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.nextInChain = nullptr;
    config.width = m_width;
    config.height = m_height;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.format = preferredFormat;
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    config.device = wgpuDevice;
    config.presentMode = supportsMailbox ? WGPUPresentMode_Mailbox : WGPUPresentMode_Fifo,
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(m_wgpuSurface, &config);

    //WGPUSwapChainDescriptor swapChainDesc = {};
    //swapChainDesc.width = backbufferSize.width;
    //swapChainDesc.height = backbufferSize.height;
    //swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;

    //WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(m_wgpuSurface, m_device->wgpuAdapter());
    //swapChainDesc.format = swapChainFormat;
    //m_format = WebGPUHelper::WGPUTextureFormatToTextureFormat(swapChainFormat);
	
    //swapChainDesc.presentMode = WGPUPresentMode_Fifo;
    //m_wgpuSwapChain = wgpuDeviceCreateSwapChain(m_device->wgpuDevice(), m_wgpuSurface, &swapChainDesc);
    //if (!m_wgpuSwapChain) {
    //    LN_LOG_ERROR("wgpuDeviceCreateSwapChain failed.");
    //    return err();
    //}


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
    m_wgpuSurface = nullptr;
}

uint32_t WebGPUSwapChain::getBackbufferCount() {
    return BackbufferCount;
}

void WebGPUSwapChain::acquireNextImage(int* outImageIndex) {
    *outImageIndex = m_imageIndex;
    
	// Get the surface texture
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(m_wgpuSurface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal) {
        LN_LOG_ERROR("wgpuSurfaceGetCurrentTexture failed.");
        return;
    }
    
	// Create a view for this surface texture
    WGPUTextureViewDescriptor viewDescriptor;
    viewDescriptor.nextInChain = nullptr;
    viewDescriptor.label = { "Surface texture view", 20 };
    viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
    viewDescriptor.dimension = WGPUTextureViewDimension_2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = WGPUTextureAspect_All;
    WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

    // https://github.com/eliemichel/LearnWebGPU-Code/blob/step025/main.cpp
#ifndef WEBGPU_BACKEND_WGPU
    // We no longer need the texture, only its view
    // (NB: with wgpu-native, surface textures must not be manually released)
    wgpuTextureRelease(surfaceTexture.texture);
#endif // WEBGPU_BACKEND_WGPU

    WebGPURenderTarget* renderTarget = m_currentRenderTargets[m_imageIndex];
    renderTarget->wrapTextureView(targetView);
}

RHIResource* WebGPUSwapChain::getRenderTarget(int imageIndex) const {
    return m_currentRenderTargets[imageIndex];
}

Result<> WebGPUSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    LN_NOTIMPLEMENTED();
    return ok();
}

void WebGPUSwapChain::present() {
    wgpuSurfacePresent(m_wgpuSurface);

#if defined(WEBGPU_BACKEND_DAWN)
    wgpuDeviceTick(m_device->wgpuDevice());
#elif defined(WEBGPU_BACKEND_WGPU)
    wgpuDevicePoll(m_device->wgpuDevice(), false, nullptr);
#endif

    m_imageIndex = (m_imageIndex + 1) % BackbufferCount;
}

} // namespace detail
} // namespace ln
