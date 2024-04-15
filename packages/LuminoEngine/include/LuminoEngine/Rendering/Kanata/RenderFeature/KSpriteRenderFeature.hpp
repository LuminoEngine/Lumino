#pragma once
#include "../Common.hpp"

namespace ln {
namespace kanata {

class SpriteRenderFeature : public URefObject {
public:
    SpriteRenderFeature(detail::RenderingManager* manager);

    [[deprecated]]
    void beginBatch(BatchCollector* collector, Material* material);
    [[deprecated]]
    void endBatch(BatchCollector* collector);

    // srcRect は UV 座標系上の値を設定する。 (通常0～1)
    // 以前は 2D メインな Sprite なのでピクセル単位で指定していたが、
    // 考え方として他の RenderFeature と同様に「最終的な描画に使うメッシュを作る」方針で統一したい。
    [[deprecated]]
    void drawSprite(const SpriteData& sprite);

    void drawSpritesDirect(BatchCollector* collector, Material* material, const SpriteData* sprite, int32_t count);

private:
    void flush(const SpriteData* sprite, int32_t spriteCount);
    void putVertices(const SpriteData& sprite, Vertex* vertices) const;

    BatchCollector* m_currentCollector;
    Material* m_material;
    Array<SpriteData> m_sprites;
};

} // namespace kanata
} // namespace ln

