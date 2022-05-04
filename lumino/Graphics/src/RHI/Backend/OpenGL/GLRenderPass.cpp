﻿#include "OpenGLDeviceContext.hpp"
#include "GLCommandList.hpp"
#include "GLRenderPass.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLRenderPass

GLRenderPass::GLRenderPass()
    : m_device(nullptr)
    , m_clearFlags(ClearFlags::All)
    , m_clearColor()
    , m_clearDepth(1.0f)
    , m_clearStencil(0x00)
    , m_fbo(0) {
}

Result GLRenderPass::init(OpenGLDevice* device, const DeviceFramebufferState& createInfo, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    m_device = device;

    //for (auto i = 0; i < buffers.renderTargets.size(); i++) {
    //    m_renderTargets[i] = static_cast<GLTextureBase*>(buffers.renderTargets[i]);
    //}

    //m_depthBuffer = static_cast<GLDepthBuffer*>(buffers.depthBuffer);
    m_clearFlags = clearFlags;
    m_clearColor = clearColor;
    m_clearDepth = clearDepth;
    m_clearStencil = clearStencil;
    GL_CHECK(glGenFramebuffers(1, &m_fbo));

    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

        GLRenderTargetTexture* backbuffer = static_cast<GLRenderTargetTexture*>(createInfo.renderTargets[0]);
        RHIExtent3D baseSize = backbuffer->extentSize();

        m_viewSize.width = baseSize.width;
        m_viewSize.height = baseSize.height;

        // color buffers
        std::array<GLenum, MaxMultiRenderTargets> buffers;
        int renderTargetsCount = createInfo.renderTargets.size();
        int maxCount = std::min(renderTargetsCount, m_device->caps().MAX_COLOR_ATTACHMENTS);
        // int actualCount = 0;
        for (int i = 0; i < renderTargetsCount; ++i) {
            if (createInfo.renderTargets[i]) {
                LN_CHECK(createInfo.renderTargets[i]->extentSize() == baseSize);
                GLuint id = static_cast<GLTextureBase*>(createInfo.renderTargets[i])->id();
                GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0));
                buffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            else {
                GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0));
                buffers[i] = GL_NONE;
            }
        }

        // depth buffer
        if (createInfo.depthBuffer) {
            LN_CHECK(createInfo.depthBuffer->extentSize() == baseSize);
            GLuint id = static_cast<GLDepthBuffer*>(createInfo.depthBuffer)->id();
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id));
        }
        else {
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
            GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
        }

        LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "glCheckFramebufferStatus failed 0x%08x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

        GL_CHECK(glDrawBuffers(buffers.size(), buffers.data()));

        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    return ok();
}

Result GLRenderPass::initFromNativeFBO(GLuint fbo, int32_t width, int32_t height) {
    // ユーザーが作成した FBO ならば次のコードでサイズが取れるのだが、
    // OpenGL デフォルトの FBO は glGetFramebufferAttachmentParameteriv で失敗する。
    // 一応、デフォルトの FBO がバインドされている場合、glGetIntegerv(GL_FRAMEBUFFER_BINDING) は 0 をを返す。
    // 
    // このため width, height をユーザーコードからもらう必要がある。
    //
    // なお、デフォルト FBO からサイズを取得する方法はサポートされていない。
    // https://stackoverflow.com/questions/29764925/how-to-get-set-the-width-and-height-of-the-default-framebuffer
#if 0   
    // 現在バインドされているフレームバッファ名を取得
    GLint currentFrameBufferName = 0;
    GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBufferName));

    // あとで元に戻すために現在バインドされているレンダーバッファ名を取得
    GLint oldRenderBufferName = 0;
    GL_CHECK(glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRenderBufferName));

    // 現在のフレームバッファにアタッチされているカラーバッファのレンダーバッファ名を取得
    GLint colorBufferName = 0;
    GL_CHECK(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &colorBufferName));

    // レンダーバッファ(カラーバッファ)をバインド
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, colorBufferName));

    // カラーバッファの幅と高さを取得
    GLint width = 0;
    GLint height = 0;
    GL_CHECK(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width));
    GL_CHECK(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height));

    // レンダーバッファのバインドを元に戻す
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, oldRenderBufferName));
#endif
    m_fbo = fbo;
    m_clearFlags = ClearFlags::None;
    m_viewSize.width = width;
    m_viewSize.height = height;
    return ok();
}

void GLRenderPass::dispose() {
    if (m_fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    IRenderPass::dispose();
}

RHIExtent2D GLRenderPass::viewSize() const {
    return m_viewSize;
}

void GLRenderPass::bind(GLGraphicsContext* context) {
#if 0
    auto fbo = context->fbo();
    if (fbo) {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    }

    auto backbuffer = renderTarget(0);
    auto baseSize = backbuffer->extentSize();

    // color buffers
    std::array<GLenum, MaxMultiRenderTargets> buffers;
    int renderTargetsCount = m_renderTargets.size();
    int maxCount = std::min(renderTargetsCount, m_device->caps().MAX_COLOR_ATTACHMENTS);
    // int actualCount = 0;
    for (int i = 0; i < renderTargetsCount; ++i) {
        if (m_renderTargets[i]) {
            LN_CHECK(m_renderTargets[i]->extentSize() == baseSize);
            GLuint id = static_cast<GLTextureBase*>(m_renderTargets[i])->id();
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, id, 0));
            buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        else {
            GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0));
            buffers[i] = GL_NONE;
        }
    }

    // depth buffer
    if (m_depthBuffer) {
        LN_CHECK(m_depthBuffer->extentSize() == baseSize);
        GLuint id = static_cast<GLDepthBuffer*>(m_depthBuffer)->id();
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id));
    }
    else {
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0));
    }

    LN_ENSURE(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "glCheckFramebufferStatus failed 0x%08x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

    GL_CHECK(glDrawBuffers(buffers.size(), buffers.data()));
#endif

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    OpenGLHelper::clearBuffers(m_clearFlags, m_clearColor, m_clearDepth, m_clearStencil);
}

} // namespace detail
} // namespace ln