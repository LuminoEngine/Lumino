#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/ToolPanesArea.hpp"
#include "../App/NavigatorManager.hpp"
#include "../AssetEditor/AssetEditorModel.hpp"


namespace lna {
class TilemapSceneNavigator;
class TilemapSceneEditorModel;
class TilemapSceneModePane;

class TilemapSceneEditor
    : public lna::AssetEditorModel
{
public:
    ln::Result init();
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(lna::EditorPaneKind kind) override;

	ln::TilemapComponent* targetTilemapComponent() const { return m_tilemap->tilemapComponent(); }

    void clearSelectedObjects();
    void addSelectedObject(ln::WorldObject* obj);


private:
	void WorldRenderView_OnUIEvent(ln::UIEventArgs* e);
	void handleTickEvent(ln::UITimerEventArgs* e);

    Ref<ln::AssetModel> m_assetModel;
	Ref<TilemapSceneEditorModel> m_model;

    Ref<TilemapSceneModePane> m_modePane;
    Ref<ln::EditorPane> m_inspectorPane;

    Ref<ln::List<Ref<ln::EditorPane>>> m_modePanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_inspectorPanes;
    Ref<ln::List<Ref<ln::EditorPane>>> m_toolPanes;
    Ref<ln::UIVBoxLayout3> m_mainLayout;

    // Tilemap
	Ref<ln::UIActiveTimer> m_timer;
    Ref<ln::World> m_mainWorld;
	Ref<ln::UIViewport> m_mainViewport;
	Ref<ln::WorldRenderView> m_mainWorldRenderView;
	Ref<ln::Camera> m_mainCamera;
	Ref<ln::Tilemap> m_tilemap;
	Ref<ln::TilemapLayer> m_currentLayer;
    bool m_tilePutting = true;

    // Editing
    ln::List<ln::WorldObject*> m_selectedObjects;
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
