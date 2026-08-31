#include <LuminoBase/Logger.hpp>
#if defined(__EMSCRIPTEN__)
#include <emscripten/html5.h>
#else
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
#endif
#include "WebGL2SwapChain.hpp"
#include "WebGL2Device.hpp"
#include "WebGL2CommandBuffer.hpp"

namespace ln::rhi::webgl2 {

VoidResult WebGL2SwapChain::init(WebGL2Device* device, const SwapChainDesc& desc) {
    m_device = device;

    if (auto r = initSurface(desc); !r) {
        return LN_FORWARD_ERROR(r);
    }

    glGenFramebuffers(1, &m_presentFbo);
    if (!m_presentFbo) {
        return LN_MAKE_ERROR("glGenFramebuffers failed.");
    }
    if (auto r = recreateBackbuffer(desc.width, desc.height); !r) {
        return LN_FORWARD_ERROR(r);
    }

    m_commandBuffer = Ref<WebGL2CommandBuffer>::adopt(new WebGL2CommandBuffer());
    if (auto r = m_commandBuffer->init(device); !r) {
        return LN_FORWARD_ERROR(r);
    }

    LN_LOG_INFO("[WebGL2] SwapChain created. (%ux%u)", desc.width, desc.height);
    return LN_MAKE_SUCCESS();
}

#if defined(__EMSCRIPTEN__)

VoidResult WebGL2SwapChain::initSurface(const SwapChainDesc& desc) {
    // WebGL のコンテキストはデバイスの生成時に canvas と結び付いている。ここで別の canvas を
    // 指定されても切り替えられないため、食い違いは警告して device 側の canvas を使う。
    const char* selector = static_cast<const char*>(desc.nativeWindowHandle);
    m_canvasSelector = (selector && selector[0] != '\0') ? selector : m_device->canvasSelector();
    if (m_canvasSelector != m_device->canvasSelector()) {
        LN_LOG_WARNING(
            "[WebGL2] SwapChain canvas '%s' differs from the device canvas '%s'. "
            "A WebGL context cannot be moved between canvases; the device canvas is used.",
            m_canvasSelector.c_str(), m_device->canvasSelector().c_str());
        m_canvasSelector = m_device->canvasSelector();
    }
    return LN_MAKE_SUCCESS();
}

void WebGL2SwapChain::resizeDefaultFramebuffer(uint32_t width, uint32_t height) {
    emscripten_set_canvas_element_size(
        m_canvasSelector.c_str(), static_cast<int>(width), static_cast<int>(height));
}

void WebGL2SwapChain::swapBuffers() {
    // 実際の表示はブラウザが requestAnimationFrame の区切りで行うため、
    // ここから present に相当する呼び出しは無い。
}

void WebGL2SwapChain::finalizeSurface() {
}

#else // !__EMSCRIPTEN__

VoidResult WebGL2SwapChain::initSurface(const SwapChainDesc& desc) {
    auto* glfwWindow = static_cast<GLFWwindow*>(desc.nativeWindowHandle);
    if (!glfwWindow) {
        // ウィンドウ無しのオフスクリーン描画。デバイスの pbuffer をカレントのまま使う。
        LN_LOG_INFO("[WebGL2] SwapChain has no window; rendering offscreen only.");
        return LN_MAKE_SUCCESS();
    }
#ifdef _WIN32
    m_eglSurface = eglCreateWindowSurface(
        m_device->eglDisplay(), m_device->eglConfig(),
        static_cast<EGLNativeWindowType>(glfwGetWin32Window(glfwWindow)), nullptr);
    if (m_eglSurface == EGL_NO_SURFACE) {
        return LN_MAKE_ERROR("eglCreateWindowSurface failed. (0x%04x)",
                             static_cast<unsigned>(eglGetError()));
    }
    if (eglMakeCurrent(m_device->eglDisplay(), m_eglSurface, m_eglSurface,
                       m_device->eglContext()) != EGL_TRUE) {
        return LN_MAKE_ERROR("eglMakeCurrent(window surface) failed. (0x%04x)",
                             static_cast<unsigned>(eglGetError()));
    }
    eglSwapInterval(m_device->eglDisplay(), desc.vsync ? 1 : 0);
    return LN_MAKE_SUCCESS();
#else
    // ponytail: Windows のみ対応。X11 / Wayland が要るようになったら
    // glfwGetX11Window / glfwGetWaylandWindow で分岐を足す。
    return LN_MAKE_ERROR("The desktop WebGL2 (ANGLE) backend supports Windows only.");
#endif
}

void WebGL2SwapChain::resizeDefaultFramebuffer(uint32_t /*width*/, uint32_t /*height*/) {
    // ウィンドウサーフェスのサイズはウィンドウ自体が決めるため、ここでは何もしない。
}

void WebGL2SwapChain::swapBuffers() {
    if (m_eglSurface != EGL_NO_SURFACE) {
        eglSwapBuffers(m_device->eglDisplay(), m_eglSurface);
    }
}

void WebGL2SwapChain::finalizeSurface() {
    if (m_eglSurface != EGL_NO_SURFACE) {
        // コンテキストを pbuffer へ戻してからサーフェスを破棄する。
        // 破棄後もデバイスの GL リソースの解放が続くため、カレントを外してはいけない。
        m_device->makeDefaultSurfaceCurrent();
        eglDestroySurface(m_device->eglDisplay(), m_eglSurface);
        m_eglSurface = EGL_NO_SURFACE;
    }
}

#endif // __EMSCRIPTEN__

VoidResult WebGL2SwapChain::recreateBackbuffer(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    TextureDesc texDesc;
    texDesc.width = width;
    texDesc.height = height;
    texDesc.format = TextureFormat::RGBA8Unorm;
    texDesc.usage = TextureUsage::RenderTarget | TextureUsage::Sampled | TextureUsage::CopySrc;

    auto tex = Ref<WebGL2Texture>::adopt(new WebGL2Texture());
    if (auto r = tex->init(m_device, texDesc); !r) {
        return LN_FORWARD_ERROR(r);
    }
    m_colorTexture = std::move(tex);

    m_colorView = Ref<WebGL2TextureView>::adopt(new WebGL2TextureView());
    m_colorView->init(m_colorTexture.get());

    // present の blit 元。カラーテクスチャを作り直すたびに張り替える。
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_presentFbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, m_colorTexture->handle(), 0);
    const GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        return LN_MAKE_ERROR("Present framebuffer is incomplete. (0x%04x)", static_cast<unsigned>(status));
    }

    // 既定のフレームバッファの解像度を合わせる。
    resizeDefaultFramebuffer(width, height);
    return LN_MAKE_SUCCESS();
}

TextureView* WebGL2SwapChain::acquireNextTexture() {
    return m_colorView.get();
}

void WebGL2SwapChain::present() {
    // 通常は Renderer::endFrame が先に submit しているため、ここでの submit は空振りになる。
    // RHI を直接使うコード (examples) のために残してある。
    m_commandBuffer->submit();

    // オフスクリーンのバックバッファを既定のフレームバッファへ転送する。
    // 転送先の y を逆順に指定して上下を反転する。
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_presentFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // 直前のパイプラインが残したシザーとカラーマスクを戻してから転送する。
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBlitFramebuffer(
        0, 0, static_cast<GLint>(m_width), static_cast<GLint>(m_height),
        0, static_cast<GLint>(m_height), static_cast<GLint>(m_width), 0,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    swapBuffers();
}

CommandBuffer* WebGL2SwapChain::getCurrentCommandBuffer() {
    return m_commandBuffer.get();
}

VoidResult WebGL2SwapChain::resize(uint32_t width, uint32_t height) {
    if (width == m_width && height == m_height) {
        return LN_MAKE_SUCCESS();
    }
    auto r = recreateBackbuffer(width, height);
    if (!r) return LN_FORWARD_ERROR(r);
    LN_LOG_INFO("[WebGL2] SwapChain resized to %ux%u", width, height);
    return LN_MAKE_SUCCESS();
}

void WebGL2SwapChain::finalize() {
    m_commandBuffer = nullptr;
    m_colorView = nullptr;
    m_colorTexture = nullptr;
    if (m_presentFbo) {
        if (m_device && m_device->isContextCurrent()) glDeleteFramebuffers(1, &m_presentFbo);
        m_presentFbo = 0;
    }
    finalizeSurface();
    SwapChain::finalize();
}

} // namespace ln::rhi::webgl2
