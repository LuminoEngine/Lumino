#pragma once
#include "GLHelper.hpp"
#include "GLTextures.hpp"
#include "GLDepthBuffer.hpp"

namespace ln {
namespace detail {

class GLRenderPass : public IRenderPass {
public:
    GLRenderPass();
    Result init(OpenGLDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
    Result initFromNativeFBO(GLuint fbo, int32_t width, int32_t height);
    void dispose() override;
    RHIExtent2D viewSize() const override;

    //GLRenderTargetTexture* renderTargetx(int index) const { return static_cast<GLRenderTargetTexture*>(m_renderTargets[index]); }
    //GLDepthBuffer* depthBuffer() const { return static_cast<GLDepthBuffer*>(m_depthBuffer); }

    //SizeI viewSize() const;
    void bind(GLGraphicsContext* context);

private:
    OpenGLDevice* m_device;
    //std::array<Ref<GLTextureBase>, MaxMultiRenderTargets> m_renderTargets;
    //Ref<GLDepthBuffer> m_depthBuffer;
    ClearFlags m_clearFlags;
    Color m_clearColor;
    float m_clearDepth;
    uint8_t m_clearStencil;
    GLuint m_fbo;
    RHIExtent2D m_viewSize;
};

} // namespace detail
} // namespace ln
