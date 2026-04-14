#if !defined(__EMSCRIPTEN__)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
#endif // !__EMSCRIPTEN__

#include <cstring>
#include <LuminoBase/Logger.hpp>
#include "WebGPUSwapChain.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUCommandBuffer.hpp"
#include "WebGPUTextureView.hpp"
#include "WebGPUHelpers.hpp"

namespace ln::rhi::webgpu {

WebGPUSwapChain::WebGPUSwapChain() = default;

VoidResult WebGPUSwapChain::init(WebGPUDevice* device, const SwapChainDesc& desc) {
    m_device = device;
    m_width = desc.width;
    m_height = desc.height;

    // Create WGPUSurface
    {
#if defined(__EMSCRIPTEN__)
        // Web: canvas selector 文字列から surface を作成する。
        // desc.nativeWindowHandle は const char* (CSS セレクタ) として渡される。
        const char* canvasSelector = static_cast<const char*>(desc.nativeWindowHandle);
        if (!canvasSelector || canvasSelector[0] == '\0') {
            return LN_MAKE_ERROR("Invalid canvas selector for WebGPU surface creation.");
        }

        WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasSource =
            WGPU_EMSCRIPTEN_SURFACE_SOURCE_CANVAS_HTML_SELECTOR_INIT;
        canvasSource.selector.data = canvasSelector;
        canvasSource.selector.length = strlen(canvasSelector);

        WGPUSurfaceDescriptor surfaceDesc = WGPU_SURFACE_DESCRIPTOR_INIT;
        surfaceDesc.nextInChain = &canvasSource.chain;
        m_surface = wgpuInstanceCreateSurface(device->wgpuInstance(), &surfaceDesc);
        if (!m_surface) {
            return LN_MAKE_ERROR("Failed to create WGPUSurface from canvas.");
        }
        LN_LOG_INFO("[WebGPU] Canvas surface created: selector='%s', %ux%u",
                    canvasSelector, desc.width, desc.height);
#else
        auto* glfwWindow = static_cast<GLFWwindow*>(desc.nativeWindowHandle);
#ifdef _WIN32
        WGPUSurfaceSourceWindowsHWND hwndSource = WGPU_SURFACE_SOURCE_WINDOWS_HWND_INIT;
        hwndSource.hinstance = GetModuleHandle(nullptr);
        hwndSource.hwnd = glfwGetWin32Window(glfwWindow);

        WGPUSurfaceDescriptor surfaceDesc = WGPU_SURFACE_DESCRIPTOR_INIT;
        surfaceDesc.nextInChain = &hwndSource.chain;
        m_surface = wgpuInstanceCreateSurface(device->wgpuInstance(), &surfaceDesc);
#else
        #error "WebGPU surface creation not implemented for this platform"
#endif
        if (!m_surface) {
            return LN_MAKE_ERROR("Failed to create WGPUSurface.");
        }
#endif // __EMSCRIPTEN__
    }

    // Query surface capabilities
    WGPUSurfaceCapabilities caps = WGPU_SURFACE_CAPABILITIES_INIT;
    if (wgpuSurfaceGetCapabilities(m_surface, device->wgpuAdapter(), &caps) != WGPUStatus_Success) {
        return LN_MAKE_ERROR("wgpuSurfaceGetCapabilities failed.");
    }
    if (caps.formatCount == 0) {
        return LN_MAKE_ERROR("No supported surface formats.");
    }
    m_surfaceFormat = caps.formats[0];
    LN_LOG_INFO("[WebGPU] Surface format: %d", static_cast<int>(m_surfaceFormat));

    // Choose present mode: prefer Mailbox (non-vsync) or Fifo (vsync)
    WGPUPresentMode presentMode = WGPUPresentMode_Fifo;
    if (!desc.vsync) {
        for (size_t i = 0; i < caps.presentModeCount; ++i) {
            if (caps.presentModes[i] == WGPUPresentMode_Mailbox) {
                presentMode = WGPUPresentMode_Mailbox;
                break;
            }
        }
    }

    // Configure the surface
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.device = device->wgpuDevice();
    config.format = m_surfaceFormat;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.width = desc.width;
    config.height = desc.height;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    config.presentMode = presentMode;
    wgpuSurfaceConfigure(m_surface, &config);

    // Create the backbuffer view wrapper (reused each frame)
    m_currentBackbufferView = Ref<WebGPUTextureView>::adopt(new WebGPUTextureView());
    m_currentBackbufferView->initFromExternal(nullptr, m_surfaceFormat, m_width, m_height);

    // Create CommandBuffers for each in-flight frame
    m_maxFrames = 2;
    for (uint32_t i = 0; i < m_maxFrames; ++i) {
        auto cmdBuf = Ref<WebGPUCommandBuffer>::adopt(new WebGPUCommandBuffer());
        auto result = cmdBuf->init(device);
        if (!result) {
            return LN_MAKE_ERROR("Failed to create WebGPUCommandBuffer.");
        }
        m_commandBuffers.push_back(std::move(cmdBuf));
    }

    return LN_MAKE_SUCCESS();
}

TextureView* WebGPUSwapChain::acquireNextTexture() {
    // Release previous frame's texture resources
    if (m_currentTextureView) {
        wgpuTextureViewRelease(m_currentTextureView);
        m_currentTextureView = nullptr;
    }
    if (m_currentTexture) {
        wgpuTextureRelease(m_currentTexture);
        m_currentTexture = nullptr;
    }

    // Get current texture from the surface
    WGPUSurfaceTexture surfaceTexture = WGPU_SURFACE_TEXTURE_INIT;
    wgpuSurfaceGetCurrentTexture(m_surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        LN_LOG_ERROR("[WebGPU] wgpuSurfaceGetCurrentTexture failed: status=%d", static_cast<int>(surfaceTexture.status));
        return nullptr;
    }
    m_currentTexture = surfaceTexture.texture;

    // Create a view for this frame's texture
    WGPUTextureViewDescriptor viewDesc = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
    viewDesc.format = m_surfaceFormat;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.baseMipLevel = 0;
    viewDesc.mipLevelCount = 1;
    viewDesc.baseArrayLayer = 0;
    viewDesc.arrayLayerCount = 1;
    viewDesc.aspect = WGPUTextureAspect_All;
    m_currentTextureView = wgpuTextureCreateView(m_currentTexture, &viewDesc);
    if (!m_currentTextureView) {
        LN_LOG_ERROR("[WebGPU] wgpuTextureCreateView for swap chain failed.");
        return nullptr;
    }

    // Update the wrapper view
    m_currentBackbufferView->rewrap(m_currentTextureView);

    // Begin the command buffer for this frame
    if (!m_commandBuffers[m_currentFrame]->begin()) {
        return nullptr;
    }

    return m_currentBackbufferView.get();
}

void WebGPUSwapChain::present() {
    // Submit the command buffer
    m_commandBuffers[m_currentFrame]->submit();

    // Present the surface
    // On the web, requestAnimationFrame handles presentation automatically;
    // wgpuSurfacePresent is unsupported in emdawnwebgpu.
#ifndef __EMSCRIPTEN__
    wgpuSurfacePresent(m_surface);
#endif

    // Round-robin frame index
    m_currentFrame = (m_currentFrame + 1) % m_maxFrames;
}

CommandBuffer* WebGPUSwapChain::getCurrentCommandBuffer() {
    return m_commandBuffers[m_currentFrame].get();
}

void WebGPUSwapChain::finalize() {
    // Wait for GPU to finish
    if (m_device && m_device->wgpuDevice()) {
        m_device->waitIdle();
    }

    m_commandBuffers.clear();
    m_currentBackbufferView = nullptr;

    if (m_currentTextureView) {
        wgpuTextureViewRelease(m_currentTextureView);
        m_currentTextureView = nullptr;
    }
    if (m_currentTexture) {
        wgpuTextureRelease(m_currentTexture);
        m_currentTexture = nullptr;
    }

    if (m_surface) {
        wgpuSurfaceUnconfigure(m_surface);
        wgpuSurfaceRelease(m_surface);
        m_surface = nullptr;
    }

    SwapChain::finalize();
}

} // namespace ln::rhi::webgpu
