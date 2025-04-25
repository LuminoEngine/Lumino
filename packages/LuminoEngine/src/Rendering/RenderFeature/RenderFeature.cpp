#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "../SceneRenderer.hpp"

namespace ln {
namespace detail {


} // namespace detail

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

