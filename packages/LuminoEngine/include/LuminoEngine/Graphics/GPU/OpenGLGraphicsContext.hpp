// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "SwapChain.hpp"

namespace ln {

/**
 */
class OpenGLGraphicsContext : public GraphicsContext {
public:
    struct Settings {
        PlatformWindow* window = nullptr; // nullptr の場合、OpenGL Context の生成・管理を内部で行わない
        uint32_t defaultFramebuffer = 0;
        int32_t width = 0;
        int32_t height = 0;
    };

    static Ref<OpenGLGraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

    /** Lumino 外部で作成された OpenGL Context を使用しているかどうかを取得します。 */
    bool isUseExternalGLContext() const;

private:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;
    void onCreateRHIObjects() override;

    Ref<detail::OpenGLDevice> m_device;
    PlatformWindow* m_window;
    Ref<RenderTargetTexture> m_wrapedRenderTarget;
    Ref<RenderPass> m_wrapedRenderPass;
    //Ref<detail::GLRenderTargetTexture> m_wrapedRHIRenderTarget;
    //Ref<detail::GLRenderPass> m_wrapedRHIRenderPass;
    uint32_t m_externalDefaultFBO;
    int32_t m_externalWidth;
    int32_t m_externalHeight;
};

} // namespace ln
