/*
	動的頂点バッファのNoOverwriteとDiscard
	http://blogs.msdn.com/b/ito/archive/2011/05/22/no-overwrite-or-discard.aspx
*/
#include "Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include "GeometryRendererImpl.h"
#include "GraphicsHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// GeometryRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRenderer* GeometryRenderer::Create(GraphicsManager* manager)
{
	return LN_NEW GeometryRenderer(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRenderer::GeometryRenderer(GraphicsManager* manager)
	: m_manager(NULL)
	, m_internal(NULL)
{
	LN_REFOBJ_SET(m_manager, manager);
	m_internal = LN_NEW GeometryRendererCore(m_manager, m_manager->GetGraphicsDevice());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRenderer::~GeometryRenderer()
{
	LN_SAFE_RELEASE(m_internal);
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::BeginPass()
{
	LN_CALL_COMMAND(BeginPass, GeometryRendererCore::BeginPassCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::EndPass()
{
	LN_CALL_COMMAND(EndPass, GeometryRendererCore::EndPassCommand);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::SetTransform(const Matrix& matrix)
{
	LN_CALL_COMMAND(SetTransform, GeometryRendererCore::SetTransformCommand, matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::SetViewProjTransform(const Matrix& matrix)
{
	LN_CALL_COMMAND(SetViewProjTransform, GeometryRendererCore::SetViewProjTransformCommand, matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::SetTexture(Driver::ITexture* texture)
{
	LN_CALL_COMMAND(SetTexture, GeometryRendererCore::SetTextureCommand, texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	LN_CALL_COMMAND(DrawLine, GeometryRendererCore::DrawLineCommand, from, to, fromColor, toColor);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::DrawSquare(
	float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
	float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
	float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
	float x4, float y4, float z4, float u4, float v4, const ColorF& c4)
{
	if (m_manager->GetRenderingType() == RenderingType::Deferred) {
		GeometryRendererCore::Vertex _v1, _v2, _v3, _v4;
		_v1.Position.Set(x1, y1, z1); _v1.TexUV.Set(u1, v1); _v1.Color = c1;
		_v2.Position.Set(x2, y2, z2); _v2.TexUV.Set(u2, v2); _v2.Color = c2;
		_v3.Position.Set(x3, y3, z3); _v3.TexUV.Set(u3, v3); _v3.Color = c3;
		_v4.Position.Set(x4, y4, z4); _v4.TexUV.Set(u4, v4); _v4.Color = c4;

		m_manager->GetPrimaryRenderingCommandList()->AddCommand<GeometryRendererCore::DrawSquareCommand>(m_internal, _v1, _v2, _v3, _v4);
			//Vector3, Vector2(), c1,
			//Vector3(x2, y2, z2), Vector2(u2, v2), c2,
			//Vector3(x3, y3, z3), Vector2(u3, v3), c3,
			//Vector3(x4, y4, z4), Vector2(u4, v4), c4);
		//GeometryRendererCore::DrawSquareCommand::AddCommand(
		//	m_manager->GetPrimaryRenderingCommandList(), m_core,
		//	Vector3(x1, y1, z1), Vector2(u1, v1), c1,
		//	Vector3(x2, y2, z2), Vector2(u2, v2), c2,
		//	Vector3(x3, y3, z3), Vector2(u3, v3), c3,
		//	Vector3(x4, y4, z4), Vector2(u4, v4), c4);
	}
	else {
		m_internal->DrawSquare(
			x1, y1, z1, u1, v1, c1,
			x2, y2, z2, u2, v2, c2,
			x3, y3, z3, u3, v3, c3,
			x4, y4, z4, u4, v4, c4);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRenderer::DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color)
{
	LN_CALL_COMMAND(DrawRect, GeometryRendererCore::DrawRectCommand, destRect, texUVRect, color);
}


//=============================================================================
// GeometryRendererCore
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRendererCore::GeometryRendererCore(GraphicsManager* manager, Driver::IGraphicsDevice* device)
	: m_manager(manager)
	, m_currentShaderPass(NULL)
{
	m_device = device;
	m_renderer = device->GetRenderer();
	CreateInternal();
	m_manager->AddResourceObject(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRendererCore::~GeometryRendererCore()
{
	m_manager->RemoveResourceObject(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		m_device.SafeRelease();
		m_renderer.SafeRelease();
		m_vertexBuffer.SafeRelease();
		m_indexBuffer.SafeRelease();
		m_shaderParam.Shader.SafeRelease();
		m_dummyTexture.SafeRelease();
	}
	else
	{
		CreateInternal();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::CreateInternal()
{
	//-----------------------------------------------------
	// 各描画オブジェクト用の頂点バッファ

	m_vertexBuffer.Attach(m_device->CreateVertexBuffer(Vertex::GetLayout(), 3, 256, NULL, DeviceResourceUsage_Dynamic), false);
	m_indexBuffer.Attach(m_device->CreateIndexBuffer(256, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic), false);

	//-----------------------------------------------------
	// シェーダ
	const unsigned char code[] =
	{
#include "Resource/GeometryRenderer.fx.h"
	};
	const size_t codeLen = LN_ARRAY_SIZE_OF(code);

	ShaderCompileResult result;
	m_shaderParam.Shader.Attach(m_device->CreateShader(code, codeLen, &result), false);
	if (result.Level != ShaderCompileResultLevel_Success) {
		printf(result.Message);	// TODO:仮
	}

	m_shaderParam.varWorldMatrix = m_shaderParam.Shader->GetVariableByName(_T("g_worldMatrix"));
	m_shaderParam.varViewProjMatrix = m_shaderParam.Shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shaderParam.varTexture = m_shaderParam.Shader->GetVariableByName(_T("g_texture"));
	m_shaderParam.techMainDraw = m_shaderParam.Shader->GetTechnique(0);
	m_shaderParam.passP0 = m_shaderParam.techMainDraw->GetPass(0);

	//-----------------------------------------------------
	// ダミーテクスチャ

	m_dummyTexture.Attach(m_device->CreateTexture(Size(32, 32), 1, TextureFormat_R8G8B8A8, NULL), false);
	Driver::IGraphicsDevice::ScopedLockContext lock(m_device);
	BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();

	//-----------------------------------------------------
	// デフォルト値

	SetTransform(Matrix::Identity);
	SetViewProjTransform(Matrix::Identity);
	SetTexture(NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::BeginPass()
{
	m_currentShaderPass = m_shaderParam.passP0;
	m_currentShaderPass->Apply();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::EndPass()
{
	m_currentShaderPass = NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::SetTransform(const Matrix& matrix)
{
	m_shaderParam.varWorldMatrix->SetMatrix(matrix);
	if (m_currentShaderPass) {
		m_currentShaderPass->Apply();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::SetViewProjTransform(const Matrix& matrix)
{
	m_shaderParam.varViewProjMatrix->SetMatrix(matrix);
	if (m_currentShaderPass) {
		m_currentShaderPass->Apply();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::SetTexture(Driver::ITexture* texture)
{
	if (texture) {
		m_shaderParam.varTexture->SetTexture(texture);
	}
	else {
		m_shaderParam.varTexture->SetTexture(m_dummyTexture);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor)
{
	Vertex* v = (Vertex*)m_vertexBuffer->Lock();
	v[0].Position = from;
	v[0].Color = fromColor;
	v[0].TexUV = Vector2::Zero;
	v[1].Position = to;
	v[1].Color = toColor;
	v[1].TexUV = Vector2::Zero;
	m_vertexBuffer->Unlock();

	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_renderer->DrawPrimitive(PrimitiveType_LineList, 0, 1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::DrawSquare(
	float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
	float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
	float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
	float x4, float y4, float z4, float u4, float v4, const ColorF& c4)
{
	Vertex* v = (Vertex*)m_vertexBuffer->Lock();
	v[0].Position.Set(x1, y1, z1);
	v[0].Color = c1;
	v[0].TexUV.Set(u1, v1);
	v[1].Position.Set(x2, y2, z2);
	v[1].Color = c2;
	v[1].TexUV.Set(u2, v2);
	v[2].Position.Set(x3, y3, z3);
	v[2].Color = c3;
	v[2].TexUV.Set(u3, v3);
	v[3].Position.Set(x4, y4, z4);
	v[3].Color = c4;
	v[3].TexUV.Set(u4, v4);
	m_vertexBuffer->Unlock();

	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GeometryRendererCore::DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color)
{
	return DrawSquare(
        (float)destRect.X,					(float)destRect.Y,						0.0f,	(float)texUVRect.X,						(float)texUVRect.Y,						color,	
        (float)destRect.X + destRect.Width,	(float)destRect.Y,						0.0f,	(float)texUVRect.X + texUVRect.Width,	(float)texUVRect.Y,						color,	
        (float)destRect.X,					(float)destRect.Y + destRect.Height,	0.0f,	(float)texUVRect.X,						(float)texUVRect.Y + texUVRect.Height,	color,	
		(float)destRect.X + destRect.Width, (float)destRect.Y + destRect.Height,	0.0f,	(float)texUVRect.X + texUVRect.Width,	(float)texUVRect.Y + texUVRect.Height,	color);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
