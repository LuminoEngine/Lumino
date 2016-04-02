
#pragma once
#include "Common.h"
#include <Lumino/Graphics/Texture.h>
#include "SceneNode.h"
#include "VisualNodeParams.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// VisualNode
class VisualNode
	: public SceneNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/// 可視状態の設定
	void SetVisible(bool visible) { m_isVisible = visible; }

	/// 可視状態の判定
	bool IsVisible() const { return m_isVisible; }

	/// 不透明度の設定 (0.0 ～ 1.0)
	void SetOpacity(float opacity, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).Opacity = opacity; }

	/// 不透明度の取得
	float GetOpacity(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).Opacity; }

	/// 乗算色の設定
	void SetColorScale(const ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale = color; }
	void SetColorScale(float r, float g, float b, float a = 1.0f, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale = ColorF(r, g, b, a); }

	/// 乗算色の取得
	const ColorF& GetColorScale(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale; }

	/// ブレンドカラーの設定
	void SetBlendColor(const ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor = color; }

	/// ブレンドカラーの取得
	const ColorF& GetBlendColor(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor; }

	/// 色調の設定
	void SetTone(const ToneF& tone, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).Tone = tone; }

	/// 色調の取得
	const ToneF& GetTone(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).Tone; }

	/// UV 変換行列の設定
	void SetUVTransform(const Matrix& matrix, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform = matrix; }

	/// UV 変換行列の設定
	const Matrix& GetUVTransform(int subsetIndex = -1)  const { return m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform; }

	/// シェーダの設定
	void SetShader(MMEShader* shader, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader = shader; }

	/// シェーダの取得
	MMEShader* GetShader(int subsetIndex = -1) { return m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader; }

	/// 合成方法の設定
	void SetBlendMode(BlendMode mode) { m_renderState.Blend = mode; }

	/// 合成方法の取得
	BlendMode GetBlendMode() const { return m_renderState.Blend; }

	/// 深度テストの有効設定
	void SetDepthTestEnabled(bool flag) { m_depthStencilState.DepthTestEnabled = flag; }

	/// 深度テストの有効判定
	bool IsDepthTestEnabled() const { return m_depthStencilState.DepthTestEnabled; }

	/// 深度Writeの有効設定
	void SetDepthWriteEnabled(bool flag) { m_depthStencilState.DepthWriteEnabled = flag; }

	/// 深度書き込みの有効判定
	bool IsDepthWriteEnabled() const { return m_depthStencilState.DepthWriteEnabled; }

	/// カリング方法の設定
	void SetCullingMode(CullingMode mode) { m_renderState.Culling = mode; }

	/// カリング方法の取得
	CullingMode GetCullingMode() const { return m_renderState.Culling; }

public:


	const RenderState& GetRenderState() const { return m_renderState; }
	const DepthStencilState& GetDepthStencilState() const { return m_depthStencilState; }
	int GetSubsetCount() const { return m_subsetCount; }
	const Internal::VisualNodeParams& GetVisualNodeParams() const { return m_visualNodeParams; }

	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_VisualNode; }
	virtual void UpdateFrameHierarchy(SceneNode* parent) override;
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList);
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
	void CreateCore(SceneGraphManager* manager, int subsetCount);

protected:
	int						m_subsetCount;

	friend class RenderingPass;
	Internal::VisualNodeParams		m_visualNodeParams;

	// 以下のプロパティはサブセット単位で管理しない。
	// いずれも、いわゆる設定の継承を考慮する必要があるもの。
	// レンダリングステートなんかはサブセット単位で設定できるようにすることも可能だけど、
	// 実際にサブセット単位で設定したいことってあるの？って考えるとノード単位でいいと思う。
	// ちなみに Unity はシェーダで設定するようだ。
	RenderState	m_renderState;	///< レンダリングステート
	DepthStencilState	m_depthStencilState;
	bool					m_isVisible;

	LightNodeList			m_affectLightList;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
