
#include "Internal.hpp"
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/SamplerState.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/Engine/Debug.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include "../Engine/EngineManager.hpp"
#include "UIStyleInstance.hpp"
#include "UIManager.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>

#include "../Effect/EffectManager.hpp" // TODO: tests

namespace ln {

namespace detail {

//==============================================================================
// UIInputInjector

UIInputInjector::UIInputInjector(UIFrameWindow* owner)
    : m_owner(owner)
    , m_pressedButton(MouseButtons::None)
    , m_modifierKeys(ModifierKeys::None) {
}

bool UIInputInjector::injectMouseMove(float clientX, float clientY, float grabOffsetX, float grabOffsetY) {
    m_mousePosition.set(clientX, clientY);

    // キャプチャ中のコントロールがあればそちらに送る
    UIElement* sender = capturedElement();

    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseMoveEvent, m_pressedButton, clientX, clientY, 0, m_modifierKeys, true);
        args->grabOffsetX = grabOffsetX;
        args->grabOffsetY = grabOffsetY;
        sender->raiseEvent(args);
        return args->handled;
    }

    updateMouseHover(m_mousePosition);

    sender = mouseHoveredElement();
    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseMoveEvent, m_pressedButton, clientX, clientY, 0, m_modifierKeys, true);
        args->grabOffsetX = grabOffsetX;
        args->grabOffsetY = grabOffsetY;
        sender->raiseEvent(args);
        return args->handled;
    }

    return false;
}

bool UIInputInjector::injectMouseButtonDown(MouseButtons button) {
    m_pressedButton = button;

    // マウスクリック回数の処理
    MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];
    tracker.clickCount++;

    double curTime = 0.001 * Environment::getTickCount();
    float elapsed = (float)(curTime - tracker.lastTime);
    if (elapsed > detail::UIManager::MouseButtonClickTimeout ||
        mouseHoveredElement() != tracker.hoverElement ||
        tracker.clickCount > 3) {
        tracker.clickCount = 1;
        tracker.hoverElement = mouseHoveredElement();
    }
    tracker.lastTime = curTime;

    // キャプチャ中のコントロールがあればそちらに送る
    UIElement* sender = capturedElement();
    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseDownEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, m_modifierKeys, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    // イベント中に UIElement が削除されることに備え、毎回検索する
    updateMouseHover(m_mousePosition);

    sender = mouseHoveredElement();
    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseDownEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, m_modifierKeys, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    return false;
}

bool UIInputInjector::injectMouseButtonUp(MouseButtons button) {
    m_pressedButton = MouseButtons::None;

    MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];

    // キャプチャ中のUI要素があればそちらに送る
    UIElement* sender = capturedElement();
    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseUpEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, m_modifierKeys, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    // イベント中に UIElement が削除されることに備え、毎回検索する
    updateMouseHover(m_mousePosition);

    // マウス位置にUI要素があればそちらに送る
    sender = mouseHoveredElement();
    if (sender) {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseUpEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, m_modifierKeys, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectMouseWheel(int delta) {
    // キャプチャ中のUI要素があればそちらに送る
    UIElement* sender = capturedElement();
    if (sender) {
        auto args = UIMouseWheelEventArgs::create(sender, UIEvents::MouseWheelEvent, delta, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    // イベント中に UIElement が削除されることに備え、毎回検索する
    updateMouseHover(m_mousePosition);

    // マウス位置にUI要素があればそちらに送る
    sender = mouseHoveredElement();
    if (sender) {
        auto args = UIMouseWheelEventArgs::create(sender, UIEvents::MouseWheelEvent, delta, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectKeyDown(Keys keyCode, ModifierKeys modifierKeys) {
    if (keyCode == Keys::LAlt || keyCode == Keys::RAlt)
        m_modifierKeys.set(ModifierKeys::Alt);
    else if (keyCode == Keys::LShift || keyCode == Keys::RShift)
        m_modifierKeys.set(ModifierKeys::Shift);
    else if (keyCode == Keys::LCtrl || keyCode == Keys::RCtrl)
        m_modifierKeys.set(ModifierKeys::Control);

    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender) {
        auto args = UIKeyEventArgs::create(sender, UIEvents::KeyDownEvent, keyCode, modifierKeys, 0, true);
        sender->raiseEvent(args);
        if (args->handled) {
            return true;
        }
    }

    if (m_owner) {
        auto args = UIKeyEventArgs::create(m_owner, UIEvents::KeyDownEvent, keyCode, modifierKeys, 0, true);
        m_owner->raiseEvent(args);
        if (args->handled) {
            return true;
        }
    }

    return false;
}

bool UIInputInjector::injectKeyUp(Keys keyCode, ModifierKeys modifierKeys) {
    if (keyCode == Keys::LAlt || keyCode == Keys::RAlt)
        m_modifierKeys.unset(ModifierKeys::Alt);
    else if (keyCode == Keys::LShift || keyCode == Keys::RShift)
        m_modifierKeys.unset(ModifierKeys::Shift);
    else if (keyCode == Keys::LCtrl || keyCode == Keys::RCtrl)
        m_modifierKeys.unset(ModifierKeys::Control);

    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender) {
        auto args = UIKeyEventArgs::create(sender, UIEvents::KeyUpEvent, keyCode, modifierKeys, 0, true);
        sender->raiseEvent(args);
        if (args->handled) {
            return true;
        }
    }

    if (m_owner) {
        auto args = UIKeyEventArgs::create(m_owner, UIEvents::KeyUpEvent, keyCode, modifierKeys, 0, true);
        m_owner->raiseEvent(args);
        if (args->handled) {
            return true;
        }
    }

    return false;
}

bool UIInputInjector::injectTextInput(Char ch) {
    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender) {
        auto args = UIKeyEventArgs::create(sender, UIEvents::TextInputEvent, Keys::Unknown, ModifierKeys::None, ch, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

void UIInputInjector::updateMouseHover(const Point& clientPt) {
    m_owner->m_manager->updateMouseHover(m_owner->renderView(), clientPt);
}

UIElement* UIInputInjector::capturedElement() {
    return m_owner->m_manager->capturedElement();
}

UIElement* UIInputInjector::forcusedElement() {
    return m_owner->m_manager->forcusedElement();
}

UIElement* UIInputInjector::mouseHoveredElement() {
    return m_owner->m_manager->mouseHoverElement();
}

} //  namespace detail

//==============================================================================
// UIFrameWindow

/*
    UIFrameWindow はネイティブウィンドウと一致する一番外側の UIElement で、
    サイズはネイティブウィンドウのクライアントサイズと一致する。
    こうしておかないと非常にイメージがしづらくなる。

    ImGui 有効時は、MainViewportToolPane の ContentRect が UIFrameWindow の ContentRect となる。
    これは Padding を設けて UIFrameWindow 以下の UITree を Arrange するのと等しい。
*/

UIFrameWindow::UIFrameWindow()
    : m_updateMode(UIFrameWindowUpdateMode::EventDispatches)
    , m_ImGuiLayerEnabled(false)
    , m_realtimeRenderingEnabled(true)
    , m_layoutContext(makeObject_deprecated<UILayoutContext>()) {
    m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

UIFrameWindow::~UIFrameWindow() {
}

Result<> UIFrameWindow::init(const InitInfo& initInfo) {
    if (!UIDomainProvidor::init()) return err();

    m_manager = detail::EngineDomain::uiManager();
    specialElementFlags().set(detail::UISpecialElementFlags::FrameWindow);
    m_inputInjector = makeRef<detail::UIInputInjector>(this);
    invalidate(detail::UIElementDirtyFlags::Style | detail::UIElementDirtyFlags::Layout, false);

    if (detail::RenderingManager::instance()) {
        m_renderView = Ref<UIRenderView>(LN_NEW UIRenderView(), false);
        m_renderView->init();
        m_renderView->setRootElement(this);
    }

    m_manager->addFrameWindow(this);

    if (initInfo.platformWindow) {
        setupPlatformWindow(initInfo.platformWindow, !initInfo.createSwapChain);
    }
    else {
        WindowCreationSettings settings;
        auto* platformManager = detail::PlatformManager::instance();
        setupPlatformWindow(
            platformManager->createWindow(settings),
            false);
    }

    // EditorMode の時の初回描画用
    invalidateVisual();

    onInit();

    return ok();
}

void UIFrameWindow::onDispose(bool explicitDisposing) {
    if (m_imguiContext) {
        m_imguiContext->dispose();
        m_imguiContext = nullptr;
    }

    if (m_renderView) {
        m_renderView->dispose();
        m_renderView = nullptr;
    }
    if (m_swapChain) {
        m_swapChain->dispose();
        m_swapChain = nullptr;
    }

    if (m_platformWindow) {
        m_platformWindow->detachEventListener(this);
        if (!specialElementFlags().hasFlag(detail::UISpecialElementFlags::MainWindow)) {
            // TODO: platformManager とるよりも m_platformWindow->dispose() で消せるようにした方がいいかも
            detail::PlatformManager::instance()->windowManager()->destroyWindow(m_platformWindow);
        }
        m_platformWindow = nullptr;
    }

    if (m_manager) {
        m_manager->removeFrameWindow(this);
        m_manager = nullptr;
    }

    UIDomainProvidor::onDispose(explicitDisposing);
}

void UIFrameWindow::setAllowDragDrop(bool value) {
    m_platformWindow->setAllowDragDrop(value);
}

bool UIFrameWindow::isAllowDragDrop() const {
    return m_platformWindow->isAllowDragDrop();
}

ImGuiDockManager* UIFrameWindow::dockManager() const {
    return m_imguiContext->dockManager();
}

//void UIFrameWindow::setImGuiContext(UIImGuiContext* context) {
//    setImGuiLayerEnabled(true);
//    m_imguiContext->setImGuiContext(context);
//}

//void UIFrameWindow::setSize(float width, float height)
//{
//    m_requestedSize.set(width, height);
//}

void UIFrameWindow::setImGuiLayerEnabled(bool value) {
    m_ImGuiLayerEnabled = value;

    if (m_ImGuiLayerEnabled && !m_imguiContext) {
        m_imguiContext = std::make_unique<detail::ImGuiIntegration>();
        if (!m_imguiContext->init(this)) {
            return;
        }
    }
}

void UIFrameWindow::onInit() {

}

// DesignNote: 
// UIFrameWindow はゲームアプリで使われることは少ない。ほとんどエディタアプリで使われる。
// エディタアプリでは合わせて ImGui もよく使う。そのため UIFrameWindow に統合している。
void UIFrameWindow::onImGui() {
}

void UIFrameWindow::setupPlatformWindow(PlatformWindow* platformMainWindow, bool useExternalSwapChain) {
    m_platformWindow = platformMainWindow;

    // TODO: このフラグは PlatformWindow に持たせていいかも
    if (!useExternalSwapChain) {
        m_swapChain = makeObject_deprecated<SwapChain>(platformMainWindow);
    }

    m_platformWindow->attachEventListener(this);

    resetSizeFromPlatformWindow();
}

void UIFrameWindow::resetSizeFromPlatformWindow() {
    SizeI nativeSize;
    m_platformWindow->getSize(&nativeSize);

    Size size = nativeSize.toFloatSize();
    setWidth(size.width);
    setHeight(size.height);
    m_clientSize = size;
}

void UIFrameWindow::renderContents(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) {
    if (m_swapChain) {
        if (LN_ASSERT(!commandList)) return;
        if (LN_ASSERT(!renderTarget)) return;
        m_renderingGraphicsContext = m_swapChain->currentCommandList2();
        m_renderingGraphicsContext->beginCommandRecoding();
        commandList = m_renderingGraphicsContext;
        renderTarget = m_swapChain->currentBackbuffer();
    }
    else {
        // for external graphics context.
    }

    if (m_ImGuiLayerEnabled) {
        // TODO: time
        m_imguiContext->updateFrame(0.0166f);
        m_imguiContext->prepareRender(m_clientSize.width, m_clientSize.height);
        m_imguiContext->renderContents();
        m_imguiContext->render(commandList, renderTarget);
    }
    else {
        if (m_renderView) {
            m_renderView->render(commandList, renderTarget);
        }
    }

    if (m_swapChain) {
        m_renderingGraphicsContext->endCommandRecoding();
    }
}

void UIFrameWindow::present() {
    renderContents(nullptr, nullptr);
    m_swapChain->present();
}

void UIFrameWindow::updateLayoutIfNeeded() {
    updateStyleTree();
    updateLayoutTree();
}

SwapChain* UIFrameWindow::swapChain() const {
    return m_swapChain;
}

void UIFrameWindow::updateStyleTree() {
    if (width() != m_clientSize.width || height() != m_clientSize.height) {
        // ユーザープログラムから  UIElement としてのサイズが変更された
        m_clientSize.set(width(), height());
        m_platformWindow->setSize(SizeI::fromFloatSize(m_clientSize));
    }

    updateStyleHierarchical(m_manager->styleContext(), m_manager->finalDefaultStyle());
    m_dirtyFlags.unset(detail::UIElementDirtyFlags::Style);
}

void UIFrameWindow::updateLayoutTree() {
    if (!m_ImGuiLayerEnabled) {
        updateLayoutTreeInternal(m_clientSize);
    }
}

void UIFrameWindow::updateLayoutTreeInternal(const Size& contentSize) {
    if (m_renderView) {
        m_layoutContext->m_dpiScale = platformWindow()->dpiFactor();
        m_layoutContext->m_styleContext = m_manager->styleContext();

        //Rect clientRect(0, 0, m_clientSize);

        // ここでは ClientRect を使ってレイアウトする。
        // ImGui Layer の領域は、arrangeOverride() 時に Padding のように扱うことで、
        // MainWindow の子要素が Pane 内に表示されているように見せる。
        m_renderView->setActualSize(m_clientSize);
        m_renderView->updateUILayout(m_layoutContext);
    }

    m_dirtyFlags.unset(detail::UIElementDirtyFlags::Layout);

    //updateLayout(clientRect);
    //   updateFinalLayoutHierarchical(clientRect);
    //   // TODO: ↑のものは↓のm_renderViewのonUpdateUILayout()でおなじことやってる。まとめたいなぁ…
    //   if (m_renderView) {
    //       m_renderView->adornerLayer()->measureLayout(m_clientSize);
    //       m_renderView->adornerLayer()->arrangeLayout(clientRect);
    //   }
}

// 強制的にウィンドウサイズとする
Size UIFrameWindow::measureOverride(UILayoutContext* layoutContext, const Size& constraint) {
    UIDomainProvidor::measureOverride(layoutContext, constraint);

    // TODO: DPI チェック
    return m_clientSize;
    //int childCount = logicalChildren().size();
    //for (int i = 0; i < childCount; i++)
    //{
    //	UIElement* child = logicalChildren().at(i);
    //	child->measureLayout(constraint);
    //}

    //// TODO: DPI チェック
    //return m_clientSize;
}

// 強制的にウィンドウサイズとする
Size UIFrameWindow::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) {
    Rect rect = (m_ImGuiLayerEnabled) ? m_contentArea : Rect(0, 0, desiredSize());

    UIFrameLayout2::staticArrangeLogicalChildren(layoutContext, this, rect);

    return finalArea.getSize();

    //return UIDomainProvidor::arrangeOverride(layoutContext, Rect(0, 0, desiredSize()));
    //Rect rect = (m_ImGuiLayerEnabled) ? m_contentArea : Rect(0, 0, desiredSize());

    //return UIDomainProvidor::arrangeOverride(layoutContext, rect);
    //int childCount = logicalChildren().size();
    //for (int i = 0; i < childCount; i++)
    //{
    //	UIElement* child = logicalChildren().at(i);
    //	child->arrangeLayout(Rect(0, 0, finalSize));	// TODO: padding
    //}

    //return UIElement::arrangeOverride(desiredSize());
}

void UIFrameWindow::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle) {
    UIDomainProvidor::onUpdateStyle(styleContext, finalStyle);

    m_renderView->setBackgroundColor(styleContext->mainTheme->color(UIThemeConstantPalette::DefaultBackgroundColor));
}

void UIFrameWindow::render(UIRenderingContext* context, const Matrix& parentTransform) {
    UIDomainProvidor::render(context, parentTransform);

    if (m_debugInterface) {
        m_debugInterface->renderOnUI(context);
    }
}

void UIFrameWindow::onRender(UIRenderingContext* context) {
    UIDomainProvidor::onRender(context);

    //detail::EngineDomain::effectManager()->testDraw(m_renderingGraphicsContext);
}

//void UIFrameWindow::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
//{
//	UIContainerElement::onUpdateStyle(styleContext, finalStyle);
//}
//
//void UIFrameWindow::onUpdateLayout(const Rect& finalGlobalRect)
//{
//	UIContainerElement::onUpdateLayout(finalGlobalRect);
//}

bool UIFrameWindow::onPlatformEvent(const PlatformEventArgs& e) {
    if (m_ImGuiLayerEnabled) {
        if (m_imguiContext->handlePlatformEvent(e)) {
            return true;
        }
    }

    switch (e.type) {
        case PlatformEventType::close:
            //exit();
            break;
        case PlatformEventType::MouseDown:
            if (m_inputInjector->injectMouseButtonDown(e.mouse.button)) return true;
            break;
        case PlatformEventType::MouseUp:
            if (m_inputInjector->injectMouseButtonUp(e.mouse.button)) return true;
            break;
        case PlatformEventType::MouseMove: {
            auto pt = m_platformWindow->pointFromScreen(PointI(e.mouseMove.screenX, e.mouseMove.screenY));
            if (m_inputInjector->injectMouseMove(pt.x, pt.y, e.mouseMove.grabOffsetX, e.mouseMove.grabOffsetY)) return true;
            break;
        }
        case PlatformEventType::MouseWheel:
            if (m_inputInjector->injectMouseWheel(e.wheel.delta)) return true;
            break;
        case PlatformEventType::KeyDown:
            if (m_inputInjector->injectKeyDown(e.key.keyCode, e.key.modifierKeys)) return true;
            //// TODO: デバッグ表示切替
            //if (m_configData.acceleratorKeys.toggleShowDiag != nullptr &&
            //	m_configData.acceleratorKeys.toggleShowDiag->EqualKeyInput(e.key.keyCode, e.key.modifierKeys) &&
            //	m_diagViewer != nullptr)
            //{
            //	m_diagViewer->toggleDisplayMode();
            //}
            break;
        case PlatformEventType::KeyUp:
            if (m_inputInjector->injectKeyUp(e.key.keyCode, e.key.modifierKeys)) return true;
            break;
        case PlatformEventType::KeyChar:
            if (m_inputInjector->injectTextInput(e.key.keyChar)) return true;
            break;
        case PlatformEventType::WindowSizeChanged: {
            if (m_platformWindow && m_swapChain) {
                int w, h;
                m_platformWindow->getFramebufferSize(&w, &h);
                detail::SwapChainInternal::resizeBackbuffer(m_swapChain, w, h);
            }

            resetSizeFromPlatformWindow();
            break;
        }

        case PlatformEventType::DragEnter: {
            LN_NOTIMPLEMENTED();
            //auto args = UIDragDropEventArgs::create(UIEvents::DragEnterEvent, e.dragDrop.data, *e.dragDrop.effect);
            //onDragEnter(args);
            //*e.dragDrop.effect = args->effect();
            break;
        }
        case PlatformEventType::DragDrop: {
            LN_NOTIMPLEMENTED();
            //auto args = UIDragDropEventArgs::create(UIEvents::DragDropEvent, e.dragDrop.data, *e.dragDrop.effect);
            //onDragDrop(args);
            break;
        }
        default:
            break;
    }
    return false;
}

void UIFrameWindow::onRoutedEvent(UIEventArgs* e) {
    if (m_ImGuiLayerEnabled && m_imguiContext) {
        m_imguiContext->handleUIEvent(e);
    }

    //   if (e->type() == UIEvents::RequestVisualUpdateEvent) {
    //       if (m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Style)) {
    //           //UIContext* context = getContext();
    //           //updateStyleHierarchical(context->styleContext(), context->finalDefaultStyle());		// TODO: 直接呼ぶのではなく、RenderView 経由で読んでもらう
    //           //// TODO: ↑のものは↓のm_renderViewのonUpdateUILayout()でおなじことやってる。まとめたいなぁ…
    //           //if (m_renderView) {
    //           //    m_renderView->adornerLayer()->updateStyleHierarchical(context->styleContext(), context->finalDefaultStyle());
    //           //}
    //		if (m_renderView) {
    //			m_renderView->updateUIStyle(m_manager->styleContext(), m_manager->finalDefaultStyle());
    //		}
    //       }
    //       if (m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Layout)) {
    //           updateLayoutTree();
    //       }
    //       e->handled = true;
    //       return;
    //   }
    //else
    if (e->type() == UIEvents::RequestVisualRedrawEvent) {
        if (!m_realtimeRenderingEnabled && m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Render)) {
            present();
        }
        e->handled = true;
        return;
    }

    m_manager->handleGlobalRoutedEvent(e);
}

void UIFrameWindow::invalidate(detail::UIElementDirtyFlags flags, bool toAncestor) {
    if (m_updateMode == UIFrameWindowUpdateMode::EventDispatches) {
        //if (!m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Style) && testFlag(flags, detail::UIElementDirtyFlags::Style)) {
        //	postEvent(UIEventArgs::create(this, UIEvents::RequestVisualUpdateEvent));
        //}
        //if (!m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Layout) && testFlag(flags, detail::UIElementDirtyFlags::Layout)) {
        //	postEvent(UIEventArgs::create(this, UIEvents::RequestVisualUpdateEvent));
        //}
        if (!m_dirtyFlags.hasFlag(detail::UIElementDirtyFlags::Render) && testFlag(flags, detail::UIElementDirtyFlags::Render)) {
            postEvent(UIEventArgs::create(this, UIEvents::RequestVisualRedrawEvent));
        }
    }

    UIDomainProvidor::invalidate(flags, toAncestor);
}

//==============================================================================
// UIMainWindow

UIMainWindow::UIMainWindow() {
    specialElementFlags().set(detail::UISpecialElementFlags::MainWindow);
}

UIMainWindow::~UIMainWindow() {
}

Result<> UIMainWindow::init(bool useExternalSwapChain) {

    // サブクラスの init 等で、AllowDragDrop や WindowSize など PlatformWindow が実態をもつプロパティにアクセス試合ことがある。
    // そのためこの時点で PlatformWindow をアタッチしておきたい。
    InitInfo info;
    info.platformWindow = detail::PlatformManager::instance()->mainWindow();
    info.createSwapChain = !useExternalSwapChain;
    LN_TRY(UIFrameWindow::init(info));

    m_updateMode = UIFrameWindowUpdateMode::Polling;


    // TODO: ここでいい？
    onLoaded();

    return ok();
}

//void UIMainWindow::init(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize)
//{
//    UIFrameWindow::init();
//	setupPlatformWindow(platformMainWindow, backbufferSize);
//    m_graphicsContext = m_manager->graphicsManager()->mainWindowGraphicsContext();
//}

#if 0
//==============================================================================
// UINativeFrameWindow

UINativeFrameWindow::UINativeFrameWindow() {
}

void UINativeFrameWindow::init() {
    UIFrameWindow::init();
}

void UINativeFrameWindow::attachRenderingThread(/*RenderingType renderingType*/) {
    // TODO: GraphicsCommandList の持ち方を変えた。要検討
    assert(0);
    //if (LN_REQUIRE(!m_graphicsContext)) return;
    //m_graphicsContext = makeObject_deprecated<GraphicsCommandList>(renderingType);
}

void UINativeFrameWindow::detachRenderingThread() {
    // TODO: GraphicsCommandList の持ち方を変えた。要検討
    assert(0);
    //if (m_graphicsContext) {
    //    m_graphicsContext->dispose();
    //    m_graphicsContext = nullptr;
    //}
}

void UINativeFrameWindow::onDispose(bool explicitDisposing) {
    UIFrameWindow::onDispose(explicitDisposing);
    // TODO: GraphicsCommandList の持ち方を変えた。要検討
    assert(0);
    //if (m_graphicsContext) {
    //	m_graphicsContext->dispose();
    //	m_graphicsContext = nullptr;
    //}
}

void UINativeFrameWindow::beginRendering(RenderTargetTexture* renderTarget) {
    m_renderingRenderTarget = renderTarget;
    m_depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    m_renderingGraphicsContext->enterRenderState();
    m_renderingGraphicsContext->resetState();
    ////m_renderingGraphicsContext->setRenderTarget(0, renderTarget);
    ////m_renderingGraphicsContext->setDepthBuffer(m_depthBuffer);
    //m_renderPass->setRenderTarget(0, renderTarget);
    //m_renderPass->setDepthBuffer(m_depthBuffer);
    //m_renderingGraphicsContext->setRenderPass(m_renderPass);
}

void UINativeFrameWindow::renderContents() {
    if (m_renderView) {
        m_renderView->setRootElement(this);
        m_renderView->render(m_renderingGraphicsContext, /*m_renderView->m_renderingContext, */m_renderingRenderTarget);
    }
}

void UINativeFrameWindow::endRendering() {
    if (m_depthBuffer) {
        DepthBuffer::releaseTemporary(m_depthBuffer);
        m_depthBuffer = nullptr;
    }
    m_renderingRenderTarget = nullptr;

    m_renderingGraphicsContext->leaveRenderState();
}
#endif

} // namespace ln
