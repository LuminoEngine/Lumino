
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UISprite.hpp>
#include "../Rendering/SpriteRenderFeature.hpp"

namespace ln {

//==============================================================================
// UISprite

UISprite::UISprite()
{
}

void UISprite::initialize()
{
    UIElement::initialize();
    m_material = newObject<Material>();
    m_size = Size(-1, -1);
    m_sourceRect = Rect(0, 0, -1, -1);
}

void UISprite::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
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

    //context->drawSprite(
    //    Matrix(), Size(200, 100), Vector2(0, 0), Rect(0, 0, 1, 1), Color::White,
    //    SpriteBaseDirection::Basic2D, BillboardType::None, m_material);
}

} // namespace ln

