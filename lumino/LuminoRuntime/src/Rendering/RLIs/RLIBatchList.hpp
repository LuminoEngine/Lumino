#pragma once
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
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

	//
	//void prepareState(const CameraInfo& cameraInfo, RenderStage* stage, RenderDrawElement* element);

	//
	//void render(GraphicsCommandList* graphicsContext, SceneRendererPass* pass, const FrameBuffer& defaultFrameBuffer, const CameraInfo& cameraInfo);

    // TODO: とりいそぎ
    RenderTargetTexture* renderTarget = nullptr;
    DepthBuffer* depthBuffer = nullptr;

	const CameraInfo* m_mainCameraInfo;




	// ↓RenderFeature から使って良いもの

	//bool lastBatchRenderFeatureIs(RenderFeature* renderFeature) const;
	bool equalsLastBatchState(RenderFeature* renderFeature, const RLIBatchState& state);

	template<class TBatch>
	TBatch* addNewBatch(RenderFeature* owner, const RLIBatchState& state)
	{
		void* buffer = m_allocator->allocate(sizeof(TBatch));
		TBatch* batch = new (buffer)TBatch();
		add(batch, owner, state);
		return batch;
	}

private:
	void add(RenderFeatureBatch* batch, RenderFeature* owner, const RLIBatchState& state);

	detail::RenderingManager* m_manager;
	Ref<detail::LinearAllocator> m_allocator;
	detail::RenderStage* m_currentStage;
	RenderFeatureBatch* m_head;
	RenderFeatureBatch* m_tail;
};

} // namespace detail
} // namespace ln

