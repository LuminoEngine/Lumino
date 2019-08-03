
#include "AssetBrowserNavigator.hpp"
#include "NavigatorManager.hpp"

//==============================================================================
// NavigationBar

void NavigationBar::init()
{
    UIItemsControl::init();
    //m_layout = ln::makeObject<ln::UIVBoxLayout2>();
    //addElement(m_layout);
    //setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());
    setItemsLayoutPanel(ln::makeObject<ln::UIVBoxLayout2>());
    setWidth(ItemSize);
    setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 8));
    setHorizontalAlignment(ln::HAlignment::Left);
    setVerticalAlignment(ln::VAlignment::Stretch);
    //setHorizontalContentAlignment(ln::HAlignment::Center);
    //setVerticalContentAlignment(ln::VAlignment::Center);
}

void NavigationBar::addItem(ln::UIElement* element)
{
    
	//auto textblock = ln::makeObject<ln::UITextBlock>();
	//textblock->setText(text);
 //   textblock->setTextColor(ln::Color::White);
 //   textblock->setHorizontalAlignment(ln::HAlignment::Center);
 //   textblock->setVerticalAlignment(ln::VAlignment::Center);

    // TODO: ContentAlignment でカバーしたい
    element->setHorizontalAlignment(ln::HAlignment::Center);
    element->setVerticalAlignment(ln::VAlignment::Center);
    element->setFontSize(24);

	auto item = ln::makeObject<ln::UICollectionItem>();
	item->addElement(element);
    item->setWidth(ItemSize);
    item->setHeight(ItemSize);
    //item->setVerticalAlignment(ln::VAlignment::Top);    // TODO: demo 臨時対応


    // TODO: demo
    static int count = 0;
    if (count == 0) {
        element->setTextColor(ln::Color::White);
        item->setBorderThickness(ln::Thickness(4, 0, 0, 0));
        item->setBorderColor(ln::UIColors::get(ln::UIColorHues::LightGreen));
    }
    else {
        element->setTextColor(ln::UIColors::get(ln::UIColorHues::Grey, 4));
    }
    count++;


	UIItemsControl::addItem(item);
}

//==============================================================================
// Navigator

Navigator::Navigator()
{
}

//==============================================================================
// NavigatorManager

NavigatorManager::NavigatorManager()
{
}

void NavigatorManager::init()
{
    UIControl::init();

    m_layout = ln::makeObject<ln::UIStackLayout2>();
    m_layout->setOrientation(ln::Orientation::Horizontal);
    //m_layout->lastStretch = true;
    //setLayoutPanel(m_layout);
    addElement(m_layout);

    m_navigationBar = ln::makeObject<NavigationBar>();
    m_layout->addChild(m_navigationBar);
}

void NavigatorManager::resetNavigators()
{
    m_assetBrowserNavigator = ln::makeObject<AssetBrowserNavigator>();
    m_navigationBar->addItem(m_assetBrowserNavigator->createNavigationBarItem());
    m_layout->addChild(m_assetBrowserNavigator->createView());


    //// Test:
    //{
    //    auto icon = ln::makeObject<ln::UIIcon>();
    //    icon->setIconName(u"music");
    //    m_navigationBar->addItem(icon);
    //    icon = ln::makeObject<ln::UIIcon>();
    //    icon->setIconName(u"cog");
    //    m_navigationBar->addItem(icon);
    //}

    setCurrent(m_assetBrowserNavigator);
}

void NavigatorManager::setCurrent(Navigator* nav)
{
    if (nav) {
        navigationViewOpen();
    }
    else {
        navigationViewClose();
    }
}
