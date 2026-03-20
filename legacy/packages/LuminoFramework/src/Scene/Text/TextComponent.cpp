
#include "Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoFramework/Scene/Text/TextComponent.hpp>

namespace ln {

//==============================================================================
// TextComponent

LN_OBJECT_IMPLEMENT(TextComponent, VisualComponent) {}

TextComponent::TextComponent()
    : m_text()
	, m_color(Color::Black)
	, m_anchorPoint(0.5, 0.5)
	, m_font(makeRef<detail::FontRequester>())
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
	m_font->font = value;
}

Font* TextComponent::font() const
{
	return m_font->font;
}

void TextComponent::setFontSize(int value)
{
	m_font->size = value;
}

int TextComponent::fontSize() const
{
	return m_font->size;
}

void TextComponent::onRender(RenderingContext* context)
{
    context->setMaterial(Material::defaultMaterial());
	context->drawTextSprite(m_text, m_color, m_anchorPoint, SpriteBaseDirection::ZMinus, m_font);
}

} // namespace ln

