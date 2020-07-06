#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../UIExtension.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"
#include "../App/NavigatorManager.hpp"

class AssetBrowserNavigatorExtension;
namespace lna {
class Project;

class AssetBrowserTreeViewModel
    : public ln::UIFileSystemCollectionModel
{
protected:
    virtual bool onTestFilter(const ln::Path& path) override;
};

class AssetBrowserTreeView
    : public ln::UITreeView
{
public:
    void init(AssetBrowserNavigatorExtension* owner);
    void setPath(const ln::Path& path);

protected:
    virtual void onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e) override;
    virtual Ref<ln::UITreeItem> onRenderItem(ln::UICollectionItemViewModel* viewModel) override;

private:
    AssetBrowserNavigatorExtension* m_owner;
    Ref<AssetBrowserTreeViewModel> m_model;
};


class AssetBrowserListViewModel
    : public ln::UIFileSystemCollectionModel
{
protected:
    virtual bool onTestFilter(const ln::Path& path) override;

};

class AssetBrowserListView
    : public ln::UIListView
{
public:
    void init(AssetBrowserNavigatorExtension* owner);
    void setPath(const ln::Path& path);
    const Ref<AssetBrowserListViewModel>& model() const { return m_model; }
    const ln::Path& path() const { return m_path; }

protected:

private:
    AssetBrowserNavigatorExtension* m_owner;
    Ref<AssetBrowserListViewModel> m_model;
    ln::Path m_path;
};



//class AssetBrowserNavigatorExtension
//    : public ln::Object
//    , public ln::IAssetNavigatorExtension
//{
//public:
//    void init();
//    void setAssetListPathFromTreeClick(const ln::Path& path);
//
//protected:
//    virtual const ln::Char* id() const { return u"AD796B88-7EAA-4AC9-B9F8-22CB366823E2"; }
//    virtual const ln::Char* displayName() const { return u"AssetBrowserNavigatorExtension"; }
//    virtual void onAttached() override;
//    virtual void onDetached() override;
//    virtual ln::NavigationMenuItem* getNavigationMenuItem() override;
//	virtual ln::UIElement* getNavigationPane() override;
//
//private:
//    void onImport();
//
//    Ref<ln::NavigationMenuItem> m_navbarItemContent;
//    Ref<ln::UISplitter> m_splitter;
//    Ref<AssetBrowserTreeView> m_treeView;
//    Ref<ln::UIVBoxLayout2> m_layout2;
//    Ref<ln::UIButton> m_importButton;
//    Ref<AssetBrowserListView> m_listView;
//};
//
//class AssetBrowserEditor : public lna::AssetEditorModel
//{
//public:
//    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
//    virtual void onClosed() override;
//};
//
//class AssetBrowserEditorExtension
//    : public ln::Object
//    , public ln::IAssetEditorExtension
//{
//public:
//    AssetBrowserEditorExtension();
//    virtual const ln::Char* id() const { return u"769E40B5-AB4D-45A0-94BC-21F84F958DE6"; }
//    virtual const ln::Char* displayName() const { return u"AssetBrowserEditorExtension"; }
//    virtual const ln::Char* typeKeyword() const { return nullptr; }
//    virtual ln::Ref<lna::AssetEditorModel> createEditor() override;
//
//private:
//};

class AssetBrowserPane
    : public NavigatorContentPane
{
public:
    bool init(lna::EditorContext* context);

private:
};

class AssetBrowserNavigator
    : public Navigator
{
public:
    bool init(lna::EditorContext* context);
    ln::UIElement* getNavigationMenuItem() override;
    ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::UIIcon> m_navigationItem;
    Ref<AssetBrowserPane> m_mainPane;
};

} // namespace lna
