
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilesetNavigator.hpp"
#include "TilesetEditor.hpp"


namespace lna {

//==============================================================================
// TilesetView

void TilesetView::setTileset(ln::Tileset* tileset)
{
    m_tileset = tileset;
}

void TilesetView::onRender(ln::UIRenderingContext* context)
{
    if (m_tileset && m_tileset->material()) {
        auto material = m_tileset->material();
        auto texture = material->mainTexture();
        if (texture) {
            context->drawImage(ln::Rect(0, 0, texture->width(), texture->height()), material);
        }
    }
}

//==============================================================================
// TilesetEditor

ln::Result TilesetEditor::init()
{
    m_tilesetView = ln::makeObject<TilesetView>();
    m_tilesetView->setHorizontalAlignment(ln::HAlignment::Stretch);
    m_tilesetView->setVerticalAlignment(ln::VAlignment::Stretch);
    return true;
}

void TilesetEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    frame->addElement(m_tilesetView);

    auto tileset = dynamic_cast<ln::Tileset*>(asset->target());
    m_tilesetView->setTileset(tileset);
}

void TilesetEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> TilesetEditor::getEditorPanes(lna::EditorPaneKind kind)
{
    return nullptr;
}

//==============================================================================
// TilesetEditorPloxy

Ref<lna::AssetEditor> TilesetEditorPloxy::createEditor()
{
    return ln::makeObject<TilesetEditor>();
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


#endif

