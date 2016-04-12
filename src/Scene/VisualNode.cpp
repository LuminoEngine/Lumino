
#pragma once
#include "../Internal.h"
#include "MME/MMEShaderTechnique.h"
#include "MME/MMEShader.h"
#include "RenderingPass.h"
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/VisualNode.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// VisualNode
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualNode, SceneNode);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNode::VisualNode()
	: m_isVisible(true)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNode::~VisualNode()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::CreateCore(SceneGraphManager* manager, int subsetCount)
{
	SceneNode::CreateCore(manager);
	m_visualNodeParams.Create(subsetCount);
	m_subsetCount = subsetCount;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// 親が VisualNode であれば Visual 関係のプロパティを継承して更新する
	// TODO: 描画関係のデータは UpdateFrame でやるべきではないような気もする。
	if (parent != NULL && parent->GetSceneNodeType() == SceneNodeType_VisualNode) {
		m_visualNodeParams.UpdateSubsetRenderParam(&static_cast<VisualNode*>(parent)->m_visualNodeParams);
	}
	else {
		m_visualNodeParams.UpdateSubsetRenderParam(NULL);
	}

	SceneNode::UpdateFrameHierarchy(parent, deltaTime);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList)
{
	if (IsVisible())
	{
		// TODO: 視錘台カリング等

		// TOOD: Zソート用の距離を計算


		// このノードは描画できる
		renderingNodeList->Add(this);

		SceneNode::UpdateViewFlustumHierarchy(camera, renderingNodeList, renderingLightList);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::UpdateAffectLights(LightNodeList* renderingLightList, int maxCount)
{
	/*
		まず全てのライトに、このノードとの距離をセットする。
		その後近い順にソートして、ソート結果の先頭から必要な数だけ取りだしている。
		ライトの数が少なければどんなアルゴリズムにしても大差はないと思うが、
		ノード単位でソートが実行されるので速度的に少し心配。
		先頭数個が確定したときにソートを終了する等、最適化の余地はあるが…。
	*/

	m_affectLightList.Resize(maxCount);

	// ソート基準値の計算
	LN_FOREACH(Light* light, *renderingLightList) {
		light->m_tmpDistance = (light->m_combinedGlobalMatrix.GetPosition() - m_combinedGlobalMatrix.GetPosition()).GetLengthSquared();
	}

	// ソート
	std::stable_sort(renderingLightList->begin(), renderingLightList->end(), CmpLightSort);

	// 出力 (足りない分は NULL で埋める)
	int mn = renderingLightList->GetCount();
	int n = maxCount;
	int i = 0;
	for (; i < mn; ++i) {
		m_affectLightList[i] = renderingLightList->GetAt(i);
	}
	for (; i < n; ++i) {
		m_affectLightList[i] = NULL;
	}
}
bool VisualNode::CmpLightSort(const Light* left, const Light* right)
{
	if (left->m_priority == right->m_priority)
	{
		// 距離は昇順。近いほうを先に描画する。
		return left->m_zDistance < right->m_zDistance;
	}
	// 優先度は降順。高いほうを先に描画する。
	return left->m_priority < right->m_priority;
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::Render(RenderingParams& params)
{
	// レンダリングステートの設定
	params.Renderer->SetRenderState(m_renderState);

	for (int iSubset = 0; iSubset < m_subsetCount; iSubset++)
	{
		// 今回のパスで本当に必要な情報 (使用するシェーダ等) を取得する
		RenderingPriorityParams priorityParams;
		params.Pass->SelectPriorityParams(this, iSubset, &priorityParams);
		if (priorityParams.Hide) {	// このパスでは描画しない
			return;
		}
		params.Shader = priorityParams.Shader;

		// シェーダのノード単位データを更新する
		priorityParams.Shader->UpdateNodeParams(this, params.CurrentCamera, m_affectLightList);

		// テクニックの検索
		//m_visualNodeParams.GetCombinedSubsetParams
		MMEShaderTechnique* tech = priorityParams.Shader->FindTechnique(
			(InternalRenderingPass)params.Pass->GetPassID(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.Texture.IsNull(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.SphereTexture.IsNull(),
			!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.ToonTexture.IsNull(),
			false,	// TODO
			iSubset);

		// テクニックが見つからなかった。この条件に当てはまるのは、テクニックのターゲットサブセット範囲が指定されていて、
		// iSubset がいずれにもマッチしなかった場合。この場合はデフォルトのシェーダを探す。
		if (tech == NULL)
		{
			if (params.Pass->GetDefaultShader() != NULL)
			{
				tech = params.Pass->GetDefaultShader()->FindTechnique(
					(InternalRenderingPass)params.Pass->GetPassID(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.Texture.IsNull(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.SphereTexture.IsNull(),
					!m_visualNodeParams.GetCombinedSubsetParams(iSubset).Material.ToonTexture.IsNull(),
					false,	// TODO
					iSubset);
				if (tech == NULL) {
					// デフォルトのシェーダにも一致するテクニックが見つからなかった。
					// この iSubset は描画しない。というかできない。
					continue;
				}
			}
		}

		// コマンド経由で描画実行
		ShaderScriptCommandList::DrawParams dp;
		dp.Params = &params;
		dp.RenderingNode = this;
		dp.SubsetIndex = iSubset;
		tech->GetDrawingCommandList().Execute(dp);
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::DrawSubsetInternal(SceneGraphRenderingContext* dc, int subsetIndex, MMEShader* shader, ShaderPass* pass)
{
	// シェーダのサブセット単位のデータを更新する
	if (shader != NULL) {
		shader->UpdateSubsetParams(m_visualNodeParams.GetCombinedSubsetParams(subsetIndex));
	}

	// パス開始
	if (pass != NULL) {
		//pass->Apply();
		dc->GetRenderingContext()->SetShaderPass(pass);
	}

	// サブセット描画の本体
	DrawSubset(dc, subsetIndex);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNode::OnRender(SceneGraphRenderingContext* dc)
{
	int subsetCount = GetSubsetCount();
	for (int i = 0; i < subsetCount; ++i)
	{
		dc->Pass->RenderSubset(dc, this, i);
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
