#pragma once
#include "../Common.hpp"

namespace ln {
namespace detail {
class InstructionBatchProxy;
}

/**
 *
 * @note
 * 従来、 SpriteRenderer だったものを置き換えたもの。
 */
class BatchRenderer : public Object {
public:
    static BatchRenderer* get();

    void begin(CommandList* commandList, Material* material);
    void end();
    Material* currentMaterial() const;

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

    BatchRenderer();
    Result_deprecated<> init();
private:

    CommandList* m_commandList;
    detail::InstructionBatchProxy* m_currentProxy;

    friend class detail::RenderingManager;
};

} // namespace ln

