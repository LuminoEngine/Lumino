#pragma once
#include "NavigatorManager.hpp"

class AssetBrowserTreeView : public ln::UITreeView
{
public:
};



class AssetBrowserNavigator : public Navigator
{
public:
    virtual ln::UIElement* createNavigationBarItem();
    virtual ln::UIElement* createView();

private:
    Ref<ln::UIIcon> m_navbarItem;

};

