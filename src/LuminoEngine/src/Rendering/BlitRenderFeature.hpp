#pragma once
#include "../Graphics/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"

namespace ln {
class VertexLayout;
class VertexBuffer;
namespace detail {
class RenderingManager;

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class BlitRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
    BlitRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("BlitRenderFeatureStageParameters"))
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

// MeshSection 単位で描画する
class BlitRenderFeature
	: public RenderFeature
{
public:
    BlitRenderFeature();
	void init(RenderingManager* manager);

	void blit(GraphicsContext* context);

	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;

private:
	class Batch : public RenderFeatureBatch
	{
	public:
		int requestedCount;
	};

	RenderingManager* m_manager;
#ifdef LN_RENDERING_MIGRATION
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
#else
	void blitImplOnRenderThread(ICommandList* context);

    Ref<IVertexBuffer> m_vertexBuffer;
    Ref<IVertexDeclaration> m_vertexDeclaration;
	//GraphicsManager*		m_manager;
	//Driver::IRenderer*		m_renderer;
#endif
	int m_requestedCount;
};


} // namespace detail
} // namespace ln

