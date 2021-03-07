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

RequestBatchResult BlitRenderFeature::blit(detail::RenderFeatureBatchList* batchList, GraphicsContext* context)
{
	// blit は基本的にステート変更ごとに単発である。
	// 複数回呼ばれるのは全く同じ描画を複数回呼び出して、例えば加算合成を重ねるようなときに使う。
	m_requestedCount++;
	return RequestBatchResult::Staging;
}

void BlitRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	if (m_requestedCount > 0) {
		auto batch = batchList->addNewBatch<Batch>(this);
		batch->requestedCount = m_requestedCount;
		m_requestedCount = 0;
	}
}

void BlitRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->drawPrimitive(0, 2);
}

} // namespace detail
} // namespace ln

