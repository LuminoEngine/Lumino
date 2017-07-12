
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIComboBox.h>	// TODO: UIPopup
#include "UIManager.h"
#include "EventArgsPool.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// UILayoutView
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
UIPopuoContainer::UIPopuoContainer()
{
}

//------------------------------------------------------------------------------
UIPopuoContainer::~UIPopuoContainer()
{
}

//------------------------------------------------------------------------------
void UIPopuoContainer::initialize()
{
}

//------------------------------------------------------------------------------
void UIPopuoContainer::SetPopup(UIPopup* popup)
{
	m_popup = popup;
}

//------------------------------------------------------------------------------
UIPopup* UIPopuoContainer::GetPopup() const
{
	return m_popup;
}

} // namespace detail


//==============================================================================
// UILayoutView
//==============================================================================
//------------------------------------------------------------------------------
UILayoutView::UILayoutView()
	: m_ownerNativeWindow(nullptr)
	, m_ownerContext(nullptr)
	, m_mouseHoverElement(nullptr)
	, m_mouseClickTrackers{}
{
}

//------------------------------------------------------------------------------
UILayoutView::~UILayoutView()
{
}

//------------------------------------------------------------------------------
void UILayoutView::initialize(UIContext* ownerContext, PlatformWindow* ownerNativeWindow)
{
	UIControl::initialize();

	m_ownerContext = ownerContext;
	m_ownerNativeWindow = ownerNativeWindow;
}

//------------------------------------------------------------------------------
void UILayoutView::updateLayout(const Size& viewSize)
{
	m_viewPixelSize = viewSize;

	//if (m_rootElement != nullptr)
	{
		setSize(m_viewPixelSize);

		// TODO: ここは getRootStyleTable を使うべき？
		// 今は UILayoutView::updateLayout() からしか呼ばれていないので問題ないが…。
		applyTemplateHierarchy(getOwnerContext()->getRootStyleTable(), nullptr);

		UIControl::updateLayout(getViewPixelSize());


		for (auto& popup : m_popupContainers)
		{
			popup->GetPopup()->updateLayoutForInPlacePopup(getViewPixelSize());
		}
	}
}

//------------------------------------------------------------------------------
void UILayoutView::render(DrawingContext* g)
{
	UIControl::render(g);

	for (auto& popup : m_popupContainers)
	{
		popup->GetPopup()->render(g);
	}
}

//------------------------------------------------------------------------------
bool UILayoutView::updateMouseHover(const PointF& mousePos)
{
	UIElement* old = m_mouseHoverElement;

	// TODO:IME側のイベントを処理する
	//if ( m_pIme != NULL )
	//{
	//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
	//	{
	//		goto EXIT;
	//	}
	//}

	// 通常のウィンドウのイベントを処理する
	//if (m_rootElement != NULL)
	{
		m_mouseHoverElement = checkMouseHoverElement(mousePos);
		if (m_mouseHoverElement != nullptr) {
			goto EXIT;
		}
	}

	m_mouseHoverElement = nullptr;

EXIT:
	// 新旧それぞれの Element に MouseLeave、MouseEnter イベントを送る
	if (m_mouseHoverElement != old)
	{
		auto args = UIMouseEventArgs::create(UIEvents::MouseEnterEvent, MouseButtons::None, mousePos.x, mousePos.y, 0, true);
		if (old != nullptr)
		{
			old->onEvent(detail::UIInternalEventType::MouseLeave, args);
		}

		// ↑の MouseLeave イベントとで使いまわしているのでリセットを忘れずに
		args->handled = false;

		if (m_mouseHoverElement != nullptr)
		{
			return m_mouseHoverElement->onEvent(detail::UIInternalEventType::MouseEnter, args);
		}
	}

	return false;
}

//------------------------------------------------------------------------------
void UILayoutView::openPopup(UIPopup* popup)
{
	auto container = newObject<detail::UIPopuoContainer>();
	container->SetPopup(popup);
	m_popupContainers.add(container);
}

//------------------------------------------------------------------------------
void UILayoutView::closePopup(UIPopup* popup)
{
	m_popupContainers.removeIf([popup](const RefPtr<detail::UIPopuoContainer>& ptr) { return ptr->GetPopup() == popup; });
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void UILayoutView::captureMouse(UIElement* element)
//{
//	m_capturedElement = element;
//	m_ownerNativeWindow->captureMouse();
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void UILayoutView::releaseMouseCapture(UIElement* element)
//{
//	if (m_capturedElement == element)
//	{
//		m_capturedElement = NULL;
//		m_ownerNativeWindow->releaseMouseCapture();
//	}
//}

//------------------------------------------------------------------------------
//bool UILayoutView::InjectViewportSizeChanged(int width, int height)
//{
//	m_viewPixelSize.Set(width, height);
//	return false;
//}

//------------------------------------------------------------------------------
bool UILayoutView::injectMouseMove(float clientX, float clientY)
{
	m_mousePosition.set(clientX, clientY);

	// キャプチャ中のコントロールがあればそちらに送る
	if (m_manager->getMouseCapturedElement() != nullptr)
	{
		auto args = UIMouseEventArgs::create(UIEvents::MouseMoveEvent, MouseButtons::None, clientX, clientY, 0, true);
		return m_manager->getMouseCapturedElement()->onEvent(detail::UIInternalEventType::MouseMove, args);
	}
	updateMouseHover(PointF(clientX, clientY));
	if (m_mouseHoverElement == nullptr) return false;

	auto args = UIMouseEventArgs::create(UIEvents::MouseMoveEvent, MouseButtons::None, clientX, clientY, 0, true);
	return m_mouseHoverElement->onEvent(detail::UIInternalEventType::MouseMove, args);
}

//------------------------------------------------------------------------------
bool UILayoutView::injectMouseButtonDown(MouseButtons button, float clientX, float clientY)
{
	m_mousePosition.set(clientX, clientY);

	// マウスクリック回数の処理
	MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];
	tracker.ClickCount++;

	double curTime = 0.001 * Environment::getTickCount();
	float elapsed = (float)(curTime - tracker.LastTime);
	if (elapsed > detail::UIManager::MouseButtonClickTimeout ||
		m_mouseHoverElement != tracker.HoverElement ||
		tracker.ClickCount > 3)
	{
		tracker.ClickCount = 1;
		tracker.HoverElement = m_mouseHoverElement;
	}
	tracker.LastTime = curTime;

	// キャプチャ中のコントロールがあればそちらに送る
	if (m_manager->getMouseCapturedElement() != nullptr)
	{
		auto args = UIMouseEventArgs::create(UIEvents::MouseDownEvent, button, clientX, clientY, tracker.ClickCount, true);
		return m_manager->getMouseCapturedElement()->onEvent(detail::UIInternalEventType::MouseButtonDown, args);
	}
	if (m_mouseHoverElement == nullptr) return false;

	auto args = UIMouseEventArgs::create(UIEvents::MouseDownEvent, button, clientX, clientY, tracker.ClickCount, true);
	return m_mouseHoverElement->onEvent(detail::UIInternalEventType::MouseButtonDown, args);
}

//------------------------------------------------------------------------------
bool UILayoutView::injectMouseButtonUp(MouseButtons button, float clientX, float clientY)
{
	m_mousePosition.set(clientX, clientY);

	// キャプチャ中のUI要素があればそちらに送る
	if (m_manager->getMouseCapturedElement() != nullptr)
	{
		auto args = UIMouseEventArgs::create(UIEvents::MouseUpEvent, button, clientX, clientY, 0, true);
		return m_manager->getMouseCapturedElement()->onEvent(detail::UIInternalEventType::MouseButtonUp, args);
	}
	// マウス位置にUI要素があればそちらに送る
	if (m_mouseHoverElement != nullptr)
	{
		auto args = UIMouseEventArgs::create(UIEvents::MouseUpEvent, button, clientX, clientY, 0, true);
		return m_mouseHoverElement->onEvent(detail::UIInternalEventType::MouseButtonUp, args);
	}
	return false;
}

//------------------------------------------------------------------------------
bool UILayoutView::injectMouseWheel(int delta)
{
	// キャプチャ中のUI要素があればそちらに送る
	if (m_manager->getMouseCapturedElement() != nullptr)
	{
		auto args = UIMouseWheelEventArgs::create(UIEvents::MouseWheelEvent, delta, true);
		return m_manager->getMouseCapturedElement()->onEvent(detail::UIInternalEventType::MouseWheel, args);
	}
	// マウス位置にUI要素があればそちらに送る
	if (m_mouseHoverElement != nullptr)
	{
		auto args = UIMouseWheelEventArgs::create(UIEvents::MouseWheelEvent, delta, true);
		return m_mouseHoverElement->onEvent(detail::UIInternalEventType::MouseWheel, args);
	}
	return false;
}

//------------------------------------------------------------------------------
bool UILayoutView::injectKeyDown(Keys keyCode, ModifierKeys modifierKeys)
{
	// フォーカスを持っているUI要素に送る
	if (m_ownerContext->setFocusElement() != nullptr)
	{
		auto args = UIKeyEventArgs::create(UIEvents::KeyDownEvent, keyCode, modifierKeys, 0, true);
		return m_ownerContext->setFocusElement()->onEvent(detail::UIInternalEventType::KeyDown, args);
	}
	return false;
}

//------------------------------------------------------------------------------
bool UILayoutView::injectKeyUp(Keys keyCode, ModifierKeys modifierKeys)
{
	// フォーカスを持っているUI要素に送る
	if (m_ownerContext->setFocusElement() != nullptr)
	{
		auto args = UIKeyEventArgs::create(UIEvents::KeyUpEvent, keyCode, modifierKeys, 0, true);
		return m_ownerContext->setFocusElement()->onEvent(detail::UIInternalEventType::KeyUp, args);
	}
	return false;
}

//------------------------------------------------------------------------------
bool UILayoutView::injectTextInput(TCHAR ch)
{
	// フォーカスを持っているUI要素に送る
	if (m_ownerContext->setFocusElement() != nullptr)
	{
		auto args = UIKeyEventArgs::create(UIEvents::TextInputEvent, Keys::Unknown, ModifierKeys::None, ch, true);
		return m_ownerContext->setFocusElement()->onEvent(detail::UIInternalEventType::TextInput, args);
	}
	return false;
}

//------------------------------------------------------------------------------
detail::SpcialUIElementType UILayoutView::getSpcialUIElementType() const
{
	return detail::SpcialUIElementType::LayoutRoot;
}

LN_NAMESPACE_END
