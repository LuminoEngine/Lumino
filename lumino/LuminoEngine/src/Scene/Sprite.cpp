
#include "Internal.hpp"
#include <LuminoGraphics/Texture.hpp>
#include <LuminoEngine/Visual/SpriteComponent.hpp>
#include <LuminoEngine/Scene/Sprite.hpp>
#include <LuminoEngine/Scene/World.hpp>

namespace ln {

//==============================================================================
// Sprite

LN_OBJECT_IMPLEMENT(Sprite, VisualObject) {}

Ref<Sprite> Sprite::create()
{
    return makeObject<Sprite>();
}

Ref<Sprite> Sprite::create(Texture* texture)
{
	return makeObject<Sprite>(texture, -1, -1);
}

Ref<Sprite> Sprite::create(Texture* texture, float width, float height)
{
    return makeObject<Sprite>(texture, width, height);
}

Ref<Sprite> Sprite::create(SpriteSheet* frameSet)
{
	return makeObject<Sprite>(frameSet);
}

Ref<Sprite> Sprite::load(StringRef filePath)
{
	return makeObject<Sprite>(Texture2D::load(filePath));
}

Ref<Sprite> Sprite::loadEmoji(StringRef code)
{
	return makeObject<Sprite>(Texture2D::loadEmoji(code));
}

Ref<Sprite> Sprite::loadEmoji(uint32_t codePoint)
{
	return makeObject<Sprite>(Texture2D::loadEmoji(codePoint));
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
    m_component = makeObject<SpriteComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
}

void Sprite::init(Texture* texture)
{
	init();
	setTexture(texture);
}

void Sprite::init(Texture* texture, float width, float height)
{
    init();
    setSize(Size(width, height));
    setTexture(texture);
}

void Sprite::init(SpriteSheet* frameSet)
{
	init();
	setFrameSet(frameSet);
}

void Sprite::setTexture(Texture* value)
{
	//if (m_testDelegate) {
	//	int a = m_testDelegate->call(10);
	//	std::cout << "m_testDelegate: " << a << std::endl;
	//}

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

void Sprite::setFrameSet(SpriteSheet* value)
{
	m_component->setFrameSet(value);
}

void Sprite::setFrameIndex(int index)
{
	m_component->setFrameIndex(index);
}

SpriteSheet* Sprite::spriteSheet() const
{
    return m_component->spriteSheet();
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

//==============================================================================
// Sprite::BuilderDetails

Sprite::BuilderDetails::BuilderDetails()
    : texture()
    , size(-1.f, 1.f)
{
}

void Sprite::BuilderDetails::apply(Sprite* p) const
{
    VisualObject::BuilderDetails::apply(p);
    p->setTexture(texture);
    p->setSize(size);
}

} // namespace ln

