
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>

namespace ln {

//==============================================================================
// RenderFeature

void RenderFeature::onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo)
{
}

bool RenderFeature::drawElementTransformNegate() const
{
    return false;
}

} // namespace ln

