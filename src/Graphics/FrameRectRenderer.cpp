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

	Driver::ITexture* srcTexture = m_state.texture;
	if (srcTexture == nullptr) srcTexture = m_manager->GetDummyDeviceTexture();


	// 枠
	{
		// TODO: thickness が left しか対応できていない
		PutFrameRectangle(rect, m_state.borderThickness, srcTexture, m_state.srcRect, m_state.wrapMode);
	}

	// Inner
	if (m_state.imageDrawMode == BrushImageDrawMode::BoxFrame)
	{
		RectF dstRect = rect;
		dstRect.x += m_state.borderThickness.Left;
		dstRect.y += m_state.borderThickness.Top;
		dstRect.width -= m_state.borderThickness.Right + m_state.borderThickness.Left;
		dstRect.height -= m_state.borderThickness.Bottom + m_state.borderThickness.Top;

		Rect srcRect = m_state.srcRect;
		srcRect.x += m_state.borderThickness.Left;
		srcRect.y += m_state.borderThickness.Top;
		srcRect.width -= m_state.borderThickness.Right + m_state.borderThickness.Left;
		srcRect.height -= m_state.borderThickness.Bottom + m_state.borderThickness.Top;

		Size texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

		PutRectangle(dstRect, srcRect, uvSrcRect, srcTexture, m_state.wrapMode);
	}


	// Flush
	{

		if (m_indexCache.GetCount() == 0) { return; }

		const SizeI& viewPixelSize = m_renderer->GetRenderTarget(0)->GetSize();
		m_shader.varViewportSize->SetVector(Vector4((float)viewPixelSize.width, (float)viewPixelSize.height, 0, 0));

		// 描画する
		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
		m_shader.varTone->SetVector(ToneF());
		m_shader.varTexture->SetTexture(srcTexture);
		m_shader.varGlyphMaskSampler->SetTexture(m_manager->GetDummyDeviceTexture());
		m_renderer->SetShaderPass(m_shader.pass);
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
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * faceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(faceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
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
void FrameRectRendererCore::PutRectangle(const RectF& rect, const Rect& srcPixelRect, const RectF& srcUVRect, Driver::ITexture* srcTexture, BrushWrapMode wrapMode)
{
	if (wrapMode == BrushWrapMode::Stretch)
	{
		PutRectangleStretch(rect, srcUVRect);
	}
	else if (wrapMode == BrushWrapMode::Tile)
	{
		PutRectangleTiling(rect, srcPixelRect, srcUVRect, srcTexture);
	}
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutFrameRectangle(const RectF& rect, const ThicknessF& borderThickness, Driver::ITexture* srcTexture, Rect srcRect, BrushWrapMode wrapMode)
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

	Size texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

	ThicknessF dstFrame;
	ThicknessF uvFrame;
	Thickness srcFrame;
	{
		ThicknessF baseThickness = borderThickness;

		// 横幅が小さいため、枠幅も狭めなければならない
		if (rect.width < baseThickness.Left + baseThickness.Right)
		{
			baseThickness.Left = rect.width / 2;
			baseThickness.Right = rect.width / 2;
		}
		// 縦幅が小さいため、枠幅も狭めなければならない
		if (rect.height < baseThickness.Top + baseThickness.Bottom)
		{
			baseThickness.Top = rect.height / 2;
			baseThickness.Bottom = rect.height / 2;
		}

		dstFrame.Left = baseThickness.Left;
		dstFrame.Right = baseThickness.Right;
		dstFrame.Top = baseThickness.Top;
		dstFrame.Bottom = baseThickness.Bottom;
		uvFrame.Left = baseThickness.Left * texSize.width;
		uvFrame.Right = baseThickness.Right * texSize.width;
		uvFrame.Top = baseThickness.Top * texSize.height;
		uvFrame.Bottom = baseThickness.Bottom * texSize.height;
		srcFrame.Left = (int)baseThickness.Left;	// 型変換回数を減らすため、あらかじめ int 化しておく
		srcFrame.Right = (int)baseThickness.Right;
		srcFrame.Top = (int)baseThickness.Top;
		srcFrame.Bottom = (int)baseThickness.Bottom;
	}

	RectF outerRect = rect;
	RectF innerRect(outerRect.x + dstFrame.Left, outerRect.y + dstFrame.Top, outerRect.width - (dstFrame.Left + dstFrame.Right), outerRect.height - (dstFrame.Top + dstFrame.Bottom));
	RectF outerUVRect = uvSrcRect;
	RectF innerUVRect(outerUVRect.x + uvFrame.Left, outerUVRect.y + uvFrame.Top, outerUVRect.width - (uvFrame.Left + uvFrame.Right), outerUVRect.height - (uvFrame.Top + uvFrame.Bottom));
	Rect  outerSrcRect = srcRect;
	Rect  innerSrcRect(outerSrcRect.x + srcFrame.Left, outerSrcRect.y + srcFrame.Top, outerSrcRect.width - (srcFrame.Left + srcFrame.Right), outerSrcRect.height - (srcFrame.Top + srcFrame.Bottom));

	// 左上	■□□
	//		□　□
	//		□□□
	PutRectangle(
		RectF(outerRect.GetLeft(), outerRect.GetTop(), dstFrame.Left, dstFrame.Top),
		Rect(outerSrcRect.GetLeft(), outerSrcRect.GetTop(), srcFrame.Left, srcFrame.Top),
		RectF(outerUVRect.GetLeft(), outerUVRect.GetTop(), uvFrame.Left, uvFrame.Top),
		srcTexture, wrapMode);

	// 上	□■□
	//		□　□
	//		□□□
	PutRectangle(
		RectF(innerRect.GetLeft(), outerRect.GetTop(), innerRect.width, dstFrame.Top),
		Rect(innerSrcRect.GetLeft(), outerSrcRect.GetTop(), innerSrcRect.width, srcFrame.Top),
		RectF(innerUVRect.GetLeft(), outerUVRect.GetTop(), innerUVRect.width, uvFrame.Top),
		srcTexture, wrapMode);

	// 右上	□□■
	//		□　□
	//		□□□
	PutRectangle(
		RectF(innerRect.GetRight(), outerRect.GetTop(), dstFrame.Right, dstFrame.Top),
		Rect(innerSrcRect.GetRight(), outerSrcRect.GetTop(), srcFrame.Right, srcFrame.Top),
		RectF(innerUVRect.GetRight(), outerUVRect.GetTop(), uvFrame.Right, uvFrame.Top),
		srcTexture, wrapMode);

	// 右	□□□
	//		□　■
	//		□□□
	PutRectangle(
		RectF(innerRect.GetRight(), innerRect.GetTop(), dstFrame.Right, innerRect.height),
		Rect(innerSrcRect.GetRight(), innerSrcRect.GetTop(), srcFrame.Right, innerSrcRect.height),
		RectF(innerUVRect.GetRight(), innerUVRect.GetTop(), uvFrame.Right, innerUVRect.height),
		srcTexture, wrapMode);

	// 右下	□□□
	//		□　□
	//		□□■
	PutRectangle(
		RectF(innerRect.GetRight(), innerRect.GetBottom(), dstFrame.Right, dstFrame.Bottom),
		Rect(innerSrcRect.GetRight(), innerSrcRect.GetBottom(), srcFrame.Right, srcFrame.Bottom),
		RectF(innerUVRect.GetRight(), innerUVRect.GetBottom(), uvFrame.Right, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 下	□□□
	//		□　□
	//		□■□
	PutRectangle(
		RectF(innerRect.GetLeft(), innerRect.GetBottom(), innerRect.width, dstFrame.Bottom),
		Rect(innerSrcRect.GetLeft(), innerSrcRect.GetBottom(), innerSrcRect.width, srcFrame.Bottom),
		RectF(innerUVRect.GetLeft(), innerUVRect.GetBottom(), innerUVRect.width, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 左下	□□□
	//		□　□
	//		■□□
	PutRectangle(
		RectF(outerRect.GetLeft(), innerRect.GetBottom(), dstFrame.Left, dstFrame.Bottom),
		Rect(outerSrcRect.GetLeft(), innerSrcRect.GetBottom(), srcFrame.Left, srcFrame.Bottom),
		RectF(outerUVRect.GetLeft(), innerUVRect.GetBottom(), uvFrame.Left, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 左	□□□
	//		■　□
	//		□□□
	PutRectangle(
		RectF(outerRect.GetLeft(), innerRect.GetTop(), dstFrame.Left, innerRect.height),
		Rect(outerSrcRect.GetLeft(), innerSrcRect.GetTop(), srcFrame.Left, innerSrcRect.height),
		RectF(outerUVRect.GetLeft(), innerUVRect.GetTop(), uvFrame.Left, innerUVRect.height),
		srcTexture, wrapMode);
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
void FrameRectRenderer::SetState(TextureBrush* brush, const Matrix& world, const Matrix& viewProj)
{
	LN_CHECK_ARG(brush != nullptr);

	FrameRectRendererState state;
	state.worldTransform = world;
	state.viewProjTransform = viewProj;
	state.imageDrawMode = brush->GetImageDrawMode();
	state.borderThickness = brush->GetBorderThickness();
	state.srcRect = brush->GetSourceRect();
	state.wrapMode = brush->GetWrapMode();
	state.texture = (brush->GetTexture() != nullptr) ? brush->GetTexture()->GetDeviceObject() : nullptr;
	LN_CHECK_STATE(state.texture != nullptr);

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetState, m_manager,
		FrameRectRendererCore*, m_core,
		FrameRectRendererState, state,
		{
			m_core->SetState(state);
		});
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
