﻿
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "RenderingManager.hpp"
#include "RenderStage.hpp"

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
// RenderDrawElement

RenderDrawElement::RenderDrawElement()
	: m_stage(nullptr)
	, m_next(nullptr)
{
}

RenderDrawElement::~RenderDrawElement()
{
}

void RenderDrawElement::onElementInfoOverride(ElementInfo* elementInfo, ShaderTechniqueClass_MeshProcess* meshProcess)
{
}

void RenderDrawElement::onSubsetInfoOverride(SubsetInfo* subsetInfo)
{
}

//==============================================================================
// RenderStage

RenderStage::RenderStage()
	: frameBufferStageParameters(nullptr)
	, geometryStageParameters(nullptr)
	, renderFeatureStageParameters(nullptr)
	, renderFeature(nullptr)
{
}

bool RenderStage::equals(const RenderStage* other) const
{
    // clear の場合は renderFeatureStageParameters が null になる。
    bool renderFeatureStageParametersEqual = false;
    if (renderFeatureStageParameters != other->renderFeatureStageParameters && renderFeatureStageParameters == nullptr) {
        renderFeatureStageParametersEqual = true;
    }
    else if (renderFeatureStageParameters == nullptr || other->renderFeatureStageParameters == nullptr) {
        renderFeatureStageParametersEqual = false;
    }
    else {
        renderFeatureStageParametersEqual = renderFeatureStageParameters->equals(other->renderFeatureStageParameters);
    }

	return
        renderFeatureStageParametersEqual &&
		frameBufferStageParameters->equals(other->frameBufferStageParameters) &&
		geometryStageParameters->equals(other->geometryStageParameters) &&
		renderFeature == other->renderFeature;
}

void RenderStage::flush(GraphicsContext* context)
{
	if (renderFeature) {
		renderFeature->flush(context);
	}
}

AbstractMaterial* RenderStage::getMaterialFinal(AbstractMaterial* priorityValue, AbstractMaterial* sceneDefaultMaterial) const
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

ShadingModel RenderStage::getShadingModelFinal(AbstractMaterial* finalMaterial) const
{
	assert(finalMaterial);

	// specified meshObj->setXXXX()
	if (geometryStageParameters && geometryStageParameters->shadingModel.hasValue())
		return geometryStageParameters->shadingModel.value();

	//  specified meshObj->getMaterial(0)->setXXXX() etc...
	return finalMaterial->shadingModel;
}

BlendMode RenderStage::getBlendModeFinal(AbstractMaterial* finalMaterial) const
{
	if (finalMaterial && finalMaterial->getBlendMode().hasValue())
		return finalMaterial->getBlendMode().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_blendMode.hasValue())
		return geometryStageParameters->m_blendMode.value();

	// default
	return BlendMode::Normal;
}

CullMode RenderStage::getCullingModeFinal(AbstractMaterial* finalMaterial) const
{
	if (finalMaterial && finalMaterial->getCullingMode().hasValue())
		return finalMaterial->getCullingMode().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_cullingMode.hasValue())
		return geometryStageParameters->m_cullingMode.value();

	// default
	return CullMode::Back;
}

ComparisonFunc RenderStage::getDepthTestFuncFinal(AbstractMaterial* finalMaterial) const
{
	if (finalMaterial && finalMaterial->isDepthTestEnabled().hasValue())
		return (finalMaterial->isDepthTestEnabled().value()) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_depthTestEnabled.hasValue())
		return (geometryStageParameters->m_depthTestEnabled.value()) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;

	// default
    return ComparisonFunc::LessEqual;
}

bool RenderStage::isDepthWriteEnabledFinal(AbstractMaterial* finalMaterial) const
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

const ToneF& RenderStage::getToneFinal(RenderDrawElement* element) const
{
    return element->builtinEffectData->tone;
}

//==============================================================================
// DrawElementList

DrawElementList::DrawElementList(RenderingManager* manager)
	: m_dataAllocator(makeRef<LinearAllocator>(manager->stageDataPageManager()))
	, m_headElement(nullptr)
	, m_tailElement(nullptr)
	, m_headFrameData(nullptr)
	, m_tailFrameData(nullptr)
{
}

DrawElementList::~DrawElementList()
{
	clear();
}

void DrawElementList::clear()
{
	m_dataAllocator->cleanup();
	m_renderStageList.clear();

	// destruct frame data.
	{
		IDrawElementListFrameData* p = m_headFrameData;
		while (p) {
			p->~IDrawElementListFrameData();
			p = p->m_nextFrameData;
		}
		m_headFrameData = nullptr;
		m_tailFrameData = nullptr;
	}

	// destruct draw elements.
	{
		RenderDrawElement* p = m_headElement;
		while (p) {
			p->~RenderDrawElement();
			p = p->next();
		}
		m_headElement = nullptr;
		m_tailElement = nullptr;
	}

	m_dynamicLightInfoList.clear();
}

RenderStage* DrawElementList::addNewRenderStage()
{
	return newFrameData<RenderStage>();
}

void DrawElementList::addElement(RenderStage* parentStage, RenderDrawElement* element)
{
	if (LN_REQUIRE(parentStage)) return;
	if (LN_REQUIRE(element)) return;
	if (LN_REQUIRE(!element->m_stage)) return;

	element->m_stage = parentStage;

	if (!m_headElement) {
		m_headElement = element;
	}
	else {
		m_tailElement->m_next = element;
	}
	m_tailElement = element;
}

void DrawElementList::addFrameData(IDrawElementListFrameData* data)
{
	LN_CHECK(data);

	if (!m_headFrameData) {
		m_headFrameData = data;
	}
	else {
		m_tailFrameData->m_nextFrameData = data;
	}
	m_tailFrameData = data;
}

//==============================================================================
// DrawElementListCollector

void DrawElementListCollector::clear()
{
    m_lists.clear();
	//for (auto& list : m_lists) {
	//	list.clear();
	//}
}

void DrawElementListCollector::addDrawElementList(/*RendringPhase phase, */DrawElementList* list)
{
    m_lists.add(list);
	//m_lists[(int)phase].add(list);
}

const List<DrawElementList*>& DrawElementListCollector::lists(/*RendringPhase phase*/) const
{
    return m_lists;// [(int)phase];
}

} // namespace detail
} // namespace ln

