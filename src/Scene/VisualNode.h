
#pragma once

#include <Lumino/Graphics/Texture.h>
#include "MME/MMEShaderTechnique.h"
#include "SceneNode.h"
#include "VisualNodeParams.h"

namespace Lumino
{
namespace Scene
{

/// VisualNode
class VisualNode
	: public SceneNode
{
public:
	VisualNode();
	virtual ~VisualNode();

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
	void SetColorScale(const Graphics::ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale = color; }

	/// 乗算色の取得
	const Graphics::ColorF& GetColorScale(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).ColorScale; }

	/// ブレンドカラーの設定
	void SetBlendColor(const Graphics::ColorF& color, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor = color; }

	/// ブレンドカラーの取得
	const Graphics::ColorF& GetBlendColor(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).BlendColor; }

	/// 色調の設定
	void SetTone(const Graphics::Tone& tone, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).Tone = tone; }

	/// 色調の取得
	const Graphics::Tone& GetTone(int subsetIndex = -1) const { return m_visualNodeParams.GetSubsetParams(subsetIndex).Tone; }

	/// UV 変換行列の設定
	void SetUVTransform(const Matrix& matrix, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform = matrix; }

	/// UV 変換行列の設定
	const Matrix& GetUVTransform(int subsetIndex = -1)  const { return m_visualNodeParams.GetSubsetParams(subsetIndex).UVTransform; }

	/// シェーダの設定
	void SetShader(MMEShader* shader, int subsetIndex = -1) { m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader = shader; }

	/// シェーダの取得
	MMEShader* GetShader(int subsetIndex = -1) { return m_visualNodeParams.GetSubsetParams(subsetIndex).SceneShader; }

	/// 合成方法の設定
	void SetBlendMode(Graphics::BlendMode mode) { m_renderState.Blend = mode; }

	/// 合成方法の取得
	Graphics::BlendMode GetBlendMode() const { return m_renderState.Blend; }

	/// 深度テストの有効設定
	void SetEnableDepthTest(bool flag) { m_renderState.DepthTest = flag; }

	/// 深度テストの有効判定
	bool IsEnableDepthTest() const { return m_renderState.DepthTest; }

	/// 深度Writeの有効設定
	void SetEnableDepthWrite(bool flag) { m_renderState.DepthWrite = flag; }

	/// 深度書き込みの有効判定
	bool IsEnableDepthWritet() const { return m_renderState.DepthWrite; }

	/// カリング方法の設定
	void SetCullingMode(Graphics::CullingMode mode) { m_renderState.Culling = mode; }

	/// カリング方法の取得
	Graphics::CullingMode GetCullingMode() const { return m_renderState.Culling; }

public:

	/// 作成
	void Create(SceneGraphManager* manager, int subsetCount);

	const Graphics::RenderState& GetRenderState() const { return m_renderState; }
	int GetSubsetCount() const { return m_subsetCount; }
	const VisualNodeParams& GetVisualNodeParams() const { return m_visualNodeParams; }

	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_VisualNode; }
	virtual void UpdateFrameHierarchy(SceneNode* parent, SceneNodeList* renderingNodeList);
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList);
	virtual void UpdateAffectLights(LightNodeList* renderingLightList, int maxCount);

	/// ノード単位の描画情報の更新。この後すぐ一連のサブセット描画が始まる
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader) {}


	// IMMESubsetRenerer
//	virtual void OnDrawSubset(int subsetIndex, void* userData, MMEShader* shader, pass) { DrawSubset(*((RenderingParams*)userData), subsetIndex); }

	void DrawSubsetInternal(RenderingParams& params, int subsetIndex, MMEShader* shader, Graphics::ShaderPass* pass);

	virtual void DrawSubset(RenderingParams& params, int subsetIndex) = 0;

	virtual LightNodeList* GetAffectLightList() { return &m_affectLightList; }

	/// ライトソート用の比較関数 (距離と優先度でソート)
	static bool CmpLightSort(const Light* left, const Light* right);

protected:
	int						m_subsetCount;

	friend class RenderingPass;
	VisualNodeParams		m_visualNodeParams;

	// 以下のプロパティはサブセット単位で管理しない。
	// いずれも、いわゆる設定の継承を考慮する必要があるもの。
	// レンダリングステートなんかはサブセット単位で設定できるようにすることも可能だけど、
	// 実際にサブセット単位で設定したいことってあるの？って考えるとノード単位でいいと思う。
	Graphics::RenderState	m_renderState;	///< レンダリングステート
	bool					m_isVisible;

	LightNodeList			m_affectLightList;
};

} // namespace Scene
} // namespace Lumino
