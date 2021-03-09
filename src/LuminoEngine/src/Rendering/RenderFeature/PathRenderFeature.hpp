#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../Graphics/RHIs/GraphicsDeviceContext.hpp"
#include "../RLIs/RLIBatchList.hpp"

struct NVGcontext;
struct GLNVGcontext;

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
	void onDispose(bool explicitDisposing) override;

	RenderingManager* manager() const { return m_manager; }

	RequestBatchResult draw(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, GraphicsContext* context);

	void beginRendering() override;
	void submitBatch(GraphicsContext* context, RenderFeatureBatchList* batchList) override;
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

	RenderingManager* m_manager;
	BatchData m_batchData;
	NVGcontext* m_nvgContext;
	GLNVGcontext* m_glnvgContext;
};

} // namespace detail
} // namespace ln

