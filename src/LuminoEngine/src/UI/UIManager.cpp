
#include "Internal.hpp"
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include "UIEventArgsPool.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// UIManager

const float UIManager::MouseButtonClickTimeout = 0.3f;

UIManager::UIManager()
	: m_graphicsManager(nullptr)
    , m_primaryElement(nullptr)
    , m_mouseHoverElement(nullptr)
{
}

void UIManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "UIManager Initialization started.";

	m_graphicsManager = settings.graphicsManager;
    m_eventArgsPool = makeRef<EventArgsPool>();
    //m_mainContext = makeObject<UIContext>();

    LN_LOG_DEBUG << "UIManager Initialization ended.";
}

void UIManager::dispose()
{
    m_eventArgsPool = nullptr;
    m_mainContext = nullptr;
}

void UIManager::setPrimaryElement(UIContainerElement* element)
{
    m_primaryElement = element;
}

void UIManager::setMainContext(UIContext* context)
{
    m_mainContext = context;
}

UIContainerElement* UIManager::primaryElement() const
{
    return m_primaryElement;
}

bool UIManager::updateMouseHover(UIFrameWindow* mouseEventSource, const Point& frameClientPosition)
{
    if (LN_REQUIRE(mouseEventSource)) return false;

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
        m_mouseHoverElement = mouseEventSource->lookupMouseHoverElement(frameClientPosition);
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
}

} // namespace detail
} // namespace ln

