
#include "Internal.hpp"
#include <LuminoEngine/UI/UIElement.hpp>

namespace ln {

//==============================================================================
// UIElement

UIElement::UIElement()
{
}

void UIElement::initialize()
{
    Object::initialize();
}

int UIElement::getVisualChildrenCount() const
{
	return 0;
}

UIElement* UIElement::getVisualChild(int index) const
{
	return nullptr;
}

void UIElement::onRender(UIRenderingContext* context)
{
}

void UIElement::render(UIRenderingContext* context)
{
	onRender(context);

	// child elements
	int count = getVisualChildrenCount();
	for (int i = 0; i < count; i++) {
		getVisualChild(i)->render(context);
	}
}

} // namespace ln

