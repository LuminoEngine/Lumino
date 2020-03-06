
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapLayer.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

namespace ln {

//==============================================================================
// MeshTilemapLayer

LN_OBJECT_IMPLEMENT(MeshTilemapLayer, Object) {}

MeshTilemapLayer::MeshTilemapLayer()
	: m_tiles()
	, m_sizeX(0)
	, m_sizeY(0)
	, m_sizeZ(0)
{
}

void MeshTilemapLayer::init()
{
	Object::init();
}

void MeshTilemapLayer::resize(int sizeX, int sizeY, int sizeZ)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_sizeZ = sizeZ;
	m_tiles.resize(m_sizeX * m_sizeY * m_sizeZ);
}

void MeshTilemapLayer::setTileId(int x, int y, int z, MeshTileFaceDirection dir, int id)
{
	if (LN_REQUIRE(isValidIndex(x, y, z))) return;
	tile(x, y, z).id[(int)dir] = id;
}

void MeshTilemapLayer::draw(RenderingContext* context, const MeshTileset* tileset)
{
	// TODO: View culling

	for (int y = 0; y < m_sizeY; y++) {
		for (int z = 0; z < m_sizeZ; z++) {
			for (int x = 0; x < m_sizeX; x++) {
				const detail::MeshTile& t = tile(x, y, z);

				context->setBaseTransfrom(Matrix::makeTranslation(x, y, z));
				tileset->drawTile(context, t);
			}
		}
	}


	//   context->setMaterial(m_material);
	   //for (int i = 0; i < 48; i++) {
	   //	context->setBaseTransfrom(Matrix::makeTranslation(i, 0, 0));
	   //	context->drawMesh(m_mesh, i);
	   //}
}

} // namespace ln

