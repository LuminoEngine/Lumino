
#pragma once

#include "RenderingPass.h"

namespace Lumino
{
namespace Scene
{

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

} // namespace Scene
} // namespace Lumino
