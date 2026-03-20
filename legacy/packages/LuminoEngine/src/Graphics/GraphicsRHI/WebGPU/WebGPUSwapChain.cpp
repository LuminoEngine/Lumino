#ifdef LN_OS_WIN32
#include <Windows.h>
#endif
#include <LuminoEngine/Platform/PlatformSupport.hpp>
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
    , m_currentRenderTargets{}
    , m_nativeCurrentTexture(nullptr)
    , m_nativeCurrentTextureView(nullptr) {
}

MaybeResult WebGPUSwapChain::init(WebGPUDevice* device, const SwapChainCreateInfo& createInfo) {
    m_device = device;
    m_wgpuSurface = getWGPUSurface(createInfo);

    if (createInfo.window) {
        m_width = createInfo.backbufferSize.width;
        m_height = createInfo.backbufferSize.height;
    }
    else {
        // Emscripten ではサイズ 0,0 で wgpuSurfaceConfigure() すると、
        // 自動的に現在の canvas サイズに合わせてくれる。
        m_width = 0;
        m_height = 0;
    }

    WGPUDevice wgpuDevice = m_device->wgpuDevice();
    WGPUAdapter wgpuAdapter = m_device->wgpuAdapter();

    // Querying Surface Capabilities
    // https://webgpu-native.github.io/webgpu-headers/Surfaces.html#Surface-Capabilities
    WGPUTextureFormat preferredFormat = WGPUTextureFormat_Undefined;
    bool supportsMailbox = false;
    {
        WGPUSurfaceCapabilities capabilities = WGPU_SURFACE_CAPABILITIES_INIT;
#ifdef LN_WEBGPU_LEGACY
        wgpuSurfaceGetCapabilities(m_wgpuSurface, wgpuAdapter, &capabilities);
#else
        WGPUStatus result = wgpuSurfaceGetCapabilities(m_wgpuSurface, wgpuAdapter, &capabilities);
        if (result != WGPUStatus_Success) {
            return LN_MAKE_ERROR("wgpuSurfaceGetCapabilities failed. %d", result);
        }
#endif
        preferredFormat = capabilities.formats[0];
        for (size_t i = 0; i < capabilities.presentModeCount; i++) {
            WGPUPresentMode mode = capabilities.presentModes[i];
            if (mode == WGPUPresentMode_Mailbox) {
                supportsMailbox = true;
            }
        }
        wgpuSurfaceCapabilitiesFreeMembers(capabilities);
    }

    // Configure the surface
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.nextInChain = nullptr;
    config.device = wgpuDevice;
    config.format = preferredFormat;
    config.usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_CopySrc; // CopySrc is for screen capture
    config.width = m_width;
    config.height = m_height;
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    config.presentMode = supportsMailbox ? WGPUPresentMode_Mailbox : WGPUPresentMode_Fifo,
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    wgpuSurfaceConfigure(m_wgpuSurface, &config);

    // NOTE: VSync 
    // https://groups.google.com/g/dawn-graphics/c/_KMbPd98kUE
    // NoWait でレンダリングしているとき、
    // - WGPUPresentMode_Mailbox だとすごく早くフレームが進む
    // - WGPUPresentMode_Fifo だと、フレームが進むのが遅くなる。


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

    
    if (!createInfo.window) {
        //WGPUSurfaceTexture surfaceTexture;
        //wgpuSurfaceGetCurrentTexture(m_wgpuSurface, &surfaceTexture);
        //m_width = wgpuTextureGetWidth(surfaceTexture.texture);
        //m_height = wgpuTextureGetHeight(surfaceTexture.texture);
        m_width = 800;
        m_height = 600;
        //std::cout << "surfaceTexture: " << surfaceTexture.texture << std::endl;
        std::cout << "canvas backbuffer size: " << m_width << "," << m_height << std::endl;
    }


    for (int i = 0; i < BackbufferCount; i++) {
        auto renderTarget = makeRef<WebGPURenderTarget>();
        auto result = renderTarget->initForSwapChainWrapper(m_device, m_width, m_height, m_format, preferredFormat);
        if (!result) {
            return LN_TO_ERROR(result);
        }
        m_currentRenderTargets[i] = renderTarget;
    }
	
    // https://hackmd.io/@webgpu/HJdib9rOD#GPUFence--gt-GPUQueueonSubmittedWorkDone
	
    return LN_MAKE_SUCCESS();
}

void WebGPUSwapChain::onDestroy() {
    for (auto& i : m_currentRenderTargets) {
        i->destroy();
    }
    m_currentRenderTargets = {};
    releaseCurrentTexture();
    m_wgpuSurface = nullptr;
}

uint32_t WebGPUSwapChain::getBackbufferCount() {
    return BackbufferCount;
}

void WebGPUSwapChain::acquireNextImage(int* outImageIndex) {
    releaseCurrentTexture();
    *outImageIndex = m_imageIndex;
    
	// Get the surface texture
    // NOTE: Dawn では surfaceTexture.texture は毎回異なる値を返したので、
    //   これをもとにバックバッファ数を推定することは無理そう。
    WGPUSurfaceTexture surfaceTexture;
    wgpuSurfaceGetCurrentTexture(m_wgpuSurface, &surfaceTexture);
#ifdef LN_WEBGPU_LEGACY
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
        LN_LOG_ERROR("wgpuSurfaceGetCurrentTexture failed.");
        return;
    }
#else
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal) {
        LN_LOG_ERROR("wgpuSurfaceGetCurrentTexture failed.");
        return;
    }
#endif
    m_nativeCurrentTexture = surfaceTexture.texture;
    
	// Create a view for this surface texture
    WGPUTextureViewDescriptor viewDescriptor = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    viewDescriptor.nextInChain = nullptr;
    viewDescriptor.label = LN_WEBGPU_MAKE_STRING_VIEW("Surface texture view");
    viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
    viewDescriptor.dimension = WGPUTextureViewDimension_2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = WGPUTextureAspect_All;
    m_nativeCurrentTextureView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

    WebGPURenderTarget* renderTarget = m_currentRenderTargets[m_imageIndex];
    renderTarget->wrapTextureView(surfaceTexture.texture, m_nativeCurrentTextureView);
}

RHIResource* WebGPUSwapChain::getRenderTarget(int imageIndex) const {
    return m_currentRenderTargets[imageIndex];
}

Result_deprecated<> WebGPUSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    LN_NOTIMPLEMENTED();
    return ok();
}

void WebGPUSwapChain::present() {
    wgpuSurfacePresent(m_wgpuSurface);

//#if defined(WEBGPU_BACKEND_DAWN)
//    wgpuDeviceTick(m_device->wgpuDevice());
//#elif defined(WEBGPU_BACKEND_WGPU)
//    wgpuDevicePoll(m_device->wgpuDevice(), false, nullptr);
//#endif

    m_imageIndex = (m_imageIndex + 1) % BackbufferCount;
}

// https://webgpu-native.github.io/webgpu-headers/Surfaces.html#Surface-Presenting
void WebGPUSwapChain::releaseCurrentTexture() {
    if (m_nativeCurrentTextureView) {
        wgpuTextureViewRelease(m_nativeCurrentTextureView);
        m_nativeCurrentTextureView = nullptr;
    }
    if (m_nativeCurrentTexture) {
        wgpuTextureRelease(m_nativeCurrentTexture);
        m_nativeCurrentTexture = nullptr;
    }
}

// https://github.com/eliemichel/glfw3webgpu/blob/main/glfw3webgpu.c
WGPUSurface WebGPUSwapChain::getWGPUSurface(const SwapChainCreateInfo& createInfo) const {
#ifdef __EMSCRIPTEN__
    WGPUSurfaceDescriptorFromCanvasHTMLSelector fromCanvasHTMLSelector;
    fromCanvasHTMLSelector.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
    fromCanvasHTMLSelector.chain.next = NULL;
    fromCanvasHTMLSelector.selector = "#my_canvas";

    WGPUSurfaceDescriptor surfaceDescriptor = {};
    surfaceDescriptor.nextInChain = &fromCanvasHTMLSelector.chain;
#endif

#ifdef LN_OS_WIN32
    HWND hWnd = reinterpret_cast<HWND>(PlatformSupport::getWin32WindowHandle(createInfo.window));
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    WGPUChainedStruct chainedStruct1 = {};
    chainedStruct1.next = nullptr;
#ifdef WEBGPU_BACKEND_DAWN
    chainedStruct1.sType = WGPUSType_SurfaceSourceWindowsHWND;
#else
    chainedStruct1.sType = WGPUSType_SurfaceDescriptorFromWindowsHWND;
#endif

    WGPUSurfaceDescriptorFromWindowsHWND hwndDesc = {};
    hwndDesc.chain = chainedStruct1;
    hwndDesc.hinstance = hInstance;
    hwndDesc.hwnd = hWnd;

    WGPUSurfaceDescriptor surfaceDescriptor = {};
    surfaceDescriptor.nextInChain = &hwndDesc.chain;
#endif

    return wgpuInstanceCreateSurface(m_device->nativeInstance(), &surfaceDescriptor);
}

} // namespace detail
} // namespace ln
