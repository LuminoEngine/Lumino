
#pragma once
#include "../Common.hpp"

namespace ln {
//namespace detail {
//class DrawSpriteSFBatchProxy;
//}

/**
 * SpriteRenderer
 */
class SpriteRenderer : public Object {
public:
    static SpriteRenderer* get();

    void begin(RenderingContext* renderingContext, Material* material);
    void begin(CommandList* commandList, Material* material);
    void end();

    void drawSprite(const SpriteData& sprite);
    void drawSprite(
        const Matrix& transform,
        const Size& size,
        const Vector2& anchorRatio,
        const Rect& srcUVRect,
        const Color& color,
        SpriteBaseDirection baseDirection,
        BillboardType billboardType,
        const Flags<SpriteFlipFlags>& flipFlags);

private:
    SpriteRenderer();
    Result<> init();
    void flush();

    static const size_t MaxBatchSprites = 16383;

    CommandList* m_commandList;
    Material* m_material;
    std::vector<SpriteData> m_sprites;

    friend class detail::RenderingManager;
};

} // namespace ln

