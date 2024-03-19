#pragma once
#include <LuminoEngine/GraphicsRHI/GraphicsExtension.hpp>
#include <LuminoGraphics/Rendering/RenderFeature.hpp>
#include "../RLIs/RLIBatchList.hpp"

namespace ln {
namespace detail {

class ExtensionRenderFeature
	: public RenderFeature
{
public:
    ExtensionRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult invoke(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList, INativeGraphicsExtension* extension);

protected:
	virtual void beginRendering() override;
    virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;

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

