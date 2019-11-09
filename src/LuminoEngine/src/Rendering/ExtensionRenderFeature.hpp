#pragma once
#include "../Graphics/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"

namespace ln {
namespace detail {

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class ExtensionRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
    ExtensionRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("ExtensionRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

class ExtensionRenderFeature
	: public RenderFeature
{
public:
    ExtensionRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult invoke(INativeGraphicsExtension* extension);

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
    BatchData m_batchData;
};

} // namespace detail
} // namespace ln

