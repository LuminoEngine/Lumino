
#include "Internal.hpp"
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
#include "RenderElement.hpp"

namespace ln {
namespace detail {

const BuiltinEffectData BuiltinEffectData::DefaultValue;

//==============================================================================
// IDrawElementListFrameData

IDrawElementListFrameData::IDrawElementListFrameData()
	: m_nextFrameData(nullptr)
{
}

IDrawElementListFrameData::~IDrawElementListFrameData()
{
}

//==============================================================================
// RenderStage

RenderStage::RenderStage()
	: frameBufferStageParameters(nullptr)
	, geometryStageParameters(nullptr)
	, renderFeature(nullptr)
{
}

bool RenderStage::equals(const RenderStage* other) const
{
	return
		frameBufferStageParameters->equals(other->frameBufferStageParameters) &&
		geometryStageParameters->equals(other->geometryStageParameters) &&
		renderFeature == other->renderFeature && 
		m_objectId == other->m_objectId;
}

void RenderStage::flush(GraphicsCommandList* context) {
	if (renderFeature) {
		// TODO: batch list
		renderFeature->submitBatch(context, nullptr);
	}
}

Material* RenderStage::getMaterialFinal(Material* priorityValue, Material* sceneDefaultMaterial) const
{
	// specified drawXXXX()
	if (priorityValue) {

		// 優先マテリアルを使う場合、ステートは基本のマテリアルと同一でなければならない。
		// この時点で、DrawElement は 不透明・半透明  によって描画パスの振り分けが終わっているので、
		// 振り分け結果とつじつまが合わなくなるようなステートにしてはならない。
		// なお、シェーダ変数は異なっていてもかまわない。
		LN_CHECK(geometryStageParameters->m_material->equalStatus(priorityValue));

		return priorityValue;
	}

	// specified context->setMaterial() or meshObj->setMaterial()
	if (geometryStageParameters->m_material) return geometryStageParameters->m_material;

    // default は SceneRenderer に決めてもらう
	return sceneDefaultMaterial;
}

// TODO: deprecated: RLIMaterial
ShadingModel RenderStage::getShadingModelFinal(const Material* finalMaterial) const
{
	assert(finalMaterial);

	// specified meshObj->setXXXX()
	if (geometryStageParameters && geometryStageParameters->shadingModel.hasValue())
		return geometryStageParameters->shadingModel.value();

	//  specified meshObj->getMaterial(0)->setXXXX() etc...
	return finalMaterial->shadingModel();
}

// TODO: deprecated: RLIMaterial
BlendMode RenderStage::getBlendModeFinal(const Material* finalMaterial) const
{
	if (finalMaterial && finalMaterial->getBlendMode().hasValue())
		return finalMaterial->getBlendMode().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_blendMode.hasValue())
		return geometryStageParameters->m_blendMode.value();

	// default
	return BlendMode::Normal;
}

// TODO: deprecated: RLIMaterial
CullMode RenderStage::getCullingModeFinal(const Material* finalMaterial) const
{
	if (finalMaterial && finalMaterial->getCullingMode().hasValue())
		return finalMaterial->getCullingMode().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_cullingMode.hasValue())
		return geometryStageParameters->m_cullingMode.value();

	// default
	return CullMode::Back;
}

// TODO: deprecated: RLIMaterial
ComparisonFunc RenderStage::getDepthTestFuncFinal(const Material* finalMaterial) const
{
	if (finalMaterial && finalMaterial->isDepthTestEnabled().hasValue())
		return (finalMaterial->isDepthTestEnabled().value()) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_depthTestEnabled.hasValue())
		return (geometryStageParameters->m_depthTestEnabled.value()) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;

	// default
    return ComparisonFunc::LessEqual;
}

// TODO: deprecated: RLIMaterial
bool RenderStage::isDepthWriteEnabledFinal(const Material* finalMaterial) const
{
	if (finalMaterial && finalMaterial->isDepthWriteEnabled().hasValue())
		return finalMaterial->isDepthWriteEnabled().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_depthWriteEnabled.hasValue())
		return geometryStageParameters->m_depthWriteEnabled.value();

	// default
	return true;
}

float RenderStage::getOpacityFinal(RenderDrawElement* element) const
{
    return element->builtinEffectData->opacity;
}

const Color& RenderStage::getColorScaleFinal(RenderDrawElement* element) const
{
    return element->builtinEffectData->colorScale;
}

const Color& RenderStage::getBlendColorFinal(RenderDrawElement* element) const
{
    return element->builtinEffectData->blendColor;
}

const ColorTone& RenderStage::getToneFinal(RenderDrawElement* element) const
{
    return element->builtinEffectData->tone;
}

//void RenderStage::applyFrameBufferStatus(RenderPass* renderPass, const RenderStage* stage, const FrameBuffer& defaultFrameBufferInPass)
//{
//	RenderTargetTexture* renderTarget0 = nullptr;
//
//	// RenderTarget
//	{
//		for (int i = 0; i < MaxMultiRenderTargets; i++)
//		{
//			RenderTargetTexture* target = stage->getRenderTargetFinal(i);
//			if (!target) {
//				target = defaultFrameBufferInPass.renderTarget[i];
//			}
//
//			renderPass->setRenderTarget(i, target);
//
//			if (i == 0) {
//				renderTarget0 = target;
//			}
//		}
//	}
//
//	// DepthBuffer
//	{
//		DepthBuffer* depthBuffer = stage->getDepthBufferFinal();
//		if (depthBuffer) {
//			renderPass->setDepthBuffer(depthBuffer);
//		}
//		else {
//			renderPass->setDepthBuffer(defaultFrameBufferInPass.depthBuffer);
//		}
//	}
//
//	// Viewport
//	{
//		//const RectI& rect = stage->getViewportRectFinal();//getViewportRect();
//		//if (rect.width < 0)
//		//{
//		//	stateManager->setViewport(RectI(0, 0, renderTarget0->getSize()));
//		//}
//		//else
//		//{
//		//	stateManager->setViewport(rect);
//		//}
//		// TODO:Viewport
//		// TODO: m_scissorRect
//	}
//}

} // namespace detail
} // namespace ln

