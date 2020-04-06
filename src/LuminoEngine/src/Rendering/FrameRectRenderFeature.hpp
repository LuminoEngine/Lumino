#pragma once
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/Drawing.hpp>
#include "RenderStage.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

#if 1

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class FrameRectRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	FrameRectRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("FrameRectRenderFeatureStageParameters"))
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

class FrameRectRenderFeature
	: public RenderFeature
{
public:
	FrameRectRenderFeature();
	void init(RenderingManager* manager);

	RequestBatchResult drawRequest(GraphicsContext* context, const Rect& rect, const Matrix& worldTransform, const Thickness& borderThickness, const Rect& srcRect, Sprite9DrawMode wrapMode, const SizeI& srcTextureSize);

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

	void prepareBuffers(GraphicsContext* context, int spriteCount);
	void addSprite(GraphicsContext* context, const Vector3& pos0, const Vector2& uv0, const Vector3& pos1, const Vector2& uv1, const Vector3& pos2, const Vector2& uv2, const Vector3& pos3, const Vector2& uv3);
	void putRectangleStretch(GraphicsContext* context, const Rect& rect, const Rect& srcUVRect);
	void putRectangleTiling(GraphicsContext* context, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect);
	void putRectangle(GraphicsContext* context, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, Sprite9DrawMode wrapMode);
	void putFrameRectangle(GraphicsContext* context, const Rect& rect, const Thickness& borderThickness, Rect srcRect, Sprite9DrawMode wrapMode, const SizeI& srcTextureSize);

	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	int m_buffersReservedSpriteCount;
	BatchData m_batchData;
	Vertex* m_mappedVertices;

    const Matrix* m_worldTransform;
};

#else

class InternalFrameRectRenderer
    : public RefObject
{
public:
	InternalFrameRectRenderer();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }

    void draw(const Rect& rect, const Matrix& worldTransform, BrushImageDrawMode imageDrawMode, const Thickness& borderThickness, const Rect& srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize);
	void flush(ICommandList* context);

private:
	void prepareBuffers(int spriteCount);
    void addVertex(const Vector3& pos, const Vector2& uv);
    void putRectangleStretch(const Rect& rect, const Rect& srcUVRect);
    void putRectangleTiling(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect);
    void putRectangle(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, BrushWrapMode wrapMode);
    void putFrameRectangle(const Rect& rect, const Thickness& borderThickness, Rect srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize);

	RenderingManager* m_manager;
	Ref<IVertexDeclaration> m_vertexDeclaration;
	Ref<IVertexBuffer> m_vertexBuffer;
	Ref<IIndexBuffer> m_indexBuffer;

    Ref<LinearAllocator> m_vertexAllocator;
    List<Vertex*> m_vertices;

	size_t m_buffersReservedSpriteCount;
};

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class FrameRectRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	FrameRectRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("FrameRectRenderFeatureStageParameters"))
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

class FrameRectRenderFeature
	: public RenderFeature
{
public:
	FrameRectRenderFeature();
	void init(RenderingManager* manager);

    void draw(GraphicsContext* context, const Rect& rect, const Matrix& worldTransform, BrushImageDrawMode imageDrawMode, const Thickness& borderThickness, const Rect& srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize);

protected:
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
    virtual bool drawElementTransformNegate() const override { return true; }

private:
	Ref<InternalFrameRectRenderer> m_internal;
};
#endif

class DrawFrameRectElement : public RenderDrawElement
{
public:
    Rect rect;
    Matrix transform;
    BrushImageDrawMode imageDrawMode;
    Thickness borderThickness;
    Rect srcRect;
	Sprite9DrawMode wrapMode;

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
	{
		m_srcTextureSize.width = subsetInfo->materialTexture->width();
		m_srcTextureSize.height = subsetInfo->materialTexture->height();

		return static_cast<detail::FrameRectRenderFeature*>(renderFeature)->drawRequest(
            context, rect, transform, borderThickness, srcRect, wrapMode, m_srcTextureSize);
    }

private:
    SizeI m_srcTextureSize;
};

} // namespace detail
} // namespace ln

