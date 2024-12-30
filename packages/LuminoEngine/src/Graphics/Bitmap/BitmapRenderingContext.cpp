#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/Font/detail/TextLayoutEngine.hpp>
#include <LuminoEngine/Graphics/Bitmap/Bitmap.hpp>
#include <LuminoEngine/Graphics/Bitmap/BitmapRenderingContext.hpp>
#include "BitmapEncoding.hpp"

namespace ln {

BitmapRenderingContext::BitmapRenderingContext()
    : m_texture(nullptr) {
}

BitmapRenderingContext::~BitmapRenderingContext() {
}

void BitmapRenderingContext::drawText(
    const StringView& text,
    const Rect& rect,
    Font* font,
    const Color& color,
    TextAlignment alignment,
    TextDrawMode drawing,
    float strokeWidth) {
    if (LN_ASSERT(font)) return;
    Bitmap2D* bitmap = getBitmap();
    if (!bitmap) return;
    detail::BitmapTextRenderer renderer;
    renderer.render(bitmap, text, rect, font, color, alignment, drawing, strokeWidth);
}

MaybeResult BitmapRenderingContext::init(Texture2D* texture) {
    if (LN_ASSERT(texture)) return err();
    m_texture = texture;
    return ok();
}

Bitmap2D* BitmapRenderingContext::getBitmap() {
    if (m_texture) {
        return m_texture->map(MapMode::Write);
    }
    return nullptr;
}

} // namespace ln
