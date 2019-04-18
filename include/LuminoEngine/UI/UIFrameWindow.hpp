#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "UIContainerElement.hpp"
#include "../Platform/PlatformEvent.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
namespace detail { class PlatformWindow; class UIManager; }
class SwapChain;
class UIRenderView;
class UIViewport;

namespace detail {
   
class UIInputInjector
    : public RefObject
{
public:
    UIInputInjector(UIElement* owner);
    bool injectMouseMove(float clientX, float clientY);
    bool injectMouseButtonDown(MouseButtons button);
    bool injectMouseButtonUp(MouseButtons button);
    bool injectMouseWheel(int delta);
    bool injectKeyDown(Keys keyCode, ModifierKeys modifierKeys);
    bool injectKeyUp(Keys keyCode, ModifierKeys modifierKeys);
    bool injectTextInput(Char ch);

private:
    void updateMouseHover(float clientX, float clientY);
    UIElement* capturedElement();
    UIElement* forcusedElement();
    UIElement* mouseHoveredElement();

    struct MouseClickTracker
    {
        double lastTime = 0.0;
        int clickCount = 0;
        UIElement* hoverElement = nullptr;
    };

    UIElement* m_owner;
    Point m_mousePosition;
    MouseClickTracker m_mouseClickTrackers[8];
};

} // namespace detail

class LN_API UIFrameWindow
	: public UIContainerElement
	, public detail::IPlatforEventListener
{
public:

	virtual void onDispose(bool explicitDisposing) override;

	void renderContents();
	void present();



	// TODO: UI ツリーのシステムをちゃんと作っていないが、ひとまず UIViewport を使った動きを作りたいので一時的に設けてある
	//Ref<UIViewport> m_viewport;
	//virtual int getVisualChildrenCount() const override { return 1; }
	//virtual UIElement* getVisualChild(int index) const override;

    // TODO: inernal
	SwapChain* swapChain() const;
	const Ref<DepthBuffer>& depthBuffer() const { return m_depthBuffer; }
    //void updateLayout();
	void updateLayoutTree();
	const Ref<detail::PlatformWindow>& platformWindow() const { return m_platformWindow; }

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

private:
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
    Ref<detail::UIInputInjector> m_inputInjector;
	Ref<SwapChain>	m_swapChain;
	Ref<DepthBuffer> m_depthBuffer;
	Ref<UIRenderView> m_renderView;

	Event<UIEventHandler> m_onClosed;
	bool m_autoDisposePlatformWindow;
};


//class LN_API UIMainWindow
//	: public UIFrameWindow
//{
//public:
//
//LN_CONSTRUCT_ACCESS :
//	UIMainWindow();
//	virtual ~UIMainWindow();
//	void init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);
//
//private:
//};

} // namespace ln

