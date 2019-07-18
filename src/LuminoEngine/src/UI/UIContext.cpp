
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
    : m_finalDefaultStyle(makeRef<detail::UIStyleInstance>())
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
    //element->m_context = this;
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
		//--------------------------------
        // UIButton
		{
			auto c = sheet->addStyleClass(u"UIButton");
			{
				auto s = c->style();
				s->horizontalContentAlignment = HAlignment::Center;
				s->verticalContentAlignment = VAlignment::Center;
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 3);
				s->cornerRadius = CornerRadius(4);
				s->shadowBlurRadius = 2;
				s->shadowOffsetY = 2;
			}
			//// UIButton.test
			//{
			//	auto s = makeObject<UIStyle>();
			//	s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
			//	c->addClassStyle(u"test", s);
			//}
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

		//--------------------------------
		// UIThumb
		{
			auto c = sheet->addStyleClass(u"UIThumb");
			// UIThumb
			{
				auto s = c->style();
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 8);
				s->cornerRadius = CornerRadius(4);
			}
			// UIThumb.UITrack-Thumb
			{
				auto s = makeObject<UIStyle>();
				s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
				c->addClassStyle(u"UITrack-Thumb", s);
			}
		}
		//--------------------------------
		// UITrack
		{
			auto c = sheet->addStyleClass(u"UITrack");
			{
				auto s = c->style();
				s->backgroundColor = UIColors::get(UIColorHues::Amber, 8);
			}
		}
    }

    m_styleContext->addStyleSheet(sheet);
    m_styleContext->build();
}

} // namespace ln

