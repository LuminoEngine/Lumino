#pragma once
#include <LuminoPlatform/PlatformEvent.hpp>
#include <LuminoGraphics/Common.hpp>
#include "Common.hpp"
#include "UIEvents.hpp"
#include "UIRenderView.hpp"

namespace ln {
namespace detail {
class UIManager;
class DebugInterface;
class ImGuiIntegration;
class MainViewportToolPane;
}
class PlatformWindow;
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
    bool injectMouseMove(float clientX, float clientY, float grabOffsetX, float grabOffsetY);
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
    Flags<ModifierKeys> m_modifierKeys;
};

} // namespace detail

class LN_API UIFrameWindow
	: public UIDomainProvidor
	, public IPlatforEventListener
{
public:
    /** ウィンドウ上へのファイルのドラッグ＆ドロップを許可するかどうかを設定します。 */
    LN_METHOD(Property)
    void setAllowDragDrop(bool value);

    /** ウィンドウ上へのファイルのドラッグ＆ドロップを許可するかどうかを取得します。 */
    LN_METHOD(Property)
    bool isAllowDragDrop() const;

    //void setSize(float width, float height);



	virtual void onDispose(bool explicitDisposing) override;

	void present();



	// TODO: UI ツリーのシステムをちゃんと作っていないが、ひとまず UIViewport を使った動きを作りたいので一時的に設けてある
	//Ref<UIViewport> m_viewport;
	//virtual int getVisualChildrenCount() const override { return 1; }
	//virtual UIElement* getVisualChild(int index) const override;

    // TODO: inernal
	SwapChain* swapChain() const;
	const Ref<DepthBuffer>& depthBuffer() const { return m_depthBuffer; }
    void updateStyleTree();
	void updateLayoutTree();
	const Ref<PlatformWindow>& platformWindow() const { return m_platformWindow; }
    const Ref<UIRenderView>& renderView() const { return m_renderView; }
    //const Ref<GraphicsContext>& graphicsContext() const { return m_graphicsContext; }
    const Ref<detail::UIInputInjector>& inputInjector() const{ return m_inputInjector; }

    void setImGuiLayerEnabled(bool value);

protected:
    void setupPlatformWindow(PlatformWindow* platformMainWindow, const SizeI& backbufferSize);
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
	//virtual void onUpdateLayout(const Rect& finalGlobalRect) override;
	virtual void render(UIRenderingContext* context, const Matrix& parentTransform) override;
    virtual void onRender(UIRenderingContext* context) override;

    // TODO: internal
    void resetSizeFromPlatformWindow();

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
    void init(bool mainWindow = false);    // Swapchain 無し。外部制御用

public:  // TODO: internal
	bool onPlatformEvent(const PlatformEventArgs& e) override;
    void onRoutedEvent(UIEventArgs* e) override;


	//detail::UIManager* m_manager;
	Ref<PlatformWindow>	m_platformWindow;
    Ref<detail::UIInputInjector> m_inputInjector;
    Ref<UILayoutContext> m_layoutContext;
    //Ref<GraphicsContext> m_graphicsContext;
	GraphicsContext* m_renderingGraphicsContext;
	Ref<SwapChain>	m_swapChain;
	Ref<DepthBuffer> m_depthBuffer;
	//Ref<RenderPass> m_renderPass;
	Ref<UIRenderView> m_renderView;
    Size m_clientSize;
    Rect m_contentArea;
	UIFrameWindowUpdateMode m_updateMode;
	std::unique_ptr<detail::ImGuiIntegration> m_imguiContext;
	Ref<detail::DebugInterface> m_debugInterface;

	Event<UIGeneralEventHandler> m_onClosed;
	Event<UIGeneralEventHandler> m_onImGuiLayer;
	bool m_ImGuiLayerEnabled;

    // RedrawEvent で描画しなくなる。
    // ゲームランタイムでは true, エディタでは false.
    bool m_realtimeRenderingEnabled;

private:
    void updateLayoutTreeInternal(const Size& contentSize);
    void invalidate(detail::UIElementDirtyFlags flags, bool toAncestor) override;

    friend class detail::MainViewportToolPane;
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
    //void setClientSize(const Size& value) { UIFrameWindow::resetSize(value); }

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

