
#include "Internal.hpp"
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include "../Rendering/RenderingPipeline.hpp"

namespace ln {

//==============================================================================
// WorldRenderView

WorldRenderView::WorldRenderView()
{
}

WorldRenderView::~WorldRenderView()
{
}

void WorldRenderView::initialize()
{
    RenderView::initialize();
    m_sceneRenderingPipeline = makeRef<detail::RenderingPipeline>();
}

} // namespace ln

