
#pragma once
#include "../Common.hpp"

namespace ln {
//namespace detail {
//class DrawSpriteSFBatchProxy;
//}

/**
 * SpriteRenderer
 * 
 * なぜ 3DSprite の原点は左下で、2DSprite の原点は左上なのか？
 * ----------
 * テクスチャ座標に合わせる、というより、ワールド上に配置される Square-Primitive の原点を指定したい、という考えでデザインしたため。
 * 
 * また、ワールド座標系と合わせているゲームエンジンが多いため。
 * - Unity, UE4, Cocos2d-x は左下が原点 (Cocos2d-x は 2D 用であるが、Y-Up)
 * - PixiJS は左上が原点 (座標系は一般的な 2DCanvas 同様、Y-Down)
 * 
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
    Result init();
    void flush();

    static const size_t MaxBatchSprites = 16383;

    CommandList* m_commandList;
    Material* m_material;
    std::vector<SpriteData> m_sprites;

    friend class detail::RenderingManager;
};

} // namespace ln

