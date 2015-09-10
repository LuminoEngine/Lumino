
#pragma once

#include "../RenderingPass.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

/// MMDPass をキーとしてシェーダテクニックを検索する RenderingPass
class MMERenderingPass
	: public RenderingPass
{
public:
	MMERenderingPass(SceneGraphManager* manager, MMDPass mmdPass, MMEShader* ownerShader = NULL);
	virtual ~MMERenderingPass();

private:
	virtual void RenderNode(RenderingParams& params, SceneNode* node);

private:
	/// このパスのデフォルト&優先シェーダと、指定したノードのシェーダから描画に使用するテクニックを選択する。
	void SelectPriorityParams(SceneNode* node, int subsetIndex, RenderingPriorityParams* outParams);

private:
	MMDPass		m_mmdPass;
	MMEShader*	m_ownerShader;			///< このパス (OFFSCREENRENDERTARGET) の生成元となったシェーダ
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
