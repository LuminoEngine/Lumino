
#include "../../Internal.h"
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/VisualNode.h>
#include "ShaderScriptCommandList.h"
#include "MMEShaderTechnique.h"
#include "MMERenderingPass.h"
#include "MmdMaterial.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// RenderingPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMERenderingPass::MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader)
	: RenderingPass(manager)
	, m_mmdPass(mmdPass)
	, m_ownerShader(nullptr)
{
	LN_REFOBJ_SET(m_ownerShader, ownerShader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMERenderingPass::~MMERenderingPass()
{
	LN_SAFE_RELEASE(m_ownerShader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMERenderingPass::RenderNode(SceneGraphRenderingContext* dc, SceneNode* node)
{
	VisualNode* visualNode = static_cast<VisualNode*>(node);

	// レンダリングステートの設定
	const detail::VisualNodeRenderState& state = visualNode->GetVisualNodeRenderState();
	RenderingContext* r = dc->GetRenderingContext();
	r->ResetStates();
	r->SetBlendMode(state.blendMode);
	r->SetCullingMode(state.cullingMode);
	r->SetDepthTestEnabled(state.depthTestEnabled);
	r->SetDepthWriteEnabled(state.depthWriteEnabled);
	visualNode->OnRender(dc);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMERenderingPass::RenderSubset(SceneGraphRenderingContext* dc, VisualNode* visualNode, int subset)
{
	// 今回のパスで本当に必要な情報 (使用するシェーダ等) を取得する
	RenderingPriorityParams priorityParams;
	SelectPriorityParams(visualNode, subset, &priorityParams);
	if (priorityParams.Hide) {	// このパスでは描画しない
		return;
	}
	dc->Shader = priorityParams.Shader;

	// ノード単位データを更新する
	priorityParams.Shader->UpdateNodeParams(visualNode, dc->CurrentCamera, *visualNode->GetAffectLightList());
	visualNode->UpdateNodeRenderingParams(priorityParams.Shader);

	//const Material& material = visualNode->GetVisualNodeParams().GetCombinedSubsetParams(subset).Material;
	const detail::MaterialInstance* materialInstance = visualNode->GetMaterialList().GetMaterialInstance(subset);
	bool useTexture = materialInstance->m_owner->GetTexture() != nullptr;
	bool useSphereTexture = false;
	bool useToonTexture = false;
	if (materialInstance->m_owner->GetMaterialTypeId() == detail::MmdMaterialTypeId)
	{
		MmdMaterial* mmdMat = static_cast<MmdMaterial*>(materialInstance->m_owner);
		useSphereTexture = mmdMat->GetSphereTexture() != nullptr;
		useToonTexture = mmdMat->GetToonTexture() != nullptr;
	}

		
	Material2* material = materialInstance->m_owner;

	// テクニックの検索
	MMEShaderTechnique* tech = priorityParams.Shader->FindTechnique(
		m_mmdPass,
		useTexture,
		useSphereTexture,
		useToonTexture,
		false,	// TODO
		subset);

	// テクニックが見つからなかった。この条件に当てはまるのは、テクニックのターゲットサブセット範囲が指定されていて、
	// iSubset がいずれにもマッチしなかった場合。この場合はデフォルトのシェーダを探す。
	if (tech == NULL)
	{
		if (dc->Pass->GetDefaultShader() != NULL)
		{
			tech = dc->Pass->GetDefaultShader()->FindTechnique(
				m_mmdPass,
				useTexture,
				useSphereTexture,
				useToonTexture,
				false,	// TODO
				subset);
			if (tech == NULL) {
				// デフォルトのシェーダにも一致するテクニックが見つからなかった。
				// この iSubset は描画しない。というかできない。
				return;
			}
		}
	}

	// コマンド経由で描画実行
	ShaderScriptCommandList::DrawParams dp;
	dp.Params = dc;
	dp.RenderingNode = visualNode;
	dp.SubsetIndex = subset;
	tech->GetDrawingCommandList().Execute(dp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMERenderingPass::SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams)
{
	outParams->Shader = nullptr;

	Internal::RenderingPassClientData* data = &node->m_renderingPassClientDataList[m_internalEntryID];

	// node の m_internalID 番目のフィルタ情報を設定したものが this ではない。
	// (node が新しく作成されたオブジェクトであるか、前の RenderingPass が解放され ID が返却された後、新たに作成された RenderingPass が同じ ID を取得した場合)
	// ノード名をチェックして割り当てるシェーダのインデックスを node に持たせておく。
	// ・・・
	// 具体的には、新しい OffscreenRenderTarget が作られたときや、一度破棄され、
	// あたらしい OffscreenRenderTarget に同じ ID が割り当てられたときこの if の中に入る。
	// つまり、この if の中は、ある Node に対してこの Pass から初めて描画が行われたときに走る初期化処理。
	if (data->OwnerPass != this)
	{
		data->OwnerPass = this;
		data->PriorityShaderIndex = -1;	// 一致なしを考慮して、初期値は非表示グループ

		int count = m_priorityEntryList.GetCount();
		for (int i = 0; i < count; ++i)
		{
			PriorityParamsEntry& e = m_priorityEntryList[i];

			// "*" は全てのノードを対象とするキー。そのまま追加。
			if (e.MatchingNameKey == _T("*"))
			{
				data->PriorityShaderIndex = i;
				break;
			}
			// このオフスクリーンRTを持つシェーダが設定されているノード自身
			else if (e.MatchingNameKey == _T("self") && node->GetSceneNodeType() == SceneNodeType_VisualNode)
			{
				LN_NOTIMPLEMENTED();
				//if (static_cast<VisualNode*>(node)->GetShader(-1) == m_ownerShader)
				//{
				//	data->PriorityShaderIndex = i;
				//	break;
				//}
			}
			// ワイルドカード付きの比較
			else if (StringTraits::Match(e.MatchingNameKey.c_str(), node->GetName().c_str()))
			{
				data->PriorityShaderIndex = i;
				break;
			}
		}
	}

	// この OffscreenScene を生成したシェーダと同一のシェーダを持っているものを適当にチョイスする
	// (OffscreenScene を含むシェーダは基本的に共有をサポートしない。そのため、一意になるはず)
	//if (obj->getShader() && obj->getShader() == mSceneShader) {
	//	mOffscreenOwner = obj;
	//}

	// グループ分けされなかったノードはデフォルト値で返す
	if (data->PriorityShaderIndex = -1)
	{
		if (m_priorityEntryList.IsEmpty() && node->GetSceneNodeType() == SceneNodeType_VisualNode)
		{
			//LN_NOTIMPLEMENTED();
			// 優先パラメータ未設定。 (OFFSCREENRENDERTARGET ではない)
			// ノードの持っているシェーダを返す。
			//outParams->Shader = static_cast<VisualNode*>(node)->GetVisualNodeParams().GetCombinedSubsetParams(subsetIndex).SceneShader;//m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader;
		}

		outParams->Hide = false;
	}
	// 登録されているグループ
	else
	{
		// TODO: subsetIndex を考慮したシェーダの割り当て

		outParams->Shader = m_priorityEntryList[data->PriorityShaderIndex].Params.Shader;
		outParams->Hide = m_priorityEntryList[data->PriorityShaderIndex].Params.Hide;
	}

	// ここまでで何も選択されていなければデフォルトのシェーダを使用する
	if (outParams->Shader == NULL) {
		outParams->Shader = m_defaultShader;
	}

	// ここまでで何も選択されていないということは、デフォルトのシェーダすらなかったということ。
	// 描画を行わないダミーパスと考えることもできるが、そんなの何に使うのか。
	// とりあえず現状では想定外である。
	LN_THROW(outParams->Shader != NULL, InvalidOperationException);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
