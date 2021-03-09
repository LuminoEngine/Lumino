﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../Graphics/GraphicsManager.hpp"
#include "../RenderingManager.hpp"
#include "BlitRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// BlitRenderFeature

BlitRenderFeature::BlitRenderFeature()
	: m_manager(nullptr)
	, m_requestedCount(0)
{
}

void BlitRenderFeature::init(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;

	m_vertexLayout = m_manager->standardVertexDeclaration();

#ifdef LN_COORD_RH
	Vertex vertices[4] =
	{
		Vertex(Vector3(-1,  1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White),
		Vertex(Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White),
		Vertex(Vector3(1,  1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White),
		Vertex(Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White),
	};
#else
	Vertex vertices[4] =
	{
		{ Vector3(-1,  1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White, Vector4(1, 0, 0, 1) },
		{ Vector3(1,  1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White, Vector4(1, 0, 0, 1) },
		{ Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White, Vector4(1, 0, 0, 1) },
		{ Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White, Vector4(1, 0, 0, 1) },
	};
#endif
	m_vertexBuffer = makeObject<VertexBuffer>(sizeof(vertices), vertices, GraphicsResourceUsage::Static);
}

RequestBatchResult BlitRenderFeature::blit(detail::RenderFeatureBatchList* batchList, const RLIBatchState& state)
{
	// blit は基本的にスクリーンコピーで使われるため、毎回 Material が異なる。
	// 特に Batch する意味は無い。かえってオーバーヘッドが乗ってしまう。
	batchList->addNewBatch<RenderFeatureBatch>(this);
	return RequestBatchResult::Submitted;
}

void BlitRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	LN_UNREACHABLE();
}

void BlitRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->drawPrimitive(0, 2);
}

} // namespace detail
} // namespace ln

