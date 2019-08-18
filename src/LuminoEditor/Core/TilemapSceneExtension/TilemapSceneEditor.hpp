#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/ToolPanesArea.hpp"
#include "../App/NavigatorManager.hpp"


namespace lna {
class TilemapSceneModePane;

class TilemapSceneListModel
    : public ln::UIFileSystemCollectionModel
{
protected:
    virtual bool onTestFilter(const ln::Path& path) override { return path.hasExtension(ln::AssetModel::AssetFileExtension); }
};

class TilemapSceneListPane
    : public ln::UIControl
{
public:
    void init();

private:
    void addButton_onClick(ln::UIEventArgs* e);
    void listView_onItemClick(ln::UIClickEventArgs* e);

    Ref<TilemapSceneListModel> m_model;
    Ref<ln::UIListView> m_listview;
    ln::Path m_assetRootDir;
};

class TilemapSceneNavigator
    : public Navigator
{
public:
    void init();
    virtual ln::UIElement* getNavigationMenuItem() override;
    virtual ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::UIIcon> m_navigationBarItem;
    Ref<TilemapSceneListPane> m_sceneListPane;
};

class TilemapSceneEditor
    : public ln::AssetEditor
{
public:
    ln::Result init();
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(ln::EditorPaneKind kind) override;

private:
    Ref<TilemapSceneModePane> m_modePane;
    Ref<ln::EditorPane> m_inspectorPane;

    Ref<ln::List<Ref<ln::EditorPane>>> m_modePanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_inspectorPanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_toolPanes;
};

class TilemapSceneEditorPloxy
    : public ln::AssetEditorPloxy
{
public:
    virtual ln::String targetTypeName() override { return u"Scene"; }
    virtual Ref<ln::AssetEditor> createEditor() override;
};

class TilemapSceneEditorExtensionModule
    : public ln::Object
    , public ln::IPluginModule
{
public:
    virtual void onActivate(lna::EditorContext* context) override;
    virtual void onDeactivate(lna::EditorContext* context) override;

private:
    Ref<TilemapSceneNavigator> m_navigator;
    Ref<TilemapSceneEditorPloxy> m_editorPloxy;
};

} // namespace lna 

#if 0

class SceneList
    : public ln::UIControl
{
public:
    void init();

private:
    void addButton_onClick(ln::UIEventArgs* e);
    void listView_onItemClick(ln::UIClickEventArgs* e);

    Ref<ln::UIFileSystemCollectionModel> m_model;
    Ref<ln::UIListView> m_listview;
    ln::Path m_assetRootDir;
};

class SceneNavigatorExtension
    : public ln::Object
    , public ln::IAssetNavigatorExtension
{
public:
protected:
    virtual const ln::Char* id() const { return u"F181C3E6-BD91-47B1-939A-D00357F345A1"; }
    virtual const ln::Char* displayName() const { return u"SceneNavigatorExtension"; }
    virtual void onAttached() override;
    virtual void onDetached() override {}
    virtual ln::NavigationMenuItem* getNavigationMenuItem() override;
    virtual ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::NavigationMenuItem> m_item;
    Ref<ln::UIElement> m_list;
};

class TilemapSceneEditor : public ln::AssetEditor
{
public:
    ln::Result init();
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(ln::EditorPaneKind kind) override;

private:
    Ref<ln::EditorPane> m_modePane;
    Ref<ln::EditorPane> m_inspectorPane;

    Ref<ln::List<Ref<ln::EditorPane>>> m_modePanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_inspectorPanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_toolPanes;
};

class TilemapSceneEditorExtension
    : public ln::Object
    , public ln::IAssetEditorExtension
{
public:
    TilemapSceneEditorExtension();
    virtual const ln::Char* id() const { return u"92AFD1C1-4A6D-4DC3-8B38-5D842F6346AD"; }
    virtual const ln::Char* displayName() const { return u"TilemapSceneEditorExtension"; }
    virtual const ln::Char* typeKeyword() const { return u"Scene"; }
    virtual ln::Ref<ln::AssetEditor> createEditor() override;

private:
};

#endif
