
#pragma once
#include "../Graphics/RenderState.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

namespace detail
{

struct VisualNodeRenderState
{
	BlendMode	blendMode = BlendMode::Normal;
	CullingMode	cullingMode = CullingMode::Back;
	bool		depthTestEnabled = true;
	bool		depthWriteEnabled = true;
};

} // namespace detail 

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

