
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/UIElement.hpp>
#include <LuminoFramework/UI/UIContext.hpp>
#include <LuminoFramework/UI/UIEvents.hpp>
#include <LuminoFramework/UI/UIFrameWindow.hpp>
#include "UIManager.hpp"

namespace ln {

#if 0
//==============================================================================
// UIContext

UIContext::UIContext()
    : m_finalDefaultStyle(makeRef<detail::UIStyleInstance>())
{
}

void UIContext::init()
{
    Object::init();

	

	m_styleContext = makeObject_deprecated<UIStyleContext>();
	//m_defaultStyle = makeObject_deprecated<UIStyle>();
	//m_defaultStyle->setupDefault();
    //m_finalDefaultStyle->setupDefault();


	if (String::compare(detail::EngineDomain::uiManager()->defaultThemeName(), u"Chocotelier", CaseSensitivity::CaseInsensitive) == 0) {
		auto theme = makeObject_deprecated<UITheme>();
		theme->buildLumitelier();
		m_styleContext->addStyleSheet(theme->styleSheet());
		m_styleContext->mainTheme = theme;
	}
	else {
		setupDefaultStyle();
	}


	m_finalDefaultStyle->backgroundMaterial = makeObject_deprecated<Material>();
    detail::UIStyleInstance::updateStyleDataHelper(m_styleContext, nullptr, m_styleContext->mainTheme->defaultStyle(), m_finalDefaultStyle);
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
	auto defaultStyle = makeObject_deprecated<UIStyle>();
	defaultStyle->setupDefault();
	defaultStyle->fontSize = 15;

    auto theme = makeObject_deprecated<UITheme>();
	theme->setDefaultStyle(defaultStyle);

    theme->setSpacing(8); // MUI default
    theme->add(u"control.background", UIColors::get(UIColorHues::Grey, 2));
	theme->add(u"collection.hoverBackground", UIColors::get(UIColorHues::LightGreen, 0));
    theme->add(u"collection.selectedBackground", UIColors::get(UIColorHues::LightGreen, 2));
    theme->add(u"tab.activeBackground", UIColors::get(UIColorHues::White));
    theme->add(u"tab.inactiveBackground", UIColors::get(UIColorHues::Grey, 3));

	Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

	//Color containerBackground = UIColors::get(UIColorHues::Grey, 3);
	//Color activeControlBackground = UIColors::get(UIColorHues::Grey, 0);

	theme->setColor(UIThemeConstantPalette::DefaultBackgroundColor, Color::White.withAlpha(0.5f));
	theme->setColor(UIThemeConstantPalette::DefaultMainColor, UIColors::get(UIColorHues::Grey, 2));
	theme->setColor(UIThemeConstantPalette::DefaultTextColor, Color::Black);
	theme->setColor(UIThemeConstantPalette::PrimaryMainColor, UIColors::get(UIColorHues::LightGreen, 5));
	theme->setColor(UIThemeConstantPalette::PrimaryTextColor, Color::White);
	theme->setColor(UIThemeConstantPalette::SecondaryMainColor, UIColors::get(UIColorHues::Orange, 5));
	theme->setColor(UIThemeConstantPalette::SecondaryTextColor, Color::White);
	theme->setColor(UIThemeConstantPalette::ErrorMainColor, UIColors::get(UIColorHues::Red, 5));
	theme->setColor(UIThemeConstantPalette::ErrorTextColor, Color::White);
	theme->setColor(UIThemeConstantPalette::WarningMainColor, UIColors::get(UIColorHues::Orange, 3));
	theme->setColor(UIThemeConstantPalette::WarningTextColor, Color::Black);
	theme->setColor(UIThemeConstantPalette::InfoMainColor, UIColors::get(UIColorHues::Blue, 3));
	theme->setColor(UIThemeConstantPalette::InfoTextColor, Color::White);
	theme->setColor(UIThemeConstantPalette::SuccessMainColor, UIColors::get(UIColorHues::Green, 3));
	theme->setColor(UIThemeConstantPalette::SuccessTextColor, Color::White);

    auto sheet = makeObject_deprecated<UIStyleSheet>();

    {
		//--------------------------------
        // UIButton
		{
			auto e = sheet->addStyleSet(u"UIButton");
			{
				auto s = e->mainStyleClass()->mainStyle();
				s->minWidth = 64;//
                s->minHeight = theme->lineContentHeight();
                s->margin = Thickness(8);   // TODO: spacing?
                s->padding = theme->spacing(1);
				s->hAlignment = UIHAlignment::Center;
				s->vAlignment = UIVAlignment::Center;
				s->horizontalContentAlignment = UIHAlignment::Center;
				s->verticalContentAlignment = UIVAlignment::Center;
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 3);
				s->cornerRadius = CornerRadius(4);
				s->shadowBlurRadius = 4;
                //s->shadowSpreadRadius = -1;
				s->shadowOffsetY = 1;
                s->shadowColor = Color(0, 0, 0, 0.5);
			}
			//// UIButton.test
			//{
			//	auto s = makeObject_deprecated<UIStyle>();
			//	s->backgroundColor = UIColors::get(UIColorHues::Blue, 4);
			//	c->addClassStyle(u"test", s);
			//}
			// UIButton:MouseOver
			{
				auto s = makeObject_deprecated<UIStyle>();
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
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 6);
			}
			if (auto s = sheet->obtainStyle(u"UIToggleButton:Checked")) {
                s->backgroundColor = UIColors::get(UIColorHues::Grey, 5);
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
				auto s = makeObject_deprecated<UIStyle>();

				s->margin = Thickness(2);
				s->backgroundColor = UIColors::get(UIColorHues::Grey, 4);
				s->cornerRadius = CornerRadius(4);
				s->hAlignment = UIHAlignment::Stretch;
				s->vAlignment = UIVAlignment::Stretch;

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
				s->hAlignment = UIHAlignment::Stretch;
				s->vAlignment = UIVAlignment::Stretch;
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
				s->hAlignment = UIHAlignment::Stretch;
				s->vAlignment = UIVAlignment::Stretch;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-IncreaseButton:MouseOver")) {
				s->backgroundColor = Color::Transparency;
			}
			if (auto s = sheet->obtainStyle(u"UIButton.UITrack-IncreaseButton:Pressed")) {
				s->backgroundColor = Color::Transparency;
			}

		}
        //--------------------------------
        // UIListView
        {
            if (auto s = sheet->obtainStyle(u"UIListView")) {
                s->backgroundColor = theme->get(u"control.background");
                s->padding = Thickness(0, theme->spacing(1));
            }
        }
        //--------------------------------
        // UIListViewItem
        {
            if (auto s = sheet->obtainStyle(u"UIListViewItem")) {
                s->minHeight = 30;
                s->padding = theme->spacing(1);
            }
            if (auto s = sheet->obtainStyle(u"UIListViewItem:Selected")) {
                s->backgroundColor = theme->get(u"collection.selectedBackground");
            }
        }
        //--------------------------------
        // UIListBoxItem
        {
            if (auto s = sheet->obtainStyle(u"UIListBoxItem")) {
                //s->backgroundColor = Color::Green;
            }
            if (auto s = sheet->obtainStyle(u"UIListBoxItem:MouseOver")) {
                s->backgroundColor = theme->get(u"collection.hoverBackground");
            }
            if (auto s = sheet->obtainStyle(u"UIListBoxItem:Selected")) {
                s->backgroundColor = theme->get(u"collection.selectedBackground");
            }
        }
		//--------------------------------
		// UIComboBox
		{
			if (auto s = sheet->obtainStyle(u"UIComboBox")) {
				s->minHeight = theme->lineContentHeight();
				s->setBorderColor(Color::Gray);
				s->borderThickness = 1;
				s->padding = theme->spacing(1);
				s->hAlignment = UIHAlignment::Center;
				s->vAlignment = UIVAlignment::Center;

				auto icon = makeObject_deprecated<UIStyleDecorator>();
				icon->setIconName(u"angle-down", 15);
				icon->m_hAlignment = UIHAlignment::Right;
				icon->m_margin = Thickness(0, 0, theme->spacing(1), 0);
				icon->m_color = Color::Gray;
				s->decorators.add(icon);
			}
		}
		//--------------------------------
		// UIComboBoxItem
		{
			if (auto s = sheet->obtainStyle(u"UIComboBoxItem")) {
			}
			if (auto s = sheet->obtainStyle(u"UIComboBoxItem:MouseOver")) {
				s->backgroundColor = theme->get(u"collection.hoverBackground");
			}
			if (auto s = sheet->obtainStyle(u"UIComboBoxItem:Selected")) {
				s->backgroundColor = theme->get(u"collection.selectedBackground");
			}
		}
        //--------------------------------
        // UITreeItem
        {
            if (auto s = sheet->obtainStyle(u"UITreeItem")) {
                s->minHeight = 30;
                s->hAlignment = UIHAlignment::Stretch;
                s->vAlignment = UIVAlignment::Top;
                //s->borderThickness = 1;
                //s->setBorderColor(Color::Gray);
            }
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander")) {   // VisualState によらず常に有効。個別にしたければ:Normalを付ける。
                s->width = 16;
                s->height = 16;
                s->hAlignment = UIHAlignment::Center;
                s->vAlignment = UIVAlignment::Center;
                s->backgroundColor = Color::Transparency;
            }
            //if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:MouseOver")) {
            //}
            //if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Pressed")) {
            //}
            if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Checked")) {
				auto icon = makeObject_deprecated<UIStyleDecorator>();
				icon->setIconName(u"angle-down", 15);
				s->decorators.add(icon);
            }
			if (auto s = sheet->obtainStyle(u"UIToggleButton.UITreeItem-Expander:Unchecked")) {
				auto icon = makeObject_deprecated<UIStyleDecorator>();
				icon->setIconName(u"angle-right", 15);
				s->decorators.add(icon);
			}
        }
		//--------------------------------
		// UITabBar
        {
            if (auto s = sheet->obtainStyle(u"UITabBar")) {
                s->backgroundColor = theme->get(u"control.background");
            }
        }
		//--------------------------------
		// UITabItem
		{
			if (auto s = sheet->obtainStyle(u"UITabItem")) {
                s->padding = theme->spacing(1);
                s->backgroundColor = theme->get(u"tab.inactiveBackground");
			}
			if (auto s = sheet->obtainStyle(u"UITabItem:Selected")) {
                s->backgroundColor = theme->get(u"tab.activeBackground");
			}
		}
        //--------------------------------
        // UITextField
        {
            if (auto s = sheet->obtainStyle(u"UITextField")) {
                s->padding = Thickness(4);
                s->borderThickness = Thickness(1);
                s->setBorderColor(Color::Gray);
                //s->backgroundColor = theme->get(u"tab.inactiveBackground");
            }
        }
        //--------------------------------
        // UIPopup
        {
            if (auto s = sheet->obtainStyle(u"UIPopup")) {
                s->minWidth = 8;
                s->minHeight = 8;
                s->padding = theme->spacing(1);
                s->backgroundColor = UIColors::get(UIColorHues::White);
                s->cornerRadius = CornerRadius(4);
                s->shadowBlurRadius = 4;
                s->shadowOffsetY = 2;
            }
        }
		//--------------------------------
		// UIDialog
		{
			if (auto s = sheet->obtainStyle(u"UIDialog")) {
				s->minWidth = 200;
				s->minHeight = 200;
				s->padding = theme->spacing(1);
				s->backgroundColor = UIColors::get(UIColorHues::White);
				s->cornerRadius = CornerRadius(4);
				s->shadowBlurRadius = 4;
				s->shadowOffsetY = 2;
			}
		}
    }

    m_styleContext->addStyleSheet(sheet);
	m_styleContext->mainTheme = theme;
    //m_styleContext->build();
}
#endif

} // namespace ln

