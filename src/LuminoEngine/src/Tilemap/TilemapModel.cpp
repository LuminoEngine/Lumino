
#include "Internal.hpp"
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

