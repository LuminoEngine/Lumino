
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UISprite.hpp>
#include "../Rendering/SpriteRenderFeature.hpp"

namespace ln {

//==============================================================================
// UISprite

Ref<UISprite> UISprite::create()
{
    return newObject<UISprite>();
}

Ref<UISprite> UISprite::create(Texture* texture)
{
    return newObject<UISprite>(texture);
}

UISprite::UISprite()
{
}

void UISprite::init()
{
    UIElement::init();
    m_material = newObject<Material>();
    m_size = Size(-1, -1);
    m_sourceRect = Rect(0, 0, -1, -1);
    setBlendMode(BlendMode::Alpha);
}

void UISprite::init(Texture* texture)
{
    init();
    setTexture(texture);
}

void UISprite::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

void UISprite::setSourceRect(const Rect & rect)
{
    m_sourceRect = rect;
}

void UISprite::setSourceRect(float x, float y, float width, float height)
{
    m_sourceRect = Rect(x, y, width, height);
}

const Rect & UISprite::sourceRect() const
{
    return m_sourceRect;
}

void UISprite::onRender(UIRenderingContext* context)
{
    Size renderSize;
    Rect renderSourceRect;
    detail::SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(
        m_material->mainTexture(), m_size, m_sourceRect, &renderSize, &renderSourceRect);

    context->drawSprite(
        Matrix(), renderSize, Vector2(0, 0), renderSourceRect, Color::White,
        SpriteBaseDirection::Basic2D, BillboardType::None, m_material);
}

} // namespace ln

