
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../Graphics/GraphicsManager.hpp"
#include "../RenderingManager.hpp"
#include "ClearRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ClearRenderFeature

RequestBatchResult ClearRenderFeature::clear(detail::RenderFeatureBatchList* batchList, ClearFlags flags, const Color& color, float depth, uint8_t stencil)
{
	m_data.flags = flags;
	m_data.color = color;
	m_data.depth = depth;
	m_data.stencil = stencil;
	return RequestBatchResult::Staging;
}

void ClearRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<ClearBatch>(this);
	batch->data = m_data;
}

void ClearRenderFeature::renderBatch(GraphicsContext* context, detail::RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<ClearBatch*>(batch);
	context->clear(localBatch->data.flags, localBatch->data.color, localBatch->data.depth, localBatch->data.stencil);
}

} // namespace detail
} // namespace ln

