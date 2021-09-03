
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Visual/EmojiComponent.hpp>

namespace ln {

//==============================================================================
// EmojiComponent

EmojiComponent::EmojiComponent()
{
}

bool EmojiComponent::init()
{
	return VisualComponent::init();
}

void EmojiComponent::onRender(RenderingContext* context)
{
	context->drawChar('A', Color::Red);
}

} // namespace ln

