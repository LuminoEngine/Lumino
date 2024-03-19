
#include "../../LuminoRuntime/src/GraphicsRHI/OpenGL/GLHelper.hpp"
#include "../../LuminoRuntime/src/GraphicsRHI/OpenGL/OpenGLDeviceContext.hpp"
#include "../../LuminoRuntime/src/GraphicsRHI/OpenGL/GLTextures.hpp"
#include "../../LuminoRuntime/src/GraphicsRHI/OpenGL/GLRenderPass.hpp"
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/GPU/GraphicsExtensionOpenGL.hpp>

namespace ln {

GraphicsCommandList* OpenGLIntegration::getCommandListFromCurrentContext() {
    auto* manager = detail::GraphicsManager::instance();
    return manager->getOpenGLIntegrationCommandList();
}

RenderPass* OpenGLIntegration::getRenderPass(GLuint fbo, int32_t width, int32_t height) {

    // TODO: これは dispose のタイミングが NG なので Manager とかに持たせないとダメ
    static Ref<RenderPass> cachedRenderPass;
    if (!cachedRenderPass) {
        auto rhiRenderTarget = makeRef<detail::GLRenderTargetTexture>();
        if (!rhiRenderTarget->init(0, width, height)) {
            return nullptr;
        }

        // GLint fbo;
        // GL_CHECK(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &fbo));

        // glGetFramebufferAttachmentParameteriv

        // GL_CHECK(glBindTexture(GL_TEXTURE_2D, renderTargetId));
        // GLint width = 0;
        // GLint height = 0;
        // GL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width));
        // GL_CHECK(glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height));

        auto rhiRenderPass = makeRef<detail::GLRenderPass>();
        if (!rhiRenderPass->initFromNativeFBO(fbo, rhiRenderTarget)) {
            return nullptr;
        }

        cachedRenderPass = makeObject_deprecated<RenderPass>(rhiRenderPass);
    }

    return cachedRenderPass;
}


} // namespace ln
