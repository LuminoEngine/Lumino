#if 0
#include "../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneTR.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// HugePlane
//==============================================================================

//------------------------------------------------------------------------------
HugePlanePtr HugePlane::create(const Vector3& direction)
{
	auto ptr = HugePlanePtr::makeRef();
	ptr->initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), direction);
	return ptr;
}

//------------------------------------------------------------------------------
HugePlane::HugePlane()
	: VisualComponent()
{
}

//------------------------------------------------------------------------------
HugePlane::~HugePlane()
{
}

//------------------------------------------------------------------------------
void HugePlane::initialize(SceneGraph* sceneGraph, const Vector3& direction)
{
	VisualComponent::initialize(sceneGraph, 1);
	sceneGraph->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_mesh = RefPtr<MeshResource>::makeRef();
	m_mesh->initialize(sceneGraph->GetManager()->GetGraphicsManager());
	m_mesh->CreateSquarePlane(Vector2(1, 1), direction, MeshCreationFlags::DynamicBuffers);

}

//------------------------------------------------------------------------------
void HugePlane::SetTexture(Texture* texture)
{
	m_materialList->GetAt(0)->SetMaterialTexture(texture);
}

//------------------------------------------------------------------------------
void HugePlane::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	struct Line
	{
		Vector3	from;
		Vector3	to;
	};

	if (subsetIndex == 0)
	{
		/*
			まず、視錐台の全ての辺について、m_direction を向く平面との交点を求める。
			求めることができた全ての交点の中から、
		*/
		//auto& vf = dc->CurrentCamera->getViewFrustum();
		//Vector3 points[8];
		//vf.getCornerPoints(points);

		//Line lines[12] =
		//{
		//	{ points[0], points[1] },
		//	{ points[1], points[2] },
		//	{ points[2], points[3] },
		//	{ points[3], points[0] },

		//	{ points[0], points[4] },
		//	{ points[1], points[5] },
		//	{ points[2], points[6] },
		//	{ points[3], points[7] },

		//	{ points[4], points[5] },
		//	{ points[5], points[6] },
		//	{ points[6], points[7] },
		//	{ points[7], points[4] },
		//};

		//Plane plane(m_direction, 0);
		//List<Vector3> hits;
		//for (Line& li : lines)
		//{
		//	Vector3 pt;
		//	if (plane.intersects(li.from, li.to, &pt))
		//	{
		//		hits.Add(pt);
		//	}
		//}

		//Vector3 minPos, maxPos;
		//for (Vector3& p : hits)
		//{
		//	minPos = Vector3::min(p, minPos);
		//	maxPos = Vector3::max(p, maxPos);
		//}

		//m_mesh->setPosition(0, Vector3(minPos.x, 0, maxPos.z));
		//m_mesh->setPosition(1, Vector3(minPos.x, 0, minPos.z));
		//m_mesh->setPosition(2, Vector3(maxPos.x, 0, maxPos.z));
		//m_mesh->setPosition(3, Vector3(maxPos.x, 0, minPos.z));
		//m_mesh->setUV(0, Vector2(-1.0f, 1.0f));
		//m_mesh->setUV(1, Vector2(-1.0f, -1.0f));
		//m_mesh->setUV(2, Vector2(1.0f, 1.0f));
		//m_mesh->setUV(3, Vector2(1.0f, -1.0f));

		dc->DrawMesh(m_mesh, m_mesh->m_attributes[subsetIndex].StartIndex, m_mesh->m_attributes[subsetIndex].PrimitiveNum);
	}
}

} // namespace tr
LN_NAMESPACE_END
#endif
