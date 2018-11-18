
#include "Internal.hpp"
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>

namespace ln {

//==============================================================================
// Sprite

Ref<Sprite> Sprite::create(float width, float height, Texture* texture)
{
    return newObject<Sprite>(width, height, texture);
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::initialize()
{
    WorldObject::initialize();
    m_component = newObject<SpriteComponent>();
    addComponent(m_component);
}

void Sprite::initialize(float width, float height, Texture* texture)
{
    initialize();
    setSize(Size(width, height));
    setTexture(texture);
}

void Sprite::setTexture(Texture* texture)
{
    m_component->setTexture(texture);
}

void Sprite::setSize(const Size& size)
{
    m_component->setSize(size);
}

} // namespace ln

