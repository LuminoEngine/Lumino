
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>

#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>

namespace ln {

//==============================================================================
// MeshTilemapModel

LN_OBJECT_IMPLEMENT(MeshTilemapModel, Object) {}

struct AutoTileInfo
{
	uint8_t subtiles[4];	// [top-left][top-right][bottom-left][bottom-light]
};

static AutoTileInfo g_AutoTileTable[48] =
{
	// Block tiles.
	/*[0]*/  {1,1,1,1},{2,1,1,1},{3,3,1,1},{1,2,1,1}, {2,2,1,1},{4,1,4,1},{5,3,4,1},{4,2,4,1},
	/*[8]*/  {1,4,1,4},{2,4,1,4},{3,5,1,4},{4,4,4,4}, {5,5,4,4},{1,1,2,1},{2,1,2,1},{3,3,2,1},
	/*[16]*/ {1,2,2,1},{2,2,2,1},{4,3,4,1},{1,4,2,4}, {2,4,2,4},{3,5,2,4},{1,1,3,3},{2,1,3,3},
	/*[24]*/ {3,3,3,3},{1,2,3,3},{2,2,3,3},{4,1,5,3}, {5,3,5,3},{4,2,5,3},{1,4,3,5},{2,4,3,5},
	/*[32]*/ {3,5,3,5},{4,4,5,5},{5,5,5,5},{1,1,1,2}, {2,1,1,2},{3,3,1,2},{1,2,1,2},{2,2,1,2},
	/*[40]*/ {4,1,4,2},{5,3,4,2},{4,2,4,2},{1,1,2,2}, {2,1,2,2},{3,3,2,2},{1,2,2,2},{2,2,2,2},
};

// SubTileId([0] is invalid) => PhysicalTileIndex
static int g_AutoTileSourcePosTable_TkoolXP[4][6] =	// TkoolXP and Wolf
{
	// [top-left]
	{ -1, 26, 4, 14, 24, 12 },
	// [top-right]
	{ -1, 27, 5, 15, 29, 17 },
	// [bottom-left]
	{ -1, 32, 10, 44, 30, 42 },
	// [bottom-light]
	{ -1, 33, 11, 45, 35, 47 },
};

MeshTilemapModel::MeshTilemapModel()
{
}

void MeshTilemapModel::init()
{
    Object::init();

	auto tex = Texture2D::load(u"autotile1");
	int subtilePixelSize = 16;
	int hc = 6;
	int vc = 8;
	//auto subTileUVSize = Vector2(static_cast<float>(hc) / tex->width(), static_cast<float>(vc) / tex->height());
	auto subTileUVSize = Vector2(1.0f / static_cast<float>(hc), 1.0f / static_cast<float>(vc));

	m_mesh = makeObject<Mesh>((4 * 4 * 48), (6 * 4 * 48));

	int offsetV = 0;
	int offsetI = 0;
	for (int i = 0; i < 48; i++) {
		const auto& info = g_AutoTileTable[i];
		int startIndex = offsetI;
		Vector3 pysOffsets[4] = { { 0.0, 0.5, 0.0 }, { 0.5, 0.5, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.5, 0.0, 0.0 } };

		// [top-left], [top-right], [bottom-left], [bottom-light]
		for (int iCorner = 0; iCorner < 4; iCorner++) {
			int index_tl = g_AutoTileSourcePosTable_TkoolXP[iCorner][info.subtiles[iCorner]];
			auto uv = Vector2(subTileUVSize.x * (index_tl % hc), subTileUVSize.y * (index_tl / hc));
			m_mesh->setVertex(offsetV + 0, Vertex{ pysOffsets[iCorner] + Vector3(0, 0.5, 0), Vector3::UnitZ, Vector2(uv.x, uv.y), Color::White });
			m_mesh->setVertex(offsetV + 1, Vertex{ pysOffsets[iCorner] + Vector3(0.5, 0.5, 0), Vector3::UnitZ, Vector2(uv.x + subTileUVSize.x, uv.y), Color::White });
			m_mesh->setVertex(offsetV + 2, Vertex{ pysOffsets[iCorner] + Vector3(0, 0, 0), Vector3::UnitZ, Vector2(uv.x, uv.y + subTileUVSize.y), Color::White });
			m_mesh->setVertex(offsetV + 3, Vertex{ pysOffsets[iCorner] + Vector3(0.5, 0, 0), Vector3::UnitZ, Vector2(uv.x + subTileUVSize.x, uv.y + subTileUVSize.y), Color::White });
			m_mesh->setIndex(offsetI + 0, offsetV + 0);
			m_mesh->setIndex(offsetI + 1, offsetV + 1);
			m_mesh->setIndex(offsetI + 2, offsetV + 2);
			m_mesh->setIndex(offsetI + 3, offsetV + 2);
			m_mesh->setIndex(offsetI + 4, offsetV + 1);
			m_mesh->setIndex(offsetI + 5, offsetV + 3);
			offsetV += 4;
			offsetI += 6;
		}

		m_mesh->addSection(startIndex, 8, 0, PrimitiveTopology::TriangleList);
	}

	m_material = Material::create(tex);
}

void MeshTilemapModel::draw(RenderingContext* context, const Matrix& transform)
{
    context->setMaterial(m_material);
	for (int i = 0; i < 48; i++) {
		context->setBaseTransfrom(Matrix::makeTranslation(i, 0, 0));
		context->drawMesh(m_mesh, i);
	}
}

} // namespace ln

