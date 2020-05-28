#pragma once
#include "../Graphics/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
namespace detail {
class RenderingManager;

// MeshSection 単位で描画する
class BlitRenderFeature
	: public RenderFeature
{
public:
    BlitRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult blit(detail::RenderFeatureBatchList* batchList, GraphicsContext* context);

	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

private:
	class Batch : public RenderFeatureBatch
	{
	public:
		int requestedCount;
	};

	RenderingManager* m_manager;
    Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	int m_requestedCount;
};


} // namespace detail
} // namespace ln

