
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../Font/FontCore.hpp"
#include "../../Graphics/RHIs/GraphicsDeviceContext.hpp"
#include "../../Graphics/GraphicsManager.hpp"
#include "../../Rendering/RenderingManager.hpp"
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
	m_lastFont = nullptr;
	m_vertexLayout = manager->standardVertexDeclaration();
	prepareBuffers(nullptr, 2048);
	//m_batchData.spriteOffset = 0;
	//m_batchData.spriteCount = 0;
	m_spriteCount = 0;
}

RequestBatchResult SpriteTextRenderFeature::drawText(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState, 
	GraphicsContext* context,
	const FormattedText* text,
	const Vector2& anchor,
	SpriteBaseDirection baseDirection,
	const Ref<SamplerState>& samplerState,
	const Matrix& transform)
{
	m_drawingFormattedText = text;
	m_drawingTransform = transform;
	m_drawingAnchor = anchor;
	m_drawingBaseDirection = baseDirection;
	m_drawingSamplerState = samplerState;

	FontCore* fontCore = resolveFontCore(text->font, text->fontRequester, batchList->m_mainCameraInfo, transform);
	

	// 3D 空間に書く場合
	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D && text->fontRequester)
	{
		FontGlobalMetrics metrix;
		fontCore->getGlobalMetrics(&metrix);
		//float h = std::abs(metrix.descender - metrix.ascender);

		float s = metrix.virutalSpaceFactor * (static_cast<float>(text->fontRequester->size) / Font::DefaultSize);

		m_drawingTransform = transform.getRotationMatrix();
		m_drawingTransform.scale(s);
		m_drawingTransform.translate(transform.position());
		
	}

	beginLayout();
	TextLayoutEngine::layout(fontCore, text->text.c_str(), text->text.length(), text->area, 0, text->textAlignment);
	//auto result2 = resolveCache(batchList, context);
	Batch* batch = endLayoutAndAcquireBatch(batchList, batchState, context, fontCore, transform);
	buildSpriteList(batch, context);

	return RequestBatchResult::Submitted;
	//assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	//return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

RequestBatchResult SpriteTextRenderFeature::drawChar(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	GraphicsContext* context,
	Font* font,
	uint32_t codePoint,
	const Color& color,
	const Matrix& transform)
{
	m_drawingAnchor = Vector2::Zero;
	m_drawingBaseDirection = SpriteBaseDirection::Basic2D;
	m_drawingSamplerState = nullptr;

	FontCore* fontCore = resolveFontCore(font, nullptr, batchList->m_mainCameraInfo, transform);

	beginLayout();
	addLayoutedGlyphItem(codePoint, Vector2::Zero, color, transform);
	//auto result2 = resolveCache(batchList, context);
	Batch* batch = endLayoutAndAcquireBatch(batchList, batchState, context, fontCore, transform);
	buildSpriteList(batch, context);

	return RequestBatchResult::Submitted;
	//assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	//return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

RequestBatchResult SpriteTextRenderFeature::drawFlexGlyphRun(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	GraphicsContext* context,
	Font* font,
	const FlexGlyphRun* glyphRun,
	const Vector2& anchor,
	SpriteBaseDirection baseDirection,
	const Matrix& transform)
{
	m_drawingAnchor = anchor;
	m_drawingBaseDirection = baseDirection;
	m_drawingSamplerState = nullptr;

	// まず最初に要求された Font で Layout を実行してみる。
	// そうすることで本当に必要な文字と、それがテクスチャに収まるかどうか (Batch を切り替えるべきか) がわかる。
	FontCore* fontCore = resolveFontCore(font, nullptr, batchList->m_mainCameraInfo, transform);
	beginLayout();
	for (int i = 0; i < glyphRun->glyphCount; i++) {
		auto& glyph = glyphRun->owner->glyphs()[glyphRun->startIndex + i];
		Color color = glyphRun->color;
		color.a *= glyph.opacity;
		if (glyph.timeOffset <= glyphRun->owner->time() && color.a > 0.0f) {
			addLayoutedGlyphItem(glyph.codePoint, glyph.pos.xy(), color, transform);
		}
	}


	Batch* batch = endLayoutAndAcquireBatch(batchList, batchState, context, fontCore, transform);
	buildSpriteList(batch, context);








	return RequestBatchResult::Submitted;
	//assert(!(result == RequestBatchResult::Submitted && result2 == RequestBatchResult::Submitted));
	//return (result == RequestBatchResult::Submitted || result2 == RequestBatchResult::Submitted) ? RequestBatchResult::Submitted : RequestBatchResult::Staging;
}

void SpriteTextRenderFeature::beginRendering()
{
	m_lastFont = nullptr;
	m_cacheTexture = nullptr;

	for (auto& font : m_renderingFonts) {
		font->endCacheUsing();
	}
	m_renderingFonts.clear();
	//m_batchData.spriteOffset = 0;
	//m_batchData.spriteCount = 0;
	m_spriteCount = 0;
}

void SpriteTextRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
#ifdef LN_RLI_BATCH
	LN_UNREACHABLE();
#else
	if (m_mappedVertices) {
		// TODO: unmap (今は自動だけど、明示した方が安心かも)
	}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;
	batch->overrideTexture = m_cacheTexture;
	if (m_drawingSamplerState)
		batch->overrideSamplerState = m_drawingSamplerState;
	else
		batch->overrideSamplerState = detail::EngineDomain::graphicsManager()->defaultSamplerState();

	m_batchData.spriteOffset = m_batchData.spriteOffset + m_batchData.spriteCount;
	m_batchData.spriteCount = 0;

    // 次の draw で Submit 発生しないようにする
    m_cacheTexture = nullptr;
#endif
}

void SpriteTextRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);
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

FontCore* SpriteTextRenderFeature::resolveFontCore(Font* font, FontRequester* fontRequester, const CameraInfo* cameraInfo, const Matrix& transform) const
{
	float scale = 1.0f;	// TODO: DPI

	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
		// View との距離からフォントサイズをスケーリングしてみる。
		// TODO: Ortho だとうまく動かない
		auto d = (cameraInfo->viewPosition - transform.position());
		if (!Vector3::nearEqual(d, Vector3::Zero)) {
			scale = std::floor(Math::lerp(5.0f, 1.0f, std::min(d.length() / 15.0f, 1.0f)));
		}
	}

	auto result = RequestBatchResult::Staging;

	FontCore* newfontCore = nullptr;

	if (fontRequester) {
		newfontCore = fontRequester->resolveFontCore(scale);
	}
	else {
		newfontCore = FontHelper::resolveFontCore(font, scale);
	}

	return newfontCore;
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

SpriteTextRenderFeature::Batch* SpriteTextRenderFeature::endLayoutAndAcquireBatch(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	GraphicsContext* context,
	FontCore* newFontCore,
	const Matrix& transform)
{
	bool newBatch = false;

	// EndLayout (Glyph レイアウト結果を元に、テクスチャを生成する)
	newFontCore->getFontGlyphTextureCache(&m_cacheRequest);
	if (m_cacheTexture && m_cacheTexture != m_cacheRequest.texture) {
		newBatch = true;
	}

	m_cacheTexture = m_cacheRequest.texture;




	if (!newBatch) {
		if (batchList->equalsLastBatchState(this, batchState) && newFontCore == m_lastFont) {
		}
		else {
			newBatch = true;
		}
	}






	m_lastFont = newFontCore;




	Batch* batch;
	if (newBatch) {
		batch = batchList->addNewBatch<Batch>(this, batchState);
		batch->finalMaterial()->m_worldTransform = nullptr;	// VertexShade での World 座標変換は不要
		batch->data.spriteOffset = m_spriteCount;
		batch->data.spriteCount = 0;
		batch->overrideTexture = m_cacheTexture;
		if (m_drawingSamplerState)
			batch->overrideSamplerState = m_drawingSamplerState;
		else
			batch->overrideSamplerState = detail::EngineDomain::graphicsManager()->defaultSamplerState();

		m_renderingFonts.push_back(newFontCore);
	}
	else {
		batch = static_cast<Batch*>(batchList->lastBatch());
	}



	return batch;
}


//void SpriteTextRenderFeature::endLayout()
//{
//}

//RequestBatchResult SpriteTextRenderFeature::resolveCache(RenderFeatureBatchList* batchList, GraphicsContext* context)
//{
//	m_currentFont->getFontGlyphTextureCache(&m_cacheRequest);
//	if (m_cacheTexture && m_cacheTexture != m_cacheRequest.texture) {
//		submitBatch(context, batchList);
//		return RequestBatchResult::Submitted;
//	}
//	else {
//        m_cacheTexture = m_cacheRequest.texture;
//		return RequestBatchResult::Staging;
//	}
//}

void SpriteTextRenderFeature::buildSpriteList(Batch* batch, GraphicsContext* context)
{
	//size_t spriteCount = m_batchData.spriteOffset + m_batchData.spriteCount;
	size_t dataCount = m_glyphLayoutDataList.size();
	prepareBuffers(context, m_spriteCount + dataCount);

	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->writableData());

	Vector2 posOffset;
	if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
		auto area = renderAreaSize();
		posOffset = Vector2(-area.width * (m_drawingAnchor.x), -area.height * (1.0f - m_drawingAnchor.y));
		//posOffset = Vector2(-area.width * 0.5f, -area.height * 0.5f);
		//posOffset += Vector2(area.width * (m_drawingAnchor.x - 0.5f), area.height * (m_drawingAnchor.y - 0.5f));
		//posOffset.x -= area.width * 0.5f;
		//posOffset.y -= area.height * 0.5f;
	}

	auto srcTexture = m_cacheRequest.texture;
	Size texSizeInv(1.0f / srcTexture->width(), 1.0f / srcTexture->height());
	for (auto i = 0; i < dataCount; i++)
	{
		auto* vertices = m_mappedVertices + (m_spriteCount * 4);

		auto& data = m_glyphLayoutDataList[i];

		auto srcRect = m_cacheRequest.glyphs[i].info.srcRect;
		Rect uvSrcRect((float)srcRect.x, (float)srcRect.y, (float)srcRect.width, (float)srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		Rect dstRect(data.position + posOffset, (float)srcRect.width, (float)srcRect.height);
		addSprite(batch, vertices, data.transform, dstRect, uvSrcRect, data.color);

	}
}

void SpriteTextRenderFeature::addSprite(Batch* batch, Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color)
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	float signY = (m_drawingBaseDirection == SpriteBaseDirection::Basic2D) ? 1.0f : -1.0f;

	float lu = srcUVRect.getLeft();
	float tv = srcUVRect.getTop();
	float ru = srcUVRect.getRight();
	float bv = srcUVRect.getBottom();

	buffer[0].set(Vector3::transformCoord(Vector3(rect.getLeft(), rect.getTop() * signY, 0), transform), Vector3::UnitZ, Vector2(lu, tv), color);	// 左上
	buffer[1].set(Vector3::transformCoord(Vector3(rect.getLeft(), rect.getBottom() * signY, 0), transform), Vector3::UnitZ, Vector2(lu, bv), color);// 左下
	buffer[2].set(Vector3::transformCoord(Vector3(rect.getRight(), rect.getTop() * signY, 0), transform), Vector3::UnitZ, Vector2(ru, tv), color);	// 右上
	buffer[3].set(Vector3::transformCoord(Vector3(rect.getRight(), rect.getBottom() * signY, 0), transform), Vector3::UnitZ, Vector2(ru, bv), color);// 右下

	// pixel snap
	if (isPixelSnapEnabled()) {
		for (int i = 0; i < 4; i++) {
			buffer[i].position.x = std::round(buffer[i].position.x);
			buffer[i].position.y = std::round(buffer[i].position.y);
		}
	}

	m_spriteCount++;
	batch->data.spriteCount++;
}

} // namespace detail
} // namespace ln

