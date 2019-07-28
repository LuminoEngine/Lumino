
#include "AssetBrowserNavigator.hpp"
#include "NavigatorManager.hpp"

//==============================================================================
// NavigationBar

void NavigationBar::init()
{
    UIItemsControl::init();
    setWidth(40);
    setBackgroundColor(ln::Color::Gray);
    setHorizontalAlignment(ln::HAlignment::Left);
    setVerticalAlignment(ln::VAlignment::Stretch);
}

void NavigationBar::addItem(ln::UIElement* element)
{
	//auto textblock = ln::makeObject<ln::UITextBlock>();
	//textblock->setText(text);
 //   textblock->setTextColor(ln::Color::White);
 //   textblock->setHorizontalAlignment(ln::HAlignment::Center);
 //   textblock->setVerticalAlignment(ln::VAlignment::Center);

	auto item = ln::makeObject<ln::UICollectionItem>();
	item->addElement(element);
    item->setWidth(ItemSize);
    item->setHeight(ItemSize);
    item->setBorderThickness(ln::Thickness(4, 0, 0, 0));
    item->setBorderColor(ln::UIColors::get(ln::UIColorHues::LightGreen));

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

    m_layout = ln::makeObject<ln::UIStackLayout>();
    m_layout->setOrientation(ln::Orientation::Horizontal);
    m_layout->lastStretch = true;
    setLayoutPanel(m_layout);

    m_navigationBar = ln::makeObject<NavigationBar>();
    addElement(m_navigationBar);
}

void NavigatorManager::resetNavigators()
{
    m_assetBrowserNavigator = ln::makeObject<AssetBrowserNavigator>();
    m_navigationBar->addItem(m_assetBrowserNavigator->createNavigationBarItem());




    auto model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    model->setRootPath(u"D:/Proj/LN/Lumino");

    auto treeView = ln::makeObject<ln::UITreeView>();
    treeView->setViewModel(model);
    treeView->setWidth(200);
    treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
    addElement(treeView);
    treeView->getGridLayoutInfo()->layoutRow = 0;

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
