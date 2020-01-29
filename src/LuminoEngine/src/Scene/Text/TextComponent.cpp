
#include "Internal.hpp"
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
{
}

bool TextComponent::init()
{
	if (!VisualComponent::init()) return false;
    setBlendMode(BlendMode::Alpha);
    setCullMode(CullMode::None);
	return true;
}

void TextComponent::onRender(RenderingContext* context)
{
	context->drawTextSprite(m_text, m_color, m_anchorPoint, SpriteBaseDirection::ZMinus);
}

} // namespace ln

