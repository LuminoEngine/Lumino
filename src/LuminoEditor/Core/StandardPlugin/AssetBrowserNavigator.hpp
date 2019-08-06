#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../UIExtension.hpp"


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


class AssetBrowserNavigatorExtension
    : public ln::Object
    , public ln::IAssetNavigatorExtension
{
public:
    void init();

protected:
    virtual const ln::Char* id() const { return u"AD796B88-7EAA-4AC9-B9F8-22CB366823E2"; }
    virtual const ln::Char* displayName() const { return u"AssetBrowserNavigatorExtension"; }
    virtual void onAttached() override;
    virtual void onDetached() override;
    virtual ln::NavigationMenuItem* getNavigationMenuItem() override;

private:
    Ref<ln::NavigationMenuItem> m_navbarItemContent;
    Ref<AssetBrowserTreeView> m_treeView;
};
