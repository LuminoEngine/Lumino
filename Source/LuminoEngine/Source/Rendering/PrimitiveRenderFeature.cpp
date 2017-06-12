
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Material.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/RendererImpl.h"
#include "../Graphics/RenderingCommand.h"
#include "../Graphics/GraphicsManager.h"
#include "PrimitiveRenderFeature.h"

#define LN_CALL_CORE_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded) { \
		m_manager->GetPrimaryRenderingCommandList()->AddCommand<command>(m_core, __VA_ARGS__); \
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
		m_core->SetState(m_mode);
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
	void execute() { m_core->DrawLine(m_from, m_fromColor, m_to, m_toColor); }
};

//==============================================================================
struct PrimitiveRendererCore_DrawSquare : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	PrimitiveRendererCore::DrawSquareData	m_data;

	void create(PrimitiveRendererCore* core, const PrimitiveRendererCore::DrawSquareData& data) { m_core = core; m_data = data; }
	void execute() { m_core->DrawSquare(m_data); }
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

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * DefaultFaceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);

	m_vertexCache.reserve(DefaultFaceCount * 4);
	m_indexCache.reserve(DefaultFaceCount * 6);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::SetState(PrimitiveRendererMode mode)
{
	m_mode = mode;
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	AddVertex(from, Vector2::Zero, fromColor);
	AddVertex(to, Vector2::Zero, toColor);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::DrawSquare(const DrawSquareData& data)
{
	uint16_t i = m_vertexCache.getCount();
	m_indexCache.add(i + 0);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 3);

	AddVertex(data.pos[0], data.uv[0], data.color[0]);
	AddVertex(data.pos[1], data.uv[1], data.color[1]);
	AddVertex(data.pos[2], data.uv[2], data.color[2]);
	AddVertex(data.pos[3], data.uv[3], data.color[3]);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::flush()
{
	// サイズが足りなければ再作成
	auto* device = m_manager->GetGraphicsDevice();
	if (m_vertexBuffer->getByteCount() < m_vertexCache.GetBufferUsedByteCount())
	{
		LN_SAFE_RELEASE(m_vertexBuffer);
		m_vertexBuffer = device->CreateVertexBuffer(m_vertexCache.GetBufferUsedByteCount(), nullptr, ResourceUsage::Dynamic);
	}
	if (m_indexBuffer->getByteCount() < m_indexCache.GetBufferUsedByteCount())
	{
		LN_SAFE_RELEASE(m_indexBuffer);
		m_indexBuffer = device->CreateIndexBuffer(m_indexCache.GetBufferUsedByteCount(), nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
	}

	// 描画する
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

	{
		if (m_mode == PrimitiveRendererMode::TriangleList)
		{
			m_renderer->SetVertexDeclaration(m_manager->GetDefaultVertexDeclaration()->GetDeviceObject());
			m_renderer->SetVertexBuffer(0, m_vertexBuffer);
			m_renderer->SetIndexBuffer(m_indexBuffer);
			m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);
		}
		else if (m_mode == PrimitiveRendererMode::LineList)
		{
			m_renderer->SetVertexDeclaration(m_manager->GetDefaultVertexDeclaration()->GetDeviceObject());
			m_renderer->SetVertexBuffer(0, m_vertexBuffer);
			m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, m_vertexCache.getCount() / 2);
		}
	}

	// キャッシュクリア
	m_vertexCache.clear();
	m_indexCache.clear();
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::RequestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex)
{
	assert(vb != nullptr);
	assert(ib != nullptr);
	*outBeginVertexIndex = m_vertexCache.getCount();
	*vb = m_vertexCache.Request(vertexCount);
	*ib = m_indexCache.Request(indexCount);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::AddVertex(const Vector3& pos, const Vector2& uv, const Color& color)
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
void PrimitiveRenderFeature::DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	SetPrimitiveRendererMode(PrimitiveRendererMode::LineList);
	CheckUpdateState();
	LN_CALL_CORE_COMMAND(DrawLine, PrimitiveRendererCore_DrawLine, from, fromColor, to, toColor);
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::DrawSquare(
	const Vector3& position1, const Vector2& uv1, const Color& color1,
	const Vector3& position2, const Vector2& uv2, const Color& color2,
	const Vector3& position3, const Vector2& uv3, const Color& color3,
	const Vector3& position4, const Vector2& uv4, const Color& color4)
{
	SetPrimitiveRendererMode(PrimitiveRendererMode::TriangleList);
	CheckUpdateState();
	PrimitiveRendererCore::DrawSquareData data;
	data.pos[0] = position1; data.uv[0] = uv1; data.color[0] = color1;
	data.pos[1] = position2; data.uv[1] = uv2; data.color[1] = color2;
	data.pos[2] = position3; data.uv[2] = uv3; data.color[2] = color3;
	data.pos[3] = position4; data.uv[3] = uv4; data.color[3] = color4;
	LN_CALL_CORE_COMMAND(DrawSquare, PrimitiveRendererCore_DrawSquare, data);
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::DrawRectangle(const Rect& rect)
{
	float l = rect.GetLeft();
	float t = rect.getTop();
	float r = rect.getRight();
	float b = rect.GetBottom();
	DrawSquare(
		Vector3(l, t, 0), Vector2(0, 0), Color::White,
		Vector3(l, b, 0), Vector2(0, 1), Color::White,
		Vector3(r, t, 0), Vector2(1, 0), Color::White,
		Vector3(r, b, 0), Vector2(1, 1), Color::White);
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::flush()
{
	if (m_flushRequested)
	{
        if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded) {
            m_manager->GetPrimaryRenderingCommandList()->AddCommand<PrimitiveRendererCore_FlushCommand>(m_core);
        }
        else {
            m_core->flush();
        }
        m_flushRequested = false;
	}
}

//------------------------------------------------------------------------------
bool PrimitiveRenderFeature::IsStandaloneShader() const { return false; }
void PrimitiveRenderFeature::OnActivated() { m_stateModified = true; }
void PrimitiveRenderFeature::OnDeactivated() { flush(); }

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::SetPrimitiveRendererMode(PrimitiveRendererMode mode)
{
	if (mode != m_mode)
	{
		m_mode = mode;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void PrimitiveRenderFeature::CheckUpdateState()
{
	if (m_stateModified)
	{
		flush();

		LN_CALL_CORE_COMMAND(SetState, PrimitiveRendererCore_SetStateCommand, m_mode);
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
	auto* device = m_manager->GetGraphicsDevice();

	Vertex vertices[4] =
	{
		{ Vector3(-1,  1, 0), Vector2(0, 0), Vector3::Zero, Color::White },
		{ Vector3(-1, -1, 0), Vector2(0, 1), Vector3::Zero, Color::White },
		{ Vector3( 1,  1, 0), Vector2(1, 0), Vector3::Zero, Color::White },
		{ Vector3( 1, -1, 0), Vector2(1, 1), Vector3::Zero, Color::White },
	};
	m_vertexBuffer.attach(device->CreateVertexBuffer(sizeof(vertices), vertices, ResourceUsage::Static), false);

	m_commonMaterial = RefPtr<Material>::makeRef();
	m_commonMaterial->initialize();
}

//------------------------------------------------------------------------------
Material* BlitRenderer::GetCommonMaterial() const
{
	return m_commonMaterial;
}

//------------------------------------------------------------------------------
void BlitRenderer::Blit()
{
	auto* _this = this;
	LN_ENQUEUE_RENDER_COMMAND_1(
		Blit, m_manager,
		BlitRenderer*, _this,
		{
			_this->BlitImpl();
		});
}

//------------------------------------------------------------------------------
void BlitRenderer::BlitImpl()
{
	auto* device = m_manager->GetGraphicsDevice();
	auto* renderer = device->GetRenderer();
	renderer->SetVertexDeclaration(m_manager->GetDefaultVertexDeclaration()->GetDeviceObject());
	renderer->SetVertexBuffer(0, m_vertexBuffer);
	renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
}

//------------------------------------------------------------------------------
bool BlitRenderer::IsStandaloneShader() const { return false; }
void BlitRenderer::flush() {}
void BlitRenderer::OnActivated() {}
void BlitRenderer::OnDeactivated() {}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
