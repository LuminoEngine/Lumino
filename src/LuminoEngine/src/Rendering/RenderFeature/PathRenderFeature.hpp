#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../Graphics/GraphicsDeviceContext.hpp"
#include "../RenderStage.hpp"

namespace ln {
namespace detail {

// NanoVG は RenderState を細かく制御しながら描画する必要があるため、
// ある程度抽象化された RenderingContext ではなく、GraphicsContext を使わなければ描画しきれない。
class PathRenderFeature
	: public RenderFeature
{
public:
	PathRenderFeature();
	void init(RenderingManager* manager);

	void beginRendering() override;
	void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

private:
	struct BatchData
	{
		int offset;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	BatchData m_batchData;
};

} // namespace detail
} // namespace ln

