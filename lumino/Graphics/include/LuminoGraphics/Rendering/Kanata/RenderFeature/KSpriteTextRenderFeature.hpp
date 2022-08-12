#pragma once
#include "../../../Font/Common.hpp"
#include <LuminoGraphics/Font/detail/TextLayoutEngine.hpp>
#include "../../../../../src/TextRendering/FontGlyphCache.hpp"
#include "../Common.hpp"

namespace ln {
namespace kanata {

class SpriteTextRenderFeature
    : public URefObject
    , public detail::TextLayoutEngine {
public:
    SpriteTextRenderFeature(detail::RenderingManager* manager);

    void beginBatch(BatchCollector* collector, Material* material);
    void endBatch(BatchCollector* collector);
    void drawText(const detail::FormattedText* text, const Vector2& anchor, SpriteBaseDirection baseDirection, const Ref<SamplerState>& samplerState, const Matrix& transform);

protected:
    // TextLayoutEngine interface
    void onPlacementGlyph(UTF32 ch, const Vector2& pos, const Size& size) override;

private:
    struct GlyphData {
        Matrix transform;
        Vector2 position;
        Color color;
    };

    bool isPixelSnapEnabled() const { return m_drawingBaseDirection == SpriteBaseDirection::Basic2D; }
    detail::FontCore* resolveFontCore(Font* font, detail::FontRequester* fontRequester, const RenderViewPoint* viewInfo, const Matrix& transform) const;
    void beginLayout();
    void endLayout(detail::FontCore* newFontCore, const Matrix& transform);
    void addLayoutedGlyphItem(uint32_t codePoint, const Vector2& pos, const Color& color, const Matrix& transform);
    void buildSpriteList();
    void addSprite(Vertex* buffer, const Matrix& transform, const Rect& rect, const Rect& srcUVRect, const Color& color);

    BatchCollector* m_currentCollector;
    Material* m_material;

    detail::FontGlyphTextureCacheRequest m_cacheRequest;  // 1フレーム中繰り返し使う可変バッファ
    std::vector<GlyphData> m_glyphLayoutDataList; // 1フレーム中繰り返し使う可変バッファ
    Texture2D* m_cacheTexture;

    // for onPlacementGlyph
    const detail::FormattedText* m_drawingFormattedText;
    Matrix m_drawingTransform;
    Vector2 m_drawingAnchor;
    SpriteBaseDirection m_drawingBaseDirection = SpriteBaseDirection::Basic2D;
    SamplerState* m_drawingSamplerState = nullptr;
};

} // namespace kanata
} // namespace ln

