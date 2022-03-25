#pragma once
#include <LuminoPlatform/Common.hpp>
#include <LuminoPlatform/PlatformEvent.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>

namespace ln {
namespace detail {

enum class EventProcessingMode {
    Polling,
    Wait, // イベントが発生するまで待つ。 新しい入力を受け取ったときにだけレンダリングの更新が必要な場合につかう。Editor 用。
};

class PlatformWindowManager
    : public RefObject {
public:
    PlatformWindowManager(PlatformManager* manager);
    virtual ~PlatformWindowManager() = default;

    PlatformManager* manager() const { return m_manager; }

    virtual void dispose() = 0;
    virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings, PlatformWindow* mainWindow) = 0;
    virtual void destroyWindow(PlatformWindow* window) = 0;
    virtual void processSystemEventQueue(EventProcessingMode mode) = 0;
    virtual OpenGLContext* getOpenGLContext() const = 0;

private:
    PlatformManager* m_manager;
};

} // namespace detail
} // namespace ln
