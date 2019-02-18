
#include "Internal.hpp"
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Shader/ShaderInterfaceFramework.hpp>
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

void RenderFeature::updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo)
{
    updateRenderParametersDefault(tech, cameraInfo, elementInfo, subsetInfo);
}

void RenderFeature::updateRenderParametersDefault(ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo)
{
    detail::ShaderSemanticsManager* semanticsManager = detail::ShaderHelper::semanticsManager(tech->shader());
    semanticsManager->updateCameraVariables(cameraInfo);
    semanticsManager->updateElementVariables(cameraInfo, elementInfo);
    semanticsManager->updateSubsetVariables(subsetInfo);
}

} // namespace ln

