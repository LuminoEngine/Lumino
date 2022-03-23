#pragma once
#ifdef LN_GLFW
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>
#include <LuminoPlatform/detail/GLFWPlatformWindow.hpp>
#include <LuminoPlatform/detail/OpenGLContext.hpp>

namespace ln {
namespace detail {

class GLFWPlatformWindowManager
    : public PlatformWindowManager {
public:
    GLFWPlatformWindowManager(PlatformManager* manager);
    virtual ~GLFWPlatformWindowManager();

    Result init(bool withOpenGLAPI);
    void dispose() override;
    Ref<PlatformWindow> createMainWindow(const WindowCreationSettings& settings) override;
    Ref<PlatformWindow> createSubWindow(const WindowCreationSettings& settings) override;
    void destroyWindow(PlatformWindow* window) override;
    void processSystemEventQueue(EventProcessingMode mode) override;
    OpenGLContext* getOpenGLContext() const override;
    bool withOpenGLAPI() const { return m_withOpenGLAPI; }

private:
    Ref<GLFWContext> m_glContext;
    bool m_withOpenGLAPI;
};

} // namespace detail
} // namespace ln

#endif
