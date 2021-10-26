#pragma once
#include <LuminoGraphics/GraphicsExtension.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../../../Graphics/src/RHIs/GraphicsDeviceContext.hpp"
#include "../RLIs/RLIBatchList.hpp"

namespace ln {
namespace detail {

class ExtensionRenderFeature
	: public RenderFeature
{
public:
    ExtensionRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult invoke(GraphicsContext* context, detail::RenderFeatureBatchList* batchList, INativeGraphicsExtension* extension);

protected:
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

private:

    struct BatchData
    {
        int offset;
        int count;
    };

    class Batch : public RenderFeatureBatch
    {
    public:
        BatchData data;
    };

    std::vector<INativeGraphicsExtension*> m_extensions;
    NativeGraphicsExtensionRenderPassPreCondition m_precondition;
    BatchData m_batchData;
};

} // namespace detail
} // namespace ln

