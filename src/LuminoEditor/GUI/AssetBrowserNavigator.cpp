
#include <Project.hpp>
#include "AssetBrowserNavigator.hpp"

//==============================================================================
// AssetBrowserNavigator

ln::UIElement* AssetBrowserNavigator::createNavigationBarItem()
{
    m_navbarItem = ln::makeObject<ln::UIIcon>();
    m_navbarItem->setIconName(u"file");
    return m_navbarItem;
}

ln::UIElement* AssetBrowserNavigator::createView()
{
    return nullptr;
}
