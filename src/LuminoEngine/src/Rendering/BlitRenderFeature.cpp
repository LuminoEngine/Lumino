
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "BlitRenderFeature.hpp"
#include "RenderingManager.hpp"

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

#ifdef LN_RENDERING_MIGRATION
	m_vertexLayout = m_manager->standardVertexDeclaration();

	Vertex vertices[4] =
	{
		{ Vector3(-1,  1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White },
		{ Vector3(1,  1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White },
		{ Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White },
		{ Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White },
	};
	m_vertexBuffer = makeObject<VertexBuffer>(sizeof(vertices), vertices, GraphicsResourceUsage::Static);
#else
    Vertex vertices[4] =
    {
        { Vector3(-1,  1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White },
        { Vector3(1,  1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White },
        { Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White },
        { Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White },
    };
    m_vertexBuffer = m_manager->graphicsManager()->deviceContext()->createVertexBuffer(GraphicsResourceUsage::Static, sizeof(vertices), vertices);
    m_vertexDeclaration = m_manager->standardVertexDeclarationRHI();
#endif
}

void BlitRenderFeature::blit(GraphicsContext* context)
{
#ifdef LN_RENDERING_MIGRATION
	// blit は基本的にステート変更ごとに単発である。
	// 複数回呼ばれるのは全く同じ描画を複数回呼び出して、例えば加算合成を重ねるようなときに使う。
	m_requestedCount++;
#else
    auto* _this = this;
	ICommandList* c = GraphicsContextInternal::commitState(context);
    LN_ENQUEUE_RENDER_COMMAND_2(
        BlitRenderFeature_blit, context,
        BlitRenderFeature*, _this,
		ICommandList*, c,
        {
            _this->blitImplOnRenderThread(c);
        });
#endif
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

#ifdef LN_RENDERING_MIGRATION
#else
void BlitRenderFeature::blitImplOnRenderThread(ICommandList* context)
{
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
	context->drawPrimitive(0, 2);
}
#endif

} // namespace detail
} // namespace ln

