
#include "../Internal.h"
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/GraphicsManager.h"
#include "SMesh.h"

LN_NAMESPACE_BEGIN
namespace tr {

static Vector3 triangleNormal(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 v10 = p1 - p0;
	Vector3 v20 = p2 - p0;
	Vector3 nor = Vector3::cross(v20, v10);
	return Vector3::normalize(nor);
}

//==============================================================================
// SrEdge
//==============================================================================

void SrEdge::addLinkFace(SrFace* face)
{
	if (LN_REQUIRE(!m_linkFaces.contains(face))) return;
	m_linkFaces.add(face);
}

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
	v1->addLinkEdge(edge);
	v2->addLinkEdge(edge);

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

	SrLoop* prevLoop = nullptr;
	for (int i = 0; i < count; i++)
	{
		SrVertex* from = getVertex(indices[i]);
		SrVertex* next = (i == count - 1) ? getVertex(indices[0]) : getVertex(indices[i + 1]);

		SrEdge* edge = findEdge(from, next);
		if (!edge) edge = makeEdge(from, next);

		SrLoop* loop = makeLoop(edge, from, next);
		loop->prevLoop = prevLoop;

		face->m_loops.add(loop);
		edge->addLinkFace(face);

		prevLoop = loop;
	}

	face->m_loops.getFront()->prevLoop = prevLoop;

	return face;
}

SrEdge* SrMesh::findEdge(SrVertex* v1, SrVertex* v2)
{
	for (auto& edge : v1->getLinkEdges())
	{
		if (edge->m_vertices[0] == v2 || edge->m_vertices[1] == v2)
		{
			return edge;
		}
	}
	return nullptr;
}

void SrMesh::calculateNormals(float smoothRadius)
{
	smoothRadius /= 2;
	float smoothThr = (-1.0 * (smoothRadius / Math::PIDiv2)) + 1.0f;	// 0 .. 180 -> 1.0 .. -1.0

	// 各面の法線を求める
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
		normal = Vector3::safeNormalize(normal / (float)triangleCount, Vector3::UnitY);
		face->setNormal(normal);
	}

	// 各辺の法線を求める (隣接面の平均)
	for (auto& edge : m_edges)
	{
		auto& faces = edge->getLinkFaces();
		Vector3 normal;
		for (auto& face : faces)
		{
			normal += face->getNormal();
		}
		normal = Vector3::safeNormalize(normal / (float)faces.getCount(), Vector3::UnitY);
		edge->setNormal(normal);
	}

	// ループの法線を求める (必要に応じてスムージング)
	for (auto& face : m_faces)
	{
		for (auto& loop : face->getLoops())
		{
			if (smoothRadius == 0.0f)
			{
				loop->normal = face->getNormal();
			}
			else
			{
				Vector3 normal;
				int smoothNormalCount = 0;
				SrEdge* edges[] = { loop->prevLoop->edge, loop->edge };
				for (int i = 0; i < 2; i++)
				{
					float d = Vector3::dot(edges[i]->getNormal(), face->getNormal());
					if (d >= smoothThr)
					{
						normal += edges[i]->getNormal();
						smoothNormalCount++;
					}
				}
				if (smoothNormalCount > 0)
				{
					loop->normal = Vector3::safeNormalize(normal / (float)smoothNormalCount, Vector3::UnitY);
				}
				else
				{
					loop->normal = face->getNormal();
				}
			}
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
			mesh->setNormal(iVertex, loop->normal);
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

void SrMeshModel::addMaterial(CommonMaterial* material)
{
	materials.add(material);
}

CommonMaterial* SrMeshModel::getMaterial(int index)
{
	return materials[index];
}

void SrMeshModel::calculateNormals()
{
	for (auto& mesh : m_meshes)
	{
		mesh->calculateNormals(Math::degreesToRadians(59.5));	// TODO:
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
