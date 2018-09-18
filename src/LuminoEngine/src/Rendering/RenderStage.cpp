
#include "Internal.hpp"
#include <Lumino/Rendering/RenderFeature.hpp>
#include <Lumino/Rendering/Material.hpp>
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

//==============================================================================
// RenderStage

RenderStage::RenderStage()
	: renderFeature(nullptr)
{
}

bool RenderStage::equals(const RenderStage* other) const
{
	return
		frameBufferStageParameters->equals(other->frameBufferStageParameters) &&
		geometryStageParameters->equals(other->geometryStageParameters) &&
		renderFeatureStageParameters->equals(renderFeatureStageParameters) &&
		renderFeature == other->renderFeature;
}

void RenderStage::flush()
{
	if (renderFeature) {
		renderFeature->flush();
	}
}

AbstractMaterial* RenderStage::getMaterialFinal(AbstractMaterial* defaultValue, AbstractMaterial* priorityValue) const
{
	// specified drawXXXX()
	if (priorityValue) return priorityValue;

	// specified context->setMaterial() or meshObj->setMaterial()
	if (geometryStageParameters->m_material) return geometryStageParameters->m_material;

	// default は SceneRenderer に決めてもらう
	return defaultValue;
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
	return BlendMode::Alpha;
}

CullingMode RenderStage::getCullingModeFinal(AbstractMaterial* finalMaterial) const
{
	if (finalMaterial && finalMaterial->getCullingMode().hasValue())
		return finalMaterial->getCullingMode().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_cullingMode.hasValue())
		return geometryStageParameters->m_cullingMode.value();

	// default
	return CullingMode::Back;
}

bool RenderStage::isDepthTestEnabledFinal(AbstractMaterial* finalMaterial) const
{
	if (finalMaterial && finalMaterial->isDepthTestEnabled().hasValue())
		return finalMaterial->isDepthTestEnabled().value();

	// specified context->setXXXX() or meshObj->setXXXX()
	if (geometryStageParameters->m_depthTestEnabled.hasValue())
		return geometryStageParameters->m_depthTestEnabled.value();

	// default
	return true;
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
// DrawElementListManager

void DrawElementListManager::clear()
{
	for (auto& list : m_lists) {
		list.clear();
	}
}

void DrawElementListManager::addDrawElementList(RendringPhase phase, DrawElementList* list)
{
	m_lists[(int)phase].add(list);
}

const List<DrawElementList*>& DrawElementListManager::lists(RendringPhase phase) const
{
	return m_lists[(int)phase];
}

} // namespace detail
} // namespace ln

