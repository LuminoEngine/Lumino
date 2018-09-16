
#include "Internal.hpp"
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
	, m_headElement(nullptr)
{
}

void RenderStage::addElement(RenderDrawElement* element)
{
	if (LN_REQUIRE(element)) return;
	if (LN_REQUIRE(!element->m_stage)) return;
	element->m_stage = this;
	if (!m_headElement) {
		m_headElement = element;
	}
	else {
		m_tailElement->m_next = element;
		m_tailElement = element;
	}
}

//==============================================================================
// RenderStageList

RenderStageList::RenderStageList(RenderingManager* manager)
	: m_dataAllocator(makeRef<LinearAllocator>(manager->stageDataPageManager()))
{
}

RenderStage* RenderStageList::addNewRenderStage()
{
	return newData<RenderStage>();
}

} // namespace detail
} // namespace ln

