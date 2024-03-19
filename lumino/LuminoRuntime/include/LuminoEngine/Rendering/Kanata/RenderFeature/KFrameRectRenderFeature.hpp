#pragma once
#include "../Common.hpp"
#include "../../Drawing.hpp"

namespace ln {
namespace kanata {

class FrameRectRenderFeature : public URefObject {
public:
    FrameRectRenderFeature(detail::RenderingManager* manager);

    void beginBatch(BatchCollector* collector, Material* material);
    void endBatch(BatchCollector* collector);

    void draw(
        const Rect& rect,
        const Matrix& worldTransform,
        const Thickness& borderThickness,
        const Rect& srcRect,
        Sprite9DrawMode wrapMode);

    void putRectangleStretch(const Rect& rect, const Rect& srcUVRect);
    void putRectangleTiling(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect);
    void putRectangle(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, Sprite9DrawMode wrapMode);
    void putFrameRectangle(const Rect& rect, const Thickness& borderThickness, Rect srcRect, Sprite9DrawMode wrapMode);
    void addSprite(const Rect& rect, const Rect& srcUVRect);

private:
    struct SpriteData {
        Rect rect;
        Rect srcUVRect;
    };

    BatchCollector* m_currentCollector;
    Material* m_material;
    Size m_srcTextureSize;
    const Matrix* m_worldTransform;
    Array<SpriteData> m_sprites;
};

} // namespace kanata
} // namespace ln

