
#pragma once
#include "RenderingPass.h"
#if 0

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// 物理演算のデバッグ用描画など
class InfomationRenderingPass
	: public RenderingPass
{
public:
	InfomationRenderingPass(SceneGraphManager* manager);
	virtual ~InfomationRenderingPass();

public:
	virtual void RenderNode(SceneGraphRenderingContext* dc, SceneNode* node) override;
	virtual void RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset) override;
	virtual void PostRender(SceneGraphRenderingContext* dc) override;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
#endif
