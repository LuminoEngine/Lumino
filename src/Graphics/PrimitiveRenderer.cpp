
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Shader.h>
#include "Device/GraphicsDriverInterface.h"
#include "RendererImpl.h"
#include "RenderingCommand.h"
#include "PrimitiveRenderer.h"

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
	PrimitiveRendererCore*	m_core;
	Matrix					m_transform;
	Matrix					m_viewProj;
	Size					m_viewPixelSize;
	bool					m_useInternalShader;
	PrimitiveRendererMode	m_mode;
	Driver::IShader*		m_userShader;
	Driver::ITexture*		m_foreTexture;
	void Create(PrimitiveRendererCore* core, const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode, Driver::IShader* userShader, Driver::ITexture* foreTexture)
	{
		m_core = core;
		m_transform = world;
		m_viewProj = viewProj;
		m_viewPixelSize = viewPixelSize;
		m_useInternalShader = useInternalShader;
		m_mode = mode;
		m_userShader = userShader;
		MarkGC(m_userShader);
		m_foreTexture = foreTexture;
		MarkGC(m_foreTexture);
	}
	void Execute()
	{
		m_core->SetState(m_transform, m_viewProj, m_viewPixelSize, m_useInternalShader, m_mode, m_userShader, m_foreTexture);
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

	void Create(PrimitiveRendererCore* core, const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
	{
		m_core = core;
		m_from = from;
		m_fromColor = fromColor;
		m_to = to;
		m_toColor = toColor;
	}
	void Execute() { m_core->DrawLine(m_from, m_fromColor, m_to, m_toColor); }
};

//==============================================================================
struct PrimitiveRendererCore_DrawSquare : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	PrimitiveRendererCore::DrawSquareData	m_data;

	void Create(PrimitiveRendererCore* core, const PrimitiveRendererCore::DrawSquareData& data) { m_core = core; m_data = data; }
	void Execute() { m_core->DrawSquare(m_data); }
};

//==============================================================================
struct PrimitiveRendererCore_Blt : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	Driver::ITexture* m_source;
	Driver::ITexture* m_dest;
	Matrix	m_transform;
	Driver::IShader* m_shader;

	void Create(PrimitiveRendererCore* core, Driver::ITexture* source, Driver::ITexture* dest, const Matrix& transform, Driver::IShader* shader)
	{
		m_core = core;
		m_source = source;
		MarkGC(m_source);
		m_dest = dest;
		MarkGC(m_dest);
		m_transform = transform;
		m_shader = shader;
		MarkGC(m_shader);
	}
	void Execute() { m_core->Blt(m_source, m_dest, m_transform, m_shader); }
};

//==============================================================================
struct PrimitiveRendererCore_FlushCommand : public RenderingCommand
{
	PrimitiveRendererCore* m_core;
	void Create(PrimitiveRendererCore* core) { m_core = core; }
	void Execute() { m_core->Flush(); }
};

//==============================================================================
// PrimitiveRendererCore
//==============================================================================
//	
//static const byte_t g_PrimitiveRenderer_fx_Data[] =
//{
//#include "Resource/PrimitiveRenderer.fx.h"
//};
//static const size_t g_PrimitiveRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_PrimitiveRenderer_fx_Data);
//
//static const byte_t g_PrimitiveRendererForBlt_fx_Data[] =
//{
//#include "Resource/PrimitiveRendererForBlt.fx.h"
//};
//static const size_t g_PrimitiveRendererForBlt_fx_Len = LN_ARRAY_SIZE_OF(g_PrimitiveRendererForBlt_fx_Data);

//------------------------------------------------------------------------------
PrimitiveRendererCore::PrimitiveRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_vertexBufferForBlt(nullptr)
	, m_vertexCacheUsed(0)
	, m_foreTexture(nullptr)
	, m_mode(PrimitiveRendererMode::TriangleList)
	, m_userShader(nullptr)
	, m_useInternalShader(true)
{
}

//------------------------------------------------------------------------------
PrimitiveRendererCore::~PrimitiveRendererCore()
{
	LN_SAFE_RELEASE(m_foreTexture);
	LN_SAFE_RELEASE(m_userShader);
	LN_SAFE_RELEASE(m_shader.shader);
	//LN_SAFE_RELEASE(m_shaderForBlt.shader);
	LN_SAFE_RELEASE(m_vertexBufferForBlt);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	const int DefaultFaceCount = 2048;

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();
	m_vertexDeclaration.Attach(device->CreateVertexDeclaration(Vertex::Elements(), Vertex::ElementCount));
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * DefaultFaceCount * 4, nullptr, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(DefaultFaceCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

	m_vertexCache.Resize(DefaultFaceCount * 4);
	m_vertexCacheUsed = 0;
	m_indexCache.Reserve(DefaultFaceCount * 6);

	//-----------------------------------------------------
	// シェーダ (DrawingContext3D)
	static const byte_t code_PrimitiveRenderer_fx_Data[] =
	{
#include "Resource/PrimitiveRenderer.fx.h"
	};
	static const size_t code_PrimitiveRenderer_fx_Len = LN_ARRAY_SIZE_OF(code_PrimitiveRenderer_fx_Data);

	ShaderCompileResult r;
	m_shader.shader = device->CreateShader(code_PrimitiveRenderer_fx_Data, code_PrimitiveRenderer_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
	m_shader.varPixelStep = m_shader.shader->GetVariableByName(_T("g_pixelStep"));

	// Blt 用頂点バッファ
	Vertex tv[4];
	tv[0].position.Set(-1,  1, 0); tv[0].color = Color::White; tv[0].uv.Set(0, 0);	// 左上
	tv[1].position.Set(-1, -1, 0); tv[1].color = Color::White; tv[1].uv.Set(0, 1);	// 左下
	tv[2].position.Set( 1,  1, 0); tv[2].color = Color::White; tv[2].uv.Set(1, 0);	// 右上
	tv[3].position.Set( 1, -1, 0); tv[3].color = Color::White; tv[3].uv.Set(1, 1);	// 右下
	m_vertexDeclarationForBlt.Attach(device->CreateVertexDeclaration(Vertex::Elements(), Vertex::ElementCount));
	m_vertexBufferForBlt = device->CreateVertexBuffer(sizeof(Vertex) * 4, tv, DeviceResourceUsage_Static);

	// Blt 用デフォルトシェーダ
	//m_shaderForBlt.shader = device->CreateShader(g_PrimitiveRendererForBlt_fx_Data, g_PrimitiveRendererForBlt_fx_Len, &r);
	//LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	//m_shaderForBlt.technique = m_shaderForBlt.shader->GetTechnique(0);
	//m_shaderForBlt.pass = m_shaderForBlt.technique->GetPass(0);
	//m_shaderForBlt.varTexture = m_shaderForBlt.shader->GetVariableByName(_T("g_texture"));
	//m_shaderForBlt.varPixelStep = m_shaderForBlt.shader->GetVariableByName(_T("g_pixelStep"));

	m_vertexStride = sizeof(Vertex);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize, bool useInternalShader, PrimitiveRendererMode mode, Driver::IShader* userShader, Driver::ITexture* texture)
{
	float vw = (viewPixelSize.width != 0.0) ? (0.5f / viewPixelSize.width) : 0.0f;
	float vh = (viewPixelSize.height != 0.0) ? (0.5f / viewPixelSize.height) : 0.0f;

	// シェーダは通常用とBlt用で共用したい。これらのパラメータは一度メンバ変数においておき、本当に必要になったときにシェーダに設定する
	m_worldMatrix = world;
	m_viewProjMatrix = viewProj;
	m_pixelStep = Vector4(vw, vh, 0, 0);

	//m_shader.varWorldMatrix->SetMatrix(world);
	//m_shader.varViewProjMatrix->SetMatrix(viewProj);
	//m_shader.varPixelStep->SetVector(Vector4(vw, vh, 0, 0));
	//m_shaderForBlt.varPixelStep->SetVector(Vector4(vw, vh, 0, 0));
	m_useInternalShader = useInternalShader;
	m_mode = mode;
	LN_REFOBJ_SET(m_userShader, userShader);
	LN_REFOBJ_SET(m_foreTexture, texture);
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
	uint16_t i = GetCurrentVertexCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);
	m_indexCache.Add(i + 3);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);

	AddVertex(data.pos[0], data.uv[0], data.color[0]);
	AddVertex(data.pos[1], data.uv[1], data.color[1]);
	AddVertex(data.pos[2], data.uv[2], data.color[2]);
	AddVertex(data.pos[3], data.uv[3], data.color[3]);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::Blt(Driver::ITexture* source, Driver::ITexture* dest, const Matrix& transform, Driver::IShader* shader)
{
	// Flush 済みであることが前提。

	// 設定を保存する
	Driver::ITexture* oldRT = m_renderer->GetRenderTarget(0);
	m_renderer->SetRenderTarget(0, dest);

	// シェーダが指定されていなければデフォルトのを使う
	if (shader == nullptr)
	{
		if (m_shader.varPixelStep != nullptr)	// Dx9 のみ
		{
			m_shader.varPixelStep->SetVector(m_pixelStep);
		}
		m_shader.varWorldMatrix->SetMatrix(transform);
		m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);
		m_shader.varTexture->SetTexture(source);
		//m_shaderForBlt.varTexture->SetTexture(source);
		shader = m_shader.shader;//m_shaderForBlt.shader;
	}

	// パスごとに描画
	int techCount = shader->GetTechniqueCount();
	for (int iTech = 0; iTech < techCount; ++iTech)
	{
		Driver::IShaderTechnique* tech = shader->GetTechnique(iTech);
		int passCount = tech->GetPassCount();
		for (int iPass = 0; iPass < passCount; ++iPass)
		{
			tech->GetPass(iPass)->Apply();
			m_renderer->SetVertexDeclaration(m_vertexDeclaration);
			m_renderer->SetVertexDeclaration(m_vertexDeclarationForBlt);
			m_renderer->SetVertexBuffer(0, m_vertexBufferForBlt);
			m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
		}
	}

	// 設定を元に戻す
	m_renderer->SetRenderTarget(0, oldRT);
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::Flush()
{
	// 描画する
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetConstData(), m_vertexCacheUsed);
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());

	// ユーザーシェーダを使う場合
	if (m_userShader != nullptr)
	{
		int techCount = m_userShader->GetTechniqueCount();
		for (int iTech = 0; iTech < techCount; ++iTech)
		{
			Driver::IShaderTechnique* tech = m_userShader->GetTechnique(iTech);
			int passCount = tech->GetPassCount();
			for (int iPass = 0; iPass < passCount; ++iPass)
			{
				tech->GetPass(iPass)->Apply();

				if (m_mode == PrimitiveRendererMode::TriangleList)
				{
					m_renderer->SetVertexDeclaration(m_vertexDeclaration);
					m_renderer->SetVertexBuffer(0, m_vertexBuffer);
					m_renderer->SetIndexBuffer(m_indexBuffer);
					m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
				}
				else if (m_mode == PrimitiveRendererMode::LineList)
				{
					m_renderer->SetVertexDeclaration(m_vertexDeclaration);
					m_renderer->SetVertexBuffer(0, m_vertexBuffer);
					m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, GetCurrentVertexCount() / 2);
				}
			}
		}
	}
	// ユーザーシェーダを使わない場合
	else
	{
		if (m_useInternalShader)
		{
			Driver::ITexture* srcTexture = m_foreTexture;
			if (srcTexture == nullptr) {
				srcTexture = m_manager->GetDummyTexture();
			}

			if (m_shader.varPixelStep != nullptr)	// これは DX9 だけ
			{
				m_shader.varPixelStep->SetVector(m_pixelStep);
			}

			m_shader.varWorldMatrix->SetMatrix(m_worldMatrix);
			m_shader.varViewProjMatrix->SetMatrix(m_viewProjMatrix);
			m_shader.varTexture->SetTexture(srcTexture);
			m_shader.pass->Apply();
		}

		if (m_mode == PrimitiveRendererMode::TriangleList)
		{
			m_renderer->SetVertexDeclaration(m_vertexDeclaration);
			m_renderer->SetVertexBuffer(0, m_vertexBuffer);
			m_renderer->SetIndexBuffer(m_indexBuffer);
			m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
		}
		else if (m_mode == PrimitiveRendererMode::LineList)
		{
			m_renderer->SetVertexDeclaration(m_vertexDeclaration);
			m_renderer->SetVertexBuffer(0, m_vertexBuffer);
			m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, GetCurrentVertexCount() / 2);
		}
	}

	// キャッシュクリア
	m_vertexCacheUsed = 0;
	m_indexCache.Clear();
}

//------------------------------------------------------------------------------
void PrimitiveRendererCore::AddVertex(const Vector3& pos, const Vector2& uv, const Color& color)
{
	uint32_t size = sizeof(Vertex);
	if (m_vertexCacheUsed + size >= m_vertexCache.GetSize()) {
		m_vertexCache.Resize(m_vertexCache.GetSize() * 2);
	}

	byte_t* data = m_vertexCache.GetData();

	Vertex* v = (Vertex*)&data[m_vertexCacheUsed];
	v->position = pos;
	v->uv = uv;
	v->color = color;

	m_vertexCacheUsed += size;
}

//==============================================================================
// PrimitiveRenderer
//==============================================================================

//------------------------------------------------------------------------------
PrimitiveRenderer::PrimitiveRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_userShader(nullptr)
	, m_texture(nullptr)
	, m_useInternalShader(true)
	, m_stateModified(false)
	, m_flushRequested(false)
{
}

//------------------------------------------------------------------------------
PrimitiveRenderer::~PrimitiveRenderer()
{
	LN_SAFE_RELEASE(m_userShader);
	LN_SAFE_RELEASE(m_texture);
	LN_SAFE_RELEASE(m_core);
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;

	m_core = LN_NEW PrimitiveRendererCore();
	m_core->Initialize(m_manager);
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetTransform(const Matrix& matrix)
{
	m_transform = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetViewProjMatrix(const Matrix& matrix)
{
	m_viewProj = matrix;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetViewPixelSize(const Size& size)
{
	m_viewPixelSize = size;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetUseInternalShader(bool useInternalShader)
{
	m_useInternalShader = useInternalShader;
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetUserShader(Shader* shader)
{
	LN_REFOBJ_SET(m_userShader, shader);
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetTexture(Texture* texture)
{
	LN_REFOBJ_SET(m_texture, texture);
	m_stateModified = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::DrawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	SetPrimitiveRendererMode(PrimitiveRendererMode::LineList);
	CheckUpdateState();
	LN_CALL_CORE_COMMAND(DrawLine, PrimitiveRendererCore_DrawLine, from, fromColor, to, toColor);
	m_flushRequested = true;
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::DrawSquare(
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
void PrimitiveRenderer::DrawRectangle(const RectF& rect)
{
	float l = rect.GetLeft();
	float t = rect.GetTop();
	float r = rect.GetRight();
	float b = rect.GetBottom();
	DrawSquare(
		Vector3(l, t, 0), Vector2(0, 0), Color::White,
		Vector3(l, b, 0), Vector2(0, 1), Color::White,
		Vector3(r, t, 0), Vector2(1, 0), Color::White,
		Vector3(r, b, 0), Vector2(1, 1), Color::White);
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::Blt(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader)
{
	m_stateModified = true;	// Blt ではレンダーターゲットを切り替えたりするので、Flush しておく必要がある。
	m_flushRequested = true;	// Blt ではレンダーターゲットを切り替えたりするので、Flush しておく必要がある。
	SetPrimitiveRendererMode(PrimitiveRendererMode::TriangleList);
	CheckUpdateState();	// TODO: Blt に限っては必要ないかも？

	if (shader)
	{
		shader->TryCommitChanges();
	}
	LN_CALL_CORE_COMMAND(Blt, PrimitiveRendererCore_Blt,
		(source != nullptr) ? source->GetDeviceObject() : nullptr,
		(dest != nullptr) ? dest->GetDeviceObject() : nullptr,
		transform,
		(shader != nullptr) ? shader->m_deviceObj : nullptr);
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::Flush()
{
	if (m_flushRequested)
	{
		//LN_CALL_CORE_COMMAND(Flush, PrimitiveRendererCore_FlushCommand);
        if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded) {
            m_manager->GetPrimaryRenderingCommandList()->AddCommand<PrimitiveRendererCore_FlushCommand>(m_core);
        }
        else {
            m_core->Flush();
        }
        m_flushRequested = false;
	}
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::SetPrimitiveRendererMode(PrimitiveRendererMode mode)
{
	if (mode != m_mode)
	{
		m_mode = mode;
		m_stateModified = true;
	}
}

//------------------------------------------------------------------------------
void PrimitiveRenderer::CheckUpdateState()
{
	if (m_stateModified)
	{
		Flush();

		if (m_userShader != nullptr)
		{
			m_userShader->TryCommitChanges();
		}
		LN_CALL_CORE_COMMAND(SetState, PrimitiveRendererCore_SetStateCommand, m_transform, m_viewProj, m_viewPixelSize, m_useInternalShader, m_mode,
			(m_userShader != nullptr) ? m_userShader->m_deviceObj : nullptr,
			(m_texture != nullptr) ? m_texture->GetDeviceObject() : nullptr);
		m_stateModified = false;
	}
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
