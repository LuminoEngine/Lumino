
#include "Internal.hpp"
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIElement

UIElement::UIElement()
    : m_manager(nullptr)
{
}

void UIElement::initialize()
{
    Object::initialize();
    m_manager = detail::EngineDomain::uiManager();

    UIContainerElement* primaryElement = m_manager->primaryElement();
    if (primaryElement) {
        primaryElement->addElement(this);
    }
}

Size UIElement::arrangeOverride(const Size& finalSize)
{
    // TODO: tmp
    return finalSize;
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

void UIElement::updateLayout(const Size& size)
{
    // TODO: tmp
    arrangeOverride(size);

    // TODO: Layoutelement に実装を持っていく

    // child elements
    int count = getVisualChildrenCount();
    for (int i = 0; i < count; i++) {
        getVisualChild(i)->updateLayout(size);
    }
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

