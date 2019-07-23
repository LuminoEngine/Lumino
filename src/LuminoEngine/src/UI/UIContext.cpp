
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
    m_layoutRootElement->m_context = this;
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
			auto e = sheet->addStyleSet(u"UIButton");
			{
				auto s = e->mainStyleClass()->mainStyle();
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
				e->mainStyleClass()->addStateStyle(u"MouseOver", s);
			}
			// UIButton:Pressed
			{
				auto s = makeObject<UIStyle>();
				s->backgroundColor = UIColors::get(UIColorHues::Green, 5);
				e->mainStyleClass()->addStateStyle(u"Pressed", s);
			}
		}
		//--------------------------------
		// UIToggleButton
		{
			if (auto s = sheet->obtainStyle(u"UIToggleButton")) {
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 2);
			}
			if (auto s = sheet->obtainStyle(u"UIToggleButton:MouseOver")) {
			}
			if (auto s = sheet->obtainStyle(u"UIToggleButton:Pressed")) {
			}
			if (auto s = sheet->obtainStyle(u"UIToggleButton:Checked")) {
			}
		}
		//--------------------------------
		// UIThumb
		{
			auto e = sheet->addStyleSet(u"UIThumb");
			// UIThumb
			{
				auto s = e->mainStyleClass()->mainStyle();
				s->margin = Thickness(2);
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
				s->cornerRadius = CornerRadius(4);
				s->horizontalAlignment = HAlignment::Stretch;
				s->verticalAlignment = VAlignment::Stretch;
			}
			// UIThumb.UITrack-Thumb
			{
				auto s = makeObject<UIStyle>();
				s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
				e->mainStyleClass()->addStateStyle(u"UITrack-Thumb", s);
			}
		}
		//--------------------------------
		// UITrack
		{
			if (auto s = sheet->obtainStyle(u"UITrack")) {
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 2);
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-DecreaseButton")) {
				s->backgroundColor = Color(1, 1, 1, 0);
				s->cornerRadius = CornerRadius(0);
				s->shadowBlurRadius = 0;
				s->shadowOffsetY = 0;
				s->horizontalAlignment = HAlignment::Stretch;
				s->verticalAlignment = VAlignment::Stretch;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-DecreaseButton:MouseOver")) {	// ベース要素である UIButton の VisualState を全て上書きする必要がある。CSS と同じ動作。
				s->backgroundColor = Color::Transparency;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-DecreaseButton:Pressed")) {
				s->backgroundColor = Color::Transparency;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-IncreaseButton")) {
				s->backgroundColor = Color(1, 1, 1, 0);
				s->cornerRadius = CornerRadius(0);
				s->shadowBlurRadius = 0;
				s->shadowOffsetY = 0;
				s->horizontalAlignment = HAlignment::Stretch;
				s->verticalAlignment = VAlignment::Stretch;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-IncreaseButton:MouseOver")) {
				s->backgroundColor = Color::Transparency;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-IncreaseButton:Pressed")) {
				s->backgroundColor = Color::Transparency;
			}

		}
        //--------------------------------
        // UITreeItem
        {
            if (auto s = sheet->obtainStyle(u"UITreeItem")) {
                s->horizontalAlignment = HAlignment::Stretch;
                s->verticalAlignment = VAlignment::Top;
                s->backgroundColor = UIColors::get(UIColorHues::Amber);
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander")) {
                s->horizontalAlignment = HAlignment::Left;
                s->verticalAlignment = VAlignment::Center;
                s->backgroundColor = UIColors::get(UIColorHues::Grey);
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:MouseOver")) {
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Pressed")) {
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Checked")) {
                s->backgroundColor = UIColors::get(UIColorHues::Blue);

				auto icon = makeObject<UIStyleDecorator>();
				icon->setIconName(u"angle-down", 10);
				s->decorators.add(icon);
            }
			if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Unchecked")) {
				s->backgroundColor = UIColors::get(UIColorHues::Red);

				auto icon = makeObject<UIStyleDecorator>();
				icon->setIconName(u"angle-right", 10);
				s->decorators.add(icon);
			}
        }
    }

    m_styleContext->addStyleSheet(sheet);
    m_styleContext->build();
}

} // namespace ln

