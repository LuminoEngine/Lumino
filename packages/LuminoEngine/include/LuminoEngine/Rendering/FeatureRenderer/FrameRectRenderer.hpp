#pragma once
#include "../Common.hpp"
#include "../Drawing.hpp"

namespace ln {

class FrameRectRenderer : public Object {
public:
    void begin(Material* material);
    void end();

    void draw(
        const Rect& rect,
        const Matrix& worldTransform,
        const Thickness& borderThickness,
        const Rect& srcRect,
        Sprite9DrawMode wrapMode);

private:
    FrameRectRenderer();
};

} // namespace ln

