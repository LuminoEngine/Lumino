#pragma once
#include <LuminoCore/Base/EnumFlags.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../Graphics/RHIs/GraphicsDeviceContext.hpp"
#include "../RLIs/RLIBatchList.hpp"

namespace ln {
class VertexBuffer;
class IndexBuffer;
class Texture;
namespace detail {

class SpriteRenderFeature2 : public RenderFeature
{
public:
	// srcRect は UV 座標系上の値を設定する。 (通常0～1)
	// 以前は 2D メインな Sprite なのでピクセル単位で指定していたが、
	// 考え方として他の RenderFeature と同様に「最終的な描画に使うメッシュを作る」方針で統一したい。
	RequestBatchResult drawRequest(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		GraphicsContext* context,
		const Matrix& transform,
		const Vector2& size,
		const Vector2& anchorRatio,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType,
		SpriteFlipFlags flipFlags);

	virtual bool drawElementTransformNegate() const override { return true; }
	virtual void onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo) override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

	static void makeRenderSizeAndSourceRectHelper(Texture* texture, const Size& size, const Rect& sourceRect, Size* outSize, Rect* outSourceRect);

LN_CONSTRUCT_ACCESS:
	SpriteRenderFeature2();
	void init(RenderingManager* manager);

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

	void prepareBuffers(GraphicsContext* context, int spriteCount);

	RenderingManager* m_manager;
	Matrix m_viewMatrix;
	Matrix m_projMatrix;
	//Matrix m_viewInverseMatrix;
	//Vector3 m_viewDirection;
	//Vector3 m_viewPosition;

	// sprite-batching
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	int m_buffersReservedSpriteCount;
	int32_t m_spriteCount;
	//BatchData m_batchData;
	Vertex* m_mappedVertices;
};

} // namespace detail
} // namespace ln

