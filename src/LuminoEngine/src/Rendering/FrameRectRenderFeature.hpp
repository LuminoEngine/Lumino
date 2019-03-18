#pragma once
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {



class InternalFrameRectRenderer
    : public RefObject
{
public:
	InternalFrameRectRenderer();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }

	void flush(IGraphicsDeviceContext* context);

private:
	void prepareBuffers(int spriteCount);

	RenderingManager* m_manager;
	Ref<IVertexDeclaration> m_vertexDeclaration;
	Ref<IVertexBuffer> m_vertexBuffer;
	Ref<IIndexBuffer> m_indexBuffer;
	size_t m_spriteCount;
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

protected:
	virtual void flush(GraphicsContext* context) override;
    virtual bool drawElementTransformNegate() const override { return true; }

private:
	void flushInternal(GraphicsContext* context);

	Ref<InternalFrameRectRenderer> m_internal;
};

class DrawFrameRectElement : public RenderDrawElement
{
public:
    virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
    {
		static_cast<detail::FrameRectRenderFeature*>(renderFeatures);
    }
};

} // namespace detail
} // namespace ln

