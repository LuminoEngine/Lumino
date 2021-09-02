
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "../App/EditorContext.hpp"
#include "TilesetNavigator.hpp"

namespace lna {
    
//==============================================================================
// TilesetListPane

void TilesetListPane::init()
{
    UIControl::init();

    auto layout1 = ln::makeObject<ln::UIBoxLayout3>();
    layout1->setOrientation(ln::Orientation::Vertical);
    addElement(layout1);

    auto layout2 = ln::makeObject<ln::UIHBoxLayout2>();
    layout1->addChild(layout2);
    {
        auto caption = ln::UITextBlock::create(u"Tileset");
        caption->setMargin(ln::Thickness(8, 0));    // TODO: theme ‚©‚ç‚Æ‚è‚½‚¢
        caption->setVAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilesetListPane::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilesetListPane::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->mainProject();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Tilesets");

    m_model = ln::makeObject<TilesetListModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilesetListPane::addButton_onClick()
{

	auto tilesetTexture = ln::Texture2D::load(u"32066696-1621-4EED-820D-535BB2F22A9D");
	auto tilesetMaterial = ln::makeObject<ln::Material>();
	tilesetMaterial->setMainTexture(tilesetTexture);

	auto tileset = ln::makeObject<ln::Tileset>();
	tileset->reset(tilesetMaterial, 16, 16);


    // make unique path
	auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Tileset-", ln::AssetModel::AssetFileExtension.c_str());
	EditorContext::current()->assetDatabase()->createAsset(tileset, path);

    m_model->refresh();
}

void TilesetListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemViewModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// TilesetNavigator

void TilesetNavigator::init()
{
    m_navigationBarItem = ln::makeObject<ln::UIIcon>();
    m_navigationBarItem->setIconName(u"th");
    m_navigationBarItem->setHAlignment(ln::HAlignment::Center);
    m_navigationBarItem->setVAlignment(ln::VAlignment::Center);
    m_navigationBarItem->setFontSize(24);
    
    m_tilesetListPane = ln::makeObject<TilesetListPane>();
}

ln::UIElement* TilesetNavigator::getNavigationMenuItem()
{
    return m_navigationBarItem;
}

ln::UIElement* TilesetNavigator::getNavigationPane()
{
    return m_tilesetListPane;
}

} // namespace lna
