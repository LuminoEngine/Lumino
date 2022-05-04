#if 0
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KSpriteTextRenderFeature.hpp>
#include "../../../TextRendering/TextRenderingCache.hpp"
#include "../../../../../Graphics/src/GraphicsManager.hpp"

namespace ln {
namespace kanata {

SpriteTextRenderFeature::SpriteTextRenderFeature(detail::RenderingManager* manager)
    : m_currentCollector(nullptr)
    , m_material(nullptr) {
}

void SpriteTextRenderFeature::beginBatch(BatchCollector* collector, Material* material) {
    LN_DCHECK(!m_currentCollector);
    LN_DCHECK(collector);
    LN_DCHECK(material);
    m_currentCollector = collector;
    m_material = material;
}

void SpriteTextRenderFeature::endBatch(BatchCollector* collector) {
    LN_DCHECK(m_currentCollector);
    LN_DCHECK(m_currentCollector == collector);

    #if 0
    int32_t spriteCount = m_sprites.length();
    int32_t vertexCount = m_sprites.length() * 4;
    int32_t indexCount = m_sprites.length() * 6;
    auto view = m_currentCollector->allocateMeshBuffer(vertexCount, indexCount);

    // Put indices
    {
        auto* ib = view.indexData;
        int32_t idx = 0;
        int32_t i2 = 0;
        for (int32_t i = 0; i < spriteCount; ++i) {
            i2 = i * 6;
            idx = i * 4;
            ib[i2 + 0] = idx + 0;
            ib[i2 + 1] = idx + 1;
            ib[i2 + 2] = idx + 2;
            ib[i2 + 3] = idx + 2;
            ib[i2 + 4] = idx + 1;
            ib[i2 + 5] = idx + 3;
        }
    }

    // Put vertices
    for (int32_t i = 0; i < spriteCount; ++i) {
        putVertices(m_sprites[i], view.vertexData + (i * 4));
    }

    // Create batch
    {
        Batch* batch = m_currentCollector->newBatch<Batch>(1, m_material);
        batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
        batch->elemets2[0].indexBuffer = view.indexBuffer;
        batch->elemets2[0].firstIndex = view.firstIndex;
        batch->elemets2[0].firstVertex = view.vertexOffset;
        batch->elemets2[0].primitiveCount = indexCount / 3;
        batch->vertexLayout = m_currentCollector->standardVertexDeclaration();
        batch->primitiveTopology = PrimitiveTopology::TriangleList;
    }
    #endif

    m_currentCollector = nullptr;
    m_material = nullptr;
}

void SpriteTextRenderFeature::drawText(const detail::FormattedText* text, const Vector2& anchor, SpriteBaseDirection baseDirection, const Ref<SamplerState>& samplerState, const Matrix& transform) {
    LN_DCHECK(text);

    m_drawingFormattedText = text;
    m_drawingTransform = transform;
    m_drawingAnchor = anchor;
    m_drawingBaseDirection = baseDirection;
    m_drawingSamplerState = samplerState;

    detail::FontCore* fontCore = resolveFontCore(text->font, text->fontRequester, m_currentCollector->viewPoint(), transform);

    // 3D 空間に書く場合
    if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D && text->fontRequester) {
        detail::FontGlobalMetrics metrix;
        fontCore->getGlobalMetrics(&metrix);
        // float h = std::abs(metrix.descender - metrix.ascender);

        float s = metrix.virutalSpaceFactor * (static_cast<float>(text->fontRequester->size) / Font::DefaultSize);

        m_drawingTransform = transform.getRotationMatrix();
        m_drawingTransform.scale(s);
        m_drawingTransform.translate(transform.position());
    }

    beginLayout();
    TextLayoutEngine::layout(fontCore, text->text.c_str(), text->text.length(), text->area, 0, text->textAlignment);
    endLayout(fontCore, transform);
    buildSpriteList();
}

void SpriteTextRenderFeature::onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) {
    addLayoutedGlyphItem(ch, pos, m_drawingFormattedText->color, m_drawingTransform);
}

detail::FontCore* SpriteTextRenderFeature::resolveFontCore(Font* font, detail::FontRequester* fontRequester, const RenderViewPoint* viewInfo, const Matrix& transform) const {
    float scale = 1.0f; // TODO: DPI

    if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
        // View との距離からフォントサイズをスケーリングしてみる。
        // TODO: Ortho だとうまく動かない
        auto d = (viewInfo->viewPosition - transform.position());
        if (!Vector3::nearEqual(d, Vector3::Zero)) {
            scale = std::floor(Math::lerp(5.0f, 1.0f, std::min(d.length() / 15.0f, 1.0f)));
        }
    }

    auto result = RequestBatchResult::Staging;

    detail::FontCore* newfontCore = nullptr;

    if (fontRequester) {
        newfontCore = fontRequester->resolveFontCore(scale);
    }
    else {
        newfontCore = detail::FontHelper::resolveFontCore(font, scale);
    }

    return newfontCore;
}

void SpriteTextRenderFeature::beginLayout() {
    m_cacheRequest.glyphs.clear();
    m_glyphLayoutDataList.clear();
}

void SpriteTextRenderFeature::endLayout(detail::FontCore* newFontCore, const Matrix& transform) {
    // EndLayout (Glyph レイアウト結果を元に、テクスチャを生成する)
    {
        const auto& abstractCache = newFontCore->textRenderingCache();
        detail::TextRenderingCache* cache;
        if (abstractCache) {
            cache = static_cast<detail::TextRenderingCache*>(abstractCache.get());
        }
        else {
            auto c = makeRef<detail::TextRenderingCache>(newFontCore);
            newFontCore->setTextRenderingCache(c);
            cache = c;
        }
        cache->getFontGlyphTextureCache(&m_cacheRequest);
    }

    //if (m_cacheTexture && m_cacheTexture != m_cacheRequest.texture) {
    //    newBatch = true;
    //}

    m_cacheTexture = m_cacheRequest.texture;
}

void SpriteTextRenderFeature::addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform) {
    // TODO: Outline

    m_cacheRequest.glyphs.push_back({ codePoint });

    GlyphData data;
    data.transform = transform;
    data.position = pos;
    data.color = color;
    m_glyphLayoutDataList.push_back(data);
}

void SpriteTextRenderFeature::buildSpriteList() {
    int32_t spriteCount = m_glyphLayoutDataList.size();
    int32_t vertexCount = spriteCount * 4;
    int32_t indexCount = spriteCount * 6;
    auto view = m_currentCollector->allocateMeshBuffer(vertexCount, indexCount);

    // Write indices
    {
        auto* ib = view.indexData;
        int32_t idx = 0;
        int32_t i2 = 0;
        for (int32_t i = 0; i < spriteCount; ++i) {
            i2 = i * 6;
            idx = i * 4;
            ib[i2 + 0] = idx + 0;
            ib[i2 + 1] = idx + 1;
            ib[i2 + 2] = idx + 2;
            ib[i2 + 3] = idx + 2;
            ib[i2 + 4] = idx + 1;
            ib[i2 + 5] = idx + 3;
        }
    }

    // Write vertices
    {
        Vector2 posOffset;
        if (m_drawingBaseDirection != SpriteBaseDirection::Basic2D) {
            auto area = renderAreaSize();
            posOffset = Vector2(-area.width * (m_drawingAnchor.x), -area.height * (1.0f - m_drawingAnchor.y));
            // posOffset = Vector2(-area.width * 0.5f, -area.height * 0.5f);
            // posOffset += Vector2(area.width * (m_drawingAnchor.x - 0.5f), area.height * (m_drawingAnchor.y - 0.5f));
            // posOffset.x -= area.width * 0.5f;
            // posOffset.y -= area.height * 0.5f;
        }

        auto srcTexture = m_cacheRequest.texture;
        Size texSizeInv(1.0f / srcTexture->width(), 1.0f / srcTexture->height());
        for (int32_t i = 0; i < spriteCount; i++) {
            auto* vertices = view.vertexData + (i * 4);
            auto& data = m_glyphLayoutDataList[i];

            auto srcRect = m_cacheRequest.glyphs[i].info.srcRect;
            Rect uvSrcRect((float)srcRect.x, (float)srcRect.y, (float)srcRect.width, (float)srcRect.height);
            uvSrcRect.x *= texSizeInv.width;
            uvSrcRect.width *= texSizeInv.width;
            uvSrcRect.y *= texSizeInv.height;
            uvSrcRect.height *= texSizeInv.height;

            Rect dstRect(data.position + posOffset, (float)srcRect.width, (float)srcRect.height);
            addSprite(vertices, data.transform, dstRect, uvSrcRect, data.color);
        }
    }

    // Create batch
    {
        Batch* batch = m_currentCollector->newBatch<Batch>(1, m_material);
        batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
        batch->elemets2[0].indexBuffer = view.indexBuffer;
        batch->elemets2[0].firstIndex = view.firstIndex;
        batch->elemets2[0].firstVertex = view.vertexOffset;
        batch->elemets2[0].primitiveCount = indexCount / 3;
        batch->vertexLayout = m_currentCollector->standardVertexDeclaration();
        batch->primitiveTopology = PrimitiveTopology::TriangleList;
        batch->overrideTexture = m_cacheTexture;
        if (m_drawingSamplerState)
            batch->overrideSamplerState = m_drawingSamplerState;
        else
            batch->overrideSamplerState = detail::GraphicsManager::instance()->defaultSamplerState();

    }
}

void SpriteTextRenderFeature::addSprite(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color) {
    if (rect.isEmpty()) return; // 矩形がつぶれているので書く必要はない

    float signY = (m_drawingBaseDirection == SpriteBaseDirection::Basic2D) ? 1.0f : -1.0f;

    float lu = srcUVRect.getLeft();
    float tv = srcUVRect.getTop();
    float ru = srcUVRect.getRight();
    float bv = srcUVRect.getBottom();

    buffer[0].set(Vector3::transformCoord(Vector3(rect.getLeft(), rect.getTop() * signY, 0), transform), Vector3::UnitZ, Vector2(lu, tv), color);     // 左上
    buffer[1].set(Vector3::transformCoord(Vector3(rect.getLeft(), rect.getBottom() * signY, 0), transform), Vector3::UnitZ, Vector2(lu, bv), color);  // 左下
    buffer[2].set(Vector3::transformCoord(Vector3(rect.getRight(), rect.getTop() * signY, 0), transform), Vector3::UnitZ, Vector2(ru, tv), color);    // 右上
    buffer[3].set(Vector3::transformCoord(Vector3(rect.getRight(), rect.getBottom() * signY, 0), transform), Vector3::UnitZ, Vector2(ru, bv), color); // 右下

    // pixel snap
    if (isPixelSnapEnabled()) {
        for (int i = 0; i < 4; i++) {
            buffer[i].position.x = std::round(buffer[i].position.x);
            buffer[i].position.y = std::round(buffer[i].position.y);
        }
    }

    //m_spriteCount++;
    //batch->data.spriteCount++;
}

} // namespace kanata
} // namespace ln

#endif
