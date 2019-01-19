#pragma once
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>

namespace ln {
namespace detail {

struct WindowCreationSettings
{
	String		title;							// ウィンドウタイトル
	SizeI		clientSize = SizeI(640, 480);	// クライアント領域のピクセルサイズ
	bool		fullscreen = false;				// フルスクリーンモードで作成するかどうか
	bool		resizable = true;				// 可変ウィンドウとして作成するかどうか
	intptr_t	userWindow = 0;
};

class PlatformWindowManager
	: public RefObject
{
public:
	PlatformWindowManager();
	virtual ~PlatformWindowManager() = default;

	void init();
	virtual void dispose();
	virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings) = 0;
	virtual void destroyWindow(PlatformWindow* window) = 0;
	virtual void processSystemEventQueue() = 0;
};

} // namespace detail
} // namespace ln

