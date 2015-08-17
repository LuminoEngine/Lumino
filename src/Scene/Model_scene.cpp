
#pragma once

#include "../Internal.h"
#include "SceneGraphManager.h"
#include "Model.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// Model
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::Model()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Model::~Model()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::Create(const TCHAR* filePath, SceneGraphManager* manager)
{
	m_model.Attach(LN_NEW Modeling::Model());
	m_model->Create(manager->GetModelManager(), filePath);
	VisualNode::Create(manager, m_model->GetSubsetCount());

	// マテリアルをコピーする (急ぎ足で作ったから、もっとちゃんと考えた方が良いと思う)
	for (int i = 0; i < m_model->GetSubsetCount(); i++)
	{
		m_visualNodeParams.GetSubsetParams(i).Material = m_model->GetMaterial(i);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateFrame(float elapsedTime)
{
	m_model->GetAnimator()->AdvanceTime((double)elapsedTime);
	m_model->UpdateBoneTransformHierarchy();
	m_model->UpdateSkinningMatrices();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Model::UpdateNodeRenderingParams(MMEShader* priorityShader)
{
	Graphics::Shader* core = priorityShader->GetCoreShader();
	Graphics::ShaderVariable* v;

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
void Model::DrawSubset(RenderingParams& params, int subsetIndex)
{

	m_model->DrawSubset(subsetIndex);
}

} // namespace Scene
} // namespace Lumino
