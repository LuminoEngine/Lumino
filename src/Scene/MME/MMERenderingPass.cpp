
#include "../../Internal.h"
#include <Lumino/Graphics/Mesh.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/VisualNode.h>
#include <Lumino/Scene/Camera.h>
#include "ShaderScriptCommandList.h"
#include "MMEShaderTechnique.h"
#include "MMERenderingPass.h"
#include "MmdMaterial.h"
#include "MMEShader.h"
#include "MMEShaderErrorInfo.h"
#include "../SceneGraphManager.h"
#include "../DebugRenderer.h"
#include "../../Graphics/PrimitiveRenderer.h"	// TODO: test

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// RenderingPass
//==============================================================================

//------------------------------------------------------------------------------
MMERenderingPass::MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader)
	: RenderingPass(manager)
	, m_mmdPass(mmdPass)
	, m_ownerShader(nullptr)
{
	LN_REFOBJ_SET(m_ownerShader, ownerShader);
	CreateGridContents();
}

//------------------------------------------------------------------------------
MMERenderingPass::~MMERenderingPass()
{
	LN_SAFE_RELEASE(m_ownerShader);
}

//------------------------------------------------------------------------------
void MMERenderingPass::Initialize()
{
	// StaticMesh 用デフォルトシェーダ
	{
		static const byte_t shaderData[] =
		{
#include "../Resource/SSBasic2D.fxc.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);

		MMEShaderErrorInfo result;
		auto shader = RefPtr<MMEShader>::MakeRef();
		shader->Initialize(GetManager(), (const char*)shaderData, shaderDataLen, &result);
		m_defaultShader[detail::SceneNodeDefaultShaderClass_StaticMesh] = shader;
	}

	// SkinnedMesh 用デフォルトシェーダ
	{
		static const byte_t shaderData[] =
		{
#include "../Resource/ForwardRenderingSkinnedMesh3D.fx.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);

		MMEShaderErrorInfo result;
		auto shader = RefPtr<MMEShader>::MakeRef();
		shader->Initialize(GetManager(), (const char*)shaderData, shaderDataLen, &result);
		m_defaultShader[detail::SceneNodeDefaultShaderClass_SkinnedMesh] = shader;
	}
}

//------------------------------------------------------------------------------
void MMERenderingPass::RenderNode(SceneGraphRenderingContext* dc, SceneNode* node)
{
	VisualNode* visualNode = static_cast<VisualNode*>(node);
	visualNode->Render(dc);
}

//------------------------------------------------------------------------------
void MMERenderingPass::RenderSubset(SceneGraphRenderingContext* dc, VisualNode* visualNode, int subset)
{
	detail::SceneNodeDefaultShaderClass shaderClass = visualNode->GetShaderClass();

	// 今回のパスで本当に必要な情報 (使用するシェーダ等) を取得する
	RenderingPriorityParams priorityParams;
	SelectPriorityParams(visualNode, subset, &priorityParams, shaderClass);
	if (priorityParams.Hide) {	// このパスでは描画しない
		return;
	}
	dc->Shader = priorityParams.Shader;

	// ノードに影響するライトを列挙する
	visualNode->UpdateAffectLights(dc->renderingLightList, dc->Shader->GetRequiredLightCount());

	// ノード単位データを更新する
	priorityParams.Shader->UpdateNodeParams(visualNode, dc->GetCurrentCamera(), *visualNode->GetAffectLightList());
	visualNode->UpdateNodeRenderingParams(priorityParams.Shader);

	//const Material& material = visualNode->GetVisualNodeParams().GetCombinedSubsetParams(subset).Material;
	detail::MaterialInstance* materialInstance = visualNode->GetMaterialList()->GetMaterialInstance(subset);
	bool useTexture = false;
	bool useSphereTexture = false;
	bool useToonTexture = false;
	if (materialInstance->GetMaterialTypeId() == detail::MmdMaterialTypeId)
	{
		MmdMaterialInstance* mmdMat = static_cast<MmdMaterialInstance*>(materialInstance);
		useTexture = mmdMat->m_materialTexture != nullptr;
		useSphereTexture = mmdMat->m_sphereTexture != nullptr;
		useToonTexture = mmdMat->m_toonTexture != nullptr;
	}


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
	if (tech == nullptr)
	{
		if (dc->Pass->GetDefaultShader(shaderClass) != nullptr)
		{
			tech = dc->Pass->GetDefaultShader(shaderClass)->FindTechnique(
				m_mmdPass,
				useTexture,
				useSphereTexture,
				useToonTexture,
				false,	// TODO
				subset);
			if (tech == nullptr)
			{
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

//------------------------------------------------------------------------------
void MMERenderingPass::SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams, detail::SceneNodeDefaultShaderClass shaderClass)
{
	outParams->Shader = nullptr;

	detail::RenderingPassClientData* data = &node->m_renderingPassClientDataList[m_internalEntryID];

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
				if (static_cast<VisualNode*>(node)->GetPrimaryShader() == m_ownerShader)
				{
					data->PriorityShaderIndex = i;
					break;
				}
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
			// 優先パラメータ未設定。 (OFFSCREENRENDERTARGET ではない)
			// ノードの持っているシェーダを返す。
			outParams->Shader = dynamic_cast<MMEShader*>(static_cast<VisualNode*>(node)->GetMaterialList()->GetMaterialInstance(subsetIndex)->m_shader);
			// TODO: ↑dynamic_cast はやめたいが・・・
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
	if (outParams->Shader == nullptr) {
		outParams->Shader = m_defaultShader[shaderClass];
	}

	// ここまでで何も選択されていないということは、デフォルトのシェーダすらなかったということ。
	// 描画を行わないダミーパスと考えることもできるが、そんなの何に使うのか。
	// とりあえず現状では想定外である。
	LN_THROW(outParams->Shader != NULL, InvalidOperationException);
}

//------------------------------------------------------------------------------
void MMERenderingPass::PostRender(SceneGraphRenderingContext* dc)
{
	AdjustGridMesh(dc->GetCurrentCamera());

	m_gridPlane->GetMaterial(0)->SetMatrixParameter(_T("WorldViewProjMatrix"), dc->GetCurrentCamera()->GetViewProjectionMatrix());

	dc->ResetStates();
	dc->SetBlendMode(BlendMode::Alpha);
	dc->DrawMesh(
		m_gridPlane,
		m_gridPlane->m_attributes[0].StartIndex,
		m_gridPlane->m_attributes[0].PrimitiveNum,
		m_gridPlane->GetMaterial(0));


#if 0
	if (m_manager->GetPhysicsManager() != nullptr)
	{
		DebugRenderer dr(dc);
		// TODO: SetViewProjection は別の場所でやったほうがステート変更少なくてすむかも？
		//dc->BeginGraphicsContext()->SetViewProjectionTransform(dc->CurrentCamera->GetViewMatrix(), dc->CurrentCamera->GetProjectionMatrix());
		dc->GetPrimitiveRenderer()->SetViewProjMatrix(dc->CurrentCamera->GetViewProjectionMatrix());
		m_manager->GetPhysicsManager()->DrawDebugShapesAllWorld(&dr);
		dc->Flush();
	}
#endif
}

//------------------------------------------------------------------------------
void MMERenderingPass::CreateGridContents()
{
	detail::GraphicsManager* gm = GetManager()->GetGraphicsManager();

	// 適当な四角形メッシュ
	m_gridPlane = RefPtr<MeshResource>::MakeRef();
	m_gridPlane->Initialize(gm);
	m_gridPlane->CreateSquarePlane(Vector2(1, 1), Vector3::UnitY, MeshCreationFlags::DynamicBuffers);

	// シェーダ (DrawingContext3D)
	static const byte_t shaderCode[] =
	{
#include "../Resource/InfinitePlaneGrid.lfx.h"
	};
	static const size_t shaderCodeLen = LN_ARRAY_SIZE_OF(shaderCode);
	auto shader = RefPtr<Shader>::MakeRef();
	shader->Initialize(gm, shaderCode, shaderCodeLen);
	m_gridPlane->GetMaterial(0)->SetShader(shader);

	// 四方の辺に黒線を引いたテクスチャを作り、マテリアルにセットしておく
	SizeI gridTexSize(512, 512);
	auto gridTex = RefPtr<Texture2D>::MakeRef();
	gridTex->Initialize(gm, gridTexSize, TextureFormat::R8G8B8A8, true);
	for (int x = 0; x < gridTexSize.width; ++x)
	{
		gridTex->SetPixel(x, 0, Color(0, 0, 0, 0.5));
		gridTex->SetPixel(x, gridTexSize.width - 1, Color(0, 0, 0, 0.5));
	}
	for (int y = 0; y < gridTexSize.height; ++y)
	{
		gridTex->SetPixel(0, y, Color(0, 0, 0, 0.5));
		gridTex->SetPixel(gridTexSize.height - 1, y, Color(0, 0, 0, 0.5));
	}
	m_gridPlane->GetMaterial(0)->SetTextureParameter(_T("ObjectTexture"), gridTex);
}

//------------------------------------------------------------------------------
void MMERenderingPass::AdjustGridMesh(Camera* camera)
{
	/*
		カメラの視錐台と、グリッドを描画したい平面との衝突点から、四角形メッシュを作る。
		これで視界に映る平面全体をカバーするメッシュができる。
		あとはシェーダで、頂点の位置を利用してグリッドテクスチャをサンプリングする。
	*/

	struct Line
	{
		Vector3	from;
		Vector3	to;
	};

	auto& vf = camera->GetViewFrustum();
	Vector3 points[8];
	vf.GetCornerPoints(points);

	Line lines[12] =
	{
		{ points[0], points[1] },
		{ points[1], points[2] },
		{ points[2], points[3] },
		{ points[3], points[0] },

		{ points[0], points[4] },
		{ points[1], points[5] },
		{ points[2], points[6] },
		{ points[3], points[7] },

		{ points[4], points[5] },
		{ points[5], points[6] },
		{ points[6], points[7] },
		{ points[7], points[4] },
	};

	Plane plane(0, 1, 0, 0);
	Array<Vector3> hits;
	for (Line& li : lines)
	{
		Vector3 pt;
		if (plane.Intersects(li.from, li.to, &pt))
		{
			hits.Add(pt);
		}
	}

	Vector3 minPos, maxPos;
	for (Vector3& p : hits)
	{
		minPos = Vector3::Min(p, minPos);
		maxPos = Vector3::Max(p, maxPos);
	}

	m_gridPlane->SetPosition(0, Vector3(minPos.x, 0, maxPos.z));
	m_gridPlane->SetPosition(1, Vector3(minPos.x, 0, minPos.z));
	m_gridPlane->SetPosition(2, Vector3(maxPos.x, 0, maxPos.z));
	m_gridPlane->SetPosition(3, Vector3(maxPos.x, 0, minPos.z));
	m_gridPlane->SetUV(0, Vector2(-1.0f, 1.0f));
	m_gridPlane->SetUV(1, Vector2(-1.0f, -1.0f));
	m_gridPlane->SetUV(2, Vector2(1.0f, 1.0f));
	m_gridPlane->SetUV(3, Vector2(1.0f, -1.0f));
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
