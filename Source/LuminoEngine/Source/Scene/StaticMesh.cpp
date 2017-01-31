
#include "../Internal.h"
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMesh
//==============================================================================

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::Create(const StringRef& filePath)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateStaticMeshModel(filePath);
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateSphere(float radius, int slices, int stacks, MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, slices, stacks, flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateScreenPlane()
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->GetGraphicsManager(), MeshCreationFlags::None);
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateTeapot(MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->GetGraphicsManager(), flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
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
	LN_CHECK_ARG(meshModel != nullptr);
	m_mesh = meshModel;

	VisualNode::Initialize(owner, m_mesh->GetSubsetCount());

	m_materialList->CopyShared(m_mesh->m_materials, true);


	owner->GetManager()->GetDefaultSceneGraph3D()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
//void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	dc->DrawMesh(m_mesh, subsetIndex);
//}

//------------------------------------------------------------------------------
void StaticMesh::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(m_combinedGlobalMatrix);
	renderer->DrawMesh(m_mesh, 0, GetMainMaterial()/*GetMaterials()->GetAt(0)*/);
}

//==============================================================================
// BoxMesh
//==============================================================================

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create()
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D());
	return ptr;
}

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(const Vector3& size)
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), size);
	return ptr;
}

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(float width, float height, float depth)
{
	return Create(Vector3(width, height, depth));
}

//------------------------------------------------------------------------------
BoxMesh::BoxMesh()
{
}

//------------------------------------------------------------------------------
BoxMesh::~BoxMesh()
{
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(SceneGraph* ownerSceneGraph)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource());
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());
	StaticMesh::Initialize(ownerSceneGraph, mesh);
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(SceneGraph* ownerSceneGraph, const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size);
	StaticMesh::Initialize(ownerSceneGraph, mesh);
}

LN_NAMESPACE_END
