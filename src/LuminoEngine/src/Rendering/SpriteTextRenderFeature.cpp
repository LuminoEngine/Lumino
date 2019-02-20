﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Font/FontCore.hpp"
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
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
	, m_spriteCount(0)
	, m_buffersReservedSpriteCount(0)
{
}

void InternalSpriteTextRender::init(RenderingManager* manager)
{
    m_manager = manager;
	m_vertexDeclaration = manager->standardVertexDeclaration()->resolveRHIObject();

    prepareBuffers(512);
}

void InternalSpriteTextRender::render(IGraphicsDeviceContext* context, const GlyphData* dataList, int dataCount, ITexture* glyphsTexture, const BrushData& brushData)
{
	m_spriteCount = 0;
	prepareBuffers(dataCount);
	Vertex* buffer = reinterpret_cast<Vertex*>(m_vertexBuffer->map());

	Color color = brushData.color;
	ITexture* srcTexture = glyphsTexture;
	Size texSizeInv(1.0f / srcTexture->realSize().width, 1.0f / srcTexture->realSize().height);
	for (int i = 0; i < dataCount; ++i)
	{
		const GlyphData& data = dataList[i];
		Rect uvSrcRect((float)data.srcRect.x, (float)data.srcRect.y, (float)data.srcRect.width, (float)data.srcRect.height);
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		Rect dstRect(data.position, (float)data.srcRect.width, (float)data.srcRect.height);
		internalDrawRectangle(buffer + (i * 4), data.transform, dstRect, uvSrcRect, color);
	}

	m_vertexBuffer->unmap();
	flush(context, glyphsTexture);
}

void InternalSpriteTextRender::prepareBuffers(int spriteCount)
{
	IGraphicsDeviceContext* context = m_manager->graphicsManager()->deviceContext();

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
	}
}

void InternalSpriteTextRender::internalDrawRectangle(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color)
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

	m_spriteCount++;
}

void InternalSpriteTextRender::flush(IGraphicsDeviceContext* context, ITexture* glyphsTexture)
{
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	context->drawPrimitiveIndexed(0, m_spriteCount * 2);

    m_spriteCount = 0;
}

//==============================================================================
// SpriteTextRenderFeature

SpriteTextRenderFeature::SpriteTextRenderFeature()
	: m_internal(nullptr)
{
}

void SpriteTextRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
    m_internal = makeRef<InternalSpriteTextRender>();
    m_internal->init(manager);
}

void SpriteTextRenderFeature::drawText(GraphicsContext* context, const FormattedText* text)
{
	m_drawingGraphicsContext = context;
	m_drawingFont = FontHelper::resolveFontCore(text->font);
	m_drawingFontGlyphCache = m_drawingFont->getFontGlyphTextureCache();

	TextLayoutEngine::layout(m_drawingFont, text->text.c_str(), text->text.length(), Rect(0, 0, text->area), 0, text->textAlignment);
}

void SpriteTextRenderFeature::flush(GraphicsContext* context)
{
	flushInternal(context, m_drawingFontGlyphCache);
}

void SpriteTextRenderFeature::updateRenderParameters(detail::RenderDrawElement* element, ShaderTechnique* tech, const detail::CameraInfo& cameraInfo, const detail::ElementInfo& elementInfo, const detail::SubsetInfo& subsetInfo)
{
    auto* e = static_cast<DrawTextElement*>(element);
    FontCore* font = FontHelper::resolveFontCore(e->formattedText->font);

    // グリフのマスクテクスチャを MainTexture として使う
    detail::SubsetInfo localSubsetInfo = subsetInfo;
    localSubsetInfo.materialTexture = font->getFontGlyphTextureCache()->glyphsFillTexture();

    RenderFeature::updateRenderParameters(element, tech, cameraInfo, elementInfo, localSubsetInfo);
}

void SpriteTextRenderFeature::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size)
{
	// 必要なグリフを探す。lookupGlyphInfo() の中で、テクスチャにグリフビットマップが blt される。
	CacheGlyphInfo info;
	bool flush;
	m_drawingFontGlyphCache->lookupGlyphInfo(ch, &info, &flush);
	if (flush) {
		flushInternal(m_drawingGraphicsContext, m_drawingFontGlyphCache);
	}

	// TODO: Outline

	InternalSpriteTextRender::GlyphData data;
	data.transform = Matrix();	// TODO: 文字単位アニメーション //transform;
	data.position = pos;
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
	ITexture* glyphsTexture = GraphicsResourceHelper::resolveRHIObject<ITexture>(cache->glyphsFillTexture());

	InternalSpriteTextRender::BrushData brushData;
	brushData.color = Color::Blue;

	GraphicsManager* manager = m_internal->manager()->graphicsManager();
	IGraphicsDeviceContext* deviceContext = context->commitState();
	LN_ENQUEUE_RENDER_COMMAND_6(
		SpriteTextRenderFeature_flushInternal, manager,
		InternalSpriteTextRender*, m_internal,
		IGraphicsDeviceContext*, deviceContext,
		RenderBulkData, dataListData,
		int, dataCount,
		Ref<ITexture>, glyphsTexture,
		InternalSpriteTextRender::BrushData, brushData,
		{
			m_internal->render(
				deviceContext,
				(InternalSpriteTextRender::GlyphData*)dataListData.data(),
				dataCount,
				glyphsTexture,
				brushData);
		});

    m_drawingFontGlyphCache->onFlush();
	m_glyphLayoutDataList.clear();
	m_drawingFont = nullptr;
	m_drawingFontGlyphCache = nullptr;
}

} // namespace detail
} // namespace ln
