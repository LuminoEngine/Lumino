#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../../../../Graphics/src/RHIs/GraphicsDeviceContext.hpp"
#include "../RLIs/RLIBatchList.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;

namespace detail {

class ClearRenderFeature : public RenderFeature
{
public:
	RequestBatchResult clear(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, ClearFlags flags, const Color& color, float depth, uint8_t stencil);

protected:
    virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
    virtual void renderBatch(GraphicsCommandList* context, detail::RenderFeatureBatch* batch) override;

private:
	struct ClearInfo
	{
		ClearFlags flags;
		Color color;
		float depth;
		uint8_t stencil;
	};

	class ClearBatch : public detail::RenderFeatureBatch
	{
	public:
		ClearInfo data;
	};

	ClearInfo m_data;
};

} // namespace detail
} // namespace ln

