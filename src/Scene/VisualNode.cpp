
#include "../Internal.h"
//#include "MME/MMEShaderTechnique.h"
//#include "MME/MMEShader.h"
#include "RenderingPass.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/VisualNode.h>
#include <Lumino/Scene/SceneGraph.h>
#include "../EngineDiagCore.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
namespace detail
{

//==============================================================================
// MaterialInstance
//==============================================================================

//------------------------------------------------------------------------------
MaterialInstance::MaterialInstance(int materialTypeId)
	: m_materialTypeId(materialTypeId)
{

}

//------------------------------------------------------------------------------
MaterialInstance::~MaterialInstance()
{

}

//------------------------------------------------------------------------------
void MaterialInstance::Combine(Material* owner, Material* parent)
{
	//bool modified = false;
	//if (m_owner == nullptr || owner != m_owner || m_owner->m_modifiedForMaterialInstance)
	//{
	//	modified = true;
	//}
	//if (parent != nullptr && parent->m_modifiedForMaterialInstance)
	//{
	//	modified = true;
	//}

	//if (modified)
	//{
	//	// set
	//	m_owner = owner;
	//	OnCombine(owner, parent);
	//	m_owner->m_modifiedForMaterialInstance = false;
	//}
}

//------------------------------------------------------------------------------
void MaterialInstance::OnCombine(Material* owner, Material* parent)
{
	m_colorScale = m_owner->GetColorScale();
	m_colorScale.a *= m_owner->GetOpacity();
	m_blendColor = m_owner->GetBlendColor();
	m_tone = m_owner->GetTone();
	m_shader = m_owner->GetShader();

	// combine
	if (parent != nullptr)
	{
		m_colorScale.MultiplyClamp(parent->GetColorScale());
		m_colorScale.a *= parent->GetOpacity();
		m_blendColor.AddClamp(parent->GetBlendColor());
		m_tone.AddClamp(parent->GetTone());
		if (m_shader == nullptr) {
			m_shader = parent->GetShader();
		}
	}
}

//------------------------------------------------------------------------------
const Matrix& MaterialInstance::GetUVTransform() const
{
	return m_owner->GetUVTransform();
}

} // namespace detail

//==============================================================================
// MaterialList2
//==============================================================================

//------------------------------------------------------------------------------
MaterialList2::MaterialList2()
{
}

//------------------------------------------------------------------------------
MaterialList2::~MaterialList2()
{
}

//------------------------------------------------------------------------------
void MaterialList2::Initialize(int subMaterialCount, bool createMainMaterial)
{
	Resize(subMaterialCount);
	for (int i = 0; i < subMaterialCount; ++i)
	{
		auto m = RefPtr<Material>::MakeRef();	// TODO
		SetAt(i, m);
	}

	if (createMainMaterial)
	{
		m_mainMaterial = RefPtr<Material>::MakeRef();	// TODO
	}
}

//------------------------------------------------------------------------------
void MaterialList2::CopyShared(MaterialList* srcList, bool createMainMaterial)
{
	LN_CHECK_ARG(srcList != nullptr);

	Resize(srcList->GetCount());
	for (int i = 0; i < srcList->GetCount(); ++i)
	{
		SetAt(i, srcList->GetAt(i));
	}


	if (createMainMaterial)
	{
		m_mainMaterial = RefPtr<Material>::MakeRef();
	}
	else if (GetCount() > 0)
	{
		m_mainMaterial = GetAt(0);
	}
}

//------------------------------------------------------------------------------
Material* MaterialList2::GetMainMaterial() const
{
	return m_mainMaterial;
}

//------------------------------------------------------------------------------
void MaterialList2::UpdateMaterialInstances(SceneGraph* sceneGraph)
{
	// m_mainMaterial は親として使える？
	Material* parent = nullptr;
	if (m_mainMaterial != nullptr)
	{
		parent = m_mainMaterial;
	}

	// m_instanceList のサイズをそろえる
	int subCount = GetCount();
	if (m_instanceList.GetCount() != subCount)
	{
		int oldCount = m_instanceList.GetCount();
		int d = subCount - oldCount;
		m_instanceList.Resize(subCount);
		if (d > 0)
		{
			for (int i = 0; i < d; ++i)
			{
				m_instanceList[oldCount + i] = RefPtr<detail::MaterialInstance>(sceneGraph->CreateMaterialInstance(), false);
			}
		}
	}

	// m_instanceList の内容を作っていく
	if (subCount > 0)
	{
		for (int i = 0; i < subCount; ++i)
		{
			m_instanceList[i]->Combine(GetAt(i), parent);
		}
	}
	else if (parent != nullptr)
	{
		// parent はあるけど SubMaterial が1つも無い。parent を使う。
		if (m_instanceList.GetCount() != 1) m_instanceList.Resize(1);
		m_instanceList[0]->Combine(parent, nullptr);
	}
	else
	{
		// parent も SubMaterial も無い。デフォルトのを使う。
		if (m_instanceList.GetCount() != 1) m_instanceList.Resize(1);
		LN_NOTIMPLEMENTED();
		//m_instanceList[0].Combine(parent, nullptr);
	}

	if (parent != nullptr)
	{
		//parent->m_modifiedForMaterialInstance = false;
	}
}


//==============================================================================
// VisualNode
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualNode, SceneNode);

//------------------------------------------------------------------------------
VisualNode::VisualNode()
	: m_isVisible(true)
{
}

//------------------------------------------------------------------------------
VisualNode::~VisualNode()
{
}

//------------------------------------------------------------------------------
void VisualNode::Initialize(SceneGraph* owner, int subsetCount)
{
	SceneNode::Initialize(owner);
	m_materialList = RefPtr<MaterialList2>::MakeRef();
	m_materialList->Initialize(subsetCount, true);	// TODO: 今はとりあえず必ず mainMaterial 有り (メモリ効率悪い)
	m_subsetCount = subsetCount;
}

//------------------------------------------------------------------------------
Material* VisualNode::GetMaterial() const
{
	return m_materialList->GetMainMaterial();
}

//------------------------------------------------------------------------------
tr::ReflectionObjectList<Material*>* VisualNode::GetMaterials() const
{
	return m_materialList;
}

//------------------------------------------------------------------------------
void VisualNode::SetOpacity(float opacity, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetOpacity(opacity);
}
void VisualNode::SetColorScale(const Color& color, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetColorScale(color.To32BitColor());
}
void VisualNode::SetColorScale(float r, float g, float b, float a, int subsetIndex)
{
	SetColorScale(Color(r, g, b, a));
}
void VisualNode::SetColor(const Color32& color)
{
	SetColorScale(Color(color));
}
void VisualNode::SetColor(int r, int g, int b, int a)
{
	SetColor(Color32(r, g, b, a));
}
void VisualNode::SetBlendColor(const Color& color, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetBlendColor(color.To32BitColor());
}
void VisualNode::SetTone(const ToneF& tone, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetTone(tone);
}
void VisualNode::SetShader(Shader* shader, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	m_materialList->GetMainMaterial()->SetShader(shader);
}

//------------------------------------------------------------------------------
void VisualNode::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// TODO: 描画関係のデータは UpdateFrame でやるべきではないような気もする。
	m_materialList->UpdateMaterialInstances(m_ownerSceneGraph);

	SceneNode::UpdateFrameHierarchy(parent, deltaTime);
}

//------------------------------------------------------------------------------
void VisualNode::UpdateViewFlustumHierarchy(Camera* camera, SceneNodeArray* renderingNodeList, LightNodeList* renderingLightList)
{
	if (IsVisible())
	{
		// 境界球の調整 (ローカル座標系 → ワールド座標系)
		detail::Sphere boundingSphere = GetBoundingSphere();
		boundingSphere.center.x += m_combinedGlobalMatrix.m41;
		boundingSphere.center.y += m_combinedGlobalMatrix.m42;
		boundingSphere.center.z += m_combinedGlobalMatrix.m43;

		// 視錘台カリング
		if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
			camera->GetViewFrustum().Intersects(boundingSphere.center, boundingSphere.radius))
		{
			// このノードは描画できる
			renderingNodeList->Add(this);

			// Zソート用の距離を計算
			switch (camera->GetZSortDistanceBase())
			{
			case ZSortDistanceBase::NodeZ:
				m_zDistance = GetCombinedGlobalMatrix().GetPosition().z;
				break;
			case ZSortDistanceBase::CameraDistance:
				m_zDistance = (GetCombinedGlobalMatrix().GetPosition() - camera->GetCombinedGlobalMatrix().GetPosition()).GetLengthSquared();
				break;
			case ZSortDistanceBase::CameraScreenDistance:
				m_zDistance = Vector3::Dot(
					GetCombinedGlobalMatrix().GetPosition() - camera->GetCombinedGlobalMatrix().GetPosition(),
					camera->GetCombinedGlobalMatrix().GetFront());		// 平面と点の距離
				break;
			}
		}

		// 子ノードの処理
		SceneNode::UpdateViewFlustumHierarchy(camera, renderingNodeList, renderingLightList);
	}
}

//------------------------------------------------------------------------------
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
	for (Light* light : *renderingLightList)
	{
		light->m_tmpDistance = (light->m_combinedGlobalMatrix.GetPosition() - m_combinedGlobalMatrix.GetPosition()).GetLengthSquared();
	}

	// ソート
	std::stable_sort(renderingLightList->begin(), renderingLightList->end(), CmpLightSort);

	// 出力 (足りない分は nullptr で埋める)
	int req = std::min(m_affectLightList.GetCount(), renderingLightList->GetCount());
	int i = 0;
	for (; i < req; ++i)
	{
		m_affectLightList[i] = renderingLightList->GetAt(i);
	}
	for (; i < m_affectLightList.GetCount(); ++i)
	{
		m_affectLightList[i] = nullptr;
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
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
detail::Sphere VisualNode::GetBoundingSphere()
{
	const detail::Sphere s{ Vector3::Zero, -1 };
	return s;
}

//------------------------------------------------------------------------------
//void VisualNode::DrawSubsetInternal(SceneGraphRenderingContext* dc, int subsetIndex, MMEShader* shader, ShaderPass* pass)
//{
//	// シェーダのサブセット単位のデータを更新する
//	if (shader != nullptr) {
//		shader->UpdateSubsetParams(m_materialList->GetMaterialInstance(subsetIndex));
//	}
//
//	// パス開始
//	if (pass != nullptr) {
//		dc->SetShaderPass(pass);
//	}
//
//	// サブセット描画の本体
//	DrawSubset(dc, subsetIndex);
//}

//------------------------------------------------------------------------------
//void VisualNode::OnRender(SceneGraphRenderingContext* dc)
//{
//	// レンダリングステートの設定
//	dc->ResetStates();
//	dc->SetBlendMode(m_renderState.blendMode);
//	dc->SetCullingMode(m_renderState.cullingMode);
//	dc->SetDepthTestEnabled(m_renderState.depthTestEnabled);
//	dc->SetDepthWriteEnabled(m_renderState.depthWriteEnabled);
//
//
//	int subsetCount = GetSubsetCount();
//	for (int i = 0; i < subsetCount; ++i)
//	{
//		dc->Pass->RenderSubset(dc, this, i);
//	}
//}

//------------------------------------------------------------------------------
Shader* VisualNode::GetPrimaryShader() const
{
	// TODO: main が無ければ [0] のをつかう
	return m_materialList->GetMainMaterial()->GetShader();
}

//------------------------------------------------------------------------------
//void VisualNode::Render(SceneGraphRenderingContext* dc)
//{
//	// 描画
//	OnRender(dc);
//
//	// diag
//	if (m_manager->GetEngineDiag() != nullptr) m_manager->GetEngineDiag()->IncreaseVisualNodeDrawCount();
//}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
