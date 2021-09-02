
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapLayer.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapModel.hpp>

namespace ln {

//==============================================================================
// MeshVoxelmapModel

LN_OBJECT_IMPLEMENT(MeshVoxelmapModel, Object) {}

MeshVoxelmapModel::MeshVoxelmapModel()
{
}

void MeshVoxelmapModel::init()
{
    Object::init();



}

void MeshVoxelmapModel::setTileset(MeshVoxelset* tileset)
{
	m_tileset = tileset;
}

void MeshVoxelmapModel::addLayer(MeshVoxelmapLayer* layer)
{
	m_layers.add(layer);
}

void MeshVoxelmapModel::draw(RenderingContext* context, const Matrix& transform)
{
	if (!m_tileset) return;

	for (auto& layer : m_layers) {
		layer->draw(context, m_tileset);
	}
}

} // namespace ln

