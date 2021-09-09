#pragma once
#include <LuminoPlatform/Common.hpp>
#include <LuminoPlatform/PlatformEvent.hpp>
#include <LuminoPlatform/PlatformWindow.hpp>

namespace ln {
namespace detail {

struct WindowCreationSettings
{
	String		title;							// ウィンドウタイトル
	SizeI		clientSize = SizeI(640, 480);	// クライアント領域のピクセルサイズ
	bool		fullscreen = false;				// フルスクリーンモードで作成するかどうか
	bool		resizable = true;				// 可変ウィンドウとして作成するかどうか

    intptr_t	userWindow = 0;
    uint32_t    win32IconResourceId = 0;
};

enum class EventProcessingMode
{
    Polling,
    Wait,       // イベントが発生するまで待つ。 新しい入力を受け取ったときにだけレンダリングの更新が必要な場合につかう。Editor 用。
};

class PlatformWindowManager
	: public RefObject
{
public:
	PlatformWindowManager(PlatformManager* manager);
	virtual ~PlatformWindowManager() = default;

	PlatformManager* manager() const { return m_manager; }

	virtual void dispose() = 0;
	virtual Ref<PlatformWindow> createMainWindow(const WindowCreationSettings& settings) = 0;
	virtual Ref<PlatformWindow> createSubWindow(const WindowCreationSettings& settings) = 0;
	virtual void destroyWindow(PlatformWindow* window) = 0;
	virtual void processSystemEventQueue(EventProcessingMode mode) = 0;
	virtual OpenGLContext* getOpenGLContext() const = 0;

private:
	PlatformManager* m_manager;
};

} // namespace detail
} // namespace ln

