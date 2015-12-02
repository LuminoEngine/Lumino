
#pragma once
#include "RenderingPass.h"

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
	virtual void PostRender(SceneGraphRenderingContext* dc);
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
