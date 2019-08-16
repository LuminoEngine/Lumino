#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../UIExtension.hpp"


class AssetBrowserTreeView : public ln::UITreeView
{
public:
    void init();
    void setPath(const ln::Path& path);

protected:
    virtual void onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e) override;
    virtual Ref<ln::UITreeItem> onRenderItem(ln::UICollectionItemModel* viewModel) override;

private:
    Ref<ln::UIFileSystemCollectionModel> m_model;
};


class AssetBrowserListView : public ln::UIListView
{
public:
    void init();
    void setPath(const ln::Path& path);

protected:

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
	virtual ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::NavigationMenuItem> m_navbarItemContent;
    Ref<ln::UISplitter> m_splitter;
    Ref<AssetBrowserTreeView> m_treeView;
    Ref<AssetBrowserListView> m_listView;
};

class AssetBrowserEditor : public ln::AssetEditor
{
public:
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
};

class AssetBrowserEditorExtension
    : public ln::Object
    , public ln::IAssetEditorExtension
{
public:
    AssetBrowserEditorExtension();
    virtual const ln::Char* id() const { return u"769E40B5-AB4D-45A0-94BC-21F84F958DE6"; }
    virtual const ln::Char* displayName() const { return u"AssetBrowserEditorExtension"; }
    virtual const ln::Char* typeKeyword() const { return nullptr; }
    virtual ln::Ref<ln::AssetEditor> createEditor() override;

private:
};
