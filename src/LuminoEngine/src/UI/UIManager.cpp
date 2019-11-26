
#include "Internal.hpp"
#include <LuminoEngine/Engine/Application.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/UIActiveTimer.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include "UIEventArgsPool.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIManager

const float UIManager::MouseButtonClickTimeout = 0.3f;

UIManager::UIManager()
	: m_graphicsManager(nullptr)
    , m_application(nullptr)
    , m_primaryElement(nullptr)
    , m_mouseHoverElement(nullptr)
	, m_capturedElement(nullptr)
    , m_forcusedElement(nullptr)
{
}

void UIManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "UIManager Initialization started.";

	m_graphicsManager = settings.graphicsManager;
    m_application = settings.application;
    m_eventArgsPool = makeRef<EventArgsPool>();
    //m_mainContext = makeObject<UIContext>();

	//m_defaultLayout = makeObject<UIFrameLayout>();

    LN_LOG_DEBUG << "UIManager Initialization finished.";
}

void UIManager::dispose()
{
    m_eventArgsPool = nullptr;
    m_mainContext = nullptr;
}

void UIManager::onElementDisposing(UIElement* element)
{
    //if (m_forcusedElement == element) {
    //    m_forcusedElement = nullptr;
    //}
    //if (m_capturedElement == element) {
    //    m_capturedElement = nullptr;
    //}
    //if (m_mouseHoverElement == element) {
    //    m_mouseHoverElement = nullptr;
    //}
}

void UIManager::setPrimaryElement(UIControl* element)
{
    m_primaryElement = element;
}

UIControl* UIManager::primaryElement() const
{
	return m_primaryElement;
}

void UIManager::setMainContext(UIContext* context)
{
    m_mainContext = context;
}

void UIManager::updateMouseHover(UIRenderView* mouseEventSource, const Point& frameClientPosition)
{
    if (LN_REQUIRE(mouseEventSource)) return;


#if 1
    UIElement* hoverdElement = mouseEventSource->onLookupMouseHoverElement(frameClientPosition);
    if (m_mouseHoverElement != hoverdElement) {
        clearMouseHover();
    }

    if (hoverdElement)
    {
        m_mouseHoverElement = hoverdElement;
        auto args = UIMouseEventArgs::create(m_mouseHoverElement, UIEvents::MouseEnterEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, true);
        m_mouseHoverElement->raiseEvent(args);
    }


#else
    UIElement* old = m_mouseHoverElement;

    // TODO:IME側のイベントを処理する
    //if ( m_pIme != NULL )
    //{
    //	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
    //	{
    //		goto EXIT;
    //	}
    //}


    // m_adornerLayer を調べる
    //m_mouseHoverElement = m_adornerLayer->checkMouseHoverElement(mousePos);
    //if (m_mouseHoverElement != nullptr) {
    //	goto EXIT;
    //}

    // Popup を調べる
    //for (auto& popup : m_popupContainers)
    //{
    //    m_mouseHoverElement = popup->GetPopup()->checkMouseHoverElement(mousePos);
    //    if (m_mouseHoverElement != nullptr) {
    //        goto EXIT;
    //    }
    //}

    // 通常のウィンドウのイベントを処理する
    //if (m_rootElement != NULL)
    {
        m_mouseHoverElement = mouseEventSource->onLookupMouseHoverElement(frameClientPosition);
        if (m_mouseHoverElement != nullptr) {
            goto EXIT;
        }
    }

    m_mouseHoverElement = nullptr;

EXIT:
    // 新旧それぞれの Element に MouseLeave、MouseEnter イベントを送る
    if (m_mouseHoverElement != old)
    {
        if (old)
        {
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseLeaveEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, true);
            old->raiseEvent(args);
        }

        if (m_mouseHoverElement)
        {
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseEnterEvent, MouseButtons::None, frameClientPosition.x, frameClientPosition.y, 0, true);
            m_mouseHoverElement->raiseEvent(args);
        }
    }

    return false;
#endif
}

void UIManager::retainCapture(UIElement* element)
{
	m_capturedElement = element;
}

void UIManager::releaseCapture(UIElement* element)
{
	if (m_capturedElement == element) {
		m_capturedElement = nullptr;
	}
}

void UIManager::tryGetInputFocus(UIElement* element)
{
	activateTree(element);
	m_forcusedElement = element;
}

void UIManager::activateTree(UIElement* element)
{
	m_activationCache.clear();

	// 論理フォーカスを持っているものをすべて列挙
	UIElement* e = element;
	while (e)
	{
		m_activationCache.add(e);
		e = e->m_visualParent;
	}

	// activation と deactivation の分岐点を探しつつ deactivate する
	UIElement* branchRoot = nullptr;
	e = m_forcusedElement;
	while (e)
	{
		if (m_activationCache.contains(e)) {
			branchRoot = e;
			break;
		}

        // deactivate
        auto args = UIEventArgs::create(e, UIEvents::LostFocusEvent, true);
        e->raiseEvent(args, UIEventRoutingStrategy::Direct);

		e = e->m_visualParent;
	}

	// 基点から分岐点までを activate
	e = element;
	while (e)
	{
		if (e == branchRoot) {
			break;
		}

        // activate
        e->activateInternal();
        auto args = UIEventArgs::create(e, UIEvents::GotFocusEvent, true);
        e->raiseEvent(args, UIEventRoutingStrategy::Direct);

		e = e->m_visualParent;
	}

    m_activationCache.clear();
}

void UIManager::postEvent(UIElement* target, UIEventArgs* e)
{
    m_eventQueue.push_back({ target, e });
}

void UIManager::dispatchPostedEvents()
{
    int count = m_eventQueue.size();    // 以下で発行したイベント内からさらに postEvent されたものは次のフレームで処理したいので、今回フレームで処理する数を決める
    while (count > 0) {
        auto& item = m_eventQueue.front();
        item.target->raiseEvent(item.args);
        m_eventQueue.pop_front();
        count--;
    }
}

void UIManager::handleGlobalRoutedEvent(UIEventArgs* e)
{
    if (m_application) {
        ApplicationHelper::callOnRoutedEvent(m_application, e);
    }
}

void UIManager::registerActiveTimer(UIActiveTimer* timer)
{
	m_activeTimers.add(timer);
}

void UIManager::unregisterActiveTimer(UIActiveTimer* timer)
{
	m_activeTimers.remove(timer);
}

void UIManager::tickGlobal(float elapsedSeconds)
{
	for (auto& timer : m_activeTimers) {
		timer->tick(elapsedSeconds);
	}
}

void UIManager::clearMouseHover()
{
    if (m_mouseHoverElement)
    {
        auto args = UIMouseEventArgs::create(m_mouseHoverElement, UIEvents::MouseLeaveEvent, MouseButtons::None, 0, 0, 0, true);
        m_mouseHoverElement->raiseEvent(args);
        m_mouseHoverElement = nullptr;
    }
}

void UIManager::clearFocus()
{
    m_forcusedElement = nullptr;
}

void UIManager::handleDetachFromUITree(UIElement* element)
{
    if (m_mouseHoverElement == element) {
        clearMouseHover();
    }

    if (m_forcusedElement == element) {
        clearFocus();
    }
}

} // namespace detail
} // namespace ln

