
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>
#include <LuminoEngine/Scene/World.hpp>

namespace ln {

//==============================================================================
// Sprite

Sprite::Builder& Sprite::Builder::texture(Texture* value) noexcept
{
    m_texture = value;
    return *this;
}

Sprite::Builder& Sprite::Builder::pixelsParUnit(float value) noexcept
{
    m_pixelsParUnit = value;
    return *this;
}

Ref<Sprite> Sprite::Builder::build()
{
    return makeObject<Sprite>(*this);
}

Ref<Sprite> Sprite::Builder::buildInMainWorld()
{
    auto ptr = build();
    detail::EngineDomain::mainWorld()->addObject(ptr);
    return ptr;
}

//==============================================================================
// Sprite

LN_OBJECT_IMPLEMENT(Sprite, VisualObject) {
    context->registerType<Sprite>({});   // TODO: これ必須なのはちょっと忘れやすい・・
}

Ref<Sprite> Sprite::create(Texture* texture, float width, float height)
{
    return makeObject<Sprite>(texture, width, height);
}

Ref<Sprite> Sprite::create(SpriteFrameSet* frameSet)
{
	return makeObject<Sprite>(frameSet);
}

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

Result Sprite::init(const Builder& builder)
{
    init();
    setTexture(builder.m_texture);
    setPixelsParUnit(builder.m_pixelsParUnit);
    return true;
}

void Sprite::init()
{
    VisualObject::init();
    m_component = makeObject<SpriteComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Sprite::init(Texture* texture, float width, float height)
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

void Sprite::setTexture(Texture* value)
{
	if (m_testDelegate) {
		int a = m_testDelegate->call(10);
		std::cout << "m_testDelegate: " << a << std::endl;
	}

    m_component->setTexture(value);
}

void Sprite::setSize(const Size& value)
{
    m_component->setSize(value);
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

void Sprite::setAnchorPoint(const Vector2& value)
{
    m_component->setAnchorPoint(value);
}

const Vector2& Sprite::anchorPoint() const
{
    return m_component->anchorPoint();
}

void Sprite::setFlippedX(bool value)
{
    m_component->setFlippedX(value);
}

bool Sprite::isFlippedX() const
{
    return m_component->isFlippedX();
}

void Sprite::setFlippedY(bool value)
{
    m_component->setFlippedY(value);
}

bool Sprite::isFlippedY() const
{
    return m_component->isFlippedY();
}

void Sprite::setPixelsParUnit(float value)
{
    return m_component->setPixelsParUnit(value);
}

} // namespace ln

