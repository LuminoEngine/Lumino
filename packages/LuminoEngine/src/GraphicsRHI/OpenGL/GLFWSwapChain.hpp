#pragma once
#ifdef LN_GLFW
#include <LuminoEngine/Platform/detail/GLFWPlatformWindow.hpp>
#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {
class GLFWPlatformWindow;

class GLFWSwapChain : public GLSwapChain {
public:
    GLFWSwapChain(OpenGLDevice* device);
    virtual ~GLFWSwapChain() = default;
    bool init(PlatformWindow* window, const SizeI& backbufferSize);

    GLFWPlatformWindow* window() const { return m_window; }

    virtual void getBackendBufferSize(SizeI* outSize) override;
    void swap() override;

    void beginMakeContext() override;
    void endMakeContext() override;

private:
    GLFWPlatformWindow* m_window;
};

} // namespace detail
} // namespace ln

#endif
