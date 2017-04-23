
#include "../Internal.h"
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMesh
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMesh, VisualNode);

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::Create(StaticMeshModel* staticMeshModel)
{
	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::Create(const StringRef& filePath)
{
	auto ptr = RefPtr<StaticMesh>::MakeRef();
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateStaticMeshModel(filePath);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = RefPtr<StaticMesh>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateScreenPlane()
{
	auto ptr = RefPtr<StaticMesh>::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->GetGraphicsManager(), MeshCreationFlags::None);
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateBox()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateBox(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateBox(float width, float height, float depth)
{
	return CreateBox(Vector3(width, height, depth));
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateSphere()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateSphere(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateTeapot()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitTeapotMeshResource());
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::CreateTeapot(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->GetGraphicsManager(), size, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMesh>::MakeRef();
	ptr->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMesh::StaticMesh()
	: m_mesh()
{
}

//------------------------------------------------------------------------------
StaticMesh::~StaticMesh()
{
}

//------------------------------------------------------------------------------
void StaticMesh::Initialize(SceneGraph* owner, StaticMeshModel* meshModel)
{
	if (LN_CHECK_ARG(meshModel != nullptr)) return;
	m_mesh = meshModel;

	VisualNode::Initialize(owner, m_mesh->GetSubsetCount());

	m_materialList->CopyShared(m_mesh->m_materials, true);


	owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMesh::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(m_combinedGlobalMatrix);

	MeshResource* m = m_mesh->GetMeshResource();
	for (int i = 0; i < m->GetSubsetCount(); i++)
	{
		renderer->DrawMesh(m, i, GetMaterials()->GetAt(i));
	}
}

//==============================================================================
// Rectangle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Rectangle, VisualNode);

//------------------------------------------------------------------------------
RefPtr<Rectangle> Rectangle::Create(const RectF& rect)
{
	return NewObject<Rectangle>(rect);
}

//------------------------------------------------------------------------------
Rectangle::Rectangle()
{
}

//------------------------------------------------------------------------------
Rectangle::~Rectangle()
{
}

//------------------------------------------------------------------------------
void Rectangle::Initialize(const RectF& rect)
{
	VisualNode::Initialize(detail::EngineDomain::GetDefaultSceneGraph3D(), 1);
	detail::EngineDomain::GetDefaultSceneGraph3D()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);

	m_rect = rect;
}

//------------------------------------------------------------------------------
void Rectangle::OnRender2(DrawList* renderer)
{
	//renderer->SetShader(GetPrimaryShader());
	renderer->DrawSquarePrimitive(
		Vector3(m_rect.GetTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.GetTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.GetBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.GetBottomRight(), 0), Vector2(1, 1), Color::White);
}

LN_NAMESPACE_END
