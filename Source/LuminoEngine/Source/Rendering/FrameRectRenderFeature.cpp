/*
	枠の描画は GeometryRenderer が行うパス描画とは処理内容が大きく異なる。(入力は似ているが)
	そのため別クラスに分けた。処理の振り分けは GraphicsContext が担当する。
*/
#include "Internal.h"
#include <Lumino/Graphics/GraphicsException.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderingCommand.h"
#include "FrameRectRenderFeature.h"

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
void FrameRectRendererCore::initialize(GraphicsManager* manager)
{
	m_manager = manager;

	auto* device = m_manager->getGraphicsDevice();
	m_renderer = device->getRenderer();

	//-----------------------------------------------------
	// Vertex and Index buffers
	const int DefaultFaceCount = 512;
	RequestBuffers(512);
	m_vertexCache.reserve(DefaultFaceCount * 4);
	m_indexCache.reserve(DefaultFaceCount * 6);

	//-----------------------------------------------------
	// Shader
	static const byte_t g_Painter_fx_Data[] =
	{
#include "../Graphics/Resource/Painter.fx.h"	// TODO: もういらない
	};
	static const size_t g_Painter_fx_Len = LN_ARRAY_SIZE_OF(g_Painter_fx_Data);

	ShaderCompileResult r;
	m_shader.shader = device->CreateShader(g_Painter_fx_Data, g_Painter_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.technique = m_shader.shader->GetTechnique(0);
	m_shader.pass = m_shader.technique->getPass(0);
	m_shader.varWorldMatrix = m_shader.shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTone = m_shader.shader->GetVariableByName(_T("g_tone"));
	m_shader.varTexture = m_shader.shader->GetVariableByName(_T("g_texture"));
	m_shader.varGlyphMaskSampler = m_shader.shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varViewportSize = m_shader.shader->GetVariableByName(_T("g_viewportSize"));

	m_shader.varWorldMatrix->setMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->setMatrix(Matrix::Identity);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::SetState(const FrameRectRendererState& state)
{
	m_state = state;
	
	m_shader.varViewProjMatrix->setMatrix(m_state.viewProjTransform);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::Draw(const Matrix& transform, const Rect& rect)
{
	if (rect.isEmpty()) return;

	// TODO: とりあえず今は fill だけ

	Driver::ITexture* srcTexture = m_state.texture;
	if (srcTexture == nullptr) srcTexture = m_manager->getDummyDeviceTexture();


	// 枠
	{
		// TODO: thickness が left しか対応できていない
		PutFrameRectangle(rect, m_state.borderThickness, srcTexture, m_state.srcRect, m_state.wrapMode);
	}

	// Inner
	if (m_state.imageDrawMode == BrushImageDrawMode::BoxFrame)
	{
		Rect dstRect = rect;
		dstRect.x += m_state.borderThickness.Left;
		dstRect.y += m_state.borderThickness.Top;
		dstRect.width -= m_state.borderThickness.Right + m_state.borderThickness.Left;
		dstRect.height -= m_state.borderThickness.Bottom + m_state.borderThickness.Top;

		RectI srcRect = m_state.srcRect;
		srcRect.x += m_state.borderThickness.Left;
		srcRect.y += m_state.borderThickness.Top;
		srcRect.width -= m_state.borderThickness.Right + m_state.borderThickness.Left;
		srcRect.height -= m_state.borderThickness.Bottom + m_state.borderThickness.Top;

		Size texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

		PutRectangle(dstRect, srcRect, uvSrcRect, srcTexture, m_state.wrapMode);
	}


	// Flush
	{

		if (m_indexCache.getCount() == 0) { return; }

		//const SizeI& viewPixelSize = m_renderer->getRenderTarget(0)->GetSize();
		//if (m_shader.varViewportSize != nullptr)
		//	m_shader.varViewportSize->setVector(Vector4((float)viewPixelSize.width, (float)viewPixelSize.height, 0, 0));

		// 描画する
		m_vertexBuffer->setSubData(0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
		m_indexBuffer->setSubData(0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());
		//m_shader.varWorldMatrix->setMatrix(transform);
		//m_shader.varTone->setVector(ToneF());
		//m_shader.varTexture->setTexture(srcTexture);
		//m_shader.varGlyphMaskSampler->setTexture(m_manager->getDummyDeviceTexture());
		//m_renderer->setShaderPass(m_shader.pass);
		//m_renderer->SetVertexDeclaration(m_vertexDeclaration);

		m_renderer->SetVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
		m_renderer->SetVertexBuffer(0, m_vertexBuffer);
		m_renderer->SetIndexBuffer(m_indexBuffer);
		m_renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);

		// キャッシュクリア
		m_vertexCache.clear();
		m_indexCache.clear();
	}
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::RequestBuffers(int faceCount)
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);

	auto* device = m_manager->getGraphicsDevice();
	//m_vertexDeclaration.Attach(device->CreateVertexDeclaration(Vertex::Elements(), Vertex::ElementCount), false);
	m_vertexBuffer = device->CreateVertexBuffer(sizeof(Vertex) * faceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->CreateIndexBuffer(faceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
}

#if 0
//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleStretch(const Rect& rect, const Rect& srcUVRect)
{
	if (rect.IsEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.getTop();
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
	v.Position.Set(rect.GetLeft(), rect.getTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 2);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.getTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 2);	// 右下
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture)
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
	v.Position.Set(rect.GetLeft(), rect.getTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f, 1.0f);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f, 1.0f + blockCountH);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.getTop(), 0);		v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// 右下
	m_vertexCache.Add(v);
}
#endif

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleStretch(const Rect& rect, const Rect& srcUVRect)
{
	if (rect.isEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	uint16_t i = m_vertexCache.getCount();
	m_indexCache.add(i + 0);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 2);
	m_indexCache.add(i + 1);
	m_indexCache.add(i + 3);

	float pos_l = rect.getLeft();
	float pos_r = rect.getRight();
	float pos_t = rect.getTop();
	float pos_b = rect.getBottom();
	float uv_l = srcUVRect.getLeft();
	float uv_r = srcUVRect.getRight();
	float uv_t = srcUVRect.getTop();
	float uv_b = srcUVRect.getBottom();

	ln::Vertex v;
	v.color.set(1, 1, 1, 1);
	v.normal.set(0, 0, 1);
	v.position.set(pos_l, pos_t, 0); v.uv.set(uv_l, uv_t); m_vertexCache.add(v);	// top-left
	v.position.set(pos_l, pos_b, 0); v.uv.set(uv_l, uv_b); m_vertexCache.add(v);	// bottom-left
	v.position.set(pos_r, pos_t, 0); v.uv.set(uv_r, uv_t); m_vertexCache.add(v);	// top-right
	v.position.set(pos_r, pos_b, 0); v.uv.set(uv_r, uv_b); m_vertexCache.add(v);	// bottom-right
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture)
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	int blockCountW = std::ceil(rect.width / srcPixelRect.width);		// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	int blockCountH = std::ceil(rect.height / srcPixelRect.height);	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

	float bw = (float)srcPixelRect.width;
	float bh = (float)srcPixelRect.height;

	ln::Vertex v;
	v.color.set(1, 1, 1, 1);
	v.normal.set(0, 0, 1);

	float uv_l = srcUVRect.x;
	float uv_t = srcUVRect.y;

	for (int y = 0; y < blockCountH; y++)
	{
		float pos_t = bh * y;
		float pos_b = pos_t + bh;
		float uv_b = srcUVRect.getBottom();
		if (pos_b > rect.height)
		{
			float ratio = (1.0f - (pos_b - rect.height) / bh);
			pos_b = rect.height;
			uv_b = srcUVRect.y + srcUVRect.height * ratio;
		}

		for (int x = 0; x < blockCountW; x++)
		{
			float pos_l = bw * x;
			float pos_r = pos_l + bw;
			float uv_r = srcUVRect.getRight();
			if (pos_r > rect.width)
			{
				float ratio = (1.0f - (pos_r - rect.width) / bw);
				pos_r = rect.width;
				uv_r = srcUVRect.x + srcUVRect.width * ratio;
			}

			uint16_t i = m_vertexCache.getCount();
			m_indexCache.add(i + 0);
			m_indexCache.add(i + 1);
			m_indexCache.add(i + 2);
			m_indexCache.add(i + 2);
			m_indexCache.add(i + 1);
			m_indexCache.add(i + 3);

			v.position.set(rect.x + pos_l, rect.y + pos_t, 0); v.uv.set(uv_l, uv_t); m_vertexCache.add(v);	// top-left
			v.position.set(rect.x + pos_l, rect.y + pos_b, 0); v.uv.set(uv_l, uv_b); m_vertexCache.add(v);	// bottom-left
			v.position.set(rect.x + pos_r, rect.y + pos_t, 0); v.uv.set(uv_r, uv_t); m_vertexCache.add(v);	// top-right
			v.position.set(rect.x + pos_r, rect.y + pos_b, 0); v.uv.set(uv_r, uv_b); m_vertexCache.add(v);	// bottom-right
		}
	}

}

//------------------------------------------------------------------------------
void FrameRectRendererCore::PutRectangle(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture, BrushWrapMode wrapMode)
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
void FrameRectRendererCore::PutFrameRectangle(const Rect& rect, const ThicknessF& borderThickness, Driver::ITexture* srcTexture, RectI srcRect, BrushWrapMode wrapMode)
{
	if (srcRect.isEmpty()) return;
	assert(srcTexture != nullptr);

	if (srcRect.width == INT_MAX) {
		srcRect.width = srcTexture->getSize().width;
	}
	if (srcRect.height == INT_MAX) {
		srcRect.height = srcTexture->getSize().height;
	}

	m_vertexCache.clear();
	m_indexCache.clear();

	Size texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

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

	Rect outerRect = rect;
	Rect innerRect(outerRect.x + dstFrame.Left, outerRect.y + dstFrame.Top, outerRect.width - (dstFrame.Left + dstFrame.Right), outerRect.height - (dstFrame.Top + dstFrame.Bottom));
	Rect outerUVRect = uvSrcRect;
	Rect innerUVRect(outerUVRect.x + uvFrame.Left, outerUVRect.y + uvFrame.Top, outerUVRect.width - (uvFrame.Left + uvFrame.Right), outerUVRect.height - (uvFrame.Top + uvFrame.Bottom));
	RectI  outerSrcRect = srcRect;
	RectI  innerSrcRect(outerSrcRect.x + srcFrame.Left, outerSrcRect.y + srcFrame.Top, outerSrcRect.width - (srcFrame.Left + srcFrame.Right), outerSrcRect.height - (srcFrame.Top + srcFrame.Bottom));

	// 左上	■□□
	//		□　□
	//		□□□
	PutRectangle(
		Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.Left, dstFrame.Top),
		RectI(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.Left, srcFrame.Top),
		Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.Left, uvFrame.Top),
		srcTexture, wrapMode);

	// 上	□■□
	//		□　□
	//		□□□
	PutRectangle(
		Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.Top),
		RectI(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.Top),
		Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.Top),
		srcTexture, wrapMode);

	// 右上	□□■
	//		□　□
	//		□□□
	PutRectangle(
		Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.Right, dstFrame.Top),
		RectI(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.Right, srcFrame.Top),
		Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.Right, uvFrame.Top),
		srcTexture, wrapMode);

	// 右	□□□
	//		□　■
	//		□□□
	PutRectangle(
		Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.Right, innerRect.height),
		RectI(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.Right, innerSrcRect.height),
		Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.Right, innerUVRect.height),
		srcTexture, wrapMode);

	// 右下	□□□
	//		□　□
	//		□□■
	PutRectangle(
		Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.Right, dstFrame.Bottom),
		RectI(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.Right, srcFrame.Bottom),
		Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.Right, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 下	□□□
	//		□　□
	//		□■□
	PutRectangle(
		Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.Bottom),
		RectI(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.Bottom),
		Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 左下	□□□
	//		□　□
	//		■□□
	PutRectangle(
		Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.Left, dstFrame.Bottom),
		RectI(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.Left, srcFrame.Bottom),
		Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.Left, uvFrame.Bottom),
		srcTexture, wrapMode);

	// 左	□□□
	//		■　□
	//		□□□
	PutRectangle(
		Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.Left, innerRect.height),
		RectI(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.Left, innerSrcRect.height),
		Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.Left, innerUVRect.height),
		srcTexture, wrapMode);
}

//==============================================================================
// FrameRectRenderFeature
//==============================================================================

//------------------------------------------------------------------------------
FrameRectRenderFeature::FrameRectRenderFeature()
	: m_manager(nullptr)
	, m_core(nullptr)
{
}

//------------------------------------------------------------------------------
FrameRectRenderFeature::~FrameRectRenderFeature()
{
	LN_SAFE_RELEASE(m_core);
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::initialize(GraphicsManager* manager)
{
	m_manager = manager;

	m_core = LN_NEW FrameRectRendererCore();
	m_core->initialize(manager);
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::SetViewInfo(const Matrix& viewProj)
{
	m_viewProj = viewProj;
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::SetState(Brush* brush)
{
	m_brush = brush;
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::SetState(Brush* brush, const Matrix& world, const Matrix& viewProj)	// TODO: world いらない
{
	if (LN_CHECK_ARG(brush != nullptr)) return;

	FrameRectRendererState state;
	//state.worldTransform = world;
	state.viewProjTransform = viewProj;
	state.imageDrawMode = brush->getImageDrawMode();
	state.borderThickness = brush->getBorderThickness();
	state.srcRect = RectI::fromFloatRect(brush->getSourceRect());
	state.wrapMode = brush->getWrapMode();
	state.texture = (brush->getTexture() != nullptr) ? brush->getTexture()->resolveDeviceObject() : nullptr;
	if (LN_CHECK_STATE(state.texture != nullptr)) return;

	LN_ENQUEUE_RENDER_COMMAND_2(
		SetState, m_manager,
		FrameRectRendererCore*, m_core,
		FrameRectRendererState, state,
		{
			m_core->SetState(state);
		});
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::Draw(const Matrix& transform, const Rect& rect)
{
	SetState(m_brush, Matrix::Identity, m_viewProj);

	LN_ENQUEUE_RENDER_COMMAND_3(
		Draw, m_manager,
		FrameRectRendererCore*, m_core,
		Matrix, transform,
		Rect, rect,
		{
			m_core->Draw(transform, rect);
		});
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::flush()
{
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::onSetState(const DrawElementBatch* state)
{
	SetState(state->state.getBrush());
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
