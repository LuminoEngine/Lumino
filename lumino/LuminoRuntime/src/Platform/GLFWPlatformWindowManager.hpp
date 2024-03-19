#pragma once
#ifdef LN_GLFW
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include <LuminoEngine/Platform/detail/GLFWPlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/OpenGLContext.hpp>

namespace ln {
namespace detail {

class GLFWPlatformWindowManager
    : public PlatformWindowManager {
public:
    GLFWPlatformWindowManager(PlatformManager* manager);
    virtual ~GLFWPlatformWindowManager();

    Result<> init(bool withOpenGLAPI);
    void dispose() override;
    Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow) override;
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
