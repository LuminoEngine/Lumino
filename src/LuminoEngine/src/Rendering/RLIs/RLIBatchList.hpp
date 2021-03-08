#pragma once
#include "../RenderStage.hpp"
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

	template<class T>
	T* addNewBatch(RenderFeature* owner)
	{
		void* buffer = m_allocator->allocate(sizeof(T));
		T* batch = new (buffer)T();
		add(batch, owner);
		return batch;
	}

	RenderFeatureBatch* firstBatch() const { return m_head; }
	RenderFeatureBatch* lastBatch() const { return m_tail; }

	//
	//void prepareState(const CameraInfo& cameraInfo, RenderStage* stage, RenderDrawElement* element);

	//
	//void render(GraphicsContext* graphicsContext, SceneRendererPass* pass, const FrameBuffer& defaultFrameBuffer, const CameraInfo& cameraInfo);

    // TODO: とりいそぎ
    RenderTargetTexture* renderTarget = nullptr;
    DepthBuffer* depthBuffer = nullptr;

	const CameraInfo* m_mainCameraInfo;

private:
	void add(RenderFeatureBatch* batch, RenderFeature* owner);

	detail::RenderingManager* m_manager;
	Ref<detail::LinearAllocator> m_allocator;
	detail::RenderStage* m_currentStage;
	RenderFeatureBatch* m_head;
	RenderFeatureBatch* m_tail;
};

} // namespace detail
} // namespace ln

