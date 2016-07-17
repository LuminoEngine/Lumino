
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
#if 0
//------------------------------------------------------------------------------
RefPtr<StaticMesh> StaticMesh::Create(const StringRef& filePath)
{
	RefPtr<StaticMesh> obj(LN_NEW StaticMesh(), false);
	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph(), filePath);	// tODO: 3Dだけ？
	return obj;
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
void StaticMesh::Initialize(SceneGraph* owner, const StringRef& filePath)
{
	m_model.Attach(LN_NEW Model(), false);
	m_model->Create(owner->GetManager()->GetModelManager(), filePath);
	VisualNode::Initialize(owner, m_model->GetSubsetCount());

	// マテリアルをコピーする (急ぎ足で作ったから、もっとちゃんと考えた方が良いと思う)
	//for (int i = 0; i < m_model->GetSubsetCount(); i++)
	//{
	//	m_visualNodeParams.GetSubsetParams(i).Material = m_model->GetMaterial(i);
	//}
	LN_NOTIMPLEMENTED();

	owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMesh::OnUpdateFrame(float elapsedTime)
{
	//m_model->GetAnimator()->AdvanceTime((double)elapsedTime);
	//m_model->UpdateBoneTransformHierarchy();
	//m_model->UpdateSkinningMatrices();
}

//------------------------------------------------------------------------------
void StaticMesh::UpdateNodeRenderingParams(MMEShader* priorityShader)
{
	//Shader* core = priorityShader->GetCoreShader();
	//ShaderVariable* v;

	//v = core->FindVariable(_T("lnBoneTextureReciprocalSize"));
	//if (v) {
	//	Vector4 invSize;
	//	invSize.X = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Width;
	//	invSize.Y = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Height;
	//	v->SetVector(invSize);
	//}

	//v = core->FindVariable(_T("lnBoneTexture"));
	//if (v) {
	//	v->SetTexture(m_model->GetSkinningMatricesTexture());
	//}
}

//------------------------------------------------------------------------------
void StaticMesh::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	m_model->DrawSubset(subsetIndex);
}
#endif
LN_NAMESPACE_END
