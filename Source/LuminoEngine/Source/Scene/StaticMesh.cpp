
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
void StaticMesh::OnRender2(DrawList* renderer)
{
	renderer->SetTransform(m_combinedGlobalMatrix);
	renderer->DrawMesh(m_mesh, 0, GetMainMaterial()/*GetMaterials()->GetAt(0)*/);
}

//==============================================================================
// BoxMesh
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BoxMesh, StaticMesh);

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(MeshCreationFlags flags)
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize(flags);
	return ptr;
}

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(const Vector3& size, MeshCreationFlags flags)
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize(size, flags);
	return ptr;
}

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(float width, float height, float depth, MeshCreationFlags flags)
{
	return Create(Vector3(width, height, depth), flags);
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
void BoxMesh::Initialize(MeshCreationFlags flags)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	// TODO: HasFlag(.net)
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource(flags.TestFlag(MeshCreationFlags::ReverseFaces)));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(const Vector3& size, MeshCreationFlags flags)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size, flags);
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(float width, float height, float depth, MeshCreationFlags flags)
{
	Initialize(Vector3(width, height, depth), flags);
}

LN_NAMESPACE_END
