
#include "Internal.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "RLIBatch.hpp"
#include "RLIBatchList.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// RenderFeatureBatchList

RenderFeatureBatchList::RenderFeatureBatchList(detail::RenderingManager* manager)
	: m_manager(manager)
	, m_allocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
	, m_currentStage(nullptr)
	, m_head(nullptr)
	, m_tail(nullptr)
{
}

void RenderFeatureBatchList::clear()
{
	m_allocator->cleanup();
	m_currentStage = nullptr;
	m_head = nullptr;
	m_tail = nullptr;
}

void RenderFeatureBatchList::setCurrentStage(detail::RenderStage* stage)
{
	m_currentStage = stage;
}
//
//void RenderFeatureBatchList::addClearBatch(ClearFlags flags, const Color& color, float depth, uint8_t stencil)
//{
//
//	addNewBatch<ClearBatch>(nullptr);
//}

void RenderFeatureBatchList::add(RenderFeatureBatch* batch, RenderFeature* owner, const RLIBatchState& state)
{
	if (LN_REQUIRE(batch)) return;
	if (LN_REQUIRE(m_currentStage)) return;

	batch->m_owner = owner;
	batch->m_stage = m_currentStage;
	batch->m_material = state;

	if (!m_head) {
		m_head = m_tail = batch;
	}
	else {
		m_tail->m_next = batch;
		m_tail = batch;
	}
}

//bool RenderFeatureBatchList::lastBatchRenderFeatureIs(RenderFeature* renderFeature) const
//{
//	if (m_tail) {
//		return m_tail->renderFeature() == renderFeature;
//	}
//	else {
//		return false;
//	}
//}

bool RenderFeatureBatchList::equalsLastBatchState(RenderFeature* renderFeature, const RLIBatchState& state)
{
	if (!m_tail) return false;
	if (m_tail->renderFeature() != renderFeature) return false;
	if (!m_tail->finalMaterial()->equals(&state)) return false;
	return true;
}

} // namespace detail
} // namespace ln

