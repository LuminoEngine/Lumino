
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

Ref<Sprite> Sprite::create(SpriteFrameSet* frameSet)
{
	return newObject<Sprite>(frameSet);
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::init()
{
    VisualObject::init();
    m_component = newObject<SpriteComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Sprite::init(float width, float height, Texture* texture)
{
    init();
    setSize(Size(width, height));
    setTexture(texture);
}

void Sprite::init(SpriteFrameSet* frameSet)
{
	init();
	setFrameSet(frameSet);
}

void Sprite::setTexture(Texture* texture)
{
    m_component->setTexture(texture);
}

void Sprite::setSize(const Size& size)
{
    m_component->setSize(size);
}

void Sprite::setSourceRect(const Rect& rect)
{
    m_component->setSourceRect(rect);
}

void Sprite::setSourceRect(float x, float y, float width, float height)
{
    setSourceRect(Rect(x, y, width, height));
}

void Sprite::setFrameSet(SpriteFrameSet* value)
{
	m_component->setFrameSet(value);
}

void Sprite::setFrameIndex(int index)
{
	m_component->setFrameIndex(index);
}

const Rect& Sprite::sourceRect() const
{
    return m_component->sourceRect();
}

} // namespace ln

