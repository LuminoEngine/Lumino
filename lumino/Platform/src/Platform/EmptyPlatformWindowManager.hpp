#pragma once
#include <LuminoPlatform/detail/PlatformWindowManager.hpp>

namespace ln {
namespace detail {

class EmptyPlatformWindow
    : public PlatformWindow {
public:
    EmptyPlatformWindow();
    Result init(const WindowCreationSettings& settings);
    void setWindowTitle(const String& title) override {}
    void getSize(SizeI* size) override { *size = SizeI(); }
    void setSize(const SizeI& size) override { LN_NOTIMPLEMENTED(); }
    void getFramebufferSize(int* width, int* height) override {
        *width = 0;
        *height = 0;
    }
    void setAllowDragDrop(bool value) override;
    bool isAllowDragDrop() const override;
    PointI pointFromScreen(const PointI& screenPoint) override { return screenPoint; }
    PointI pointToScreen(const PointI& clientPoint) override { return clientPoint; }
    void grabCursor() override {}
    void releaseCursor() override {}

private:
};

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
