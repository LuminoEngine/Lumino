
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include "UIManager.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Platform/PlatformManager.hpp"

namespace ln {

namespace detail {

//==============================================================================
// UIInputInjector

UIInputInjector::UIInputInjector(UIElement* owner)
    : m_owner(owner)
{
}

bool UIInputInjector::injectMouseMove(float clientX, float clientY)
{
    m_mousePosition.set(clientX, clientY);

    // キャプチャ中のコントロールがあればそちらに送る
    UIElement* sender = capturedElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseMoveEvent, MouseButtons::None, clientX, clientY, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    updateMouseHover(clientX, clientY);

    sender = mouseHoveredElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseMoveEvent, MouseButtons::None, clientX, clientY, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    return false;
}

bool UIInputInjector::injectMouseButtonDown(MouseButtons button)
{
    // マウスクリック回数の処理
    MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];
    tracker.clickCount++;

    double curTime = 0.001 * Environment::getTickCount();
    float elapsed = (float)(curTime - tracker.lastTime);
    if (elapsed > detail::UIManager::MouseButtonClickTimeout ||
        mouseHoveredElement() != tracker.hoverElement ||
        tracker.clickCount > 3)
    {
        tracker.clickCount = 1;
        tracker.hoverElement = mouseHoveredElement();
    }
    tracker.lastTime = curTime;

    // キャプチャ中のコントロールがあればそちらに送る
    UIElement* sender = capturedElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseDownEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    sender = mouseHoveredElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseDownEvent, button, m_mousePosition.x, m_mousePosition.y, tracker.clickCount, true);
        sender->raiseEvent(args);
        return args->handled;
    }

    return false;
}

bool UIInputInjector::injectMouseButtonUp(MouseButtons button)
{
    // キャプチャ中のUI要素があればそちらに送る
    UIElement* sender = capturedElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseUpEvent, button, m_mousePosition.x, m_mousePosition.y, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    // マウス位置にUI要素があればそちらに送る
    sender = mouseHoveredElement();
    if (sender)
    {
        auto args = UIMouseEventArgs::create(sender, UIEvents::MouseUpEvent, button, m_mousePosition.x, m_mousePosition.y, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectMouseWheel(int delta)
{
    // キャプチャ中のUI要素があればそちらに送る
    UIElement* sender = capturedElement();
    if (sender)
    {
        auto args = UIMouseWheelEventArgs::create(sender, UIEvents::MouseWheelEvent, delta, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    // マウス位置にUI要素があればそちらに送る
    sender = mouseHoveredElement();
    if (sender)
    {
        auto args = UIMouseWheelEventArgs::create(sender, UIEvents::MouseWheelEvent, delta, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectKeyDown(Keys keyCode, ModifierKeys modifierKeys)
{
    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender)
    {
        auto args = UIKeyEventArgs::create(sender, UIEvents::KeyDownEvent, keyCode, modifierKeys, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectKeyUp(Keys keyCode, ModifierKeys modifierKeys)
{
    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender)
    {
        auto args = UIKeyEventArgs::create(sender, UIEvents::KeyUpEvent, keyCode, modifierKeys, 0, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

bool UIInputInjector::injectTextInput(Char ch)
{
    // フォーカスを持っているUI要素に送る
    UIElement* sender = forcusedElement();
    if (sender)
    {
        auto args = UIKeyEventArgs::create(sender, UIEvents::TextInputEvent, Keys::Unknown, ModifierKeys::None, ch, true);
        sender->raiseEvent(args);
        return args->handled;
    }
    return false;
}

void UIInputInjector::updateMouseHover(float clientX, float clientY)
{
    m_owner->context()->updateMouseHover(Point(clientX, clientY));
}

UIElement* UIInputInjector::capturedElement()
{
    // TODO:
    return nullptr;
}

UIElement* UIInputInjector::forcusedElement()
{
    // TODO:
    return nullptr;
}

UIElement* UIInputInjector::mouseHoveredElement()
{
    return m_owner->context()->mouseHoverElement();
}

} //  namespace detail

//==============================================================================
// UIFrameWindow

UIFrameWindow::UIFrameWindow()
	: m_autoDisposePlatformWindow(true)
{
}

UIFrameWindow::~UIFrameWindow()
{
}

void UIFrameWindow::initialize(detail::PlatformWindow* platformMainWindow, const SizeI& backbufferSize)
{
	UIElement::initialize();
	m_manager = detail::EngineDomain::uiManager();
    m_platformWindow = platformMainWindow;
	m_autoDisposePlatformWindow = false;
	m_swapChain = newObject<SwapChain>(platformMainWindow, backbufferSize);

    if (detail::EngineDomain::renderingManager()) {
        m_renderView = newObject<UIRenderView>();
    }

    m_inputInjector = makeRef<detail::UIInputInjector>(this);

    m_platformWindow->attachEventListener(this);
}

void UIFrameWindow::dispose()
{
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
		if (m_autoDisposePlatformWindow) {
			detail::EngineDomain::platformManager()->windowManager()->destroyWindow(m_platformWindow);	// TODO: dispose で破棄で。
		}
        m_platformWindow = nullptr;
	}
}

void UIFrameWindow::renderContents()
{
	GraphicsContext* ctx = m_manager->graphicsManager()->graphicsContext();

	ctx->setColorBuffer(0, m_swapChain->colorBuffer());
	ctx->setDepthBuffer(m_swapChain->depthBuffer());
	//ctx->clear(ClearFlags::All, Color(0.4, 0.4, 0.4), 1.0f, 0x00);
}

void UIFrameWindow::present()
{
	GraphicsContext* ctx = m_manager->graphicsManager()->graphicsContext();

	if (m_renderView)
	{
		m_renderView->setRootElement(this);
		m_renderView->render(ctx);
	}

	ctx->present(m_swapChain);
}

SwapChain* UIFrameWindow::swapChain() const
{
	return m_swapChain;
}
//
//UIElement* UIFrameWindow::getVisualChild(int index) const
//{
//	return m_viewport;
//}

void UIFrameWindow::updateLayout()
{
    SizeI size;
    m_platformWindow->getSize(&size);
    UIElement::updateLayout(size.toFloatSize());
}

bool UIFrameWindow::onPlatformEvent(const detail::PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::close:
		//exit();
		break;
    case PlatformEventType::MouseDown:
        if (m_inputInjector->injectMouseButtonDown(e.mouse.button)) return true;
        break;
    case PlatformEventType::MouseUp:
        if (m_inputInjector->injectMouseButtonUp(e.mouse.button)) return true;
        break;
    case PlatformEventType::MouseMove:
        if (m_inputInjector->injectMouseMove(e.mouseMove.screenX, e.mouseMove.screenY)) return true;
        break;
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
    case PlatformEventType::WindowSizeChanged:
        break;
    case PlatformEventType::DragEnter:
    {
        LN_NOTIMPLEMENTED();
        //auto args = UIDragDropEventArgs::create(UIEvents::DragEnterEvent, e.dragDrop.data, *e.dragDrop.effect);
        //onDragEnter(args);
        //*e.dragDrop.effect = args->effect();
        break;
    }
    case PlatformEventType::DragDrop:
    {
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

} // namespace ln

