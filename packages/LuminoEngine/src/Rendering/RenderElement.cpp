
#include "Internal.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
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
    , m_allElementList{nullptr, nullptr}
	//, m_classifiedElementList({})
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
		RenderDrawElement* p = m_allElementList.headElement;
		while (p) {
			p->~RenderDrawElement();
			p = p->next();
		}
		m_allElementList.headElement = nullptr;
		m_allElementList.tailElement = nullptr;

		//for (auto& list : m_classifiedElementList) {
		//	list.headElement = nullptr;
		//	list.tailElement = nullptr;
		//}
	}
}

RenderStage* DrawElementList::addNewRenderStage()
{
	return newFrameData<RenderStage>();
}

void DrawElementList::addElement(RenderDrawElement* element)
{
	if (LN_REQUIRE(element)) return;
	if (LN_REQUIRE(element->m_stage)) return;
	if (LN_REQUIRE(element->targetPhase != RenderPart::_Count)) return;

	// Add to AllList
	{
		if (!m_allElementList.headElement) {
			m_allElementList.headElement = element;
		}
		else {
			m_allElementList.tailElement->m_next = element;
		}
		m_allElementList.tailElement = element;
	}

	// Add to ClassifiedList
	// ※ 0.9.0 までは RenderingPipeline::render() の先頭で行っていたが、複数ビューからの描画を想定したときに非効率なので、ここで行うことにした。
	//{
	//	ElementListDetail& list = m_classifiedElementList[static_cast<int>(element->targetPhase)];

	//	if (!list.headElement) {
	//		list.headElement = element;
	//	}
	//	else {
	//		list.tailElement->m_classifiedNext = element;
	//	}
	//	list.tailElement = element;
	//}
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

#if 0
//==============================================================================
// DrawElementListCollector

void DrawElementListCollector::clear()
{
    m_lists.clear();
	//for (auto& list : m_lists) {
	//	list.clear();
	//}
}

void DrawElementListCollector::addDrawElementList(/*RenderPart phase, */DrawElementList* list)
{
    m_lists.add(list);
	//m_lists[(int)phase].add(list);
}

const List<DrawElementList*>& DrawElementListCollector::lists(/*RenderPart phase*/) const
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
#endif

} // namespace detail
} // namespace ln

