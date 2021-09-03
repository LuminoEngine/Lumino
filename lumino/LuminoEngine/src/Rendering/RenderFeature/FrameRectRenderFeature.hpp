#pragma once
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/Drawing.hpp>
#include "../RenderElement.hpp"
#include "../RenderingManager.hpp"
#include "../RLIs/RLIBatchList.hpp"

namespace ln {
namespace detail {

class FrameRectRenderFeature
	: public RenderFeature
{
public:
	FrameRectRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult drawRequest(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		const Rect& rect,
		const Matrix& worldTransform,
		const Thickness& borderThickness,
		const Rect& srcRect,
		Sprite9DrawMode wrapMode,
		const SizeI& srcTextureSize);

protected:
    virtual void beginRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
	virtual bool drawElementTransformNegate() const override { return true; }

private:
	struct BatchData
	{
		int spriteOffset;
		int spriteCount;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	void prepareBuffers(int spriteCount);
	void addSprite(Batch* batch, const Vector3& pos0, const Vector2& uv0, const Vector3& pos1, const Vector2& uv1, const Vector3& pos2, const Vector2& uv2, const Vector3& pos3, const Vector2& uv3);
	void putRectangleStretch(Batch* batch, const Rect& rect, const Rect& srcUVRect);
	void putRectangleTiling(Batch* batch, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect);
	void putRectangle(Batch* batch, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, Sprite9DrawMode wrapMode);
	void putFrameRectangle(Batch* batch, const Rect& rect, const Thickness& borderThickness, Rect srcRect, Sprite9DrawMode wrapMode, const SizeI& srcTextureSize);

	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	int m_buffersReservedSpriteCount;
	//BatchData m_batchData;
	Vertex* m_mappedVertices;

    const Matrix* m_worldTransform;
};

class DrawFrameRectElement : public RenderDrawElement
{
public:
    Rect rect;
    Matrix transform;
	Sprite9DrawMode imageDrawMode;
    Thickness borderThickness;
    Rect srcRect;

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const RLIBatchState* state) override
	{
		m_srcTextureSize.width = state->m_subsetInfo.materialTexture->width();
		m_srcTextureSize.height = state->m_subsetInfo.materialTexture->height();

		return static_cast<detail::FrameRectRenderFeature*>(renderFeature)->drawRequest(
			batchList, *state, rect, transform, borderThickness, srcRect, imageDrawMode, m_srcTextureSize);
    }

private:
    SizeI m_srcTextureSize;
};

} // namespace detail
} // namespace ln

