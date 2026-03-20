#include "Internal.hpp"
#include <LuminoFramework/Scene/Text/TextComponent.hpp>
#include <LuminoFramework/Scene/Text/Text.hpp>

namespace ln {

//==============================================================================
// Text

LN_OBJECT_IMPLEMENT(Text, VisualObject) {}

Ref<Text> Text::create()
{
	return makeObject_deprecated<Text>();
}

Ref<Text> Text::create(StringView text)
{
	return makeObject_deprecated<Text>(text);
}

Text::Text()
	: m_component(nullptr)
{
}

bool Text::init()
{
	if (!VisualObject::init()) return false;
    m_component = makeObject_deprecated<TextComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
	return true;
}

bool Text::init(StringView text)
{
	if (!init()) return false;
	setText(text);
	return true;
}

void Text::setText(StringView value)
{
	m_component->setText(value);
}

const String& Text::text() const
{
	return m_component->text();
}

void Text::setFont(Font* value)
{
	m_component->setFont(value);
}

Font* Text::font() const
{
	return m_component->font();
}

void Text::setFontSize(int value)
{
	m_component->setFontSize(value);
}

int Text::fontSize() const
{
	return m_component->fontSize();
}

void Text::setColor(const Color& value)
{
	m_component->setColor(value);
}

const Color& Text::color() const
{
	return m_component->color();
}

void Text::setAnchorPoint(const Vector2& value)
{
    m_component->setAnchorPoint(value);
}

const Vector2& Text::anchorPoint() const
{
    return m_component->anchorPoint();
}

} // namespace ln

