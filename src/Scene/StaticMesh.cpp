
#pragma once
#include "../Internal.h"
#include <Lumino/Scene/StaticMesh.h>
#include "MME/MMEShader.h"
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
	auto mesh = SceneGraphManager::Instance->GetModelManager()->CreateModelCore(filePath);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateBox(const Vector3& size)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->CreateBox(size);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateSphere(float radius, int slices, int stacks)
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->CreateSphere(radius, slices, stacks);
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshPtr StaticMesh::CreateScreenPlane()
{
	auto ptr = StaticMeshPtr::MakeRef();
	auto mesh = RefPtr<StaticMeshModel>::MakeRef();
	mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	mesh->CreateScreenPlane();
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
void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	dc->BeginGraphicsContext()->DrawMesh(m_mesh, m_mesh->m_attributes[subsetIndex].StartIndex, m_mesh->m_attributes[subsetIndex].PrimitiveNum);
}

LN_NAMESPACE_END
