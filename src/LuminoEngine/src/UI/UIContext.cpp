
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>

namespace ln {

//==============================================================================
// UIContext

UIContext::UIContext()
    : m_mouseHoverElement(nullptr)
    , m_finalDefaultStyle(makeRef<detail::UIStyleInstance>())
{
}

void UIContext::init()
{
    Object::init();

	m_defaultStyle = makeObject<UIStyle>();
	m_defaultStyle->setupDefault();
    //m_finalDefaultStyle->setupDefault();
	m_finalDefaultStyle->backgroundMaterial = makeObject<Material>();
    detail::UIStyleInstance::updateStyleDataHelper(m_defaultStyle, nullptr, m_defaultStyle, m_finalDefaultStyle);

    m_styleContext = makeObject<UIStyleContext>();
    setupDefaultStyle();
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

bool UIContext::updateMouseHover(UIFrameWindow* mouseEventSource, const Point& frameClientPosition)
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

void UIContext::updateStyleTree()
{
	if (m_layoutRootElement)
	{
		m_layoutRootElement->updateStyleHierarchical(m_styleContext, m_finalDefaultStyle);
	}
}

//void UIContext::updateLayoutTree()
//{
//	if (m_layoutRootElement)
//	{
//		m_layoutRootElement->updateLayout(Rect(0, 0, m_layoutRootElement->width(), m_layoutRootElement->height()));
//	}
//}

void UIContext::setupDefaultStyle()
{
    auto sheet = makeObject<UIStyleSheet>();

    {
        auto c = sheet->addStyleClass(u"UIButton");
        // UIButton
        {
            auto s = c->style();
            s->backgroundColor = UIColors::get(UIColorHues::Grey, 3);
            //s->borderThickness = 1;
            //s->setBorderColor(Color::Gray);
        }
        // UIButton:MouseOver
        {
            auto s = makeObject<UIStyle>();
            s->backgroundColor = UIColors::get(UIColorHues::Red, 4);
            c->addStateStyle(u"MouseOver", s);
        }
        // UIButton:Pressed
        {
            auto s = makeObject<UIStyle>();
            s->backgroundColor = UIColors::get(UIColorHues::Green, 5);
            c->addStateStyle(u"Pressed", s);
        }
    }

    m_styleContext->addStyleSheet(sheet);
    m_styleContext->build();
}

} // namespace ln

