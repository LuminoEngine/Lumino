#include <LuminoBase/Logger.hpp>
#include <emscripten/html5.h>
#include "WebGL2SwapChain.hpp"
#include "WebGL2Device.hpp"
#include "WebGL2CommandBuffer.hpp"

namespace ln::rhi::webgl2 {

VoidResult WebGL2SwapChain::init(WebGL2Device* device, const SwapChainDesc& desc) {
    m_device = device;

    // WebGL のコンテキストはデバイスの生成時に canvas と結び付いている。ここで別の canvas を
    // 指定されても切り替えられないため、食い違いは警告して device 側の canvas を使う。
    const char* selector = static_cast<const char*>(desc.nativeWindowHandle);
    m_canvasSelector = (selector && selector[0] != '\0') ? selector : device->canvasSelector();
    if (m_canvasSelector != device->canvasSelector()) {
        LN_LOG_WARNING(
            "[WebGL2] SwapChain canvas '%s' differs from the device canvas '%s'. "
            "A WebGL context cannot be moved between canvases; the device canvas is used.",
            m_canvasSelector.c_str(), device->canvasSelector().c_str());
        m_canvasSelector = device->canvasSelector();
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

    LN_LOG_INFO("[WebGL2] SwapChain created. (canvas: %s, %ux%u)",
                m_canvasSelector.c_str(), desc.width, desc.height);
    return LN_MAKE_SUCCESS();
}

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
    emscripten_set_canvas_element_size(
        m_canvasSelector.c_str(), static_cast<int>(width), static_cast<int>(height));
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

    // 実際の表示はブラウザが requestAnimationFrame の区切りで行うため、
    // ここから present に相当する呼び出しは無い。
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
        glDeleteFramebuffers(1, &m_presentFbo);
        m_presentFbo = 0;
    }
    SwapChain::finalize();
}

} // namespace ln::rhi::webgl2
