#pragma once
#include "NavigatorManager.hpp"

class AssetBrowserTreeView : public ln::UITreeView
{
public:
    void init();

protected:
    virtual void onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e) override;
    virtual Ref<ln::UITreeItem> onRenderItem(ln::UICollectionItemModel* viewModel) override;

private:
    Ref<ln::UIFileSystemCollectionModel> m_model;
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

