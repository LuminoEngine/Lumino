
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
    detail::UIStyleInstance::updateStyleDataHelper(nullptr, m_defaultStyle, m_finalDefaultStyle);

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
	Color controlBackground = UIColors::get(UIColorHues::Grey, 3);
	Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

	//Color containerBackground = UIColors::get(UIColorHues::Grey, 3);
	//Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

    auto sheet = makeObject<UIStyleSheet>();

    {
		//--------------------------------
        // UIButton
		{
			auto e = sheet->addStyleSet(u"UIButton");
			{
				auto s = e->mainStyleClass()->mainStyle();
                s->minWidth = 64;
                s->minHeight = 36;
                s->margin = Thickness(8);   // TODO: spacing?
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
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
				e->mainStyleClass()->addStateStyle(u"MouseOver", s);
			}
			// 
            if (auto s = sheet->obtainStyle(u"UIButton:Pressed")) {
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
				e->mainStyleClass()->addStateStyle(u"Pressed", s);
                s->shadowBlurRadius = 0;
                s->shadowOffsetY = 0;
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
			}
			// UIThumb.UITrack-Thumb
			{
				auto s = makeObject<UIStyle>();

				s->margin = Thickness(2);
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
				s->cornerRadius = CornerRadius(4);
				s->horizontalAlignment = HAlignment::Stretch;
				s->verticalAlignment = VAlignment::Stretch;

				s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
				e->mainStyleClass()->addStateStyle(u"UITrack-Thumb", s);
			}
			if (auto s = sheet->obtainStyle(u"UIThumb.SplitterBar"))
			{
				s->backgroundColor = Color(0, 1, 0, 0.2); // debug
				s->margin = Thickness(-2, -2, -2, -2);
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
                s->minHeight = 30;
                s->horizontalAlignment = HAlignment::Stretch;
                s->verticalAlignment = VAlignment::Top;
                //s->borderThickness = 1;
                //s->setBorderColor(Color::Gray);
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander")) {   // VisualState によらず常に有効。個別にしたければ:Normalを付ける。
                s->width = 16;
                s->height = 16;
                s->horizontalAlignment = HAlignment::Center;
                s->verticalAlignment = VAlignment::Center;
                s->backgroundColor = Color::Transparency;
            }
            //if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:MouseOver")) {
            //}
            //if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Pressed")) {
            //}
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Checked")) {
				auto icon = makeObject<UIStyleDecorator>();
				icon->setIconName(u"angle-down", 15);
				s->decorators.add(icon);
            }
			if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Unchecked")) {
				auto icon = makeObject<UIStyleDecorator>();
				icon->setIconName(u"angle-right", 15);
				s->decorators.add(icon);
			}
        }

		//--------------------------------
		// UITabItem
		{
			if (auto s = sheet->obtainStyle(u"UITabItem")) {
				s->minWidth = 100;
				s->minHeight = 30;
				s->backgroundColor = controlBackground;
			}
			if (auto s = sheet->obtainStyle(u"UITabItem:Selected")) {
				s->backgroundColor = activeControlBackground;
			}
		}
    }

    m_styleContext->addStyleSheet(sheet);
    //m_styleContext->build();
}

} // namespace ln

