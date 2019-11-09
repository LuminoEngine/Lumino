
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsExtension.hpp>
#include "ExtensionRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ExtensionRenderFeature

ExtensionRenderFeature::ExtensionRenderFeature()
{
}

void ExtensionRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
}

RequestBatchResult ExtensionRenderFeature::invoke(INativeGraphicsExtension* extension)
{
    m_extensions.push_back(extension);
    m_batchData.count++;
	return RequestBatchResult::Staging;
}

void ExtensionRenderFeature::beginRendering()
{
    m_extensions.clear();
    m_batchData.offset = 0;
    m_batchData.count = 0;
}

void ExtensionRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
    auto batch = batchList->addNewBatch<Batch>(this);
    batch->data = m_batchData;

    m_batchData.offset = m_batchData.offset + m_batchData.count;
    m_batchData.count = 0;
}

void ExtensionRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
    auto localBatch = static_cast<Batch*>(batch);
    for (int i = 0; i < localBatch->data.count; i++) {
        context->drawExtension(m_extensions[localBatch->data.offset + i]);
    }
}

} // namespace detail
} // namespace ln

