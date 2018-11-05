
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UISprite.hpp>

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
}

void UISprite::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

void UISprite::onRender(UIRenderingContext* context)
{
	context->setBlendMode(BlendMode::Add);
	context->blit(m_material);

    context->drawSprite(
        Matrix(), Size(200, 100), Vector2(0, 0), Rect(0, 0, 1, 1), Color::White,
        SpriteBaseDirection::Basic2D, BillboardType::None, m_material);


	//context->drawSprite(
	//	Matrix(), Size(20, 10), Vector2(0, 0), Rect(0, 0, 1, 1), Color::White,
	//	SpriteBaseDirection::Basic2D, BillboardType::None, m_material);

}

} // namespace ln

