
#include "../Project/Project.hpp"
#include "../Project/PluginManager.hpp"
#include <UIExtension.hpp>
#include "Application.hpp"
//#include "AssetBrowserNavigator.hpp"
#include "NavigatorManager.hpp"

//==============================================================================
// NavigationBarItem

void NavigationBarItem::init(NavigatorManager* manager, Navigator* navigator)
{
	UICollectionItem::init();
	m_navigatorManager = manager;
	m_navigator = navigator;
}

void NavigationBarItem::addIcon(const ln::StringView& iconName)
{
    auto icon = ln::makeObject<ln::UIIcon>();
    icon->setIconName(iconName);
    icon->setHAlignment(ln::UIHAlignment::Center);
    icon->setVAlignment(ln::UIVAlignment::Center);
    icon->setFontSize(24);
    addElement(icon);
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
    setHAlignment(ln::UIHAlignment::Left);
    setVAlignment(ln::UIVAlignment::Stretch);
    //setHorizontalContentAlignment(ln::UIHAlignment::Center);
    //setVerticalContentAlignment(ln::UIVAlignment::Center);
}

void NavigationBar::addNavigator(Navigator* navigator)
{
    //auto item = navigator->getNavigationMenuItem();
    //item->setWidth(ItemSize);
    //item->setHeight(ItemSize);

    //UIItemsControl::addItem(item);


    // TODO: ContentAlignment でカバーしたい
    ln::UIElement* element = navigator->getNavigationMenuItem();
    element->setHAlignment(ln::UIHAlignment::Center);
    element->setVAlignment(ln::UIVAlignment::Center);
    element->setFontSize(24);
    
    auto item = ln::makeObject<NavigationBarItem>(m_navigatorManager, navigator);
    item->addElement(element);
    item->setWidth(ItemSize);
    item->setHeight(ItemSize);
    
    UIItemsControl::addItem(item);

	m_navigatorContainers.add(item);
}

void NavigationBar::removeNavigator(Navigator* navigator)
{
	auto item = m_navigatorContainers.findIf([&](auto& x) { return x->navigator() == navigator; });
	if (item) {
		UIItemsControl::removeItem(*item);
	}
}

//==============================================================================
// NavigatorContentPane

void NavigatorContentPane::onUpdateStyle(const ln::UIStyleContext* styleContext, const ln::detail::UIStyleInstance* finalStyle)
{
    UIElement::onUpdateStyle(styleContext, finalStyle);

    setBackgroundColor(styleContext->mainTheme->color(ln::UIThemeConstantPalette::ControlBackgroundColor));
}

//==============================================================================
// NavigatorManager

NavigatorManager::NavigatorManager()
	//: m_viewExpandingSize(200)
{
}

void NavigatorManager::init()
{
    UIControl::init();

    m_layout = ln::makeObject<ln::UIBoxLayout>();
    m_layout->setOrientation(ln::UILayoutOrientation::Horizontal);
    //m_layout->lastStretch = true;
    //setLayoutPanel(m_layout);
    addElement(m_layout);

    //m_layout->setBackgroundColor(ln::Color::Red);

    m_navigationBar = ln::makeObject<NavigationBar>(this);
    m_layout->addChild(m_navigationBar);

	m_switchLayout = ln::makeObject<ln::UISwitchLayout>();
    m_switchLayout->getGridLayoutInfo()->layoutWeight = 1;  // fill in box layout
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


  //  auto exts = EditorApplication::instance()->mainProject()->pluginManager()->getAssetNavigatorExtensions();
  //  for (auto& ext : exts) {
  //      ext->onAttached();
  //      m_navigationBar->addItem(ext);
		//m_switchLayout->addChild(ext->getNavigationPane());
		//m_navigators.add(ext);
  //  }

	if (!m_navigators.isEmpty()) {
		setCurrent(m_navigators[0]);
	}
}

void NavigatorManager::addNavigator(Navigator* navigator)
{
    m_navigationBar->addNavigator(navigator);
    m_switchLayout->addChild(navigator->getNavigationPane());
    m_navigators.add(navigator);
}

void NavigatorManager::removeNavigator(Navigator* navigator)
{
    m_navigationBar->removeNavigator(navigator);
    m_switchLayout->removeChild(navigator->getNavigationPane());
    m_navigators.remove(navigator);
}

void NavigatorManager::unloadAdditionalNavigators()
{
	for (auto& nav : m_navigators) {
		m_navigationBar->removeNavigator(nav);
	}
	m_navigators.clear();
}

void NavigatorManager::setCurrent(Navigator* navigator)
{
	int index = m_navigators.indexOf(navigator);
    if (navigator) {
		m_switchLayout->setActiveIndex(index);
        navigationViewOpen();
    }
    else {
        navigationViewClose();
    }
}
