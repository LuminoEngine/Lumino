
#include "Internal.hpp"
#include <LuminoEngine/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/Shader.hpp>
#include <LuminoEngine/GPU/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../RenderStage.hpp"
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

void RenderFeature::updateRenderParameters(
	detail::ShaderSecondaryDescriptor* descriptor,
	detail::RenderDrawElement* element,
	ShaderTechnique* tech,
	const detail::RenderViewInfo& renderViewInfo,
	const detail::SceneInfo& sceneInfo,
	const detail::ElementInfo& elementInfo,
	const detail::SubsetInfo& subsetInfo)
{
    updateRenderParametersDefault(descriptor, tech, renderViewInfo, sceneInfo, elementInfo, subsetInfo);
}

void RenderFeature::updateRenderParametersDefault(
	detail::ShaderSecondaryDescriptor* descriptor,
	ShaderTechnique* tech,
	const detail::RenderViewInfo& renderViewInfo,
	const detail::SceneInfo& sceneInfo,
	const detail::ElementInfo& elementInfo,
	const detail::SubsetInfo& subsetInfo)
{
	detail::ShaderTechniqueSemanticsManager* semanticsManager = tech->semanticsManager2();
	semanticsManager->updateRenderViewVariables(descriptor, renderViewInfo, sceneInfo);	// TODO: ここだと element ごとに呼ばれるのでかなり無駄が多い。事前計算しておいて、memcpy で済ませたい
	semanticsManager->updateElementVariables(descriptor, renderViewInfo.cameraInfo, elementInfo);
	semanticsManager->updateSubsetVariables(descriptor, subsetInfo);
}

} // namespace ln

