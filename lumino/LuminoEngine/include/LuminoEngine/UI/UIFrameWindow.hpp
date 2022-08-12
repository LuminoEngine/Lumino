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
class SwapChain;
class RenderTargetTexture;
class DepthBuffer;
class UIRenderView;
class UIViewport;
class UIFrameWindow;
class ImGuiDockManager;

enum class UIFrameWindowUpdateMode {
    Polling,
    EventDispatches,
};

namespace detail {

// マウスキャプチャは inject する側でとっておくこと。UI モジュール内ではキャプチャの管理は行わない。
class UIInputInjector
    : public RefObject {
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

    struct MouseClickTracker {
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
    , public IPlatforEventListener {
public:
    /** ウィンドウ上へのファイルのドラッグ＆ドロップを許可するかどうかを設定します。 */
    LN_METHOD(Property)
    void setAllowDragDrop(bool value);

    /** ウィンドウ上へのファイルのドラッグ＆ドロップを許可するかどうかを取得します。 */
    LN_METHOD(Property)
    bool isAllowDragDrop() const;

    //void setImGuiContext(UIImGuiContext* context);

    //void setSize(float width, float height);

    ImGuiDockManager* dockManager() const;



    virtual void onDispose(bool explicitDisposing) override;


    // TODO: UI ツリーのシステムをちゃんと作っていないが、ひとまず UIViewport を使った動きを作りたいので一時的に設けてある
    //Ref<UIViewport> m_viewport;
    //virtual int getVisualChildrenCount() const override { return 1; }
    //virtual UIElement* getVisualChild(int index) const override;

    // TODO: inernal
    void renderContents(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget);
    void present();
    void updateLayoutIfNeeded();

    SwapChain* swapChain() const;
    const Ref<DepthBuffer>& depthBuffer() const { return m_depthBuffer; }
    const Ref<PlatformWindow>& platformWindow() const { return m_platformWindow; }
    const Ref<UIRenderView>& renderView() const { return m_renderView; }
    const Ref<detail::UIInputInjector>& inputInjector() const { return m_inputInjector; }

    void setImGuiLayerEnabled(bool value);

    virtual ~UIFrameWindow();

protected:
    /**
     * 
     */
    virtual void onInit();

    /**
     * ImGui の表示処理を実装します。
     *
     * この関数は ImGui::NewFrame() と ImGui::EndFrame() の間で呼び出されます。
     * onImGui() の実装の中でこれらを呼び出す必要はありません。
     */
    virtual void onImGui();

    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) override;
    //virtual void onUpdateLayout(const Rect& finalGlobalRect) override;
    virtual void render(UIRenderingContext* context, const Matrix& parentTransform) override;
    virtual void onRender(UIRenderingContext* context) override;

    // TODO: internal
    void resetSizeFromPlatformWindow();

protected:
    struct InitInfo {
        PlatformWindow* platformWindow;
        bool createSwapChain;
    };

    UIFrameWindow();
    Result init(const InitInfo& initInfo);

public: // TODO: internal
    bool onPlatformEvent(const PlatformEventArgs& e) override;
    void onRoutedEvent(UIEventArgs* e) override;

    //detail::UIManager* m_manager;
    Ref<PlatformWindow> m_platformWindow;
    Ref<detail::UIInputInjector> m_inputInjector;
    Ref<UILayoutContext> m_layoutContext;
    GraphicsCommandList* m_renderingGraphicsContext;
    Ref<SwapChain> m_swapChain;
    Ref<DepthBuffer> m_depthBuffer;
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
    void setupPlatformWindow(PlatformWindow* platformMainWindow, bool useExternalSwapChain);
    void updateStyleTree();
    void updateLayoutTree();
    void updateLayoutTreeInternal(const Size& contentSize);
    void invalidate(detail::UIElementDirtyFlags flags, bool toAncestor) override;

    friend class detail::MainViewportToolPane;
    friend class detail::ImGuiIntegration;
};

class LN_API UIMainWindow
    : public UIFrameWindow {
public:
LN_CONSTRUCT_ACCESS:
    UIMainWindow();
    virtual ~UIMainWindow();
    Result init(bool useExternalSwapChain = false); // for Editor

private:
};

#if 0
class LN_API UINativeFrameWindow
    : public UIFrameWindow {
public:
    //void setClientSize(const Size& value) { UIFrameWindow::resetSize(value); }

    void attachRenderingThread(/*RenderingType renderingType*/);
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
#endif

} // namespace ln
