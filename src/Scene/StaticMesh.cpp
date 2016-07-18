
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
StaticMeshPtr StaticMesh::CreateBox(const Vector3& size)
{
	auto ptr = StaticMeshPtr::MakeRef();
	ptr->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph());
	ptr->m_mesh = RefPtr<StaticMeshModel>::MakeRef();
	ptr->m_mesh->Initialize(SceneGraphManager::Instance->GetGraphicsManager());
	ptr->m_mesh->CreateBox(size);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMesh::StaticMesh()
{
}

//------------------------------------------------------------------------------
StaticMesh::~StaticMesh()
{
}

//------------------------------------------------------------------------------
void StaticMesh::Initialize(SceneGraph* owner)
{
	VisualNode::Initialize(owner, 1/*m_model->GetSubsetCount()*/);


	m_material = RefPtr<Material3>::MakeRef();


	owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	dc->BeginGraphicsContext()->DrawMesh(m_mesh, m_material);
}

LN_NAMESPACE_END
