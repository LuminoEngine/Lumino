
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/GraphicsManager.h"
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
	}
	else
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

static Vector3 triangleNormal(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 v10 = p1 - p0;
	Vector3 v20 = p2 - p0;
	Vector3 nor = Vector3::cross(v20, v10);
	return Vector3::normalize(nor);
}

void SrMesh::calculateNormals()
{
	for (auto& face : m_faces)
	{
		Vector3 normal;
		auto& loops = face->getLoops();
		for (int iLoop = 2; iLoop < loops.getCount(); iLoop++)
		{
			normal += triangleNormal(
				loops[0]->vertex->position,
				loops[iLoop - 1]->vertex->position,
				loops[iLoop]->vertex->position);
		}
		int triangleCount = loops.getCount() - 2;
		normal /= (float)triangleCount;
		normal = Vector3::safeNormalize(normal, Vector3::UnitY);
		face->setNormal(normal);

		for (auto& loop : loops)
		{
			loop->normal = normal;
		}
	}
}

Ref<MeshResource> SrMesh::generateMeshResource()
{
	auto mesh = MeshResource::create();
	mesh->resizeVertexBuffer(m_loops.getCount());

	// vertex buffer
	int iVertex = 0;
	int triangleCount = 0;
	for (auto& face : m_faces)
	{
		for (auto& loop : face->getLoops())
		{
			mesh->setPosition(iVertex, loop->vertex->position);
			mesh->setNormal(iVertex, loop->normal);	// TODO:
			mesh->setUV(iVertex, loop->uv);
			mesh->setColor(iVertex, loop->color);
			iVertex++;
		}

		triangleCount += face->getLoops().getCount() - 2;
	}

	// index buffer (fan)
	mesh->resizeIndexBuffer(triangleCount * 3);
	int iIndex = 0;
	iVertex = 0;
	for (auto& face : m_faces)
	{
		int firstVertex = iVertex;
		for (int iLoop = 2; iLoop < face->getLoops().getCount(); iLoop++)
		{
			mesh->setIndex(iIndex + 0, firstVertex);
			mesh->setIndex(iIndex + 1, firstVertex + iLoop - 1);
			mesh->setIndex(iIndex + 2, firstVertex + iLoop);
			iIndex += 3;
		}

		iVertex += face->getLoops().getCount();
	}

	// TODO:
	MeshAttribute sec;
	sec.MaterialIndex = 0;
	sec.StartIndex = 0;
	sec.PrimitiveNum = triangleCount;
	sec.primitiveType = PrimitiveType_TriangleList;
	mesh->addMeshSection(sec);

	return mesh;
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

void SrMeshModel::calculateNormals()
{
	for (auto& mesh : m_meshes)
	{
		mesh->calculateNormals();
	}
}

Ref<StaticMeshModel> SrMeshModel::generateStaticMeshModel()
{
	auto staticMeshModel = newObject<StaticMeshModel>(ln::detail::GraphicsManager::getInstance());

	// copy materials
	for (auto& material : materials)
	{
		staticMeshModel->addMaterial(material);
	}

	// make MeshResource
	for (auto& mesh : m_meshes)
	{
		staticMeshModel->addMeshResource(mesh->generateMeshResource());
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
