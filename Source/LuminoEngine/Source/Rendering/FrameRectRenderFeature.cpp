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
#include "../Rendering/RenderStage.h"

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
	requestBuffers(512);
	m_vertexCache.clearAndReserve(DefaultFaceCount * 4);
	m_indexCache.clearAndReserve(DefaultFaceCount * 6);

	//-----------------------------------------------------
	// Shader
	static const byte_t g_Painter_fx_Data[] =
	{
#include "../Graphics/Resource/Painter.fx.h"	// TODO: もういらない
	};
	static const size_t g_Painter_fx_Len = LN_ARRAY_SIZE_OF(g_Painter_fx_Data);

	ShaderCompileResult r;
	m_shader.shader = device->createShader(g_Painter_fx_Data, g_Painter_fx_Len, &r);
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.technique = m_shader.shader->getTechnique(0);
	m_shader.pass = m_shader.technique->getPass(0);
	m_shader.varWorldMatrix = m_shader.shader->getVariableByName(_LT("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->getVariableByName(_LT("g_viewProjMatrix"));
	m_shader.varTone = m_shader.shader->getVariableByName(_LT("g_tone"));
	m_shader.varTexture = m_shader.shader->getVariableByName(_LT("g_texture"));
	m_shader.varGlyphMaskSampler = m_shader.shader->getVariableByName(_LT("g_glyphMaskTexture"));
	m_shader.varViewportSize = m_shader.shader->getVariableByName(_LT("g_viewportSize"));

	m_shader.varWorldMatrix->setMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->setMatrix(Matrix::Identity);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::setState(const FrameRectRendererState& state)
{
	m_state = state;
	
	m_shader.varViewProjMatrix->setMatrix(m_state.viewProjTransform);
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::draw(const Matrix& transform, const Rect& rect)
{
	if (rect.isEmpty()) return;

	// TODO: とりあえず今は fill だけ

	Driver::ITexture* srcTexture = m_state.texture;
	if (srcTexture == nullptr) srcTexture = m_manager->getDummyDeviceTexture();


	// 枠
	{
		// TODO: thickness が left しか対応できていない
		putFrameRectangle(rect, m_state.borderThickness, srcTexture, m_state.srcRect, m_state.wrapMode);
	}

	// Inner
	if (m_state.imageDrawMode == BrushImageDrawMode::BoxFrame)
	{
		Rect dstRect = rect;
		dstRect.x += m_state.borderThickness.left;
		dstRect.y += m_state.borderThickness.top;
		dstRect.width -= m_state.borderThickness.getWidth();
		dstRect.height -= m_state.borderThickness.getHeight();

		RectI srcRect = m_state.srcRect;
		srcRect.x += m_state.borderThickness.left;
		srcRect.y += m_state.borderThickness.top;
		srcRect.width -= m_state.borderThickness.getWidth();
		srcRect.height -= m_state.borderThickness.getHeight();

		Size texSize((float)srcTexture->getRealSize().width, (float)srcTexture->getRealSize().height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

		putRectangle(dstRect, srcRect, uvSrcRect, srcTexture, m_state.wrapMode);
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
		//m_renderer->setVertexDeclaration(m_vertexDeclaration);

		m_renderer->setVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
		m_renderer->setVertexBuffer(0, m_vertexBuffer);
		m_renderer->setIndexBuffer(m_indexBuffer);
		m_renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);

		// キャッシュクリア
		m_vertexCache.clear();
		m_indexCache.clear();
	}
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::requestBuffers(int faceCount)
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);

	auto* device = m_manager->getGraphicsDevice();
	//m_vertexDeclaration.Attach(device->createVertexDeclaration(Vertex::Elements(), Vertex::ElementCount), false);
	m_vertexBuffer = device->createVertexBuffer(sizeof(Vertex) * faceCount * 4, nullptr, ResourceUsage::Dynamic);
	m_indexBuffer = device->createIndexBuffer(faceCount * 6, nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
}

#if 0
//------------------------------------------------------------------------------
void FrameRectRendererCore::putRectangleStretch(const Rect& rect, const Rect& srcUVRect)
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
void FrameRectRendererCore::putRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture)
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
void FrameRectRendererCore::putRectangleStretch(const Rect& rect, const Rect& srcUVRect)
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
void FrameRectRendererCore::putRectangleTiling(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture)
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
void FrameRectRendererCore::putRectangle(const Rect& rect, const RectI& srcPixelRect, const Rect& srcUVRect, Driver::ITexture* srcTexture, BrushWrapMode wrapMode)
{
	if (wrapMode == BrushWrapMode::Stretch)
	{
		putRectangleStretch(rect, srcUVRect);
	}
	else if (wrapMode == BrushWrapMode::Tile)
	{
		putRectangleTiling(rect, srcPixelRect, srcUVRect, srcTexture);
	}
}

//------------------------------------------------------------------------------
void FrameRectRendererCore::putFrameRectangle(const Rect& rect, const Thickness& borderThickness, Driver::ITexture* srcTexture, RectI srcRect, BrushWrapMode wrapMode)
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

	Size texSize((float)srcTexture->getRealSize().width, (float)srcTexture->getRealSize().height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

	Thickness dstFrame;
	Thickness uvFrame;
	ThicknessI srcFrame;
	{
		Thickness baseThickness = borderThickness;

		// 横幅が小さいため、枠幅も狭めなければならない
		if (rect.width < baseThickness.left + baseThickness.right)
		{
			baseThickness.left = rect.width / 2;
			baseThickness.right = rect.width / 2;
		}
		// 縦幅が小さいため、枠幅も狭めなければならない
		if (rect.height < baseThickness.top + baseThickness.bottom)
		{
			baseThickness.top = rect.height / 2;
			baseThickness.bottom = rect.height / 2;
		}

		dstFrame.left = baseThickness.left;
		dstFrame.right = baseThickness.right;
		dstFrame.top = baseThickness.top;
		dstFrame.bottom = baseThickness.bottom;
		uvFrame.left = baseThickness.left * texSize.width;
		uvFrame.right = baseThickness.right * texSize.width;
		uvFrame.top = baseThickness.top * texSize.height;
		uvFrame.bottom = baseThickness.bottom * texSize.height;
		srcFrame.left = (int)baseThickness.left;	// 型変換回数を減らすため、あらかじめ int 化しておく
		srcFrame.right = (int)baseThickness.right;
		srcFrame.top = (int)baseThickness.top;
		srcFrame.bottom = (int)baseThickness.bottom;
	}

	Rect outerRect = rect;
	Rect innerRect(outerRect.x + dstFrame.left, outerRect.y + dstFrame.top, outerRect.width - (dstFrame.left + dstFrame.right), outerRect.height - (dstFrame.top + dstFrame.bottom));
	Rect outerUVRect = uvSrcRect;
	Rect innerUVRect(outerUVRect.x + uvFrame.left, outerUVRect.y + uvFrame.top, outerUVRect.width - (uvFrame.left + uvFrame.right), outerUVRect.height - (uvFrame.top + uvFrame.bottom));
	RectI  outerSrcRect = srcRect;
	RectI  innerSrcRect(outerSrcRect.x + srcFrame.left, outerSrcRect.y + srcFrame.top, outerSrcRect.width - (srcFrame.left + srcFrame.right), outerSrcRect.height - (srcFrame.top + srcFrame.bottom));

	// 左上	■□□
	//		□　□
	//		□□□
	putRectangle(
		Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.left, dstFrame.top),
		RectI(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.left, srcFrame.top),
		Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.left, uvFrame.top),
		srcTexture, wrapMode);

	// 上	□■□
	//		□　□
	//		□□□
	putRectangle(
		Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.top),
		RectI(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.top),
		Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.top),
		srcTexture, wrapMode);

	// 右上	□□■
	//		□　□
	//		□□□
	putRectangle(
		Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.right, dstFrame.top),
		RectI(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.right, srcFrame.top),
		Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.right, uvFrame.top),
		srcTexture, wrapMode);

	// 右	□□□
	//		□　■
	//		□□□
	putRectangle(
		Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.right, innerRect.height),
		RectI(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.right, innerSrcRect.height),
		Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.right, innerUVRect.height),
		srcTexture, wrapMode);

	// 右下	□□□
	//		□　□
	//		□□■
	putRectangle(
		Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.right, dstFrame.bottom),
		RectI(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.right, srcFrame.bottom),
		Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.right, uvFrame.bottom),
		srcTexture, wrapMode);

	// 下	□□□
	//		□　□
	//		□■□
	putRectangle(
		Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.bottom),
		RectI(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.bottom),
		Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.bottom),
		srcTexture, wrapMode);

	// 左下	□□□
	//		□　□
	//		■□□
	putRectangle(
		Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.left, dstFrame.bottom),
		RectI(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.left, srcFrame.bottom),
		Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.left, uvFrame.bottom),
		srcTexture, wrapMode);

	// 左	□□□
	//		■　□
	//		□□□
	putRectangle(
		Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.left, innerRect.height),
		RectI(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.left, innerSrcRect.height),
		Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.left, innerUVRect.height),
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
void FrameRectRenderFeature::setViewInfo(const Matrix& viewProj)
{
	m_viewProj = viewProj;
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::setState(Brush* brush)
{
	m_brush = brush;
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::setState(Brush* brush, const Matrix& world, const Matrix& viewProj)	// TODO: world いらない
{
	if (LN_REQUIRE(brush != nullptr)) return;

	FrameRectRendererState state;
	//state.worldTransform = world;
	state.viewProjTransform = viewProj;
	state.imageDrawMode = brush->getImageDrawMode();
	state.borderThickness = brush->getBorderThickness();
	state.srcRect = RectI::fromFloatRect(brush->getSourceRect());
	state.wrapMode = brush->getWrapMode();
	state.texture = (brush->getTexture() != nullptr) ? brush->getTexture()->resolveDeviceObject() : nullptr;
	if (LN_REQUIRE(state.texture != nullptr)) return;

	LN_ENQUEUE_RENDER_COMMAND_2(
		setState, m_manager,
		FrameRectRendererCore*, m_core,
		FrameRectRendererState, state,
		{
			m_core->setState(state);
		});
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::draw(const Matrix& transform, const Rect& rect)
{
	setState(m_brush, Matrix::Identity, m_viewProj);

	LN_ENQUEUE_RENDER_COMMAND_3(
		draw, m_manager,
		FrameRectRendererCore*, m_core,
		Matrix, transform,
		Rect, rect,
		{
			m_core->draw(transform, rect);
		});
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::flush()
{
}

//------------------------------------------------------------------------------
void FrameRectRenderFeature::onSetState(const RenderStage* state)
{
	setState(state->getBrushFinal());
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
