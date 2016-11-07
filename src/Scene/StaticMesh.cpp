
#pragma once
#include "../Internal.h"
#include <Lumino/Scene/StaticMesh.h>
//#include "MME/MMEShader.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMesh
//==============================================================================

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::Create(const StringRef& filePath)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto meshResource = SceneGraphManager::Instance->GetModelManager()->CreateModelCore(filePath);
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager(), meshResource);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateBox(const Vector3& size)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeBox(SceneGraphManager::Instance->GetGraphicsManager(), size);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateSphere(float radius, int slices, int stacks, MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSphere(SceneGraphManager::Instance->GetGraphicsManager(), radius, slices, stacks, flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreatePlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateSquarePlane(const Vector2& size, const Vector3& front, MeshCreationFlags flags)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeSquarePlane(SceneGraphManager::Instance->GetGraphicsManager(), size, front, flags);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateScreenPlane()
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->InitializeScreenPlane(SceneGraphManager::Instance->GetGraphicsManager());
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
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


	owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
//void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
//{
//	dc->DrawMesh(m_mesh, subsetIndex);
//}

LN_NAMESPACE_END
