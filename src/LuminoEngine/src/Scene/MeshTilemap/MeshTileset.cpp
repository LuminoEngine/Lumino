
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
	for (int i = 0; i < 6; i++) beveled[i] = false;
}

void MeshAutoTileset::setMaterial(Material* value)
{
	m_material = value;
}

void MeshAutoTileset::buildQubeFloorAndWall()
{
	//auto tex = Texture2D::load(u"autotile1");
	//detail::MeshAutoTilesetUVMapper uvMapper(
	//	Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::XP);

	auto* tex = m_material->mainTexture();

	detail::MeshAutoTilesetUVMapper uvMapper(
		Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::MVWithWall);


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
			const auto& info = MeshTileset::AutoTileTable[i];
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

	//m_meshList = makeObject<InstancedMeshList>(m_mesh, 0);
	//m_meshList->setTransform(Matrix::makeTranslation(-5, 0, 0));
	//m_meshList->drawMesh();
	//m_meshList->setTransform(Matrix::makeTranslation(-1, 0, 0));
	//m_meshList->drawMesh();
	for (int i = 0; i < m_meshList.size(); i++) {
		m_meshList[i] = makeObject<InstancedMeshList>(m_mesh, i);
	}
}

void MeshAutoTileset::buildFloor()
{
	auto* tex = m_material->mainTexture();
	//detail::MeshAutoTilesetUVMapper uvMapper(
	//	Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::MVFloor);
	detail::MeshAutoTilesetUVMapper uvMapper(Size(tex->width(), tex->height()), Rect(0, 0, 64, 96), detail::MeshAutoTilesetUVMapper::Format::MVFloor);

	m_frameUVOffset.x = 64.0f / tex->width();
	m_frameUVOffset.y = 0;
	m_animationFrameCount = 3;

	m_mesh = makeObject<Mesh>((4 * 4 * 48) * 1, (6 * 4 * 48) * 1);



	int offsetV = 0;
	int offsetI = 0;
	for (int i = 0; i < 48; i++) {
		const auto& info = MeshTileset::AutoTileTable[i];
		int startIndex = offsetI;

		// [top-left], [top-right], [bottom-left], [bottom-light]
		Vector3 pysOffsets[4] = { { 0.0, +1.0, +1.0 }, { +0.5, +1.0, +1.0 }, { 0.0, +1.0, +0.5 }, { +0.5, +1.0, +0.5 } };
		for (int iCorner = 0; iCorner < 4; iCorner++) {
			auto uvRect = uvMapper.getUVRectFromLocalId(MeshTileFaceDirection::YPlus, i, static_cast<detail::SubtileCorner>(iCorner));
			auto p1 = pysOffsets[iCorner] + Vector3(0, 0, 0);
			auto p2 = pysOffsets[iCorner] + Vector3(0.5, 0, 0);
			auto p3 = pysOffsets[iCorner] + Vector3(0, 0, -0.5);
			auto p4 = pysOffsets[iCorner] + Vector3(0.5, 0, -0.5);
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
		setInstancedMeshList((int)MeshTileFaceDirection::YPlus, i, false, makeObject<InstancedMeshList>(m_mesh, i));
	}
}

void MeshAutoTileset::buildFloorAndSlopeWall()
{
	beveled[(int)MeshTileFaceDirection::XMinus] = true;
	beveled[(int)MeshTileFaceDirection::XPlus] = true;
	beveled[(int)MeshTileFaceDirection::ZMinus] = true;
	beveled[(int)MeshTileFaceDirection::ZPlus] = true;

	auto* tex = m_material->mainTexture();

	detail::MeshAutoTilesetUVMapper uvMapper(
		Size(tex->width(), tex->height()), Rect(0, 0, tex->width(), tex->height()), detail::MeshAutoTilesetUVMapper::Format::MVWithWall);


	m_mesh = makeObject<Mesh>((4 * 4 * 48) * 6, (6 * 4 * 48) * 6);


	float m = 0.2;
	float mh = m / 2;



	int offsetV = 0;
	int offsetI = 0;

	struct Vert
	{
		Vector3 pos;
		Vector2 uv;
	};

	auto putSquare = [&](Vert p0, Vert p1, Vert p2, Vert p3, const Rect& uvRect, const Matrix& rot, const Vector3& offset) {
		m_mesh->setVertex(offsetV + 0, Vertex{ Vector3::transformCoord(p0.pos, rot) + offset, Vector3::UnitZ, uvRect.getPointByRatio(p0.uv), Color::White });
		m_mesh->setVertex(offsetV + 1, Vertex{ Vector3::transformCoord(p1.pos, rot) + offset, Vector3::UnitZ, uvRect.getPointByRatio(p1.uv), Color::White });
		m_mesh->setVertex(offsetV + 2, Vertex{ Vector3::transformCoord(p2.pos, rot) + offset, Vector3::UnitZ, uvRect.getPointByRatio(p2.uv), Color::White });
		m_mesh->setVertex(offsetV + 3, Vertex{ Vector3::transformCoord(p3.pos, rot) + offset, Vector3::UnitZ, uvRect.getPointByRatio(p3.uv), Color::White });
		m_mesh->setIndex(offsetI + 0, offsetV + 0);
		m_mesh->setIndex(offsetI + 1, offsetV + 1);
		m_mesh->setIndex(offsetI + 2, offsetV + 2);
		m_mesh->setIndex(offsetI + 3, offsetV + 2);
		m_mesh->setIndex(offsetI + 4, offsetV + 1);
		m_mesh->setIndex(offsetI + 5, offsetV + 3);
		offsetV += 4;
		offsetI += 6;
	};

	MeshTileFaceDirection sideDirs[4] = {
		MeshTileFaceDirection::XMinus,
		MeshTileFaceDirection::XPlus,
		MeshTileFaceDirection::ZMinus,
		MeshTileFaceDirection::ZPlus };
	Matrix faceRotation[4] = {
		Matrix::makeRotationY(Math::PI / 2),
		Matrix::makeRotationY(-Math::PI / 2),
		Matrix::Identity,
		Matrix::makeRotationY(Math::PI) };
	Vector3 faceOffset[4] = {	// box の 左下手前を原点にしたい
		Vector3(0, 0.5, 0.5),
		Vector3(1, 0.5, 0.5),
		Vector3(0.5, 0.5, 0.0),
		Vector3(0.5, 0.5, 1) };
	for (int iFaceDir = 0; iFaceDir < 4; iFaceDir++) {
		MeshTileFaceDirection dir = sideDirs[iFaceDir];
		const Matrix& transform = faceRotation[iFaceDir];
		const Vector3& offset = faceOffset[iFaceDir];

		for (int i = 0; i < 48; i++) {
			const auto& info = MeshTileset::AutoTileTable[i];
			int startIndex = offsetI;

			// 作るMesh は Z=0で ZPlus をベースとし、Tile の中央が (0,0,0)

			// [top-left]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopLeft);
				int t = info.subtiles[0];
				if (t == 1 || t == 2) {	// 2=平面と斜面の接合
					putSquare(	// 平面
						{ Vector3(-0.5, 0.5, mh), Vector2(0.0, 0.0) },
						{ Vector3( 0.0, 0.5, mh), Vector2(1.0, 0.0) },
						{ Vector3(-0.5, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3( 0.0, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 3) {
					putSquare(	// 斜面・側面接合
						{ Vector3(-0.5, 0.5, m), Vector2(0.0, 0.0) },
						{ Vector3( 0.0, 0.5, m), Vector2(1.0, 0.0) },
						{ Vector3(-0.5, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3( 0.0, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 4) {
					putSquare(	// 平面上接合
						{ Vector3(-0.5 + mh, 0.5, mh), Vector2(0.0, 0.0) },
						{ Vector3( 0.0 + mh, 0.5, mh), Vector2(1.0, 0.0) },
						{ Vector3(-0.5 + mh, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3( 0.0 + mh, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 5) {
					putSquare(	// 角
						{ Vector3(-0.5 + m, 0.5, m), Vector2(0.0, 0.0) },
						{ Vector3( 0.0, 0.5, m), Vector2(1.0, 0.0) },
						{ Vector3(-0.5 + mh, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3( 0.0, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
			}
			// [top-right]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopRight);
				int t = info.subtiles[1];
				if (t == 1 || t == 2) {
					putSquare(
						{ Vector3(0.0, 0.5, mh), Vector2(0.0, 0.0) },
						{ Vector3(0.5, 0.5, mh), Vector2(1.0, 0.0) },
						{ Vector3(0.0, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3(0.5, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 3) {
					putSquare(
						{ Vector3(0.0, 0.5, m), Vector2(0.0, 0.0) },
						{ Vector3(0.5, 0.5, m), Vector2(1.0, 0.0) },
						{ Vector3(0.0, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3(0.5, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 4) {
					putSquare(
						{ Vector3(0.0 + mh, 0.5, mh), Vector2(0.0, 0.0) },
						{ Vector3(0.5 + mh, 0.5, mh), Vector2(1.0, 0.0) },
						{ Vector3(0.0 + mh, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3(0.5 + mh, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 5) {
					putSquare(
						{ Vector3(0.0, 0.5, m), Vector2(0.0, 0.0) },
						{ Vector3(0.5 - m, 0.5, m), Vector2(1.0, 0.0) },
						{ Vector3(0.0, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3(0.5 - mh, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform,  offset);
				}
				else {
					LN_UNREACHABLE();
				}
			}

			if (offsetI > startIndex) {
				m_mesh->addSection(startIndex, (offsetI - startIndex) / 3, 0, PrimitiveTopology::TriangleList);
				setInstancedMeshList((int)dir, i, false, makeObject<InstancedMeshList>(m_mesh, m_mesh->sections().size() - 1));
			}
		}
	}
	// 天板
	{
		MeshTileFaceDirection dir = MeshTileFaceDirection::YPlus;
		Matrix transform = Matrix::makeRotationX(Math::PI / 2);
		Vector3 offset = Vector3(0.5, 1.0, 0.5);

		for (int i = 0; i < 48; i++) {
			const auto& info = MeshTileset::AutoTileTable[i];
			int startIndex = offsetI;

			// 作るMesh は Z=0で ZPlus をベースとし、Tile の中央が (0,0,0)

			// [top-left]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopLeft);
				int t = info.subtiles[0];
				if (t == 1) {
					putSquare(
						{ Vector3(-0.5, 0.5, 0.0), Vector2(0.0, 0.0) },
						{ Vector3( 0.0, 0.5, 0.0), Vector2(1.0, 0.0) },
						{ Vector3(-0.5, 0.0, 0.0), Vector2(0.0, 1.0) },
						{ Vector3( 0.0, 0.0, 0.0), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 2) {
				}
				else if (t == 3) {
				}
				else if (t == 4) {
				}
				else if (t == 5) {
				}
			}
			// [top-right]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopRight);
				int t = info.subtiles[1];
				if (t == 1) {
					putSquare(
						{ Vector3(0.0, 0.5, 0.0), Vector2(0.0, 0.0) },
						{ Vector3(0.5, 0.5, 0.0), Vector2(1.0, 0.0) },
						{ Vector3(0.0, 0.0, 0.0), Vector2(0.0, 1.0) },
						{ Vector3(0.5, 0.0, 0.0), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 2) {
				}
				else if (t == 3) {
				}
				else if (t == 4) {
				}
				else if (t == 5) {
				}
			}
			// [bottom-left]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_BottomLeft);
				int t = info.subtiles[2];
				if (t == 1) {
					putSquare(
						{ Vector3(-0.5,  0.0, 0.0), Vector2(0.0, 0.0) },
						{ Vector3( 0.0,  0.0, 0.0), Vector2(1.0, 0.0) },
						{ Vector3(-0.5, -0.5, 0.0), Vector2(0.0, 1.0) },
						{ Vector3( 0.0, -0.5, 0.0), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
					//x
					//printf("%f, %f, %f, %f\n", uvRect.x, uvRect.y, uvRect.width, uvRect.height);
					//auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_BottomLeft);
				}
				else if (t == 2) {
				}
				else if (t == 3) {
				}
				else if (t == 4) {
				}
				else if (t == 5) {
				}
			}
			// [bottom-right]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_BottomRight);
				int t = info.subtiles[3];
				if (t == 1) {
					putSquare(
						{ Vector3(0.0,  0.0, 0.0), Vector2(0.0, 0.0) },
						{ Vector3(0.5,  0.0, 0.0), Vector2(1.0, 0.0) },
						{ Vector3(0.0, -0.5, 0.0), Vector2(0.0, 1.0) },
						{ Vector3(0.5, -0.5, 0.0), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 2) {
				}
				else if (t == 3) {
				}
				else if (t == 4) {
				}
				else if (t == 5) {
				}
			}

			if (offsetI > startIndex) {
				m_mesh->addSection(startIndex, (offsetI - startIndex) / 3, 0, PrimitiveTopology::TriangleList);
				setInstancedMeshList((int)dir, i, false, makeObject<InstancedMeshList>(m_mesh, m_mesh->sections().size() - 1));
			}
		}
	}


	for (int iFaceDir = 0; iFaceDir < 4; iFaceDir++) {
		MeshTileFaceDirection dir = sideDirs[iFaceDir];
		const Matrix& transform = faceRotation[iFaceDir];
		const Vector3& offset = faceOffset[iFaceDir];

		for (int i = 0; i < 48; i++) {
			const auto& info = MeshTileset::PiledAutoTileTable[i];
			int startIndex = offsetI;

			// [top-left]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopLeft, true);
				int t = info.subtiles[0];
				if (t == 1) {	// No draw.
				}
				else if (t == 2) {
				}
				else if (t == 3) {
					putSquare(
						{ Vector3(-0.5, 0.5, m), Vector2(0.0, 0.0) },
						{ Vector3(-0.5 + m, 0.5, m), Vector2(m, 0.0) },
						{ Vector3(-0.5, 0.0, mh), Vector2(0.0, 1.0) },
						{ Vector3(-0.5 + mh, 0.0, mh), Vector2(mh, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 4) {
				}
				else if (t == 5) {
				}
			}
			// [top-right]
			{
				auto uvRect = uvMapper.getUVRectFromLocalId(dir, i, detail::SubtileCorner::SubtileCorner_TopRight, true);
				int t = info.subtiles[1];
				if (t == 1) {	// No draw.
				}
				else if (t == 2) {	// No draw.
				}
				else if (t == 3) {
					putSquare(
						{ Vector3(0.5 - m, 0.5, m), Vector2(0, 0.0) },
						{ Vector3(0.5, 0.5, m), Vector2(1.0, 0.0) },
						{ Vector3(0.5 - mh, 0.0, mh), Vector2(0, 1.0) },
						{ Vector3(0.5, 0.0, mh), Vector2(1.0, 1.0) },
						uvRect, transform, offset);
				}
				else if (t == 4) {
				}
				else if (t == 5) {	// No draw.
				}
			}

			if (offsetI > startIndex) {
				m_mesh->addSection(startIndex, (offsetI - startIndex) / 3, 0, PrimitiveTopology::TriangleList);
				setInstancedMeshList((int)dir, i, true, makeObject<InstancedMeshList>(m_mesh, m_mesh->sections().size() - 1));
			}

		}
	}
}

void MeshAutoTileset::resetBatch()
{
	for (auto& batch : m_meshList) {
		if (batch) {
			batch->reset();
		}
	}
	for (auto& batch : m_dentMeshList) {
		if (batch) {
			batch->reset();
		}
	}
}

void MeshAutoTileset::setInstancedMeshList(int d, int autotileId, bool dent, InstancedMeshList* value)
{
	int index = (d * 48) + autotileId;
	if (dent)
		m_dentMeshList[index] = value;
	else
		m_meshList[index] = value;
}

InstancedMeshList* MeshAutoTileset::instancedMeshList(int d, int autotileId, bool dent) const
{
	int index = (d * 48) + autotileId;
	if (dent)
		return m_dentMeshList[index];
	else
		return m_meshList[index];
}

void MeshAutoTileset::drawVoxel(const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency, const Matrix& transform, int animationFrame) const
{
	Vector4 uvOffset;
	if (m_animationFrameCount > 0) {
		uvOffset = Vector4(m_frameUVOffset * (animationFrame % m_animationFrameCount), 0, 0);
	}

	for (int d = 0; d < 6; d++) {

		//int t1 = (tile.faceTileId[d] & 0x0F);
		//int t2 = (tile.faceTileId[d] & 0xF0) >> 4;

		//if (tile.faceTileId[d] >= MeshTileset::PiledAutoBlockOffset) {
		//	int lid = tile.faceTileId[d] - MeshTileset::PiledAutoBlockOffset;
		//if (t2 > 0) {
		//	int t = AutBlockPairMap[t1][t2];
		//	auto* batch = instancedMeshList(d, t, true);
		//	if (batch) {
		//		batch->setTransform(transform);
		//		batch->setUVOffset(uvOffset);
		//		batch->drawMesh();
		//	}
		//}
		//else
		if (tile.faceTileId[d] >= MeshTileset::PiledAutoBlockOffset) {
			int lid = tile.faceTileId[d] - MeshTileset::PiledAutoBlockOffset;
			if (lid > 0) {	// [0] は {1,1,1,1} で一切描画しないので処理不要

				auto* batch = instancedMeshList(d, lid, true);
				if (batch) {
					batch->setTransform(transform);
					batch->setUVOffset(uvOffset);
					batch->drawMesh();
				}
			}
		}
		else
		{
			auto* batch = instancedMeshList(d, tile.faceTileId[d], false);
			if (batch) {
				batch->setTransform(transform);
				batch->setUVOffset(uvOffset);
				batch->drawMesh();
			}
		}



		//if (adjacency.state[d] == detail::MeshTileFaceAdjacency::OPEN) {
		//}
		//else if (adjacency.state[d] == detail::MeshTileFaceAdjacency::CONNECTED) {
		//}
	}
}

void MeshAutoTileset::flushBatch(RenderingContext* context)
{
	context->setMaterial(m_material);
	for (auto& batch : m_meshList) {
		if (batch) {
			context->drawMeshInstanced(batch);
		}
	}
	for (auto& batch : m_dentMeshList) {
		if (batch) {
			context->drawMeshInstanced(batch);
		}
	}
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

const MeshTileset::AutoTileInfo MeshTileset::PiledAutoTileTable[48] =
{
	// Block tiles.
	{ 1, 1, 1, 1 },{ 4, 4, 1, 1 },{ 3, 1, 3, 1 },{ 5, 4, 3, 1 }, { 2, 1, 1, 1 },{ 1, 3, 1, 3 },{ 4, 5, 1, 3 },{ 1, 2, 1, 1 },
	{ 3, 3, 3, 3 },{ 5, 5, 3, 3 },{ 2, 3, 1, 3 },{ 3, 2, 3, 1 }, { 2, 2, 1, 1 },{ 1, 1, 4, 4 },{ 4, 4, 4, 4 },{ 3, 1, 5, 4 },
	{ 5, 4, 5, 4 },{ 2, 1, 4, 4 },{ 1, 3, 4, 5 },{ 4, 5, 4, 5 }, { 1, 2, 4, 4 },{ 3, 3, 5, 5 },{ 5, 5, 5, 5 },{ 2, 3, 4, 5 },
	{ 3, 2, 5, 4 },{ 2, 2, 4, 4 },{ 1, 1, 2, 1 },{ 4, 4, 2, 1 }, { 2, 1, 2, 1 },{ 1, 3, 2, 3 },{ 4, 5, 2, 3 },{ 2, 3, 2, 3 },
	{ 1, 2, 2, 1 },{ 2, 2, 2, 1 },{ 1, 1, 1, 2 },{ 4, 4, 1, 2 }, { 1, 2, 1, 2 },{ 3, 1, 3, 2 },{ 5, 4, 3, 2 },{ 2, 1, 1, 2 },
	{ 3, 2, 3, 2 },{ 2, 2, 1, 2 },{ 1, 1, 2, 2 },{ 4, 4, 2, 2 }, { 2, 1, 2, 2 },{ 1, 2, 2, 2 },{ 2, 2, 2, 2 },{ 0, 0, 0, 0 },
	///*[0]*/  {1,1,1,1},{2,1,1,1},{3,3,1,1},{1,2,1,1}, {2,2,1,1},{4,1,4,1},{5,3,4,1},{4,2,4,1},
	///*[8]*/  {1,4,1,4},{2,4,1,4},{3,5,1,4},{4,4,4,4}, {5,5,4,4},{1,1,2,1},{2,1,2,1},{3,3,2,1},
	///*[16]*/ {1,2,2,1},{2,2,2,1},{4,3,4,1},{1,4,2,4}, {2,4,2,4},{3,5,2,4},{1,1,3,3},{2,1,3,3},
	///*[24]*/ {3,3,3,3},{1,2,3,3},{2,2,3,3},{4,1,5,3}, {5,3,5,3},{4,2,5,3},{1,4,3,5},{2,4,3,5},
	///*[32]*/ {3,5,3,5},{4,4,5,5},{5,5,5,5},{1,1,1,2}, {2,1,1,2},{3,3,1,2},{1,2,1,2},{2,2,1,2},
	///*[40]*/ {4,1,4,2},{5,3,4,2},{4,2,4,2},{1,1,2,2}, {2,1,2,2},{3,3,2,2},{1,2,2,2},{2,2,2,2},
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

	auto tex = Texture2D::load(u"autotile2");
	m_material = Material::create(tex);
	m_material->shadingModel = ShadingModel::Unlit;
	m_material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));

	//m_autotileSet[0] = makeObject<MeshAutoTileset>();
	//m_autotileSet[0]->setMaterial(m_material);
	//m_autotileSet[0]->buildQubeFloorAndWall();

	//{

	//	auto tex = Texture2D::load(u"D:/Materials/Tilemap/ねくらファンタジーマップチップ素材集/マップ素材/オートタイル規格2/b_06海岸_at01.png");
	//	auto material = Material::create(tex);
	//	material->shadingModel = ShadingModel::Unlit;
	//	material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));

	//	m_autotileSet[1] = makeObject<MeshAutoTileset>();
	//	m_autotileSet[1]->setMaterial(material);
	//	m_autotileSet[1]->buildFloor();

	//}
	//
	//{


	//	auto tex = Texture2D::load(u"D:/Materials/Tilemap/ねくらファンタジーマップチップ素材集/マップ素材/オートタイル規格2/b_06海岸_at02.png");
	//	auto material = Material::create(tex);
	//	material->shadingModel = ShadingModel::Unlit;
	//	material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));

	//	m_autotileSet[2] = makeObject<MeshAutoTileset>();
	//	m_autotileSet[2]->setMaterial(material);
	//	m_autotileSet[2]->buildFloor();
	//}
	{

		m_autotileSet[3] = makeObject<MeshAutoTileset>();
		m_autotileSet[3]->setMaterial(m_material);
		m_autotileSet[3]->buildFloorAndSlopeWall();
	}
}

void MeshTileset::beginBatch()
{
	for (auto& autotileset : m_autotileSet) {
		if (autotileset) {
			autotileset->resetBatch();
		}
	}
}

void MeshTileset::drawTile(RenderingContext* context, const detail::MeshTile& tile, const detail::MeshTileFaceAdjacency& adjacency, const Matrix& transform, int animationFrame) const
{

	//context->setMaterial(m_material);

	int autotileKind = autoTileKindId(tile.tileId);
	if (autotileKind >= 0) {
		m_autotileSet[autotileKind]->drawVoxel(tile, adjacency, transform, animationFrame);

	}

	//context->drawMeshInstanced(m_meshList);
}

void MeshTileset::drawBatch(RenderingContext* context)
{
	for (auto& autotileset : m_autotileSet) {
		if (autotileset) {
			autotileset->flushBatch(context);
		}
	}
}

namespace detail {

// SubTileId([0] is invalid) => PhysicalTileIndex
static int g_AutoTileSourcePosTable_MVFloor[4][6] =
{
	// [top-left]
	{ -1, 13, 2, 10, 16, 8 },
	// [top-right]
	{ -1, 14, 3, 9, 19, 11 },
	// [bottom-left]
	{ -1, 17, 6, 22, 12, 20 },
	// [bottom-light]
	{ -1, 18, 7, 21, 15, 23 },
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
	else if (m_format == Format::MVFloor) {
		m_subtileUVSize.x = (sourceRect.width / 4) / textureSize.width;
		m_subtileUVSize.y = (sourceRect.height / 6) / textureSize.height;
	}
	else {
		LN_UNREACHABLE();
	}
}

Rect MeshAutoTilesetUVMapper::getUVRectFromLocalId(MeshTileFaceDirection direction, int autotileLocalId, SubtileCorner corner, bool beveled) const
{
	const MeshTileset::AutoTileInfo* info = (beveled) ? &MeshTileset::PiledAutoTileTable[autotileLocalId] : &MeshTileset::AutoTileTable[autotileLocalId];
	

	if (m_format == Format::XP) {
		int index_tl = g_AutoTileSourcePosTable_TkoolXP[corner][info->subtiles[corner]];
		auto offset = Vector2(m_subtileUVSize.x * (index_tl % 6), m_subtileUVSize.y * (index_tl / 6));
		return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
	}
	else if (m_format == Format::MVWithWall) {
		if (direction == MeshTileFaceDirection::YPlus || direction == MeshTileFaceDirection::YMinus) {
			int index_tl = g_AutoTileSourcePosTable_MVFloor[corner][info->subtiles[corner]];
			auto offset = Vector2(m_subtileUVSize.x * (index_tl % 4), m_subtileUVSize.y * (index_tl / 4));
			return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
		}
		else {
			int index_tl = g_AutoTileSourcePosTable_MVWall[corner][info->subtiles[corner]];
			auto offset = Vector2(m_subtileUVSize.x * (index_tl % 4), m_subtileUVSize.y * (index_tl / 4));
			return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
		}
	}
	else if (m_format == Format::MVFloor) {
		int index_tl = g_AutoTileSourcePosTable_MVFloor[corner][info->subtiles[corner]];
		auto offset = Vector2(m_subtileUVSize.x * (index_tl % 4), m_subtileUVSize.y * (index_tl / 4));
		return Rect(offset.x, offset.y, m_subtileUVSize.x, m_subtileUVSize.y);
	}
	else {
		LN_UNREACHABLE();
		return Rect();
	}
}

} // namespace detail
} // namespace ln