
#pragma once
#include "../Internal.h"
#include <Lumino/Scene/StaticMesh.h>
#include "MME/MMEShader.h"
#include <Lumino/Scene/SceneGraph.h>
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

	owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	//m_model->DrawSubset(subsetIndex);
}

LN_NAMESPACE_END
