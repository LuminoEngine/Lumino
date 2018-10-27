
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>

namespace ln {

//=============================================================================
// SpriteComponent

SpriteComponent::SpriteComponent()
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::initialize()
{
    VisualComponent::initialize();
    m_material = newObject<Material>();
}

void SpriteComponent::setTexture(Texture* texture)
{
    m_material->setMainTexture(texture);
}

void SpriteComponent::setSize(const Size& size)
{
    LN_NOTIMPLEMENTED();
}

void SpriteComponent::onRender(RenderingContext* context)
{
    context->drawSprite(
        Matrix(), Size(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::White,
        SpriteBaseDirection::ZMinus, BillboardType::None, m_material);
}

} // namespace ln

