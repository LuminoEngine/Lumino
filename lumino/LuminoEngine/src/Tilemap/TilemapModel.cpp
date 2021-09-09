
#include "Internal.hpp"
//#include <tmxlite/Map.hpp>
//#include <tmxlite/Layer.hpp>
//#include <tmxlite/TileLayer.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>

#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>

namespace ln {


//==============================================================================
// TilemapModel

LN_OBJECT_IMPLEMENT(TilemapModel, Object) {}

Ref<TilemapModel> TilemapModel::create()
{
    return makeObject<TilemapModel>();
}

TilemapModel::TilemapModel()
    : m_tilesetIdSpan(65536)
{
}

TilemapModel::~TilemapModel()
{
}

void TilemapModel::init()
{
    Object::init();

#if 0
    if (0) // TODO: test
    {
        tmx::Map map;
        if (map.load("D:/Proj/TH-10/Assets/untitled.tmx"))
        {

            const auto& layers = map.getLayers();
            int width = map.getTileCount().x;
            int height = map.getTileCount().y;

            for (const auto& layer : layers)
            {
                if (layer->getType() == tmx::Layer::Type::Object)
                {
                    //const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
                    //const auto& objects = objectLayer.getObjects();
                    //for (const auto& object : objects)
                    //{
                    //    //do stuff with object properties
                    //}
                }
                else if (layer->getType() == tmx::Layer::Type::Tile)
                {
                    const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                    //read out tile layer properties etc...
                    int x = 2;
                    int y = 1;
                    int index = y * width + x;
                    const auto tiles = tileLayer.getTiles();

                    int gid = tiles[index].ID;
                }
            }
        }
    }
#endif
}

void TilemapModel::init(const StringRef& filePath)
{
#if 0
    tmx::Map tmxMap;
    if (tmxMap.load(filePath.toStdString().c_str()))
    {
        // Tilesets
        for (const auto& tmxTileset : tmxMap.getTilesets())
        {
            auto material = ln::Material::create();
            material->setMainTexture(ln::Assets::loadTexture(ln::String::fromStdString(tmxTileset.getImagePath())));

            auto tileset = ln::makeObject<ln::Tileset>();
            tileset->setMaterial(material);
            tileset->setTilePixelSize(tmxTileset.getTileSize().x, tmxTileset.getTileSize().y);
            addTileset(tileset);

            tileset->resize(tmxTileset.getTileCount());
            for (int i = 0; i < tmxTileset.getTileCount(); i++) {
                const auto& tmxTile = tmxTileset.getTiles()[i];
                tileset->setTileImageRect(tmxTile.ID, tmxTile.imagePosition.x, tmxTile.imagePosition.y, tmxTile.imageSize.x, tmxTile.imageSize.y);
            }
        }

        int width = tmxMap.getTileCount().x;
        int height = tmxMap.getTileCount().y;

        // Layers
        for (const auto& tmxLayer : tmxMap.getLayers())
        {
            auto layer = ln::makeObject<ln::TilemapLayer>();
            layer->resize(width, height);
            layer->setOrientation(ln::TilemapOrientation::DownFlow);
            addLayer(layer);

            if (tmxLayer->getType() == tmx::Layer::Type::Object)
            {
            }
            else if (tmxLayer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tmxTileLayer = tmxLayer->getLayerAs<tmx::TileLayer>();
                const auto& tmxTiles = tmxTileLayer.getTiles();

                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        int index = y * width + x;
                        layer->setTileId(x, y, tmxTiles[index].ID);
                    }
                }
            }
        }
    }
#endif
    LN_NOTIMPLEMENTED();
}

Tileset* TilemapModel::tileset() const
{
    return m_tileset;
}

void TilemapModel::reset(int width, int height, int layers)
{
	m_layers.clear();
	for (int i = 0; i < layers; i++) {
		auto layer = makeObject<TilemapLayer>();
		layer->resize(width, height);
		m_layers.add(layer);
	}
}

void TilemapModel::setTileset(Tileset* tileset)
{
    m_tileset = tileset;
}

void TilemapModel::addLayer(TilemapLayer* layer)
{
    m_layers.add(layer);
}

TilemapLayer* TilemapModel::layer(int index) const
{
    return m_layers[index];
}

const Size& TilemapModel::tileSize() const
{
	// TODO:
	return m_layers.front()->tileSize();
}

int TilemapModel::width() const
{
	// TODO:
	return m_layers.front()->getWidth();
}

int TilemapModel::height() const
{
	// TODO:
	return m_layers.front()->getHeight();
}

bool TilemapModel::isValidTilePosition(int x, int y) const
{
	if (x < 0 || y < 0) return false;
	for (auto& layer : m_layers) {
		if (layer->getWidth() <= x || layer->getHeight() <= y) {
			return false;
		}
	}
	return true;
}

uint8_t TilemapModel::tilePassageFlags(int x, int y) const
{
	for (int i = m_layers.size() - 1; i >= 0; i--) {
		auto tileId = m_layers[i]->tileId(x, y);
		auto flags = m_tileset->tilePassageFlags(tileId);
		if ((flags & 0x10) != 0) {
			continue;	// 下層のタイルに任せる
		}
		return flags;
	}
	return 0;
}

TilemapLayer* TilemapModel::getValidFrontLayer(int x, int y) const
{
    for (int i = m_layers.size() - 1; i >= 0; i--) {
        if (m_layers[i]->isValidTileId(x, y)) {
            return m_layers[i];
        }
    }
    return nullptr;
}

void TilemapModel::render(RenderingContext* context, const Matrix& transform, const detail::TilemapBounds& bounds)
{
    for (auto& layer : m_layers)
    {
        layer->render(this, context, transform, bounds);
    }
}

bool TilemapModel::fetchTileset(int tileGlobalId, Tileset** outTileset, int* outTileLocalId)
{
    // TODO:
    *outTileset = m_tileset;
    *outTileLocalId = tileGlobalId;
    return true;
}

//void TilemapModel::serialize(Archive& ar)
//{
//	Object::serialize(ar);
//    LN_NOTIMPLEMENTED();
//
//  //  String tilesetPath;
//  //  if (ar.isSaving()) {
//		//tilesetPath = Path::makeRelative(ar.basePath(), m_tileset->m_assetFilePath);
//  //  }
//
//	//ar & makeNVP(u"tileset", tilesetPath);
// //   Assets::serializeAsAssetPath(ar, u"tileset", m_tileset);
//	//ar & makeNVP(u"layers", m_layers);
//
//  //  if (ar.isLoading()) {
//		//setTileset(dynamic_pointer_cast<Tileset>(Assets::loadAsset(Path(ar.basePath(), tilesetPath))));
//  //  }
//}

} // namespace ln

