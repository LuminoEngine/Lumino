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
    };

    static Ref<OpenGLGraphicsContext> create(const Settings& settings);

    detail::IGraphicsDevice* rhiDevice() const override;

private:
    OpenGLGraphicsContext();
    ~OpenGLGraphicsContext() override;
    bool init(const Settings& settings);
    void onDispose(bool explicitDisposing) override;

    Ref<detail::OpenGLDevice> m_device;
};

} // namespace ln
