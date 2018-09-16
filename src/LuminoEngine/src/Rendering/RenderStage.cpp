
#include "Internal.hpp"
#include <Lumino/Rendering/RenderFeature.hpp>
#include "RenderingManager.hpp"
#include "RenderStage.hpp"

namespace ln {
namespace detail {

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

void RenderStage::flush()
{
	if (renderFeature) {
		renderFeature->flush();
	}
}

//==============================================================================
// DrawElementList

DrawElementList::DrawElementList(RenderingManager* manager)
	: m_dataAllocator(makeRef<LinearAllocator>(manager->stageDataPageManager()))
	, m_headElement(nullptr)
	, m_tailElement(nullptr)
{
}

void DrawElementList::clear()
{
	m_dataAllocator->cleanup();
	m_renderStageList.clear();
	m_headElement = nullptr;
	m_tailElement = nullptr;
}

RenderStage* DrawElementList::addNewRenderStage()
{
	return newData<RenderStage>();
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
		m_tailElement = element;
	}
}

} // namespace detail
} // namespace ln

