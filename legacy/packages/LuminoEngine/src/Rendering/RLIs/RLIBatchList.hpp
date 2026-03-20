#pragma once
#include <LuminoEngine/Rendering/RenderStage.hpp>
#include "Common.hpp"
#include "RLIBatch.hpp"

namespace ln {
namespace detail {

	
class RenderFeatureBatchList
{
public:
	RenderFeatureBatchList(RenderingManager* manager);
	void clear();
	void setCurrentStage(RenderStage* stage);

	//void addClearBatch(ClearFlags flags, const Color& color, float depth, uint8_t stencil);


	RenderFeatureBatch* firstBatch() const { return m_head; }
	RenderFeatureBatch* lastBatch() const { return m_tail; }


    // TODO: とりいそぎ
    RenderTargetTexture* renderTarget = nullptr;
    DepthBuffer* depthBuffer = nullptr;

	const CameraInfo* m_mainCameraInfo;




	// ↓RenderFeature から使って良いもの

	//bool lastBatchRenderFeatureIs(RenderFeature_deprecated* renderFeature) const;
	bool equalsLastBatchState(RenderFeature_deprecated* renderFeature, const RLIBatchState& state);

	template<class TBatch>
	TBatch* addNewBatch(RenderFeature_deprecated* owner, const RLIBatchState& state)
	{
		void* buffer = m_allocator->allocate(sizeof(TBatch));
		TBatch* batch = new (buffer)TBatch();
		add(batch, owner, state);
		return batch;
	}

private:
	void add(RenderFeatureBatch* batch, RenderFeature_deprecated* owner, const RLIBatchState& state);

	detail::RenderingManager* m_manager;
	Ref<detail::LinearAllocator> m_allocator;
	detail::RenderStage* m_currentStage;
	RenderFeatureBatch* m_head;
	RenderFeatureBatch* m_tail;
};

} // namespace detail
} // namespace ln

