#pragma once
#include "GLHelper.hpp"
#include "GLTextures.hpp"
#include "GLDepthBuffer.hpp"

namespace ln {
namespace detail {

class GLRenderPass
    : public IRenderPass {
public:
    GLRenderPass();
    Result init(OpenGLDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
    void dispose() override;

    GLRenderTargetTexture* renderTarget(int index) const { return static_cast<GLRenderTargetTexture*>(m_renderTargets[index]); }
    GLDepthBuffer* depthBuffer() const { return static_cast<GLDepthBuffer*>(m_depthBuffer); }

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
};

} // namespace detail
} // namespace ln
