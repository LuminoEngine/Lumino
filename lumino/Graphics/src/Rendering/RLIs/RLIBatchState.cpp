
#include "Internal.hpp"
#include <LuminoGraphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include "RLIBatchState.hpp"

namespace ln {
namespace detail {

RLIBatchState::RLIBatchState()
{
	reset();
}

void RLIBatchState::reset()
{
	material = nullptr;
	blendMode = BlendMode::Normal;
	cullingMode = CullMode::Front;
	depthTestEnabled = true;
	depthWriteEnabled = true;
	shadingModel = ShadingModel::Default;
	primitiveTopology = PrimitiveTopology::TriangleList;
}

bool RLIBatchState::equals(const RLIBatchState* other) const
{
	return
		material == other->material &&  // TODO: Material が一致することはまずない。ちゃんと中身かhashを見ること。
		blendMode == other->blendMode &&
		cullingMode == other->cullingMode &&
		depthTestEnabled == other->depthTestEnabled &&
		depthWriteEnabled == other->depthWriteEnabled &&
		shadingModel == other->shadingModel &&
		primitiveTopology == other->primitiveTopology;
}

void RLIBatchState::mergeFrom(const GeometryStageParameters* geometoryParams, Material* finalMaterial)
{
	assert(geometoryParams);

	material = finalMaterial;

	// BlendState
	{
		if (finalMaterial && finalMaterial->getBlendMode().hasValue())
			blendMode = finalMaterial->getBlendMode().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_blendMode.hasValue())
			blendMode = geometoryParams->m_blendMode.value();
		// default
		else
			blendMode = BlendMode::Normal;
	}
	// RasterizerState
	{
		if (finalMaterial && finalMaterial->getCullingMode().hasValue())
			cullingMode = finalMaterial->getCullingMode().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_cullingMode.hasValue())
			cullingMode = geometoryParams->m_cullingMode.value();
		// default
		else
			cullingMode = CullMode::Back;
	}
	// DepthStencilState
	{
		if (finalMaterial && finalMaterial->isDepthTestEnabled().hasValue())
			depthTestEnabled = (finalMaterial->isDepthTestEnabled().value());// ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_depthTestEnabled.hasValue())
			depthTestEnabled = (geometoryParams->m_depthTestEnabled.value());// ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
		// default
		else
			depthTestEnabled = true;// ComparisonFunc::LessEqual;


	}
	{
		if (finalMaterial && finalMaterial->isDepthWriteEnabled().hasValue())
			depthWriteEnabled = finalMaterial->isDepthWriteEnabled().value();
		// specified context->setXXXX() or meshObj->setXXXX()
		else if (geometoryParams->m_depthWriteEnabled.hasValue())
			depthWriteEnabled = geometoryParams->m_depthWriteEnabled.value();
		// default
		else
			depthWriteEnabled = true;
	}
	{
		// specified meshObj->setXXXX()
		if (geometoryParams && geometoryParams->shadingModel.hasValue())
			shadingModel = geometoryParams->shadingModel.value();
		// specified meshObj->getMaterial(0)->setXXXX() etc...
		else if (finalMaterial)
			shadingModel = finalMaterial->shadingModel();
		else
			shadingModel = ShadingModel::Default;
	}
	// Topology
	{
		primitiveTopology = geometoryParams->primitiveTopology;
	}
}

} // namespace detail
} // namespace ln

