
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
// InternalSpriteTextRender

InternalSpriteTextRender::InternalSpriteTextRender()
	: m_manager(nullptr)
	, m_vertexDeclaration(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_buffersReservedSpriteCount(0)
	, m_stagingSpriteOffset(0)
	, m_stagingSpriteCount(0)
{
}

void InternalSpriteTextRender::init(RenderingManager* manager)
{
    m_manager = manager;
	m_vertexDeclaration = m_manager->standardVertexDeclarationRHI();

    prepareBuffers(512);
}

void InternalSpriteTextRender::render(ICommandList* context, const GlyphData* dataList, uint32_t dataCount, ITexture* glyphsTexture)
{
	// GPU 最適化のため、できるだけ map 範囲が被らないように循環バッファを使った実装にする。
	// 特に OpenGL (Radeon HD 8490) では、次のネイティブコマンド実行 (glFlush など) までの間に
	// 線形の実装 (flush のたびに先頭から書き直し=未送信の頂点バッファ領域を上書きする) では平均 471usec, 循環バッファの実装では 5usec と、100 倍近い差が出た。

	prepareBuffers(dataCount);

	int32_t iData = 0;
	while (iData < dataCount)
	{
		// 今回のループで描画できる最大数
		uint32_t maxCount = m_buffersReservedSpriteCount - m_stagingSpriteOffset;

		// 実際に描画する数
		uint32_t count = std::min((dataCount - iData), maxCount);

		// map
		uint32_t consumedSize = count * sizeof(Vertex) * 4;
		uint32_t nextVertexBufferOffset = m_stagingSpriteOffset * sizeof(Vertex) * 4;
		Vertex* buffer = reinterpret_cast<Vertex*>(context->map(m_vertexBuffer, nextVertexBufferOffset, consumedSize));

		// write
		ITexture* srcTexture = glyphsTexture;
		Size texSizeInv(1.0f / srcTexture->realSize().width, 1.0f / srcTexture->realSize().height);
		for (int i = 0; i < count; ++iData, ++i)
		{
			const GlyphData& data = dataList[iData];
			Rect uvSrcRect((float)data.srcRect.x, (float)data.srcRect.y, (float)data.srcRect.width, (float)data.srcRect.height);
			uvSrcRect.x *= texSizeInv.width;
			uvSrcRect.width *= texSizeInv.width;
			uvSrcRect.y *= texSizeInv.height;
			uvSrcRect.height *= texSizeInv.height;

			Rect dstRect(data.position, (float)data.srcRect.width, (float)data.srcRect.height);
			putRectangle(buffer + (i * 4), data.transform, dstRect, uvSrcRect, data.color);
		}

		// unmap
		context->unmap(m_vertexBuffer);
		m_stagingSpriteCount += count;

		// flush if buffer filling.
		if (m_stagingSpriteOffset + m_stagingSpriteCount >= m_buffersReservedSpriteCount) {
			flush(context, glyphsTexture, m_stagingSpriteOffset, m_stagingSpriteCount);
			m_stagingSpriteOffset = 0;
			m_stagingSpriteCount = 0;
		}
	}

	flush(context, glyphsTexture, m_stagingSpriteOffset, m_stagingSpriteCount);
	m_stagingSpriteOffset += m_stagingSpriteCount;
	m_stagingSpriteCount = 0;
}

void InternalSpriteTextRender::prepareBuffers(int spriteCount)
{
	IGraphicsDevice* context = m_manager->graphicsManager()->deviceContext();

	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		m_vertexBuffer = context->createVertexBuffer(GraphicsResourceUsage::Dynamic, vertexBufferSize, nullptr);

		size_t indexBufferSize = spriteCount * 6;
		std::vector<size_t> indexBuf(sizeof(uint16_t) * indexBufferSize, false);
		uint16_t* ib = (uint16_t*)indexBuf.data();
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
		m_indexBuffer = context->createIndexBuffer(
			GraphicsResourceUsage::Dynamic, IndexBufferFormat::UInt16,
			spriteCount * 6, ib);

		m_buffersReservedSpriteCount = spriteCount;
		m_stagingSpriteOffset = 0;
		m_stagingSpriteCount = 0;
	}
}

void InternalSpriteTextRender::putRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color)
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.getLeft();
	float tv = srcUVRect.getTop();
	float ru = srcUVRect.getRight();
	float bv = srcUVRect.getBottom();

	buffer[0].color = color;
	buffer[0].position.set(rect.getLeft(), rect.getTop(), 0);
	buffer[0].position.transformCoord(transform);
	buffer[0].uv.set(lu, tv);	// 左上
    buffer[0].normal = Vector3::UnitZ;

	buffer[1].color = color;
	buffer[1].position.set(rect.getRight(), rect.getTop(), 0);
	buffer[1].position.transformCoord(transform);
	buffer[1].uv.set(ru, tv);	// 右上
    buffer[1].normal = Vector3::UnitZ;

	buffer[2].color = color;
	buffer[2].position.set(rect.getLeft(), rect.getBottom(), 0);
	buffer[2].position.transformCoord(transform);
	buffer[2].uv.set(lu, bv);	// 左下
    buffer[2].normal = Vector3::UnitZ;

	buffer[3].color = color;
	buffer[3].position.set(rect.getRight(), rect.getBottom(), 0);
	buffer[3].position.transformCoord(transform);
	buffer[3].uv.set(ru, bv);	// 右下
    buffer[3].normal = Vector3::UnitZ;

    // pixel snap
    if (1) {
        for (int i = 0; i < 4; i++) {
            buffer[i].position.x = std::round(buffer[i].position.x);
            buffer[i].position.y = std::round(buffer[i].position.y);
        }
    }
}

void InternalSpriteTextRender::flush(ICommandList* context, ITexture* glyphsTexture, uint32_t startSprite, uint32_t spriteCount)
{
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	context->drawPrimitiveIndexed(startSprite * 6, spriteCount * 2);
}

//==============================================================================
// SpriteTextRenderFeature

SpriteTextRenderFeature::SpriteTextRenderFeature()
	: m_internal(nullptr)
	, m_drawingFont(nullptr)
{
}

void SpriteTextRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
    m_internal = makeRef<InternalSpriteTextRender>();
    m_internal->init(manager);
}

void SpriteTextRenderFeature::drawText(GraphicsContext* context, const FormattedText* text, const Matrix& transform)
{
	// validation. 先に updateRenderParameters によってセットされているはず
	if (LN_ENSURE(m_drawingFont)) return;

	m_drawingGraphicsContext = context;
	m_drawingFormattedText = text;
	//m_drawingFontGlyphCache = m_drawingFont->getFontGlyphTextureCache();
    m_drawingTransform = transform;

	TextLayoutEngine::layout(m_drawingFont, text->text.c_str(), text->text.length(), Rect(0, 0, text->area), 0, text->textAlignment);

	m_drawingFormattedText = nullptr;
}

void SpriteTextRenderFeature::drawChar(GraphicsContext* context, uint32_t codePoint, const Color& color, const Matrix& transform)
{
	addLayoutedGlyphItem(codePoint, Vector2::Zero, color, transform);
}

void SpriteTextRenderFeature::drawFlexGlyphRun(GraphicsContext* context, const FlexGlyphRun* glyphRun, const Matrix& transform)
{
	for (int i = 0; i < glyphRun->glyphCount; i++) {
		auto& glyph = glyphRun->owner->glyphs()[glyphRun->startIndex + i];
		Color color = glyphRun->color;
		color.a *= glyph.opacity;
		if (glyph.timeOffset <= glyphRun->owner->time() && color.a > 0.0f) {
			addLayoutedGlyphItem(glyph.codePoint, glyph.pos.xy(), color, transform);
		}
	}
}

void SpriteTextRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	flushInternal(context, m_drawingFont->getFontGlyphTextureCache());

	// TODO: add batch
}

void SpriteTextRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}

void SpriteTextRenderFeature::updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo)
{
    auto* e = static_cast<AbstractSpriteTextDrawElement*>(element);
	FontCore* fontCore = e->getFontCore(cameraInfo.dpiScale);

	// validation. 一連の draw ~ flush までは同じフォントが使い続けられなければならない
	if (m_drawingFont) {
		if (LN_ENSURE(m_drawingFont == fontCore)) {
			return;
		}
	}

	m_drawingFont = fontCore;

    // グリフのマスクテクスチャを MainTexture として使う
    detail::SubsetInfo localSubsetInfo = subsetInfo;
    localSubsetInfo.materialTexture = fontCore->getFontGlyphTextureCache()->glyphsFillTexture();

    RenderFeature::updateRenderParameters(element, tech, cameraInfo, elementInfo, localSubsetInfo);
}

void SpriteTextRenderFeature::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
	//// 必要なグリフを探す。lookupGlyphInfo() の中で、テクスチャにグリフビットマップが blt される。
	//CacheGlyphInfo info;
	//bool flush;
	//m_drawingFont->getFontGlyphTextureCache()->lookupGlyphInfo(ch, &info, &flush);
	//if (flush) {
	//	flushInternal(m_drawingGraphicsContext, m_drawingFont->getFontGlyphTextureCache());
	//}

	//// TODO: Outline

	//InternalSpriteTextRender::GlyphData data;
 //   data.transform = m_drawingTransform;
	//data.position = pos;
	//data.color = m_drawingFormattedText->color;
	//data.srcRect = info.srcRect;
	//data.outlineOffset = info.outlineOffset;
	//m_glyphLayoutDataList.push_back(data);


	addLayoutedGlyphItem(ch, pos, m_drawingFormattedText->color, m_drawingTransform);
}

void SpriteTextRenderFeature::addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform)
{
	assert(m_drawingFont);

	CacheGlyphInfo info;
	bool flush;
	m_drawingFont->getFontGlyphTextureCache()->lookupGlyphInfo(codePoint, &info, &flush);
	if (flush) {
		flushInternal(m_drawingGraphicsContext, m_drawingFont->getFontGlyphTextureCache());
	}
	// TODO: Outline


	InternalSpriteTextRender::GlyphData data;
	data.transform = transform;
	data.position = pos;
	data.color = color;
	data.srcRect = info.srcRect;
	data.outlineOffset = info.outlineOffset;
	m_glyphLayoutDataList.push_back(data);
}

void SpriteTextRenderFeature::flushInternal(GraphicsContext* context, FontGlyphTextureCache* cache)
{
	if (LN_REQUIRE(context)) return;
	if (!cache) return;
	if (m_glyphLayoutDataList.empty()) return;



	size_t dataCount = m_glyphLayoutDataList.size();
	RenderBulkData dataListData(&m_glyphLayoutDataList[0], sizeof(InternalSpriteTextRender::GlyphData) * dataCount);

	// Texture::blit で転送されるものを Flush する
	ITexture* glyphsTexture = nullptr;
	{
		// TODO: Debug ON でだが、新しい文字を作るときに 1ms くらいかかっているようだ。要調査
		glyphsTexture = GraphicsResourceInternal::resolveRHIObject<ITexture>(context, cache->glyphsFillTexture(), nullptr);
	}


	GraphicsManager* manager = m_internal->manager()->graphicsManager();
	ICommandList* c = GraphicsContextInternal::commitState(context);
	LN_ENQUEUE_RENDER_COMMAND_5(
		SpriteTextRenderFeature_flushInternal, context,
		InternalSpriteTextRender*, m_internal,
		ICommandList*, c,
		RenderBulkData, dataListData,
		int, dataCount,
		Ref<ITexture>, glyphsTexture,
		{
			m_internal->render(
				c,
				(InternalSpriteTextRender::GlyphData*)dataListData.data(),
				dataCount,
				glyphsTexture);
		});

	m_drawingFont->getFontGlyphTextureCache()->onFlush();
	m_glyphLayoutDataList.clear();
	m_drawingFont = nullptr;
}

} // namespace detail
} // namespace ln

