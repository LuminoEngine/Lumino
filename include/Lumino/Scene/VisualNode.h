
#pragma once
#include "Common.h"
#include "Detail.h"
#include <Lumino/Graphics/Texture.h>
#include "SceneNode.h"
#include "Material.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// VisualNode
class VisualNode
	: public SceneNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/** 可視状態を設定します。false の場合、ノードの描画自体行われません。(default: true) */
	void SetVisible(bool visible) { m_isVisible = visible; }

	/** 可視状態を取得します。*/
	bool IsVisible() const { return m_isVisible; }

	//-------------------------------------------------------------------------
	/** @name Main material utilities. */
	/** @{ */



	/** メインマテリアルの不透明度を設定します。(default: 1.0)*/
	void SetOpacity(float opacity, int subsetIndex = -1);


	/** メインマテリアルのカラースケールを設定します。(default: )*/
	/// 乗算色の設定TODO: Color32
	void SetColorScale(const ColorF& color, int subsetIndex = -1);
	void SetColorScale(float r, float g, float b, float a = 1.0f, int subsetIndex = -1);


	void SetColor(const Color32& color);
	void SetColor(int r, int g, int b, int a = 255);

	/// ブレンドカラーの設定 TODO: Color32
	void SetBlendColor(const ColorF& color, int subsetIndex = -1);

	/// 色調の設定 TODO: Tone32
	void SetTone(const ToneF& tone, int subsetIndex = -1);


	///// UV 変換行列の設定
	//void SetUVTransform(const Matrix& matrix, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform = matrix; }

	///// UV 変換行列の設定
	//const Matrix& GetUVTransform(int subsetIndex = -1)  const { return m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform; }

	/// シェーダの設定
	void SetShader(Shader* shader, int subsetIndex = -1);

	/** @} */

	//-------------------------------------------------------------------------
	/** @name RenderState */
	/** @{ */

	/** このノードを描画する際の合成方法を設定します。デフォルトは BlendMode::Alpha です。*/
	void SetBlendMode(BlendMode mode) { m_renderState.blendMode = mode; }

	/** このノードを描画する際の合成方法を取得します。*/
	BlendMode GetBlendMode() const { return m_renderState.blendMode; }

	/** このノードを描画する際のカリング方法を設定します。デフォルトは CullingMode::Back です。*/
	void SetCullingMode(CullingMode mode) { m_renderState.cullingMode = mode; }

	/** このノードを描画する際のカリング方法を取得します。*/
	CullingMode GetCullingMode() const { return m_renderState.cullingMode; }

	/** このノードを描画する際の深度テストの有無を設定します。デフォルトは true です。*/
	void SetDepthTestEnabled(bool flag) { m_renderState.depthTestEnabled = flag; }

	/** このノードを描画する際の深度テストの有無を取得します。*/
	bool IsDepthTestEnabled() const { return m_renderState.depthTestEnabled; }

	/** このノードを描画する際の深度書き込みの有無を設定します。デフォルトは true です。*/
	void SetDepthWriteEnabled(bool flag) { m_renderState.depthWriteEnabled = flag; }

	/** このノードを描画する際の深度書き込みの有無を取得します。*/
	bool IsDepthWriteEnabled() const { return m_renderState.depthWriteEnabled; }

	/** @} */

public:


	int GetSubsetCount() const { return m_subsetCount; }

	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_VisualNode; }
	virtual void UpdateFrameHierarchy(SceneNode* parent, float deltaTime) override;
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeArray* renderingNodeList, LightNodeList* renderingLightList);
	virtual void UpdateAffectLights(LightNodeList* renderingLightList, int maxCount);

	/// ノード単位の描画情報の更新。この後すぐ一連のサブセット描画が始まる
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) {}


	// IMMESubsetRenerer
//	virtual void OnDrawSubset(int subsetIndex, void* userData, MMEShader* shader, pass) { DrawSubset(*((RenderingParams*)userData), subsetIndex); }

	void DrawSubsetInternal(SceneGraphRenderingContext* dc, int subsetIndex, MMEShader* shader, ShaderPass* pass);

	virtual void OnRender(SceneGraphRenderingContext* dc);
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) {}

	virtual LightNodeList* GetAffectLightList() { return &m_affectLightList; }

	/// ライトソート用の比較関数 (距離と優先度でソート)
	static bool CmpLightSort(const Light* left, const Light* right);

protected:
	VisualNode();
	virtual ~VisualNode();
	void Initialize(SceneGraph* owner, int subsetCount);

LN_INTERNAL_ACCESS:
	MaterialList2& GetMaterialList() { return m_materialList; }
	//const detail::VisualNodeRenderState& GetVisualNodeRenderState() const { return m_renderState; }
	Shader* GetPrimaryShader() const;
	void Render(SceneGraphRenderingContext* dc);

protected:
	int						m_subsetCount;

	friend class RenderingPass;
	MaterialList2					m_materialList;
	detail::VisualNodeRenderState	m_renderState;
	bool					m_isVisible;

	LightNodeList			m_affectLightList;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
