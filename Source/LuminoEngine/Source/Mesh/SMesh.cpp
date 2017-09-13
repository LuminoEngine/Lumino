
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
#include "SMesh.h"

LN_NAMESPACE_BEGIN
namespace tr {

//==============================================================================
// SrMesh
//==============================================================================

void SrMesh::addVertices(int count)
{
	int start = m_vertices.getCount();
	m_vertices.resize(start + count);
	for (int i = 0; i < count; i++)
	{
		m_vertices[start + i] = newObject<SrVertex>();
	}
}

SrVertex* SrMesh::getVertex(int index)
{
	return m_vertices[index];
}

SrEdge* SrMesh::makeEdge(SrVertex* v1, SrVertex* v2)
{
	auto edge = newObject<SrEdge>();
	m_edges.add(edge);

	edge->m_vertices[0] = v1;
	edge->m_vertices[1] = v2;

	return edge;
}

SrLoop* SrMesh::makeLoop(SrEdge* edge, SrVertex* from, SrVertex* next)
{
	if (LN_REQUIRE(edge)) return nullptr;

	// 検証。from と next は edge の一部であること。
	if ((edge->m_vertices[0] == from || edge->m_vertices[1] == from) &&
		(edge->m_vertices[0] == next || edge->m_vertices[1] == next))
	{
		LN_REQUIRE(0);
		return nullptr;
	}

	auto loop = newObject<SrLoop>();
	m_loops.add(loop);
	loop->edge = edge;
	loop->vertex = from;
	return loop;
}

SrFace* SrMesh::makeFace(const int* indices, int count)
{
	if (LN_REQUIRE(indices)) return nullptr;
	if (LN_REQUIRE(count >= 3)) return nullptr;

	auto face = newObject<SrFace>();
	m_faces.add(face);

	for (int i = 0; i < count; i++)
	{
		SrVertex* from = getVertex(indices[i]);
		SrVertex* next = (i == count - 1) ? getVertex(indices[0]) : getVertex(indices[i + 1]);

		SrEdge* edge = makeEdge(from, next);
		SrLoop* loop = makeLoop(edge, from, next);

		face->m_loops.add(loop);
	}

	return face;
}

Ref<MeshResource> SrMesh::generateMeshResource()
{

}

//==============================================================================
// SrMeshModel
//==============================================================================

SrMesh* SrMeshModel::addMesh()
{
	auto mesh = newObject<SrMesh>();
	m_meshes.add(mesh);
	return mesh;
}

void SrMeshModel::addMaterial(Material* material)
{
	materials.add(material);
}

Material* SrMeshModel::getMaterial(int index)
{
	return materials[index];
}

Ref<StaticMeshModel> SrMeshModel::generateStaticMeshModel()
{
	auto staticMeshModel = newObject<StaticMeshModel>();

	// copy materials
	for (auto& material : materials)
	{
		staticMeshModel->addMaterial(material);
	}

	for (auto& mesh : m_meshes)
	{

	}

	return staticMeshModel;
}

//==============================================================================
// SrMeshOperation
//==============================================================================

void SrMeshOperation::moveVertex(SrVertex* v, const Vector3& pos)
{
	if (LN_REQUIRE(v)) return;

	v->setPosition(pos);
}

} // namespace tr
LN_NAMESPACE_END
