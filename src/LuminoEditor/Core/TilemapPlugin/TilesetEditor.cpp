
#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilesetEditor.hpp"

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
        caption->setVerticalAlignment(ln::VAlignment::Center);
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


    auto project = lna::Workspace::instance()->project();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Tilesets");

    m_model = ln::makeObject<TilesetListModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilesetListPane::addButton_onClick(ln::UIEventArgs* e)
{
    auto tileset = ln::makeObject<ln::Tileset>();
    auto asset = ln::makeObject<ln::AssetModel>(tileset);

    auto project = lna::Workspace::instance()->project();

    auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Tileset-", ln::AssetModel::AssetFileExtension.c_str());
        
    asset->saveInternal(path);

    m_model->refresh();
}

void TilesetListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// TilesetNavigator

void TilesetNavigator::init()
{
    m_navigationBarItem = ln::makeObject<ln::UIIcon>();
    m_navigationBarItem->setIconName(u"th");
    m_navigationBarItem->setHorizontalAlignment(ln::HAlignment::Center);
    m_navigationBarItem->setVerticalAlignment(ln::VAlignment::Center);
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

//==============================================================================
// TilesetEditorExtensionModule

void TilesetEditorExtensionModule::onActivate(lna::EditorContext* context)
{
    m_tilesetNavigator = ln::makeObject<TilesetNavigator>();
    context->mainWindow()->navigatorManager()->addNavigator(m_tilesetNavigator);
}

void TilesetEditorExtensionModule::onDeactivate(lna::EditorContext* context)
{
}

} // namespace lna

#if 0
//==============================================================================
// TilesetList

void TilesetList::init()
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
        caption->setVerticalAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilesetList::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilesetList::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->project();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Tilesets");

    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilesetList::addButton_onClick(ln::UIEventArgs* e)
{
    auto tileset = ln::makeObject<ln::Tileset>();
    auto asset = ln::makeObject<ln::AssetModel>(tileset);

    auto project = lna::Workspace::instance()->project();

    auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Tileset-", ln::AssetModel::AssetFileExtension.c_str());
        
    asset->saveInternal(path);

    m_model->refresh();
}

void TilesetList::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// TilesetNavigatorExtension

void TilesetNavigatorExtension::onAttached()
{
    m_item = ln::makeObject<ln::NavigationMenuItem>();
    m_item->setIconName(u"th");

	m_tilesetList = ln::makeObject<TilesetList>();
}

ln::NavigationMenuItem* TilesetNavigatorExtension::getNavigationMenuItem()
{
    return m_item;
}

ln::UIElement* TilesetNavigatorExtension::getNavigationPane()
{
	return m_tilesetList;
}

//==============================================================================
// TilesetImporter

Ref<ln::AssetModel> TilesetImporter::onImport(const ln::Path& sourceFilePath)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

//==============================================================================
// TilesetImporterExtension

int TilesetImporterExtension::matchFilePath(const ln::Path& filePath)
{
    if (filePath.hasExtension(u"png")) {    // TODO: texture support
        if (filePath.isRelative() && ln::Path::compare(filePath.parent().fileName(), u"Tileset") == 0) {
            return BasePriority + 1;
        }
        else {
            return BasePriority;
        }
    }
    return 0;
}

Ref<ln::AssetImporter> TilesetImporterExtension::createImporter(const ln::Char* assetSourceFilePath)
{
    return nullptr;
}

//==============================================================================
// TilesetEditor

void TilesetEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
}

void TilesetEditor::onClosed()
{
}

//==============================================================================
// TilesetEditorExtension

TilesetEditorExtension::TilesetEditorExtension()
{
}

ln::Ref<ln::AssetEditor> TilesetEditorExtension::createEditor()
{
    return ln::makeObject<TilesetEditor>();
}

#endif

