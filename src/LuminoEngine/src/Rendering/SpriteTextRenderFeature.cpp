
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Font/FontCore.hpp"
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "SpriteTextRenderFeature.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// SpriteTextRenderFeature

SpriteTextRenderFeature::SpriteTextRenderFeature()
	: m_mappedVertices(nullptr)
	, m_buffersReservedSpriteCount(0)
{
}

void SpriteTextRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
	m_manager = manager;
	m_currentFont = nullptr;
	m_vertexLayout = manager->standardVertexDeclaration();
	prepareBuffers(nullptr, 2048);
	m_batchData.spriteOffset = 0;
	m_batchData.spriteCount = 0;
}

RequestBatchResult SpriteTextRenderFeature::drawText(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, const FormattedText* text, const Vector2& anchor, SpriteBaseDirection baseDirection, const Matrix& transform)
{
	m_drawingFormattedText = text;
	m_drawingTransform = transform;
	m_drawingAnchor = anchor;
	m_drawingBaseDirection = baseDirection;

	auto result = updateCurrentFontAndFlushIfNeeded(batchList, context, text->font);

	// 3D 空間に書く場合
	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D)
	{
		FontGlobalMetrics metrix;
		m_currentFont->getGlobalMetrics(&metrix);
		//float h = std::abs(metrix.descender - metrix.ascender);
		m_drawingTransform.scale(metrix.virutalSpaceFactor);
	}

	beginLayout();
	TextLayoutEngine::layout(m_currentFont, text->text.c_str(), text->text.length(), text->area, 0, text->textAlignment);
	auto result2 = resolveCache(batchList, context);
	endLayout(context);

	assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

RequestBatchResult SpriteTextRenderFeature::drawChar(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* font, uint32_t codePoint, const Color& color, const Matrix& transform)
{
	m_drawingAnchor = Vector2::Zero;
	m_drawingBaseDirection = SpriteBaseDirection::Basic2D;

	auto result = updateCurrentFontAndFlushIfNeeded(batchList, context, font);

	beginLayout();
	addLayoutedGlyphItem(codePoint, Vector2::Zero, color, transform);
	auto result2 = resolveCache(batchList, context);
	endLayout(context);

	assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

RequestBatchResult SpriteTextRenderFeature::drawFlexGlyphRun(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* font, const FlexGlyphRun* glyphRun, const Vector2& anchor, SpriteBaseDirection baseDirection, const Matrix& transform)
{
	m_drawingAnchor = anchor;
	m_drawingBaseDirection = baseDirection;

	auto result = updateCurrentFontAndFlushIfNeeded(batchList, context, font);

	beginLayout();
	for (int i = 0; i < glyphRun->glyphCount; i++) {
		auto& glyph = glyphRun->owner->glyphs()[glyphRun->startIndex + i];
		Color color = glyphRun->color;
		color.a *= glyph.opacity;
		if (glyph.timeOffset <= glyphRun->owner->time() && color.a > 0.0f) {
			addLayoutedGlyphItem(glyph.codePoint, glyph.pos.xy(), color, transform);
		}
	}
	auto result2 = resolveCache(batchList, context);
	endLayout(context);

	assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

void SpriteTextRenderFeature::beginRendering()
{
	m_cacheTexture = nullptr;
}

void SpriteTextRenderFeature::endRendering()
{
    for (auto& font : m_renderingFonts) {
        font->endCacheUsing();
    }
    m_renderingFonts.clear();
}

void SpriteTextRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	if (m_mappedVertices) {
		// TODO: unmap (今は自動だけど、明示した方が安心かも)
	}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;
	batch->overrideTexture = m_cacheTexture;

    //static_cast<Texture2D*>(m_cacheTexture)->clear(Color::Red);

	m_batchData.spriteOffset = m_batchData.spriteOffset + m_batchData.spriteCount;
	m_batchData.spriteCount = 0;

    // 次の draw で Submit 発生しないようにする
    m_cacheTexture = nullptr;
}

void SpriteTextRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);

	m_batchData.spriteOffset = 0;
	m_batchData.spriteCount = 0;
}

void SpriteTextRenderFeature::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
	addLayoutedGlyphItem(ch, pos, m_drawingFormattedText->color, m_drawingTransform);
}

void SpriteTextRenderFeature::prepareBuffers(GraphicsContext* context, int spriteCount)
{
		// TODO: 実行中の map は context->map 用意した方がいいかも
	//if (context) {
	//	LN_NOTIMPLEMENTED();
	//}

	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		// VertexBuffer
		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		if (!m_vertexBuffer)
			m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferSize, GraphicsResourceUsage::Dynamic);
		else
			m_vertexBuffer->resize(vertexBufferSize);

		// IndexBuffer
		size_t indexBufferSize = spriteCount * 6;
		if (!m_indexBuffer)
			m_indexBuffer = makeObject<IndexBuffer>(indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
		else
			m_indexBuffer->resize(indexBufferSize);
		auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));	// TODO: 部分 map
		int idx = 0;
		int i2 = 0;
		for (int i = 0; i < spriteCount; ++i)
		{
			i2 = i * 6;
			idx = i * 4;
			ib[i2 + 0] = idx;
			ib[i2 + 1] = idx + 1;
			ib[i2 + 2] = idx + 2;
			ib[i2 + 3] = idx + 2;
			ib[i2 + 4] = idx + 1;
			ib[i2 + 5] = idx + 3;
		}

		m_buffersReservedSpriteCount = spriteCount;
	}
}

RequestBatchResult SpriteTextRenderFeature::updateCurrentFontAndFlushIfNeeded(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Font* newFont)
{
	float scale = 1.0f;	// TODO: DPI
	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
		scale = 5.0f;
	}

	auto result = RequestBatchResult::Staging;
	auto font = FontHelper::resolveFontCore(newFont, scale);
	if (font != m_currentFont && m_batchData.spriteCount > 0) {
		submitBatch(context, batchList);
		result = RequestBatchResult::Submitted;
        m_renderingFonts.push_back(font);
	}
	m_currentFont = font;
	return result;
}

void SpriteTextRenderFeature::beginLayout()
{
	m_cacheRequest.glyphs.clear();
	m_glyphLayoutDataList.clear();
}

void SpriteTextRenderFeature::addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform)
{
	//// TODO: Outline

	m_cacheRequest.glyphs.push_back({ codePoint });

	GlyphData data;
	data.transform = transform;
	data.position = pos;
	data.color = color;
	m_glyphLayoutDataList.push_back(data);
}

RequestBatchResult SpriteTextRenderFeature::resolveCache(detail::RenderFeatureBatchList* batchList, GraphicsContext* context)
{
	m_currentFont->getFontGlyphTextureCache(&m_cacheRequest);
	if (m_cacheTexture && m_cacheTexture != m_cacheRequest.texture) {
		submitBatch(context, batchList);
		return RequestBatchResult::Submitted;
	}
	else {
        m_cacheTexture = m_cacheRequest.texture;
		return RequestBatchResult::Staging;
	}
}

void SpriteTextRenderFeature::endLayout(GraphicsContext* context)
{
	size_t spriteCount = m_batchData.spriteOffset + m_batchData.spriteCount;
	size_t dataCount = m_glyphLayoutDataList.size();
	prepareBuffers(context, spriteCount + dataCount);

	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));

	Vector2 posOffset;
	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
		auto area = renderAreaSize();
		posOffset = Vector2(area.width * m_drawingAnchor.x, area.width * m_drawingAnchor.y);
		posOffset.x -= area.width * 0.5f;
		posOffset.y -= area.height * 0.5f;
	}

	auto srcTexture = m_cacheRequest.texture;
	Size texSizeInv(1.0f / srcTexture->width(), 1.0f / srcTexture->height());
	for (auto i = 0; i < dataCount; i++)
	{
		auto* vertices = m_mappedVertices + ((m_batchData.spriteOffset + m_batchData.spriteCount) * 4);

		auto& data = m_glyphLayoutDataList[i];

		auto srcRect = m_cacheRequest.glyphs[i].info.srcRect;
		Rect uvSrcRect((float)srcRect.x, (float)srcRect.y, (float)srcRect.width, (float)srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		Rect dstRect(data.position + posOffset, (float)srcRect.width, (float)srcRect.height);
		putRectangle(vertices, data.transform, dstRect, uvSrcRect, data.color);

		m_batchData.spriteCount++;
	}
}

void SpriteTextRenderFeature::putRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color) const
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	float signY = (m_drawingBaseDirection == SpriteBaseDirection::Basic2D) ? 1.0f : -1.0f;

	float lu = srcUVRect.getLeft();
	float tv = srcUVRect.getTop();
	float ru = srcUVRect.getRight();
	float bv = srcUVRect.getBottom();

	buffer[0].color = color;
	buffer[0].position.set(rect.getLeft(), rect.getTop() * signY, 0);
	buffer[0].position.transformCoord(transform);
	buffer[0].uv.set(lu, tv);	// 左上
	buffer[0].normal = Vector3::UnitZ;

	buffer[1].color = color;
	buffer[1].position.set(rect.getRight(), rect.getTop() * signY, 0);
	buffer[1].position.transformCoord(transform);
	buffer[1].uv.set(ru, tv);	// 右上
	buffer[1].normal = Vector3::UnitZ;

	buffer[2].color = color;
	buffer[2].position.set(rect.getLeft(), rect.getBottom() * signY, 0);
	buffer[2].position.transformCoord(transform);
	buffer[2].uv.set(lu, bv);	// 左下
	buffer[2].normal = Vector3::UnitZ;

	buffer[3].color = color;
	buffer[3].position.set(rect.getRight(), rect.getBottom() * signY, 0);
	buffer[3].position.transformCoord(transform);
	buffer[3].uv.set(ru, bv);	// 右下
	buffer[3].normal = Vector3::UnitZ;

	// pixel snap
	if (isPixelSnapEnabled()) {
		for (int i = 0; i < 4; i++) {
			buffer[i].position.x = std::round(buffer[i].position.x);
			buffer[i].position.y = std::round(buffer[i].position.y);
		}
	}
}

} // namespace detail
} // namespace ln

