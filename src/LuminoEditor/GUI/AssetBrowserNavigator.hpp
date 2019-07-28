#pragma once
#include "NavigatorManager.hpp"

class AssetBrowserTreeView : public ln::UITreeView
{
public:

protected:
    virtual void onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e) override;
};



class AssetBrowserNavigator : public Navigator
{
public:
    virtual ln::UIElement* createNavigationBarItem();
    virtual ln::UIElement* createView();

private:
    Ref<ln::UIIcon> m_navbarItem;
    Ref<AssetBrowserTreeView> m_treeView;
};

