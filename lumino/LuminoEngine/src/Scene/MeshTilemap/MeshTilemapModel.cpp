
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>

namespace ln {

//==============================================================================
// MeshTilemapModel

LN_OBJECT_IMPLEMENT(MeshTilemapModel, Object) {}

MeshTilemapModel::MeshTilemapModel()
{
}

Result<> MeshTilemapModel::init() {
    return Object::init();
}

void MeshTilemapModel::setTileset(MeshTileset* tileset)
{
	m_tileset = tileset;
}

void MeshTilemapModel::addLayer(MeshTilemapLayer* layer)
{
	m_layers.add(layer);
}

void MeshTilemapModel::draw(RenderingContext* context, const Matrix& transform)
{
	if (!m_tileset) return;

	for (auto& layer : m_layers) {
		layer->draw(context, m_tileset);
	}
}

} // namespace ln

