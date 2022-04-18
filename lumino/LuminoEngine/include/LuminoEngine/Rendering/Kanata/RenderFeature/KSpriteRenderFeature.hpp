#pragma once
#include "../Common.hpp"

namespace ln {
namespace kanata {

struct SpriteData {
    Matrix transform;
    Vector2 size;
    Vector2 anchorRatio;
    Rect srcRect;
    Color color;
    SpriteBaseDirection baseDirection;
    BillboardType billboardType;
    detail::SpriteFlipFlags flipFlags;
};

class SpriteRenderFeature : public URefObject {
public:
    SpriteRenderFeature(detail::RenderingManager* manager);

    void beginBatch(BatchCollector* collector, Material* material);
    void endBatch(BatchCollector* collector);

    // srcRect は UV 座標系上の値を設定する。 (通常0～1)
    // 以前は 2D メインな Sprite なのでピクセル単位で指定していたが、
    // 考え方として他の RenderFeature と同様に「最終的な描画に使うメッシュを作る」方針で統一したい。
    void drawSprite(const SpriteData& sprite);

private:
    void putVertices(const SpriteData& sprite, Vertex* vertices) const;

    BatchCollector* m_currentCollector;
    Material* m_material;
    Array<SpriteData> m_sprites;
};

} // namespace kanata
} // namespace ln

