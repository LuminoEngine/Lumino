#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/ToolPanesArea.hpp"
#include "../App/NavigatorManager.hpp"

namespace lna {
class TilemapSceneEditorModel;
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
    void addButton_onClick();
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

} // namespace lna 
