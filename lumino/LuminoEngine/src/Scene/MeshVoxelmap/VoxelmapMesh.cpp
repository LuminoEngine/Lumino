
#include "Internal.hpp"
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>
#include "VoxelmapMesh.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshAutoVoxelset

const Matrix VoxelmapMeshBuilder::RotationTransforms[6] = {
	Matrix::makeRotationY(Math::PI / 2),
	Matrix::makeRotationY(-Math::PI / 2),
	Matrix::makeRotationX(-Math::PI / 2),
	Matrix::makeRotationX(Math::PI / 2),
	Matrix(),
	Matrix::makeRotationY(Math::PI),
};

const Vector3 VoxelmapMeshBuilder::TransformOffsets[6] = {
	Vector3(0.0, 0.5, 0.5),
	Vector3(1.0, 0.5, 0.5),
	Vector3(0.5, 0.0, 0.5),
	Vector3(0.5, 1.0, 0.5),
	Vector3(0.5, 0.5, 0.0),
	Vector3(0.5, 0.5, 1.0),
};

void VoxelmapMeshBuilder::beginSection(MeshTileFaceDirection dir, int autovoxelLocalId, VoxelMeshFaceKind faceKind)
{
	m_curDir = dir;
	m_curUVRect = Rect::Empty;
	m_curAutovoxelLocalId = autovoxelLocalId;
	m_curFaceKind = faceKind;
	m_startVertex = m_vertices.size();
	m_offsetIndex = m_startIndex = m_indices.size();
}

void VoxelmapMeshBuilder::endSection()
{
	for (int i = m_startVertex; i < m_vertices.size(); i++) {
		auto& v = m_vertices[i];
		v.setPosition(Vector3::transformCoord(v.position.xyz(), RotationTransforms[(int)m_curDir]) + TransformOffsets[(int)m_curDir]);
		v.setNormal(Vector3::transformCoord(v.normal.xyz(), RotationTransforms[(int)m_curDir]));
	}

	if (m_offsetIndex > m_startIndex) {
		m_sections.push_back({ m_curDir, m_curAutovoxelLocalId, m_curFaceKind, { m_startIndex, (m_offsetIndex - m_startIndex) / 3, 0, PrimitiveTopology::TriangleList } });
	}
}

void VoxelmapMeshBuilder::beginSubtile()
{
	m_subtileStartVertex = m_vertices.size();
}

void VoxelmapMeshBuilder::endSubtile()
{
}

void VoxelmapMeshBuilder::putSquareChamfer(float l, float t, float r, float b, SubtileCorner corner, float depth)
{
	int v = m_vertices.size();
	float ru = 0;//depth / std::abs(r - l);
	float rv = 0;//depth / std::abs(b - t);
	switch (corner)
	{
	case ln::detail::SubtileCorner_TopLeft:
		m_vertices.push_back({ Vector3(l, t - depth, 0), Vector3::UnitZ, Vector2(0, rv), Color::White });
		m_vertices.push_back({ Vector3(l + depth, t - depth, 0), Vector3::UnitZ, Vector2(ru, rv), Color::White });
		m_vertices.push_back({ Vector3(l + depth, t, 0), Vector3::UnitZ, Vector2(ru, 0), Color::White });
		m_vertices.push_back({ Vector3(r, t, 0), Vector3::UnitZ, Vector2(1, 0), Color::White });
		m_vertices.push_back({ Vector3(r, b, 0), Vector3::UnitZ, Vector2(1, 1), Color::White });
		m_vertices.push_back({ Vector3(l, b, 0), Vector3::UnitZ, Vector2(0, 1), Color::White });
		m_indices.push_back(v + 4); m_indices.push_back(v + 5); m_indices.push_back(v + 0);
		m_indices.push_back(v + 4); m_indices.push_back(v + 0); m_indices.push_back(v + 1);
		m_indices.push_back(v + 4); m_indices.push_back(v + 1); m_indices.push_back(v + 2);
		m_indices.push_back(v + 4); m_indices.push_back(v + 2); m_indices.push_back(v + 3);
		break;
	case ln::detail::SubtileCorner_TopRight:
		m_vertices.push_back({ Vector3(l, t, 0), Vector3::UnitZ, Vector2(0, 0), Color::White });
		m_vertices.push_back({ Vector3(r - depth, t, 0), Vector3::UnitZ, Vector2(1 - ru, 0), Color::White });
		m_vertices.push_back({ Vector3(r - depth, t - depth, 0), Vector3::UnitZ, Vector2(1 - ru, rv), Color::White });
		m_vertices.push_back({ Vector3(r, t - depth, 0), Vector3::UnitZ, Vector2(1, rv), Color::White });
		m_vertices.push_back({ Vector3(r, b, 0), Vector3::UnitZ, Vector2(1, 1), Color::White });
		m_vertices.push_back({ Vector3(l, b, 0), Vector3::UnitZ, Vector2(0, 1), Color::White });
		m_indices.push_back(v + 5); m_indices.push_back(v + 0); m_indices.push_back(v + 1);
		m_indices.push_back(v + 5); m_indices.push_back(v + 1); m_indices.push_back(v + 2);
		m_indices.push_back(v + 5); m_indices.push_back(v + 2); m_indices.push_back(v + 3);
		m_indices.push_back(v + 5); m_indices.push_back(v + 3); m_indices.push_back(v + 4);
		break;
	case ln::detail::SubtileCorner_BottomRight:
		m_vertices.push_back({ Vector3(l, t, 0), Vector3::UnitZ, Vector2(0, 0), Color::White });
		m_vertices.push_back({ Vector3(r, t, 0), Vector3::UnitZ, Vector2(1, 0), Color::White });
		m_vertices.push_back({ Vector3(r, b + depth, 0), Vector3::UnitZ, Vector2(1, 1 - rv), Color::White });
		m_vertices.push_back({ Vector3(r - depth, b + depth, 0), Vector3::UnitZ, Vector2(1 - ru, 1 - rv), Color::White });
		m_vertices.push_back({ Vector3(r - depth, b, 0), Vector3::UnitZ, Vector2(1 - ru, 1), Color::White });
		m_vertices.push_back({ Vector3(l, b, 0), Vector3::UnitZ, Vector2(0, 1), Color::White });
		m_indices.push_back(v + 0); m_indices.push_back(v + 1); m_indices.push_back(v + 2);
		m_indices.push_back(v + 0); m_indices.push_back(v + 2); m_indices.push_back(v + 3);
		m_indices.push_back(v + 0); m_indices.push_back(v + 3); m_indices.push_back(v + 4);
		m_indices.push_back(v + 0); m_indices.push_back(v + 4); m_indices.push_back(v + 5);
		break;
	case ln::detail::SubtileCorner_BottomLeft:
		m_vertices.push_back({ Vector3(l, t, 0), Vector3::UnitZ, Vector2(0.0f, 0.0f), Color::White });
		m_vertices.push_back({ Vector3(r, t, 0), Vector3::UnitZ, Vector2(1.0f, 0.0f), Color::White });
		m_vertices.push_back({ Vector3(r, b, 0), Vector3::UnitZ, Vector2(1.0f, 1.0f), Color::White });
		m_vertices.push_back({ Vector3(l + depth, b, 0), Vector3::UnitZ, Vector2(ru, 1.0f), Color::White });
		m_vertices.push_back({ Vector3(l + depth, b + depth, 0), Vector3::UnitZ, Vector2(ru, 1.0f - rv), Color::White });
		m_vertices.push_back({ Vector3(l, b + depth, 0), Vector3::UnitZ, Vector2(0.0f, 1.0f - rv), Color::White });
		m_indices.push_back(v + 1); m_indices.push_back(v + 2); m_indices.push_back(v + 3);
		m_indices.push_back(v + 1); m_indices.push_back(v + 3); m_indices.push_back(v + 4);
		m_indices.push_back(v + 1); m_indices.push_back(v + 4); m_indices.push_back(v + 5);
		m_indices.push_back(v + 1); m_indices.push_back(v + 5); m_indices.push_back(v + 0);
		break;
	default:
		LN_UNREACHABLE();
		return;
	}
	m_offsetIndex += 12;
}

void VoxelmapMeshBuilder::projectUV(const Rect& uvRect)
{
	for (int i = m_subtileStartVertex; i < m_vertices.size(); i++) {
		auto& v = m_vertices[i];
		v.setUV(uvRect.getPointByRatio(v.uv.xy()));
	}
}

void VoxelmapMeshBuilder::applyTileCenterMagnitude()
{
	float mag = 0.05;
	for (auto& v : m_vertices) {
		auto pos = v.position;
		{
			float sign = (std::fmodf(pos.x, 1.0f) > 0.5f) ? 1 : -1;
			float r = 1.0f - Math::clamp01(std::abs(std::fmodf(pos.x, 1.0f)) / 0.5f);
			v.position.z += sign * mag * r;
		}
		{
			float sign = (std::fmodf(pos.x, 1.0f) > 0.5f) ? 1 : -1;
			float r = 1.0f - Math::clamp01(std::abs(std::fmodf(pos.x, 1.0f)) / 0.5f);
			v.position.y += sign * mag * r;
		}
		{
			float sign = (std::fmodf(pos.z, 1.0f) > 0.5f) ? 1 : -1;
			float r = 1.0f - Math::clamp01(std::abs(std::fmodf(pos.z, 1.0f)) / 0.5f);
			v.position.x += sign * mag * r;
		}
	}
}

void VoxelmapMeshBuilder::putSquare(const Vector3& p0, const Vector2& uv0, const Vector3& p1, const Vector2& uv1, const Vector3& p2, const Vector2& uv2, const Vector3& p3, const Vector2& uv3)
{
	int v = m_vertices.size();
	m_vertices.push_back(Vertex{ p0, Vector3::UnitZ, uv0, Color::White });
	m_vertices.push_back(Vertex{ p1, Vector3::UnitZ, uv1, Color::White });
	m_vertices.push_back(Vertex{ p2, Vector3::UnitZ, uv2, Color::White });
	m_vertices.push_back(Vertex{ p3, Vector3::UnitZ, uv3, Color::White });
	m_indices.push_back(v + 0);
	m_indices.push_back(v + 1);
	m_indices.push_back(v + 2);
	m_indices.push_back(v + 2);
	m_indices.push_back(v + 1);
	m_indices.push_back(v + 3);
	m_offsetIndex += 6;
}

void VoxelmapMeshBuilder::putSquare(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	putSquare(p0, Vector2(0, 0), p1, Vector2(1, 0), p2, Vector2(0, 1), p3, Vector2(1, 1));
}

void VoxelmapMeshBuilder::build()
{
	m_mesh = makeObject<MeshPrimitive>(m_vertices.size(), m_indices.size());
	for (int i = 0; i < m_vertices.size(); i++) {	// TODO: vector をまとめて set する機能作ってもいいかも
		m_mesh->setVertex(i, m_vertices[i]);
	}
	for (int i = 0; i < m_indices.size(); i++) {	// TODO: vector をまとめて set する機能作ってもいいかも
		m_mesh->setIndex(i, m_indices[i]);
	}

	for (auto& section : m_sections) {
		m_mesh->addSection(section.section.startIndex, section.section.primitiveCount, section.section.materialIndex, section.section.topology);
		auto list = makeObject<InstancedMeshList>(m_mesh, m_mesh->sections().size() - 1);
		int listIndex = (static_cast<int>(section.dir) * 48) + section.autovoxelLocalId;
		if (section.faceKind == VoxelMeshFaceKind::Convex) {
			m_convexMeshList[listIndex] = list;
		}
		else if (section.faceKind == VoxelMeshFaceKind::Concave) {
			m_concaveMeshList[listIndex] = list;
		}
		else {
			LN_UNREACHABLE();
		}
	}
}

} // namespace detail
} // namespace ln