
#include "Internal.hpp"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>

namespace ln {

//==============================================================================
// TilemapModel

Ref<TilemapModel> TilemapModel::create()
{
    return newObject<TilemapModel>();
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

                printf("");

            }
        }

        const auto& tilesets = map.getTilesets();
        for (const auto& tileset : tilesets)
        {
            //read out tile set properties, load textures etc...

            tileset.getImagePath();
        }
    }
}

void TilemapModel::addTileset(Tileset* tileset)
{
    m_tilesets.add({ tileset, m_tilesetIdSpan * m_tilesets.size() });
}

void TilemapModel::addLayer(TilemapLayer* layer)
{
    m_layers.add(layer);
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
    *outTileset = m_tilesets[0].tileset;
    *outTileLocalId = tileGlobalId;
    return true;
}

} // namespace ln

