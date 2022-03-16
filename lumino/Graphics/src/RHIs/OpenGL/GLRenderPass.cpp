
#include "Internal.hpp"
#include "OpenGLDeviceContext.hpp"
#include "GLCommandList.hpp"
#include "GLRenderPass.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLRenderPass

GLRenderPass::GLRenderPass() {
}

Result GLRenderPass::init(OpenGLDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    m_device = device;

    //for (auto i = 0; i < buffers.renderTargets.size(); i++) {
    //    m_renderTargets[i] = static_cast<GLTextureBase*>(buffers.renderTargets[i]);
    //}

    //m_depthBuffer = static_cast<GLDepthBuffer*>(buffers.depthBuffer);
    m_clearFlags = clearFlags;
    m_clearColor = clearColor;
    m_clearDepth = clearDepth;
    m_clearStencil = clearStencil;
    return ok();
}

void GLRenderPass::dispose() {
    IRenderPass::dispose();
}

// SizeI GLRenderPass::viewSize() const {
//     return m_renderTargets[0]->realSize();
// }

void GLRenderPass::bind(GLGraphicsContext* context) {
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

    OpenGLHelper::clearBuffers(m_clearFlags, m_clearColor, m_clearDepth, m_clearStencil);
}

} // namespace detail
} // namespace ln
