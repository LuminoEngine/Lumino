
#include "Internal.hpp"
#include "RenderingManager.hpp"
#include "RenderElement.hpp"

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

void RenderDrawElement::fixFlags(RenderDrawElementTypeFlags additionalElementFlags)
{
	m_flags = RenderDrawElementTypeFlags::None;

	auto* material = m_stage->getMaterialFinal(nullptr, nullptr);
	if (material) {
		BlendMode blendMode = m_stage->getBlendModeFinal(material);
		if (blendMode == BlendMode::Normal)
			m_flags |= RenderDrawElementTypeFlags::Opaque;
		else
			m_flags |= RenderDrawElementTypeFlags::Transparent;
	}
	else {
		m_flags |= RenderDrawElementTypeFlags::Opaque;
	}

	m_flags |= additionalElementFlags;
}

void RenderDrawElement::calculateActualPriority()
{
	uint64_t base = 0;
	if (m_flags.hasFlag(RenderDrawElementTypeFlags::BackgroundSky))
		base |= 3000;
	else if (m_flags.hasFlag(RenderDrawElementTypeFlags::Opaque))
		base |= 2000;
	else if (m_flags.hasFlag(RenderDrawElementTypeFlags::Transparent))
		base |= 4000;

	uint64_t user = 0;
	auto* material = m_stage->getMaterialFinal(nullptr, nullptr);
	if (!material) {
		user = priority;
	}
	
	m_actualPriority = (base << 32) | priority;
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

void DrawElementListCollector::addDrawElementList(/*RenderPhaseClass phase, */DrawElementList* list)
{
    m_lists.add(list);
	//m_lists[(int)phase].add(list);
}

const List<DrawElementList*>& DrawElementListCollector::lists(/*RenderPhaseClass phase*/) const
{
    return m_lists;// [(int)phase];
}

void DrawElementListCollector::classify()
{
    for (auto& list : m_classifiedElements) {
        list.clear();
    }

    for (auto& elementList : m_lists) {
        RenderDrawElement* element = elementList->headElement();
        while (element) {
            m_classifiedElements[static_cast<int>(element->targetPhase)].add(element);
            element = element->next();
        }
    }
}

} // namespace detail
} // namespace ln

