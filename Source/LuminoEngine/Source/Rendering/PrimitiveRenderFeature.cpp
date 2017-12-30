
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Material.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/CoreGraphicsRenderFeature.h"
#include "../Graphics/RenderingCommand.h"
#include "../Graphics/GraphicsManager.h"
#include "PrimitiveRenderFeature.h"

#define LN_CALL_CORE_COMMAND(func, command, ...) \
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded) { \
		m_manager->getPrimaryRenderingCommandList()->addCommand<command>(m_core, __VA_ARGS__); \
	} \
	else { \
		m_core->func(__VA_ARGS__); \
	}

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
struct PrimitiveRendererCore_SetStateCommand : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	PrimitiveRendererMode	m_mode;
	void create(PrimitiveRendererCore* core, PrimitiveRendererMode mode)
	{
		m_core = core;
		m_mode = mode;
	}
	void execute()
	{
		m_core->setState(m_mode);
	}
};

//==============================================================================
struct PrimitiveRendererCore_DrawLine : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	Vector3 m_from;
	Color m_fromColor;
	Vector3 m_to;
	Color m_toColor;

	void create(PrimitiveRendererCore* core, const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
	{
		m_core = core;
		m_from = from;
		m_fromColor = fromColor;
		m_to = to;
		m_toColor = toColor;
	}
	void execute() { m_core->drawLine(m_from, m_fromColor, m_to, m_toColor); }
};

//==============================================================================
struct PrimitiveRendererCore_DrawSquare : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	PrimitiveRendererCore::DrawSquareData	m_data;

	void create(PrimitiveRendererCore* core, const PrimitiveRendererCore::DrawSquareData& data) { m_core = core; m_data = data; }
	void execute() { m_core->drawSquare(m_data); }
};

//==============================================================================
struct PrimitiveRendererCore_FlushCommand : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	void create(PrimitiveRendererCore* core) { m_core = core; }
	void execute() { m_core->flush(); }
};

//==============================================================================
// PrimitiveRendererCore
//==============================================================================

//------------------------------------------------------------------------------
PrimitiveRendererCore::PrimitiveRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_mode(PrimitiveRendererMode::TriangleList)
{
}

//------------------------------------------------------------------------------
PrimitiveRendererCore::~PrimitiveRendererCore()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	const int DefaultFaceCount = 2048;

	auto* device = m_manager->getGraphicsDevice();
	m_renderer = device->getRenderer();
	m_vertexBuffer = device->createVertexBuffer(sizeof(Vertex) * DefaultFaceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->createIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);

	m_vertexCache.clearAndReserve(DefaultFaceCount * 4);
	m_indexCache.clearAndReserve(DefaultFaceCount * 6);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::setState(PrimitiveRendererMode mode)
{
	m_mode = mode;
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	addVertex(from, Vector2::Zero, fromColor);
	addVertex(to, Vector2::Zero, toColor);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::drawSquare(const DrawSquareData& data)
{
	uint16_t i = m_vertexCache.getCount();
	m_indexCache.add(i + 0);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 3);

	addVertex(data.pos[0], data.uv[0], data.color[0]);
	addVertex(data.pos[1], data.uv[1], data.color[1]);
	addVertex(data.pos[2], data.uv[2], data.color[2]);
	addVertex(data.pos[3], data.uv[3], data.color[3]);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::flush()
{
	// サイズが足りなければ再作成
	auto* device = m_manager->getGraphicsDevice();
	if (m_vertexBuffer->getByteCount() < m_vertexCache.getBufferUsedByteCount())
	{
		LN_SAFE_RELEASE(m_vertexBuffer);
		m_vertexBuffer = device->createVertexBuffer(m_vertexCache.getBufferUsedByteCount(), nullptr, ResourceUsage::Dynamic);
	}
	if (m_indexBuffer->getByteCount() < m_indexCache.getBufferUsedByteCount())
	{
		LN_SAFE_RELEASE(m_indexBuffer);
		m_indexBuffer = device->createIndexBuffer(m_indexCache.getBufferUsedByteCount(), nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
	}

	// 描画する
	m_vertexBuffer->setSubData(0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
	m_indexBuffer->setSubData(0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());

	{
		if (m_mode == PrimitiveRendererMode::TriangleList)
		{
			m_renderer->setVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
			m_renderer->setVertexBuffer(0, m_vertexBuffer);
			m_renderer->setIndexBuffer(m_indexBuffer);
			m_renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);
		}
		else if (m_mode == PrimitiveRendererMode::LineList)
		{
			m_renderer->setVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
			m_renderer->setVertexBuffer(0, m_vertexBuffer);
			m_renderer->drawPrimitive(PrimitiveType_LineList, 0, m_vertexCache.getCount() / 2);
		}
	}

	// キャッシュクリア
	m_vertexCache.clear();
	m_indexCache.clear();
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex)
{
	assert(vb != nullptr);
	assert(ib != nullptr);
	*outBeginVertexIndex = m_vertexCache.getCount();
	*vb = m_vertexCache.request(vertexCount);
	*ib = m_indexCache.request(indexCount);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::addVertex(const Vector3& pos, const Vector2& uv, const Color& color)
{
	Vertex v;
	v.position = pos;
	v.uv = uv;
	v.color = color;
	v.normal = -Vector3::UnitZ;
	m_vertexCache.add(v);
}

//==============================================================================
// PrimitiveRenderFeature
//==============================================================================

//------------------------------------------------------------------------------
PrimitiveRenderFeature::PrimitiveRenderFeature()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//------------------------------------------------------------------------------
PrimitiveRenderFeature::~PrimitiveRenderFeature()
{
	LN_SAFE_RELEASE(m_core);
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::initialize(GraphicsManager* manager)
{
	m_manager = manager;

	m_core = LN_NEW PrimitiveRendererCore();
	m_core->initialize(m_manager);
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	setPrimitiveRendererMode(PrimitiveRendererMode::LineList);
	checkUpdateState();
	LN_CALL_CORE_COMMAND(drawLine, PrimitiveRendererCore_DrawLine, from, fromColor, to, toColor);
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::drawSquare(
	const Vector3& position1, const Vector2& uv1, const Color& color1,
	const Vector3& position2, const Vector2& uv2, const Color& color2,
	const Vector3& position3, const Vector2& uv3, const Color& color3,
	const Vector3& position4, const Vector2& uv4, const Color& color4)
{
	setPrimitiveRendererMode(PrimitiveRendererMode::TriangleList);
	checkUpdateState();
	PrimitiveRendererCore::DrawSquareData data;
	data.pos[0] = position1; data.uv[0] = uv1; data.color[0] = color1;
	data.pos[1] = position2; data.uv[1] = uv2; data.color[1] = color2;
	data.pos[2] = position3; data.uv[2] = uv3; data.color[2] = color3;
	data.pos[3] = position4; data.uv[3] = uv4; data.color[3] = color4;
	LN_CALL_CORE_COMMAND(drawSquare, PrimitiveRendererCore_DrawSquare, data);
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::drawRectangle(const Rect& rect)
{
	float l = rect.getLeft();
	float t = rect.getTop();
	float r = rect.getRight();
	float b = rect.getBottom();
	drawSquare(
		Vector3(l, t, 0), Vector2(0, 0), Color::White,
		Vector3(r, t, 0), Vector2(1, 0), Color::White,
		Vector3(l, b, 0), Vector2(0, 1), Color::White,
		Vector3(r, b, 0), Vector2(1, 1), Color::White);
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::flush()
{
	if (m_flushRequested)
	{
        if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded) {
            m_manager->getPrimaryRenderingCommandList()->addCommand<PrimitiveRendererCore_FlushCommand>(m_core);
        }
        else {
            m_core->flush();
        }
        m_flushRequested = false;
	}
}

//------------------------------------------------------------------------------
bool PrimitiveRenderFeature::isStandaloneShader() const { return false; }
void PrimitiveRenderFeature::onActivated() { m_stateModified = true; }
void PrimitiveRenderFeature::onDeactivated() { flush(); }

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::setPrimitiveRendererMode(PrimitiveRendererMode mode)
{
	if (mode != m_mode)
	{
		m_mode = mode;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::checkUpdateState()
{
	if (m_stateModified)
	{
		flush();

		LN_CALL_CORE_COMMAND(setState, PrimitiveRendererCore_SetStateCommand, m_mode);
		m_stateModified = false;
	}
}


//==============================================================================
// BlitRenderer
//==============================================================================

//------------------------------------------------------------------------------
BlitRenderer::BlitRenderer()
	: m_manager(nullptr)
	, m_vertexBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
BlitRenderer::~BlitRenderer()
{
}

//------------------------------------------------------------------------------
void BlitRenderer::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	auto* device = m_manager->getGraphicsDevice();

	Vertex vertices[4] =
	{
		{ Vector3(-1,  1, 0), Vector2(0, 0), Vector3::Zero, Color::White },
		{ Vector3( 1,  1, 0), Vector2(1, 0), Vector3::Zero, Color::White },
		{ Vector3(-1, -1, 0), Vector2(0, 1), Vector3::Zero, Color::White },
		{ Vector3( 1, -1, 0), Vector2(1, 1), Vector3::Zero, Color::White },
	};
	m_vertexBuffer.attach(device->createVertexBuffer(sizeof(vertices), vertices, ResourceUsage::Static), false);

	m_commonMaterial = newObject<CommonMaterial>();
}

//------------------------------------------------------------------------------
CommonMaterial* BlitRenderer::getCommonMaterial() const
{
	return m_commonMaterial;
}

//------------------------------------------------------------------------------
void BlitRenderer::blit()
{
	auto* _this = this;
	LN_ENQUEUE_RENDER_COMMAND_1(
		blit, m_manager,
		BlitRenderer*, _this,
		{
			_this->blitImpl();
		});
}

//------------------------------------------------------------------------------
void BlitRenderer::blitImpl()
{
	auto* device = m_manager->getGraphicsDevice();
	auto* renderer = device->getRenderer();
	renderer->setVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
	renderer->setVertexBuffer(0, m_vertexBuffer);
	renderer->drawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
}

//------------------------------------------------------------------------------
bool BlitRenderer::isStandaloneShader() const { return false; }
void BlitRenderer::flush() {}
void BlitRenderer::onActivated() {}
void BlitRenderer::onDeactivated() {}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
