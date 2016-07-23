/*
	枠の描画は GeometryRenderer が行うパス描画とは処理内容が大きく異なる。(入力は似ているが)
	そのため別クラスに分けた。処理の振り分けは GraphicsContext が担当する。
*/
#include "Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include "Device/GraphicsDriverInterface.h"
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "FrameRectRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// FrameRectRendererCore
//==============================================================================

//------------------------------------------------------------------------------
FrameRectRendererCore::FrameRectRendererCore()
	: m_manager(nullptr)
	, m_renderer(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

//------------------------------------------------------------------------------
FrameRectRendererCore::~FrameRectRendererCore()
{
	LN_SAFE_RELEASE(m_shader.shader);
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::Initialize(GraphicsManager* manager)
{
	m_manager = manager;

	auto* device = m_manager->GetGraphicsDevice();
	m_renderer = device->GetRenderer();

	//-----------------------------------------------------
	// Vertex and Index buffers
	const int DefaultFaceCount = 512;
	RequestBuffers(512);
	m_vertexCache.Reserve(DefaultFaceCount * 4);
	m_indexCache.Reserve(DefaultFaceCount * 6);

	//-----------------------------------------------------
	// Shader
	static const byte_t g_Painter_fx_Data[] =
	{
#include "Resource/Painter.fx.h"
	};
	static const size_t g_Painter_fx_Len = LN_ARRAY_SIZE_OF(g_Painter_fx_Data);

	ShaderCompileResult r;
	m_shader.shader = device->CreateShader(g_Painter_fx_Data, g_Painter_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTone = m_shader.shader->GetVariableByName(_T("g_tone"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
	m_shader.varGlyphMaskSampler = m_shader.shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varViewportSize = m_shader.shader->GetVariableByName(_T("g_viewportSize"));

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::SetState(const FrameRectRendererState& state)
{
	m_state = state;
	m_shader.varWorldMatrix->SetMatrix(m_state.worldTransform);
	m_shader.varViewProjMatrix->SetMatrix(m_state.viewProjTransform);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::Draw(const RectF& rect)
{
	if (rect.IsEmpty()) return;

	// TODO: とりあえず今は fill だけ

	FrameTextureBrush* brush = m_state.brush;
	Driver::ITexture* srcTexture = brush->GetTexture()->GetDeviceObject();
	if (srcTexture == nullptr) srcTexture = m_manager->GetDummyDeviceTexture();


	// 枠
	{
		//Rect& srcRect = *((Rect*)brush.SourceRect);
		PutFrameRectangle(rect, brush->GetThickness(), srcTexture, brush->GetSourceRect());
	}

	// Inner
	{
		const Rect& srcRect = brush->GetInnerAreaSourceRect();
		SizeF texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

		if (brush->GetWrapMode() == BrushWrapMode_Stretch)
		{
			RectF tr = rect;
			tr.Inflate((float)-brush->GetThickness(), (float)-brush->GetThickness());
			//PutRectangleStretch(tr, uvSrcRect);
		}
		else
		{
			LN_THROW(0, NotImplementedException);
		}
	}


	// Flush
	{

		if (m_indexCache.GetCount() == 0) { return; }

		const Size& viewPixelSize = m_renderer->GetRenderTarget(0)->GetSize();
		m_shader.varViewportSize->SetVector(Vector4((float)viewPixelSize.width, (float)viewPixelSize.height, 0, 0));

		// 描画する
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
		m_shader.varTone->SetVector(ToneF());
		m_shader.varTexture->SetTexture(srcTexture);
		m_shader.varGlyphMaskSampler->SetTexture(m_manager->GetDummyDeviceTexture());
		m_shader.pass->Apply();
		m_renderer->SetVertexDeclaration(m_vertexDeclaration);
		m_renderer->SetVertexBuffer(0, m_vertexBuffer);
		m_renderer->SetIndexBuffer(m_indexBuffer);
		m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);

		// キャッシュクリア
		m_vertexCache.Clear();
		m_indexCache.Clear();
	}
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::RequestBuffers(int faceCount)
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);

	auto* device = m_manager->GetGraphicsDevice();
	m_vertexDeclaration.Attach(device->CreateVertexDeclaration(Vertex::Elements(), Vertex::ElementCount), false);
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * faceCount * 4, nullptr, DeviceResourceUsage_Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(faceCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleStretch(const RectF& rect, const RectF& srcUVRect)
{
	if (rect.IsEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float uvWidth = srcUVRect.width;
	float uvHeight = srcUVRect.height;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	Vertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 2);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 2);	// 右下
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleTiling(const RectF& rect, const Rect& srcPixelRect, const RectF& srcUVRect, Driver::ITexture* srcTexture)
{
	if (rect.IsEmpty()) return;		// 矩形がつぶれているので書く必要はない

	float uvX = srcUVRect.x;
	float uvY = srcUVRect.y;
	float uvWidth = srcUVRect.width;
	float uvHeight = srcUVRect.height;
	float blockCountW = (rect.width / srcPixelRect.width);		// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	float blockCountH = (rect.height / srcPixelRect.height);	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

	float lu = uvX;
	float tv = uvY;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	Vertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f, 1.0f);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f, 1.0f + blockCountH);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// 右下
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutFrameRectangle(const RectF& rect, float frameWidth, Driver::ITexture* srcTexture, Rect srcRect)
{
	if (srcRect.IsEmpty()) return;
	assert(srcTexture != nullptr);

	if (srcRect.width == INT_MAX) {
		srcRect.width = srcTexture->GetSize().width;
	}
	if (srcRect.height == INT_MAX) {
		srcRect.height = srcTexture->GetSize().height;
	}

	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);


	float frameWidthH = frameWidth;
	float frameWidthV = frameWidth;
	float uvFrameWidthH = frameWidth * texSize.width;
	float uvFrameWidthV = frameWidth * texSize.height;
	int frameWidthHI = (int)frameWidth;	// 型変換回数を減らすため、あらかじめ int 化しておく
	int frameWidthVI = (int)frameWidth;

	// 横幅が小さいため、枠幅も狭めなければならない
	if (rect.width < frameWidthH * 2)
	{
		float ratio = rect.width / (frameWidthH * 2);	// 元の幅から何 % になるか
		frameWidthH *= ratio;
		uvFrameWidthH *= ratio;
		frameWidthHI = (int)ceil(ratio * frameWidthHI);
	}
	// 縦幅が小さいため、枠幅も狭めなければならない
	if (rect.height < frameWidthV * 2)
	{
		float ratio = rect.height / (frameWidthV * 2);	// 元の幅から何 % になるか
		frameWidthV *= ratio;
		uvFrameWidthV *= ratio;
		frameWidthVI = (int)ceil(ratio * frameWidthVI);
	}

	RectF outerRect = rect;
	RectF innerRect(outerRect.x + frameWidthH, outerRect.y + frameWidthV, outerRect.width - frameWidthH * 2, outerRect.height - frameWidthV * 2);
	RectF outerUVRect = uvSrcRect;
	RectF innerUVRect(outerUVRect.x + uvFrameWidthH, outerUVRect.y + uvFrameWidthV, outerUVRect.width - uvFrameWidthH * 2, outerUVRect.height - uvFrameWidthV * 2);
	Rect  outerSrcRect = srcRect;
	Rect  innerSrcRect(outerSrcRect.x + frameWidthHI, outerSrcRect.y + frameWidthVI, outerSrcRect.width - frameWidthHI * 2, outerSrcRect.height - frameWidthVI * 2);

	// 左上	■□□
	//		□　□
	//		□□□
	//PutRectangleTiling(
	//	RectF(outerRect.GetLeft(), outerRect.GetTop(), frameWidthH, frameWidthV),
	//	Rect(outerSrcRect.GetLeft(), outerSrcRect.GetTop(), frameWidthHI, frameWidthVI),
	//	RectF(outerUVRect.GetLeft(), outerUVRect.GetTop(), uvFrameWidthH, uvFrameWidthV),
	//	srcTexture);

	// 上	□■□
	//		□　□
	//		□□□
	PutRectangleTiling(
		RectF(innerRect.GetLeft(), outerRect.GetTop(), innerRect.width, frameWidth),
		Rect(innerSrcRect.GetLeft(), outerSrcRect.GetTop(), innerSrcRect.width, frameWidthVI),
		RectF(innerUVRect.GetLeft(), outerUVRect.GetTop(), innerUVRect.width, uvFrameWidthV),
		srcTexture);

	// 右上	□□■
	//		□　□
	//		□□□
	//PutRectangleTiling(
	//	RectF(innerRect.GetRight(), outerRect.GetTop(), frameWidthH, frameWidthV),
	//	Rect(innerSrcRect.GetRight(), outerSrcRect.GetTop(), frameWidthHI, frameWidthVI),
	//	RectF(innerUVRect.GetRight(), outerUVRect.GetTop(), uvFrameWidthH, uvFrameWidthV),
	//	srcTexture);

	// 右	□□□
	//		□　■
	//		□□□
	PutRectangleTiling(
		RectF(innerRect.GetRight(), innerRect.GetTop(), frameWidthH, innerRect.height),
		Rect(innerSrcRect.GetRight(), innerSrcRect.GetTop(), frameWidthHI, innerSrcRect.height),
		RectF(innerUVRect.GetRight(), innerUVRect.GetTop(), uvFrameWidthH, innerUVRect.height),
		srcTexture);

	// 右下	□□□
	//		□　□
	//		□□■
	//PutRectangleTiling(
	//	RectF(innerRect.GetRight(), innerRect.GetBottom(), frameWidthH, frameWidthV),
	//	Rect(innerSrcRect.GetRight(), innerSrcRect.GetBottom(), frameWidthHI, frameWidthVI),
	//	RectF(innerUVRect.GetRight(), innerUVRect.GetBottom(), uvFrameWidthH, uvFrameWidthV),
	//	srcTexture);

	// 下	□□□
	//		□　□
	//		□■□
	PutRectangleTiling(
		RectF(innerRect.GetLeft(), innerRect.GetBottom(), innerRect.width, frameWidthV),
		Rect(innerSrcRect.GetLeft(), innerSrcRect.GetBottom(), innerSrcRect.width, frameWidthVI),
		RectF(innerUVRect.GetLeft(), innerUVRect.GetBottom(), innerUVRect.width, uvFrameWidthV),
		srcTexture);

	// 左下	□□□
	//		□　□
	//		■□□
	//PutRectangleTiling(
	//	RectF(outerRect.GetLeft(), innerRect.GetBottom(), frameWidthH, frameWidthV),
	//	Rect(outerSrcRect.GetLeft(), innerSrcRect.GetBottom(), frameWidthHI, frameWidthVI),
	//	RectF(outerUVRect.GetLeft(), innerUVRect.GetBottom(), uvFrameWidthH, uvFrameWidthV),
	//	srcTexture);

	// 左	□□□
	//		■　□
	//		□□□
	PutRectangleTiling(
		RectF(outerRect.GetLeft(), innerRect.GetTop(), frameWidthH, innerRect.height),
		Rect(outerSrcRect.GetLeft(), innerSrcRect.GetTop(), frameWidthHI, innerSrcRect.height),
		RectF(outerUVRect.GetLeft(), innerUVRect.GetTop(), uvFrameWidthH, innerUVRect.height),
		srcTexture);

	//m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	//m_renderer->SetVertexBuffer(m_vertexBuffer);
	//m_renderer->SetIndexBuffer(m_indexBuffer);
	//m_shader.varTexture->SetTexture(srcTexture);
	//m_shader.Pass->Apply();
	//m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 16);
}

//==============================================================================
// FrameRectRenderer
//==============================================================================

//------------------------------------------------------------------------------
FrameRectRenderer::FrameRectRenderer()
	: m_manager(nullptr)
	, m_core(nullptr)
{
}

//------------------------------------------------------------------------------
FrameRectRenderer::~FrameRectRenderer()
{
	LN_SAFE_RELEASE(m_core);
}

//------------------------------------------------------------------------------
void FrameRectRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;

	m_core = LN_NEW FrameRectRendererCore();
	m_core->Initialize(manager);
}

//------------------------------------------------------------------------------
void FrameRectRenderer::SetState(FrameTextureBrush* brush, const Matrix& world, const Matrix& viewProj)
{
	FrameRectRendererState state;
	state.brush = brush;
	state.worldTransform = world;
	state.viewProjTransform = viewProj;

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetState, m_manager,
		FrameRectRendererCore*, m_core,
		FrameRectRendererState, state,
		{
			m_core->SetState(state);
		});

	//m_state.fillBrush = fillBrush;
	//m_state.strokeBrush = strokeBrush;

	//LN_ENQUEUE_RENDER_COMMAND_2(
	//	SetState, m_manager,
	//	FrameRectRendererCore*, m_core,
	//	FrameRectRendererState, m_state,
	//	{
	//		m_core->SetState(m_state);
	//	});
}

//------------------------------------------------------------------------------
void FrameRectRenderer::Draw(const RectF& rect)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		Draw, m_manager,
		FrameRectRendererCore*, m_core,
		RectF, rect,
		{
			m_core->Draw(rect);
		});
}

//------------------------------------------------------------------------------
void FrameRectRenderer::Flush()
{
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
