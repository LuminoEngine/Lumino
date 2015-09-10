
#pragma once

#include "RenderingPass.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

/// 物理演算のデバッグ用描画など
class InfomationRenderingPass
	: public RenderingPass
{
public:
	InfomationRenderingPass(SceneGraphManager* manager);
	virtual ~InfomationRenderingPass();

public:
	virtual void PostRender(RenderingParams& params);
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
