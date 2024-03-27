
#include "Internal.hpp"
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/detail/GraphicsManager.hpp>
#include <LuminoEngine/Mesh/detail/MeshManager.hpp>
#include "GMesh.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GMesh

GMesh::GMesh(MeshManager* manager)
	: m_linearAllocator(makeRef<LinearAllocator>(manager->graphicsManager()->linearAllocatorPageManager()))
{
}

void GMesh::setVertexCount(int count)
{
	m_vertices.resize(count);
	for (int i = 0; i < count; i++) {
		m_vertices[i] = newData<GVertex>();
	}
}

void GMesh::reserveFaceCount(int count)
{
	m_faces.reserve(count);
}

GEdge* GMesh::makeEdge(GVertex* v1, GVertex* v2)
{
	GEdge* edge = newData<GEdge>();
	m_edges.push_back(edge);

	edge->v1 = v1;
	edge->v2 = v2;
	v1->linkEdges.push_back(edge);
	v2->linkEdges.push_back(edge);

	return edge;
}

GLoop* GMesh::makeLoop(GEdge* parentEdge, GVertex* from, GVertex* next)
{
	if (LN_REQUIRE(parentEdge)) return nullptr;

	// 検証。from と next は edge の一部であること。
	if ((parentEdge->v1 == from || parentEdge->v2 == from) &&
		(parentEdge->v1 == next || parentEdge->v2 == next))
	{
	}
	else
	{
		LN_REQUIRE(0);
		return nullptr;
	}

	GLoop* loop = newData<GLoop>();
	m_loops.push_back(loop);
	loop->edge = parentEdge;
	loop->vertex = from;
	parentEdge->linkLoops.push_back(loop);
	return loop;
}

GFace* GMesh::makeFace(const int* indices, int count)
{
	if (LN_REQUIRE(indices)) return nullptr;
	if (LN_REQUIRE(count >= 3)) return nullptr;

	GFace* face = newData<GFace>();
	m_faces.push_back(face);

	GLoop* prevLoop = nullptr;
	for (int i = 0; i < count; i++)
	{
		// 始点と終点を選択
		GVertex* fromVertex = vertex(indices[i]);
		GVertex* nextVertex = (i == count - 1) ? vertex(indices[0]) : vertex(indices[i + 1]);

		// 始点と終点を結ぶ Edge を探す。なければ作る
		GEdge* edge = findEdge(fromVertex, nextVertex);
		if (!edge) edge = makeEdge(fromVertex, nextVertex);

		GLoop* loop = makeLoop(edge, fromVertex, nextVertex);
		loop->face = face;
		loop->prevLoop = prevLoop;
		if (prevLoop) {
			prevLoop->nextLoop = loop;
		}

		if (!face->startLoop) {
			face->startLoop = loop;
		}

		//edge->addLinkFace(face);

		prevLoop = loop;
	}

	prevLoop->nextLoop = face->startLoop;
	face->startLoop->prevLoop = prevLoop;

	return face;
}

GEdge* GMesh::findEdge(GVertex* v1, GVertex* v2) const
{
	// v1 を参照している Edge のいずれかに v2 があるかを見る
	for (GEdge* edge : v1->linkEdges) {
		if (edge->v1 == v2 || edge->v2 == v2) {
			return edge;
		}
	}
	return nullptr;
}

//GVertex* GMesh::newVertex()
//
//GFace* GMesh::newFace()
//{
//
//}


//==============================================================================
// GMeshOperations

static Vector3 triangleNormal(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	Vector3 v10 = p1 - p0;
	Vector3 v20 = p2 - p0;
	Vector3 nor = Vector3::cross(v20, v10);
	return Vector3::normalize(nor);
}

void GMeshOperations::calculateNormals(GMesh* mesh, float smoothRadius)
{
	if (LN_REQUIRE(mesh)) return;

	smoothRadius /= 2;
	float smoothThr = (-1.0 * (smoothRadius / Math::PIDiv2)) + 1.0f;	// 0 .. 180 -> 1.0 .. -1.0

	// 各面の法線を求める
	for (GFace* face : mesh->m_faces)
	{
		Vector3 normal;
		face->foreachTriangleFans([&](GLoop* l1, GLoop* l2, GLoop* l3) {
			normal += triangleNormal(
				l1->vertex->position,
				l2->vertex->position,
				l3->vertex->position);
		});
		normal = Vector3::safeNormalize(normal, Vector3::UnitY);
		face->normal = normal;
	}

	//// 各辺の法線を求める (隣接面の平均)
	//for (auto& edge : m_edges)
	//{
	//	auto& faces = edge->getLinkFaces();
	//	Vector3 normal;
	//	for (auto& face : faces)
	//	{
	//		normal += face->getNormal();
	//	}
	//	normal = Vector3::safeNormalize(normal / (float)faces.getCount(), Vector3::UnitY);
	//	edge->setNormal(normal);
	//}

	// ループの法線を求める (必要に応じてスムージング)
	//for (GEdge* edge : mesh->m_edges)
	//{

	//}


	for (GLoop* loop : mesh->m_loops)
	{
		if (smoothRadius == 0.0f)
		{
			loop->normal = loop->face->normal;
		}
		//else
		//{
		//	Vector3 normal;
		//	int smoothNormalCount = 0;
		//	SrEdge* edges[] = { loop->prevLoop->edge, loop->edge };
		//	for (int i = 0; i < 2; i++)
		//	{
		//		float d = Vector3::dot(edges[i]->getNormal(), face->getNormal());
		//		if (d >= smoothThr)
		//		{
		//			normal += edges[i]->getNormal();
		//			smoothNormalCount++;
		//		}
		//	}

		//	if (smoothNormalCount > 0)
		//	{
		//		loop->normal = Vector3::safeNormalize(normal / (float)smoothNormalCount, Vector3::UnitY);
		//	}
		//	else
		//	{
		//		loop->normal = loop->face->normal;
		//	}
		//}
	}
}

Ref<MeshResource> GMeshOperations::generateMeshResource(GMesh* mesh)
{
	auto meshResource = makeObject_deprecated<MeshResource>();
	meshResource->resizeVertexBuffer(mesh->m_loops.size());
	// TODO: 仮
	meshResource->resizeIndexBuffer(6);
	int iVertex = 0;
	int iIndex = 0;
	int triangleCount = 0;
	for (GFace* face : mesh->m_faces)
	{
		// vertex buffer
		face->foreachLoops([&](GLoop* loop, int i) {
			meshResource->setVertex(iVertex, Vertex(
				loop->vertex->position,
				loop->normal,
				loop->uv,
				loop->color));
			iVertex++;
		});

		// index buffer (fan)
		int firstVertex = 0;	// TODO: 仮
		int iLoop = 0;
		face->foreachTriangleFans([&](GLoop* l1, GLoop* l2, GLoop* l3) {
			meshResource->setIndex(iIndex + 0, firstVertex);
			meshResource->setIndex(iIndex + 1, firstVertex + iLoop + 1);
			meshResource->setIndex(iIndex + 2, firstVertex + iLoop + 2);
			iIndex += 3;
			iLoop++;
		});
		triangleCount += iLoop;
	}

	// TODO:
	meshResource->resizeSections(1);
	meshResource->setSection(0, 0, triangleCount, 0);

	return meshResource;
}

} // namespace detail
} // namespace ln

