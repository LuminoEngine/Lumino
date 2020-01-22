
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilesetNavigator.hpp"
#include "TilesetEditorView.hpp"


namespace lna {

//==============================================================================
// AutoTilesetControl

void AutoTilesetControl::setTileset(ln::Tileset* tileset)
{
	m_tileset = tileset;
}

void AutoTilesetControl::onRender(ln::UIRenderingContext* context)
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
// TilesetFormControl

TilesetFormControl::TilesetFormControl(TilesetView* parent)
{
}

//==============================================================================
// TilesetControl

TilesetControl::TilesetControl()
	: m_displayTileSize(32)
	, m_displayTileCountH(8)
{
	setBorderThickness(1);
	setBorderColor(ln::Color::Gray);
}

void TilesetControl::setTileset(ln::Tileset* tileset)
{
    m_tileset = tileset;

	auto texture = m_tileset->material()->mainTexture();
	if (texture) {
		ln::Size tileSize;
		tileSize.width = texture->width() / m_displayTileCountH;
		tileSize.height = tileSize.width;
		m_displayTileScale.x = m_displayTileSize / tileSize.width;
		m_displayTileScale.y = m_displayTileSize / tileSize.height;

		setWidth(m_displayTileScale.x * texture->width());
		setHeight(m_displayTileScale.y * texture->height());
	}
}

void TilesetControl::onRender(ln::UIRenderingContext* context)
{
    if (m_tileset && m_tileset->material()) {
        auto material = m_tileset->material();
        auto texture = material->mainTexture();
        if (texture) {
            context->drawImage(ln::Rect(0, 0, m_displayTileScale.x * texture->width(), m_displayTileScale.y * texture->height()), material);
        }
    }
}

//==============================================================================
// TilesetView

TilesetView::TilesetView()
{
	m_layout1 = ln::makeObject<ln::UIHBoxLayout3>();
	addElement(m_layout1);

	m_tilesetFormControl = ln::makeObject2<TilesetFormControl>(this);
	m_tilesetFormControl->setHorizontalAlignment(ln::HAlignment::Stretch);
	m_tilesetFormControl->setVerticalAlignment(ln::VAlignment::Stretch);
	m_tilesetFormControl->setBackgroundColor(ln::Color::Coral);
	m_layout1->addChild(m_tilesetFormControl);

	m_layout2 = ln::makeObject<ln::UIVBoxLayout3>();
	m_layout1->addChild(m_layout2);

	m_autoTilesetControl = ln::makeObject<AutoTilesetControl>();
	m_autoTilesetControl->setHeight(64);
	m_autoTilesetControl->setBackgroundColor(ln::Color::Blue);
	m_layout2->addChild(m_autoTilesetControl);

	m_tilesetControl = ln::makeObject<TilesetControl>();
	m_layout2->addChild(m_tilesetControl);
}

void TilesetView::setTileset(ln::Tileset* tileset)
{
	m_tileset = tileset;
	m_autoTilesetControl->setTileset(m_tileset);
	m_tilesetControl->setTileset(m_tileset);
}

//Ref<ln::UIVBoxLayout3> TilesetView::layout()
//{
//	return m_layout;
//}


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

