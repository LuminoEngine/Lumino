
#pragma once
#include "../Internal.h"
#include <Lumino/Scene/MeshNode.h>
#include "MME/MMEShader.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// MeshNode
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MeshNode::MeshNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MeshNode::~MeshNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MeshNode::Create(const TCHAR* filePath, SceneGraphManager* manager)
{
	m_model.Attach(LN_NEW Model());
	m_model->Create(manager->GetModelManager(), filePath);
	VisualNode::CreateCore(manager, m_model->GetSubsetCount());

	// マテリアルをコピーする (急ぎ足で作ったから、もっとちゃんと考えた方が良いと思う)
	for (int i = 0; i < m_model->GetSubsetCount(); i++)
	{
		m_visualNodeParams.GetSubsetParams(i).Material = m_model->GetMaterial(i);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MeshNode::UpdateFrame(float elapsedTime)
{
	m_model->GetAnimator()->AdvanceTime((double)elapsedTime);
	m_model->UpdateBoneTransformHierarchy();
	m_model->UpdateSkinningMatrices();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MeshNode::UpdateNodeRenderingParams(MMEShader* priorityShader)
{
	Shader* core = priorityShader->GetCoreShader();
	ShaderVariable* v;

	v = core->FindVariable(_T("lnBoneTextureReciprocalSize"));
	if (v) {
		Vector4 invSize;
		invSize.X = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Width;
		invSize.Y = 1.0f / m_model->GetSkinningMatricesTexture()->GetRealSize().Height;
		v->SetVector(invSize);
	}

	v = core->FindVariable(_T("lnBoneTexture"));
	if (v) {
		v->SetTexture(m_model->GetSkinningMatricesTexture());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MeshNode::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	m_model->DrawSubset(subsetIndex);
}

LN_NAMESPACE_END
