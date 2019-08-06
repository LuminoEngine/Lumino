#include "../UIExtension.hpp"
#include "TilesetEditor.hpp"

void TilesetNavigatorExtension::onAttached()
{
    m_item = ln::makeObject<ln::NavigationMenuItem>();
    m_item->setIconName(u"th");
}

ln::NavigationMenuItem* TilesetNavigatorExtension::getNavigationMenuItem()
{
    return m_item;
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
