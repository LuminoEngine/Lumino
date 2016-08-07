
#pragma once
#include "../RenderingPass.h"

LN_NAMESPACE_BEGIN
class MeshResource;

LN_NAMESPACE_SCENE_BEGIN

/// MMDPass をキーとしてシェーダテクニックを検索する RenderingPass
class MMERenderingPass
	: public RenderingPass
{
public:
	MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader = NULL);
	virtual ~MMERenderingPass();

private:
	virtual void RenderNode(SceneGraphRenderingContext* dc, SceneNode* node) override;
	virtual void RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset) override;
	virtual void PostRender(SceneGraphRenderingContext* dc) override;

private:
	/// このパスのデフォルト&優先シェーダと、指定したノードのシェーダから描画に使用するテクニックを選択する。
	void SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams);
	void CreateGridContents();
	void AdjustGridMesh(Camera* camera);

private:
	MMDPass		m_mmdPass;
	MMEShader*	m_ownerShader;			///< このパス (OFFSCREENRENDERTARGET) の生成元となったシェーダ

	RefPtr<MeshResource>	m_gridPlane;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
