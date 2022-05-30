#pragma once
#include <LuminoPlatform//ExternalProxyPlatformWindow.hpp>
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>

namespace ln {
namespace detail {

class EmptyPlatformWindowManager
    : public PlatformWindowManager {
public:
    EmptyPlatformWindowManager(PlatformManager* manager);
    virtual ~EmptyPlatformWindowManager();

    Result init();
    void dispose() override;
    Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow) override;
    void destroyWindow(PlatformWindow* window) override;
    void processSystemEventQueue(EventProcessingMode mode) override;
    OpenGLContext* getOpenGLContext() const override;
};

} // namespace detail
} // namespace ln
