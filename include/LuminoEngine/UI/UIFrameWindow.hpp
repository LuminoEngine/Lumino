﻿#pragma once
#include "../Platform/PlatformEvent.hpp"
#include "../Graphics/Common.hpp"
#include "../Graphics/GeometryStructs.hpp"
#include "Common.hpp"
#include "UIEvents.hpp"
#include "UIContainerElement.hpp"
#include "ImGuiIntegration.hpp"

namespace ln {
namespace detail {
class PlatformWindow;
class UIManager;
class DebugInterface;
}
class RenderPass;
class GraphicsContext;
class SwapChain;
class RenderTargetTexture;
class DepthBuffer;
class UIRenderView;
class UIViewport;
class UIFrameWindow;

enum class UIFrameWindowUpdateMode
{
	Polling,
	EventDispatches,
};

namespace detail {
   
// マウスキャプチャは inject する側でとっておくこと。UI モジュール内ではキャプチャの管理は行わない。
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
    void updateMouseHover(const Point& clientPt);
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
    MouseButtons m_pressedButton;
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
    //const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }
    const Ref<detail::UIInputInjector>& inputInjector() const{ return m_inputInjector; }
	void setupPlatformWindow(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

	void setImGuiLayerEnabled(bool value) { m_ImGuiLayerEnabled = value; }

protected:
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize) override;
	//virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
	//virtual void onUpdateLayout(const Rect& finalGlobalRect) override;
	virtual void render(UIRenderingContext* context, const Matrix& parentTransform) override;
    virtual void onRender(UIRenderingContext* context) override;

    // TODO: internal
    void resetSize(const Size& size);

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
    void init();    // Swapchain 無し。外部制御用

public:  // TODO: internal
	virtual bool onPlatformEvent(const detail::PlatformEventArgs& e) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

	//detail::UIManager* m_manager;
	Ref<detail::PlatformWindow>	m_platformWindow;
    Ref<detail::UIInputInjector> m_inputInjector;
    Ref<UILayoutContext> m_layoutContext;
    //Ref<GraphicsContext> m_graphicsContext;
	GraphicsContext* m_renderingGraphicsContext;
	Ref<SwapChain>	m_swapChain;
	Ref<DepthBuffer> m_depthBuffer;
	//Ref<RenderPass> m_renderPass;
	Ref<UIRenderView> m_renderView;
    Size m_clientSize;
	UIFrameWindowUpdateMode m_updateMode;
	detail::ImGuiContext m_imguiContext;
	Ref<detail::DebugInterface> m_debugInterface;

	Event<UIEventHandler> m_onClosed;
	Event<UIEventHandler> m_onImGuiLayer;
	bool m_autoDisposePlatformWindow;
	bool m_ImGuiLayerEnabled;

private:
    virtual void invalidate(detail::UIElementDirtyFlags flags, bool toAncestor);
};

class LN_API UIMainWindow
	: public UIFrameWindow
{
public:

LN_CONSTRUCT_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void init();	// for Editor
	//void init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize);

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
	// TODO: ↓不要かも
    Ref<RenderTargetTexture> m_renderingRenderTarget;
    Ref<DepthBuffer> m_renderingDepthBuffer;
};



} // namespace ln

