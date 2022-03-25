
#include "GLHelper.hpp"
#include "OpenGLDeviceContext.hpp"
//#include "RHIs/OpenGL/GLTextures.hpp"
#include "GLRenderPass.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include "GraphicsManager.hpp"
#include <LuminoGraphics/RHI/GraphicsExtensionOpenGL.hpp>

namespace ln {

GraphicsCommandList* OpenGLIntegration::getCommandListFromCurrentContext() {
    auto* manager = detail::GraphicsManager::instance();
    return manager->getOpenGLIntegrationCommandList();
}

RenderPass* OpenGLIntegration::getRenderPass(GLuint fbo, int32_t width, int32_t height) {

    static Ref<RenderPass> ppp;
    if (!ppp) {

        //auto renderTarget = makeObject<RenderTargetTexture>(width, height);
        //detail::TextureInternal::resetNativeObject(renderTarget, 0);

        // GLint fbo;
        // GL_CHECK(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fbo));

        // glGetFramebufferAttachmentParameteriv

        // GL_CHECK(glBindTexture(GL_TEXTURE_2D, renderTargetId));
        // GLint width = 0;
        // GLint height = 0;
        // GL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
        // GL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height));

        auto rhiRenderPass = makeRef<detail::GLRenderPass>();
        if (!rhiRenderPass->initFromNativeFBO(fbo, width, height)) {
            return nullptr;
        }

        //rhiRenderPass->m_renderTargets[0] = renderTarget->reso

        ppp = makeObject<RenderPass>(rhiRenderPass);
    }



    return ppp;

}


} // namespace ln
