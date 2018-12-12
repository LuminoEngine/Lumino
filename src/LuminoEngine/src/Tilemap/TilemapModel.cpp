
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/Tileset.hpp>
#include <LuminoEngine/Tilemap/TilemapLayer.hpp>
#include <LuminoEngine/Tilemap/TilemapModel.hpp>

namespace ln {

//==============================================================================
// TilemapModel

TilemapModel::TilemapModel()
    : m_tilesetIdSpan(65536)
{
}

TilemapModel::~TilemapModel()
{
}

void TilemapModel::initialize()
{
    Object::initialize();
}

void TilemapModel::addTileset(Tileset* tileset)
{
    m_tilesets.add({ tileset, m_tilesetIdSpan * m_tilesets.size() });
}

void TilemapModel::addLayer(TilemapLayer* layer)
{
    m_layers.add(layer);
}

} // namespace ln

