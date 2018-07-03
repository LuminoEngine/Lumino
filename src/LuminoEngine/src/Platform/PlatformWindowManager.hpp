#pragma once
#include <Lumino/Platform/PlatformEvent.hpp>
#include <Lumino/Graphics/GeometryStructs.hpp>

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

class PlatformWindow
	: public RefObject
{
public:
	PlatformWindow();
	virtual ~PlatformWindow() = default;
	virtual void dispose() = 0;

	//virtual void setSize(const SizeI& size) = 0;
	//virtual void setTitleText(const StringRef& title) = 0;

	virtual void getSize(SizeI* size) = 0;

	void attachEventListener(IPlatforEventListener* listener);
	void detachEventListener(IPlatforEventListener* listener);
	bool sendEventToAllListener(const PlatformEventArgs& e);	// return : isHandled

private:
	List<IPlatforEventListener*> m_eventListeners;
};

class PlatformWindowManager
	: public RefObject
{
public:
	PlatformWindowManager();
	virtual ~PlatformWindowManager() = default;

	void initialize();
	virtual void dispose();
	virtual Ref<PlatformWindow> createWindow(const WindowCreationSettings& settings) = 0;
	virtual void destroyWindow(PlatformWindow* window) = 0;
	virtual void processSystemEventQueue() = 0;
};

} // namespace detail
} // namespace ln

