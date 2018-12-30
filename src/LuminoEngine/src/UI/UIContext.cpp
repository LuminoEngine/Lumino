
#include "Internal.hpp"
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>

namespace ln {

//==============================================================================
// UIContext

UIContext::UIContext()
    : m_mouseHoverElement(nullptr)
{
}

void UIContext::initialize()
{
    Object::initialize();
}

void UIContext::setLayoutRootElement(UIElement* element)
{
    m_layoutRootElement = element;
}

void UIContext::addElement(UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    element->m_context = this;
}

bool UIContext::updateMouseHover(const Point& mousePos)
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
        m_mouseHoverElement = m_layoutRootElement->lookupMouseHoverElement(mousePos);
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
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseLeaveEvent, MouseButtons::None, mousePos.x, mousePos.y, 0, true);
            old->raiseEvent(args);
        }

        if (m_mouseHoverElement)
        {
            auto args = UIMouseEventArgs::create(old, UIEvents::MouseEnterEvent, MouseButtons::None, mousePos.x, mousePos.y, 0, true);
            m_mouseHoverElement->raiseEvent(args);
        }
    }

    return false;
}

} // namespace ln

