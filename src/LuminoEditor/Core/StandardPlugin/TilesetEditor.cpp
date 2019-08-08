#include "../UIExtension.hpp"
#include "TilesetEditor.hpp"

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
        auto caption = ln::UITextBlock::create(u"Tilesets");
        caption->setVerticalAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->setBackgroundColor(ln::Color::Red);
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    //m_listview->setVerticalAlignment(ln::VAlignment::Stretch);
    layout1->addChild(m_listview);
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
// TilesetEditorModel

void TilesetEditorModel::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
}

void TilesetEditorModel::onClosed()
{
}

//==============================================================================
// TilesetEditorPlugin

TilesetEditorPlugin::TilesetEditorPlugin()
{
}

ln::Ref<ln::AssetEditorViewModel> TilesetEditorPlugin::createEditor()
{
    return ln::makeObject<TilesetEditorModel>();
}
