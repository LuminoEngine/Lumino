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

    // WGPUSurface を作成
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

    // サーフェスの機能を問い合わせる
    WGPUSurfaceCapabilities caps = WGPU_SURFACE_CAPABILITIES_INIT;
    if (wgpuSurfaceGetCapabilities(m_surface, device->wgpuAdapter(), &caps) != WGPUStatus_Success) {
        return LN_MAKE_ERROR("wgpuSurfaceGetCapabilities failed.");
    }
    if (caps.formatCount == 0) {
        return LN_MAKE_ERROR("No supported surface formats.");
    }
    m_surfaceFormat = caps.formats[0];
    LN_LOG_INFO("[WebGPU] Surface format: %d", static_cast<int>(m_surfaceFormat));

    // CopySrc: バックバッファを readbackTexture でキャプチャできるようにする。
    // サーフェスがサポートしている場合のみ付与する (未サポートなら警告のみ)。
    m_surfaceUsage = WGPUTextureUsage_RenderAttachment;
    if (caps.usages & WGPUTextureUsage_CopySrc) {
        m_surfaceUsage |= WGPUTextureUsage_CopySrc;
    } else {
        LN_LOG_WARNING("[WebGPU] Surface does not support CopySrc; backbuffer readback will be unavailable.");
    }

    // present モードを選択: vsync なしなら Mailbox、vsync ありなら Fifo を優先する
    WGPUPresentMode presentMode = WGPUPresentMode_Fifo;
    if (!desc.vsync) {
        for (size_t i = 0; i < caps.presentModeCount; ++i) {
            if (caps.presentModes[i] == WGPUPresentMode_Mailbox) {
                presentMode = WGPUPresentMode_Mailbox;
                break;
            }
        }
    }
    m_presentMode = presentMode;

    // サーフェスを構成
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.device = device->wgpuDevice();
    config.format = m_surfaceFormat;
    config.usage = m_surfaceUsage;
    config.width = desc.width;
    config.height = desc.height;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    config.presentMode = presentMode;
    wgpuSurfaceConfigure(m_surface, &config);

    // バックバッファビューのラッパーを作成 (毎フレーム再利用する)
    m_currentBackbufferView = Ref<WebGPUTextureView>::adopt(new WebGPUTextureView());
    m_currentBackbufferView->initFromExternal(nullptr, m_surfaceFormat, m_width, m_height);

#if !defined(__EMSCRIPTEN__)
    // readback 用キャプチャテクスチャを作成する。
    if (auto r = recreateCaptureTexture(); !r) {
        return LN_FORWARD_ERROR(r);
    }
#endif

    // in-flight フレームごとに CommandBuffer を作成
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
    // 前フレームのテクスチャリソースを解放
    if (m_currentTextureView) {
        wgpuTextureViewRelease(m_currentTextureView);
        m_currentTextureView = nullptr;
    }
    if (m_currentTexture) {
        wgpuTextureRelease(m_currentTexture);
        m_currentTexture = nullptr;
    }

    // サーフェスから現在のテクスチャを取得
    WGPUSurfaceTexture surfaceTexture = WGPU_SURFACE_TEXTURE_INIT;
    wgpuSurfaceGetCurrentTexture(m_surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal &&
        surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal) {
        LN_LOG_ERROR("[WebGPU] wgpuSurfaceGetCurrentTexture failed: status=%d", static_cast<int>(surfaceTexture.status));
        // Lost / Error はサーフェスの再構成では回復できない致命的状態で、
        // デバイスロスト時にもこの status が返る。デバイスロストとして扱う。
        // Timeout / Outdated は一時的または再構成 (リサイズ) で回復する事象のため対象外。
        if (surfaceTexture.status == WGPUSurfaceGetCurrentTextureStatus_Lost ||
            surfaceTexture.status == WGPUSurfaceGetCurrentTextureStatus_Error) {
            m_device->markDeviceLost("wgpuSurfaceGetCurrentTexture");
        }
        return nullptr;
    }
    m_currentTexture = surfaceTexture.texture;

    // このフレームのテクスチャに対するビューを作成
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

    // ラッパービューを更新する。
    // readback のコピー元は present 後も生存する永続キャプチャテクスチャを指す
    // (サーフェステクスチャは present で破棄されるため直接は使えない)。
#if !defined(__EMSCRIPTEN__)
    m_currentBackbufferView->rewrap(m_currentTextureView, m_captureTexture);
#else
    m_currentBackbufferView->rewrap(m_currentTextureView, nullptr);
#endif

    // このフレームのコマンドバッファを開始
    if (!m_commandBuffers[m_currentFrame]->begin()) {
        return nullptr;
    }

    return m_currentBackbufferView.get();
}

void WebGPUSwapChain::present() {
    // コマンドバッファを送信
    m_commandBuffers[m_currentFrame]->submit();

#if !defined(__EMSCRIPTEN__)
    // present でサーフェステクスチャが破棄される前に、readback 用の
    // 永続テクスチャへバックバッファをコピーしておく。
    copyBackbufferToCaptureTexture();
#endif

    // サーフェスを present する
    // Web では requestAnimationFrame が自動的に present を行う。
    // emdawnwebgpu では wgpuSurfacePresent は未サポート。
#ifndef __EMSCRIPTEN__
    wgpuSurfacePresent(m_surface);
#endif

    // フレームインデックスをラウンドロビンで進める
    m_currentFrame = (m_currentFrame + 1) % m_maxFrames;
}

CommandBuffer* WebGPUSwapChain::getCurrentCommandBuffer() {
    return m_commandBuffers[m_currentFrame].get();
}

VoidResult WebGPUSwapChain::resize(uint32_t width, uint32_t height) {
    if (width == m_width && height == m_height) {
        return LN_MAKE_SUCCESS();
    }

    m_device->waitIdle();

    // 現在フレームのテクスチャリソースを解放
    if (m_currentTextureView) {
        wgpuTextureViewRelease(m_currentTextureView);
        m_currentTextureView = nullptr;
    }
    if (m_currentTexture) {
        wgpuTextureRelease(m_currentTexture);
        m_currentTexture = nullptr;
    }

    // 新しいサイズでサーフェスを再構成
    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.device = m_device->wgpuDevice();
    config.format = m_surfaceFormat;
    config.usage = m_surfaceUsage;
    config.width = width;
    config.height = height;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    config.presentMode = m_presentMode;
    wgpuSurfaceConfigure(m_surface, &config);

    m_width = width;
    m_height = height;

    // バックバッファビューのラッパーのサイズを更新
    m_currentBackbufferView->initFromExternal(nullptr, m_surfaceFormat, m_width, m_height);

#if !defined(__EMSCRIPTEN__)
    // キャプチャテクスチャも新サイズで再作成する。
    if (auto r = recreateCaptureTexture(); !r) {
        return LN_FORWARD_ERROR(r);
    }
#endif

    LN_LOG_INFO("[WebGPU] SwapChain resized to %ux%u", width, height);
    return LN_MAKE_SUCCESS();
}

void WebGPUSwapChain::finalize() {
    // GPU の完了を待つ
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

#if !defined(__EMSCRIPTEN__)
    releaseCaptureTexture();
#endif

    if (m_surface) {
        wgpuSurfaceUnconfigure(m_surface);
        wgpuSurfaceRelease(m_surface);
        m_surface = nullptr;
    }

    SwapChain::finalize();
}

#if !defined(__EMSCRIPTEN__)

void WebGPUSwapChain::releaseCaptureTexture() {
    if (m_captureTexture) {
        wgpuTextureRelease(m_captureTexture);
        m_captureTexture = nullptr;
    }
}

VoidResult WebGPUSwapChain::recreateCaptureTexture() {
    releaseCaptureTexture();

    // サーフェスが CopySrc 非対応ならキャプチャ不可。テクスチャは作らない。
    if (!(m_surfaceUsage & WGPUTextureUsage_CopySrc)) {
        return LN_MAKE_SUCCESS();
    }

    WGPUTextureDescriptor texDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
    // CopyDst: サーフェスからのコピー先 / CopySrc: readback のコピー元。
    texDesc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_CopySrc;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.size = {m_width, m_height, 1};
    texDesc.format = m_surfaceFormat;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;

    m_captureTexture = wgpuDeviceCreateTexture(m_device->wgpuDevice(), &texDesc);
    if (!m_captureTexture) {
        return LN_MAKE_ERROR("Failed to create backbuffer capture texture.");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUSwapChain::copyBackbufferToCaptureTexture() {
    if (!m_captureTexture || !m_currentTexture) {
        return;
    }

    WGPUCommandEncoderDescriptor encDesc = WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT;
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_device->wgpuDevice(), &encDesc);

    WGPUTexelCopyTextureInfo src = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
    src.texture = m_currentTexture;
    src.mipLevel = 0;
    src.origin = {0, 0, 0};
    src.aspect = WGPUTextureAspect_All;

    WGPUTexelCopyTextureInfo dst = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
    dst.texture = m_captureTexture;
    dst.mipLevel = 0;
    dst.origin = {0, 0, 0};
    dst.aspect = WGPUTextureAspect_All;

    WGPUExtent3D extent = {m_width, m_height, 1};
    wgpuCommandEncoderCopyTextureToTexture(encoder, &src, &dst, &extent);

    WGPUCommandBufferDescriptor cmdDesc = WGPU_COMMAND_BUFFER_DESCRIPTOR_INIT;
    WGPUCommandBuffer cmd = wgpuCommandEncoderFinish(encoder, &cmdDesc);
    wgpuQueueSubmit(m_device->wgpuQueue(), 1, &cmd);
    wgpuCommandBufferRelease(cmd);
    wgpuCommandEncoderRelease(encoder);
}

#endif // !__EMSCRIPTEN__

} // namespace ln::rhi::webgpu
