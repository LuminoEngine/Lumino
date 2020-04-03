
#include "Internal.hpp"
#include "VoxelmapMesh.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshAutoTileset

const Matrix VoxelmapMeshBuilder::RotationTransforms[6] = {
	Matrix::makeRotationY(Math::PI / 2),
	Matrix::makeRotationY(-Math::PI / 2),
	Matrix::makeRotationX(Math::PI / 2),
	Matrix::makeRotationX(-Math::PI / 2),
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
	//m_offsetVertex =
	m_startVertex = m_vertices.size();
	m_offsetIndex = m_startIndex = m_indices.size();
}

void VoxelmapMeshBuilder::endSection()
{
	for (int i = m_startVertex; i < m_vertices.size(); i++) {
		auto& v = m_vertices[i];
		v.position = Vector3::transformCoord(v.position, RotationTransforms[(int)m_curDir]) + TransformOffsets[(int)m_curDir];
		v.normal = Vector3::transformCoord(v.normal, RotationTransforms[(int)m_curDir]);
	}

	if (m_offsetIndex > m_startIndex) {
		m_sections.push_back({ m_curDir, m_curAutovoxelLocalId, m_curFaceKind, { m_startIndex, (m_offsetIndex - m_startIndex) / 3, 0, PrimitiveTopology::TriangleList } });
	}
}

void VoxelmapMeshBuilder::beginSubtile()
{
	m_subtileStartVertex = m_vertices.size();
}

void VoxelmapMeshBuilder::projectUV(const Rect& uvRect)
{
	for (int i = m_subtileStartVertex; i < m_vertices.size(); i++) {
		auto& v = m_vertices[i];
		v.uv = uvRect.getPointByRatio(v.uv);
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
	//m_offsetVertex += 4;
	m_offsetIndex += 6;
}

void VoxelmapMeshBuilder::putSquare(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	putSquare(p0, Vector2(0, 0), p1, Vector2(1, 0), p2, Vector2(0, 1), p3, Vector2(1, 1));
}

void VoxelmapMeshBuilder::build()
{
	m_mesh = makeObject<Mesh>(m_vertices.size(), m_indices.size());
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