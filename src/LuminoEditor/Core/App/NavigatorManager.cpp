
#include <Project.hpp>
#include <PluginManager.hpp>
#include <UIExtension.hpp>
#include "Application.hpp"
//#include "AssetBrowserNavigator.hpp"
#include "NavigatorManager.hpp"

//==============================================================================
// NavigationBarItem

void NavigationBarItem::init(NavigatorManager* manager, ln::IAssetNavigatorExtension* navigator)
{
	UICollectionItem::init();
	m_navigatorManager = manager;
	m_navigator = navigator;
}

void NavigationBarItem::onSelected(ln::UIEventArgs* e)
{
	m_navigatorManager->setCurrent(m_navigator);
}

void NavigationBarItem::onUnselected(ln::UIEventArgs* e)
{
}

//==============================================================================
// NavigationBar

void NavigationBar::init(NavigatorManager* manager)
{
    UIItemsControl::init();
	m_navigatorManager = manager;
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

void NavigationBar::addItem(ln::IAssetNavigatorExtension* ext)
{
    // TODO: ContentAlignment でカバーしたい
	ln::UIElement* element = ext->getNavigationMenuItem();
    element->setHorizontalAlignment(ln::HAlignment::Center);
    element->setVerticalAlignment(ln::VAlignment::Center);
    element->setFontSize(24);

	auto item = ln::makeObject<NavigationBarItem>(m_navigatorManager, ext);
	item->addElement(element);
    item->setWidth(ItemSize);
    item->setHeight(ItemSize);

	UIItemsControl::addItem(item);
}

////==============================================================================
//// Navigator
//
//Navigator::Navigator()
//{
//}
//
//==============================================================================
// NavigatorManager

NavigatorManager::NavigatorManager()
	//: m_viewExpandingSize(200)
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

    m_navigationBar = ln::makeObject<NavigationBar>(this);
    m_layout->addChild(m_navigationBar);

	m_switchLayout = ln::makeObject<ln::UISwitchLayout>();
	m_switchLayout->setActiveIndex(-1);
	m_layout->addChild(m_switchLayout);
}

void NavigatorManager::resetNavigators()
{
	if (!m_navigators.isEmpty()) {
		LN_NOTIMPLEMENTED();
	}

    //m_assetBrowserNavigator = ln::makeObject<AssetBrowserNavigator>();
    //m_navigationBar->addItem(m_assetBrowserNavigator->createNavigationBarItem());
    //m_layout->addChild(m_assetBrowserNavigator->createView());


    auto exts = EditorApplication::instance()->mainProject()->pluginManager()->getAssetNavigatorExtensions();
    for (auto& ext : exts) {
        ext->onAttached();
        m_navigationBar->addItem(ext);
		m_switchLayout->addChild(ext->getNavigationPane());
		m_navigators.add(ext);
    }

	if (!m_navigators.isEmpty()) {
		setCurrent(m_navigators[0]);
	}
}

void NavigatorManager::setCurrent(ln::IAssetNavigatorExtension* nav)
{
	int index = m_navigators.indexOf(nav);
    if (nav) {
		m_switchLayout->setActiveIndex(index);
        navigationViewOpen();
    }
    else {
        navigationViewClose();
    }
}
