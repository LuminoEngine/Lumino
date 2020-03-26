
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "../App/EditorContext.hpp"
#include "TilemapSceneEditorModel.hpp"
#include "TilemapSceneModePane.hpp"

namespace lna {

//==============================================================================
// TilemapBrush

void TilemapBrush::setNormalTiles(ln::Tileset* tileset, const ln::RectI& range)
{
	if (LN_REQUIRE(tileset)) return;

	int beginX = range.x;
	int beginY = range.y;
	int endX = range.x + range.width;
	int endY = range.y + range.height;

	m_tiles.clear();
	for (int y = beginY; y < endY; y++) {
		for (int x = beginX; x < endX; x++) {
			int id = y * TilemapSceneModePane::DisplayTilesX + x;
			m_tiles.push_back(id);
		}
	}

	m_source = TilemapBrushSource::Tileset;
	m_range.x = beginX;
	m_range.y = beginY;
	m_range.width = endX - beginX;
	m_range.height = endY - beginY;
}

//==============================================================================
// TilemapSceneEditorModel

TilemapSceneEditorModel::TilemapSceneEditorModel()
	: m_tilemapBrush(ln::makeObject<TilemapBrush>())
{
}

ln::Result TilemapSceneEditorModel::createNewTilemapSceneAsset(lna::EditorContext* context, const ln::Path& filePath)
{

	auto tilesetAsset = context->assetDatabase()->openAsset(u"D:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/Tileset-1");

    auto scene = ln::makeObject<ln::Level>();

	auto tilemap = ln::makeObject<ln::Tilemap>();
	tilemap->setShadingModel(ln::ShadingModel::Unlit);
    scene->addObject(tilemap);

	//tilesetMaterial->setMainTexture(ln::Texture2D::create((u"D:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/BaseChip_pipo.png")));
	//auto tilesetTexture = ln::Assets::loadTexture(u"360782C3-EAD5-421E-8887-D2B10E09781C");
	//auto tilesetMaterial = ln::makeObject<ln::Material>();
	//tilesetMaterial->setMainTexture(tilesetTexture);

	auto tileset = dynamic_cast<ln::Tileset*>(tilesetAsset->target()); //ln::makeObject<ln::Tileset>();
	//tileset->reset(tilesetMaterial, 16, 16);

	auto layer = ln::makeObject<ln::TilemapLayer>();
	layer->resize(60, 30);
	layer->setTileSize(ln::Size(1, 1));
	layer->setTileId(0, 0, 1);
	layer->setTileId(1, 1, 1);
    for (auto x = 0; x < layer->getWidth(); x++) {
        layer->setTileId(x, layer->getHeight() - 1, 1);
    }
	auto tilemapModel = ln::makeObject<ln::TilemapModel>();
	tilemapModel->setTileset(tileset);
	tilemapModel->addLayer(layer);
	tilemap->setTilemapModel(tilemapModel);



    //auto tex1Asset = context->assetDatabase()->openAsset(u"D:/Proj/LN/PrivateProjects/HC0/Assets/Scenes/Objects-1.png");
    //auto tex1 = dynamic_cast<ln::Texture*>(tilesetAsset->target());
    auto tex1 = ln::Texture2D::load(u"D:/Proj/LN/PrivateProjects/HC0/Assets/Scenes/Objects-1.png");
    auto sprite1 = ln::Sprite::create(tex1, 10, 10);
    sprite1->setPosition(5, 1, 5);
    scene->addObject(sprite1);
    //"D:\Proj\LN\PrivateProjects\HC0\Assets\Scenes\Objects-1.png"

	return context->assetDatabase()->createAsset(scene, filePath);
}

} // namespace lna

