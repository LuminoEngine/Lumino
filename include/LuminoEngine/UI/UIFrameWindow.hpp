#pragma once

#include "Common.hpp"
#include "UIEvents.hpp"
#include "UIContainerElement.hpp"
#include "../Platform/PlatformEvent.hpp"
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
namespace detail { class PlatformWindow; class UIManager; }
class GraphicsContext;
class SwapChain;
class RenderTargetTexture;
class DepthBuffer;
class UIRenderView;
class UIViewport;
class UIFrameWindow;

namespace detail {
   
class UIInputInjector
    : public RefObject
{
public:
    UIInputInjector(UIFrameWindow* owner);
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

    UIFrameWindow* m_owner;
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
    const Ref<UIRenderView>& renderView() const { return m_renderView; }
    const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }
    const Ref<detail::UIInputInjector>& inputInjector() const{ return m_inputInjector; }

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

    // TODO: internal
    void resetSize(const Size& size);

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
    void init();    // Swapchain 無し。外部制御用
	void init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

protected:  // TODO: internal
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;

	detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
    Ref<detail::UIInputInjector> m_inputInjector;
    Ref<GraphicsContext> m_graphicsContext;
	Ref<SwapChain>	m_swapChain;
	Ref<DepthBuffer> m_depthBuffer;
	Ref<UIRenderView> m_renderView;
    Size m_clientSize;

	Event<UIEventHandler> m_onClosed;
	bool m_autoDisposePlatformWindow;
};

class LN_API UIMainWindow
	: public UIFrameWindow
{
public:

LN_CONSTRUCT_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

private:
};


class LN_API UINativeFrameWindow
    : public UIFrameWindow
{
public:
    void setClientSize(const Size& value) { UIFrameWindow::resetSize(value); }

    void attachRenderingThread(RenderingType renderingType);
    void detachRenderingThread();

    void beginRendering(RenderTargetTexture* renderTarget);
    void renderContents();
    void endRendering();

protected:
	virtual void onDispose(bool explicitDisposing) override;

LN_CONSTRUCT_ACCESS:
    UINativeFrameWindow();
    void init();

private:
    Ref<RenderTargetTexture> m_renderingRenderTarget;
    Ref<DepthBuffer> m_renderingDepthBuffer;
};



} // namespace ln

