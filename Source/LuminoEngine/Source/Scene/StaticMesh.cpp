
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
BoxMeshPtr BoxMesh::Create()
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
BoxMeshPtr BoxMesh::Create(const Vector3& size)
{
	auto ptr = RefPtr<BoxMesh>::MakeRef();
	ptr->Initialize(size);
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
void BoxMesh::Initialize()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	// TODO: HasFlag(.net)
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size, MeshCreationFlags::None);
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void BoxMesh::Initialize(float width, float height, float depth)
{
	Initialize(Vector3(width, height, depth));
}

//==============================================================================
// SphereMesh
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SphereMesh, StaticMesh);

//------------------------------------------------------------------------------
SphereMeshPtr SphereMesh::Create()
{
	auto ptr = RefPtr<SphereMesh>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
SphereMeshPtr SphereMesh::Create(float radius, int tessellation)
{
	auto ptr = RefPtr<SphereMesh>::MakeRef();
	ptr->Initialize(radius, tessellation);
	return ptr;
}

//------------------------------------------------------------------------------
SphereMesh::SphereMesh()
{
}

//------------------------------------------------------------------------------
SphereMesh::~SphereMesh()
{
}

//------------------------------------------------------------------------------
void SphereMesh::Initialize()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	// TODO: HasFlag(.net)
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void SphereMesh::Initialize(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}


//==============================================================================
// TeapotMesh
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TeapotMesh, StaticMesh);

//------------------------------------------------------------------------------
TeapotMeshPtr TeapotMesh::Create()
{
	auto ptr = RefPtr<TeapotMesh>::MakeRef();
	ptr->Initialize();
	return ptr;
}

//------------------------------------------------------------------------------
TeapotMeshPtr TeapotMesh::Create(float size, int tessellation)
{
	auto ptr = RefPtr<TeapotMesh>::MakeRef();
	ptr->Initialize(size, tessellation);
	return ptr;
}

//------------------------------------------------------------------------------
TeapotMesh::TeapotMesh()
{
}

//------------------------------------------------------------------------------
TeapotMesh::~TeapotMesh()
{
}

//------------------------------------------------------------------------------
void TeapotMesh::Initialize()
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->SetMeshResource(detail::ModelManager::GetInstance()->GetUnitTeapotMeshResource());
	mesh->AddMaterial(detail::ModelManager::GetInstance()->GetDefaultMaterial());
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

//------------------------------------------------------------------------------
void TeapotMesh::Initialize(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeTeapot(SceneGraphManager::Instance->GetGraphicsManager(), size, tessellation, MeshCreationFlags::None);
	StaticMesh::Initialize(SceneGraphManager::Instance->GetDefaultSceneGraph3D(), mesh);
}

LN_NAMESPACE_END
