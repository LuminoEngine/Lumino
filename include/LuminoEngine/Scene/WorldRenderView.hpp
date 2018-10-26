
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderView.hpp"

namespace ln {
namespace detail {
class SceneRenderingPipeline;
}

class WorldRenderView
	: public RenderView
{
protected:

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void initialize();

private:
    Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
};

} // namespace ln
