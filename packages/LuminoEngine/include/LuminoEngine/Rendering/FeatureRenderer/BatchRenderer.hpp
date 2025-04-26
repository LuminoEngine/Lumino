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

    void drawSprite(const Matrix& worldTransform, const SpriteData& sprite);

    BatchRenderer();
    Result_deprecated<> init();
private:

    CommandList* m_commandList;
    detail::InstructionBatchProxy* m_currentProxy;

    friend class detail::RenderingManager;
};

} // namespace ln

