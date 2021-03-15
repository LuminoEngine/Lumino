#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../App/NavigatorManager.hpp"

class NavigationBarItem;

namespace lna {

class TilesetListModel
    : public ln::UIFileSystemCollectionModel
{
protected:
    virtual bool onTestFilter(const ln::Path& path) override { return path.hasExtension(ln::AssetModel::AssetFileExtension); }
};

class TilesetListPane
    : public ln::UIControl
{
public:
    void init();

private:
    void addButton_onClick();
    void listView_onItemClick(ln::UIClickEventArgs* e);

    Ref<TilesetListModel> m_model;
    Ref<ln::UIListView> m_listview;
    ln::Path m_assetRootDir;
};

class TilesetNavigator
    : public Navigator
{
public:
    void init();
    virtual ln::UIElement* getNavigationMenuItem() override;
    virtual ln::UIElement* getNavigationPane() override;

private:
    Ref<ln::UIIcon> m_navigationBarItem;
    Ref<TilesetListPane> m_tilesetListPane;
};

} // namespace lna

