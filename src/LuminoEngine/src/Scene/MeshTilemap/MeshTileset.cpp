
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/MeshTilemap/MeshTileset.hpp>

#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>

//#include <simpleboolean/meshoperator.h>

namespace ln {

//==============================================================================
// MeshAutoTileset

MeshAutoTileset::MeshAutoTileset()
{
}

void MeshAutoTileset::init()
{
	Object::init();
}

//==============================================================================
// MeshTileset

LN_OBJECT_IMPLEMENT(MeshTileset, Object) {}


const MeshTileset::AutoTileInfo MeshTileset::AutoTileTable[48] =
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


MeshTileset::MeshTileset()
{
}

void MeshTileset::init()
{
	Object::init();

	//{

	//	simpleboolean::Mesh mesh1;
	//	simpleboolean::Mesh mesh2;
	//	simpleboolean::loadTriangulatedObj(mesh1, firstObj);
	//	simpleboolean::loadTriangulatedObj(mesh2, secondObj);

	//	simpleboolean::MeshOperator combiner;
	//	combiner.setMeshes(mesh1, mesh2);
	//	if (!combiner.combine())
	//		return 1;

	//	//if (nullptr != outputObj) {
	//		simpleboolean::Mesh unionMesh;
	//		combiner.getResult(simpleboolean::Type::Union, &unionMesh);
	//	//}

	//}


	//auto tex = Texture2D::load(u"autotile1");
	//detail::MeshAutoTilesetUVMapper uvMapper(
	//	Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::XP);
	auto tex = Texture2D::load(u"autotile2");
	detail::MeshAutoTilesetUVMapper uvMapper(
		Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::MVWithWall);



	{



		m_mesh = makeObject<Mesh>((4 * 4 * 48) * 6, (6 * 4 * 48) * 6);

		// ZMinus を、指定方向に向けるための変換行列
		const auto finalOffset = Vector3(0.5, 0.5, 0.5);
		Matrix faceTransforms[6] = {
			Matrix::makeRotationY(Math::PI / 2) * Matrix::makeTranslation(Vector3(-0.5, 0, 0) + finalOffset),
			Matrix::makeRotationY(-Math::PI / 2) * Matrix::makeTranslation(Vector3(0.5, 0, 0) + finalOffset),
			Matrix::makeRotationZ(Math::PI) * Matrix::makeRotationX(-Math::PI / 2) * Matrix::makeTranslation(Vector3(0, -0.5, 0) + finalOffset),
			Matrix::makeRotationX(Math::PI / 2) * Matrix::makeTranslation(Vector3(0, 0.5, 0) + finalOffset),
			Matrix::Identity * Matrix::makeTranslation(Vector3(0, 0, -0.5) + finalOffset),
			Matrix::makeRotationY(Math::PI) * Matrix::makeTranslation(Vector3(0, 0, 0.5) + finalOffset),
		};

		int offsetV = 0;
		int offsetI = 0;

		for (int iFaceDir = 0; iFaceDir < 6; iFaceDir++) {
			const Matrix& transform = faceTransforms[iFaceDir];

			for (int i = 0; i < 48; i++) {
				const auto& info = AutoTileTable[i];
				int startIndex = offsetI;
				Vector3 pysOffsets[4] = { { -0.5, +0.5, 0.0 }, { 0.0, +0.5, 0.0 }, { -0.5, 0.0, 0.0 }, { 0.0, 0.0, 0.0 } };

				// [top-left], [top-right], [bottom-left], [bottom-light]
				for (int iCorner = 0; iCorner < 4; iCorner++) {
					//int index_tl = g_AutoTileSourcePosTable_TkoolXP[iCorner][info.subtiles[iCorner]];
					//auto uv = Vector2(subTileUVSize.x * (index_tl % hc), subTileUVSize.y * (index_tl / hc));
					auto uvRect = uvMapper.getUVRectFromLocalId(static_cast<MeshTileFaceDirection>(iFaceDir), i, static_cast<detail::SubtileCorner>(iCorner));
					auto p1 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0, 0, 0), transform);
					auto p2 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0.5, 0, 0), transform);
					auto p3 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0, -0.5, 0), transform);
					auto p4 = Vector3::transformCoord(pysOffsets[iCorner] + Vector3(0.5, -0.5, 0), transform);
					m_mesh->setVertex(offsetV + 0, Vertex{ p1, Vector3::UnitZ, uvRect.getTopLeft(), Color::White });
					m_mesh->setVertex(offsetV + 1, Vertex{ p2, Vector3::UnitZ, uvRect.getTopRight(), Color::White });
					m_mesh->setVertex(offsetV + 2, Vertex{ p3, Vector3::UnitZ, uvRect.getBottomLeft(), Color::White });
					m_mesh->setVertex(offsetV + 3, Vertex{ p4, Vector3::UnitZ, uvRect.getBottomRight(), Color::White });
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
		}



		m_material = Material::create(tex);
	}


	m_material->shadingModel = ShadingModel::Unlit;
	m_material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));
	//m_meshList = makeObject<InstancedMeshList>(m_mesh, 0);
	//m_meshList->setTransform(Matrix::makeTranslation(-5, 0, 0));
	//m_meshList->drawMesh();
	//m_meshList->setTransform(Matrix::makeTranslation(-1, 0, 0));
	//m_meshList->drawMesh();
	for (int i = 0; i < m_meshList.size(); i++) {
		m_meshList[i] = makeObject<InstancedMeshList>(m_mesh, i);
	}
}

void MeshTileset::beginBatch()
{
	for (int i = 0; i < m_meshList.size(); i++) {
		m_meshList[i]->reset();
	}
}

void MeshTileset::drawTile(RenderingContext* context, const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency, const Matrix& transform) const
{

	//context->setMaterial(m_material);

	if (autoTileKindId(tile.tileId) >= 0) {
		for (int d = 0; d < 6; d++) {
			if (!adjacency.buried[d]) {
				//context->drawMesh(m_mesh, (d * 48) + tile.faceTileId[d]);
				int index = (d * 48) + tile.faceTileId[d];
				m_meshList[index]->setTransform(transform);
				m_meshList[index]->drawMesh();
			}
		}
	}

	//context->drawMeshInstanced(m_meshList);
}

void MeshTileset::drawBatch(RenderingContext* context)
{
	context->setMaterial(m_material);
	for (int i = 0; i < m_meshList.size(); i++) {
		if (m_meshList[i]->instanceCount() > 0) {
			context->drawMeshInstanced(m_meshList[i]);
		}
	}
}

namespace detail {

// SubTileId([0] is invalid) => PhysicalTileIndex
static int g_AutoTileSourcePosTable_MVFloor[4][6] =
{
	// [top-left]
	{ -1, 13, 2, 9, 12, 8 },
	// [top-right]
	{ -1, 14, 3, 10, 15, 11 },
	// [bottom-left]
	{ -1, 17, 6, 21, 16, 20 },
	// [bottom-light]
	{ -1, 18, 7, 22, 19, 23 },
};
static int g_AutoTileSourcePosTable_MVWall[4][6] =
{
	// [top-left]
	{ -1, 13 + 16, 24, 9 + 16, 12 + 16, 24 },
	// [top-right]
	{ -1, 14 + 16, 27 , 10 + 16, 15 + 16, 11 + 16 },
	// [bottom-left]
	{ -1, 17 + 16, 36, 21 + 16, 16 + 16, 20 + 16 },
	// [bottom-light]
	{ -1, 18 + 16, 39, 22 + 16, 19 + 16, 23 + 16 },
};

MeshAutoTilesetUVMapper::MeshAutoTilesetUVMapper(const Size& textureSize, const Rect& sourceRect, Format format)
	: m_format(format)
{
	m_globalUVOffset.x = sourceRect.x / textureSize.width;
	m_globalUVOffset.y = sourceRect.y / textureSize.height;

	if (m_format == Format::XP) {
		m_subtileUVSize.x = (sourceRect.width / 6) / textureSize.width;
		m_subtileUVSize.y = (sourceRect.height / 8) / textureSize.height;
	}
	else if (m_format == Format::MVWithWall) {
		m_subtileUVSize.x = (sourceRect.width / 4) / textureSize.width;
		m_subtileUVSize.y = (sourceRect.height / 10) / textureSize.height;
	}
	else {
		LN_UNREACHABLE();
	}
}

Rect MeshAutoTilesetUVMapper::getUVRectFromLocalId(MeshTileFaceDirection direction, int autotileLocalId, SubtileCorner corner) const
{
	if (m_format == Format::XP) {
		const auto& info = MeshTileset::AutoTileTable[autotileLocalId];
		int index_tl = g_AutoTileSourcePosTable_TkoolXP[corner][info.subtiles[corner]];
		auto offset = Vector2(m_subtileUVSize.x * (index_tl % 6), m_subtileUVSize.y * (index_tl / 6));
		return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
	}
	else if (m_format == Format::MVWithWall) {
		if (direction == MeshTileFaceDirection::YPlus || direction == MeshTileFaceDirection::YMinus) {
			const auto& info = MeshTileset::AutoTileTable[autotileLocalId];
			int index_tl = g_AutoTileSourcePosTable_MVFloor[corner][info.subtiles[corner]];
			auto offset = Vector2(m_subtileUVSize.x * (index_tl % 4), m_subtileUVSize.y * (index_tl / 4));
			return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
		}
		else {
			const auto& info = MeshTileset::AutoTileTable[autotileLocalId];
			int index_tl = g_AutoTileSourcePosTable_MVWall[corner][info.subtiles[corner]];
			auto offset = Vector2(m_subtileUVSize.x * (index_tl % 4), m_subtileUVSize.y * (index_tl / 4));
			return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
		}
	}
	else {
		LN_UNREACHABLE();
		return Rect();
	}
}

} // namespace detail
} // namespace ln