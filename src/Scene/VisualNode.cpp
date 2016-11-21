
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


//==============================================================================
// VisualNode
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualNode, SceneNode);

//------------------------------------------------------------------------------
VisualNode::VisualNode()
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
Material* VisualNode::GetMainMaterial() const
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
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetOpacity(opacity);
}
void VisualNode::SetColorScale(const Color& color, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetColorScale(color);
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
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetBlendColor(color);
}
void VisualNode::SetTone(const ToneF& tone, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetTone(tone);
}
void VisualNode::SetShader(Shader* shader, int subsetIndex)
{
	LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr);
	// TODO: サブマテリアルの設定
	m_materialList->GetMainMaterial()->SetShader(shader);
}

//------------------------------------------------------------------------------
void VisualNode::SetBlendMode(BlendMode mode) { GetMainMaterial()->SetBlendMode(mode); }

//------------------------------------------------------------------------------
//BlendMode VisualNode::GetBlendMode() const { return GetMainMaterial()->GetBlendMode(); }

//------------------------------------------------------------------------------
void VisualNode::SetCullingMode(CullingMode mode) { GetMainMaterial()->SetCullingMode(mode); }

//------------------------------------------------------------------------------
//CullingMode VisualNode::GetCullingMode() const { return GetMainMaterial()->GetCullingMode(); }

//------------------------------------------------------------------------------
void VisualNode::SetDepthTestEnabled(bool enabled) { GetMainMaterial()->SetDepthTestEnabled(enabled); }

//------------------------------------------------------------------------------
//bool VisualNode::IsDepthTestEnabled() const { return GetMainMaterial()->IsDepthTestEnabled(); }

//------------------------------------------------------------------------------
void VisualNode::SetDepthWriteEnabled(bool enabled) { GetMainMaterial()->SetDepthWriteEnabled(enabled); }

//------------------------------------------------------------------------------
//bool VisualNode::IsDepthWriteEnabled() const { return GetMainMaterial()->IsDepthWriteEnabled(); }

//------------------------------------------------------------------------------
void VisualNode::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// TODO: 描画関係のデータは UpdateFrame でやるべきではないような気もする。
	//m_materialList->UpdateMaterialInstances(m_ownerSceneGraph);

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
