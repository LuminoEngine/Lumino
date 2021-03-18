
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilemapSceneEditorModel.hpp"
#include "TilemapSceneNavigator.hpp"

namespace lna {
	
//==============================================================================
// TilemapSceneListPane

void TilemapSceneListPane::init()
{
    UIControl::init();

    auto layout1 = ln::makeObject<ln::UIBoxLayout3>();
    layout1->setOrientation(ln::Orientation::Vertical);
    addElement(layout1);

    auto layout2 = ln::makeObject<ln::UIHBoxLayout2>();
    layout1->addChild(layout2);
    {
        auto caption = ln::UITextBlock::create(u"Scene");
        caption->setMargin(ln::Thickness(8, 0));    // TODO: theme からとりたい
        caption->setVAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilemapSceneListPane::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilemapSceneListPane::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->mainProject();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Scenes");
    ln::FileSystem::createDirectory(m_assetRootDir);

    m_model = ln::makeObject<TilemapSceneListModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilemapSceneListPane::addButton_onClick()
{
	auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Scene-", ln::AssetModel::AssetFileExtension.c_str());
	TilemapSceneEditorModel::createNewTilemapSceneAsset(EditorApplication::instance()->editorContext(), path);

    //auto Scene = ln::makeObject<ln::Scene>();
    //auto asset = ln::makeObject<ln::AssetModel>(Scene);

    //auto project = lna::Workspace::instance()->project();


    //asset->saveInternal(path);

    m_model->refresh();
}

void TilemapSceneListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemViewModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// TilemapSceneNavigator

void TilemapSceneNavigator::init()
{
    m_navigationBarItem = ln::makeObject<ln::UIIcon>();
    m_navigationBarItem->setIconName(u"globe");
    m_navigationBarItem->setHAlignment(ln::HAlignment::Center);
    m_navigationBarItem->setVAlignment(ln::VAlignment::Center);
    m_navigationBarItem->setFontSize(24);
    
    m_sceneListPane = ln::makeObject<TilemapSceneListPane>();
}

ln::UIElement* TilemapSceneNavigator::getNavigationMenuItem()
{
    return m_navigationBarItem;
}

ln::UIElement* TilemapSceneNavigator::getNavigationPane()
{
    return m_sceneListPane;
}

} // namespace lna
