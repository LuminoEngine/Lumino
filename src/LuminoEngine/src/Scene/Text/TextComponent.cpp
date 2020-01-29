
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Scene/Text/TextComponent.hpp>

namespace ln {

//=============================================================================
// TextComponent

LN_OBJECT_IMPLEMENT(TextComponent, VisualComponent) {}

TextComponent::TextComponent()
    : m_text()
	, m_color(Color::Gray)
	, m_anchorPoint(0.5, 0.5)
	, m_font(nullptr)
{
}

bool TextComponent::init()
{
	if (!VisualComponent::init()) return false;
    setBlendMode(BlendMode::Alpha);
    setCullMode(CullMode::None);
	return true;
}

void TextComponent::setFont(Font* value)
{
	m_font = value;
}

Font* TextComponent::font() const
{
	return m_font;
}

void TextComponent::onRender(RenderingContext* context)
{
	context->setFont(m_font);
	context->drawTextSprite(m_text, m_color, m_anchorPoint, SpriteBaseDirection::ZMinus);
}

} // namespace ln

