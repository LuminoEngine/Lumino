
#include "Internal.hpp"
#include <LuminoEngine/UI//UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIElement

UIElement::UIElement()
: m_manager(nullptr)
,  m_localStyle(newObject<UIStyle>()) // TODO: ふつうは static なオブジェクトのほうが多くなるので、必要なやつだけ遅延作成でいいと思う
, m_actualStyle(nullptr)
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

void UIElement::setMargin(const Thickness& margin)
{
    m_localStyle->margin = margin;
}

const Thickness& UIElement::margin() const
{
    return m_localStyle->margin;
}

void UIElement::setPadding(const Thickness& padding)
{
    m_localStyle->padding = padding;
}

const Thickness& UIElement::padding() const
{
    return m_localStyle->padding;
}

void UIElement::setPosition(const Vector3 & pos)
{
    m_localStyle->position = pos;
}

const Vector3 & UIElement::position() const
{
    return m_localStyle->position.getOrDefault(Vector3::Zero);
}

void UIElement::setRotation(const Quaternion & rot)
{
    m_localStyle->rotation = rot;
}

const Quaternion & UIElement::rotation() const
{
    return m_localStyle->rotation.getOrDefault(Quaternion::Identity);
}

void UIElement::setScale(const Vector3 & scale)
{
    m_localStyle->scale = scale;
}

const Vector3 & UIElement::scale() const
{
    return m_localStyle->scale.getOrDefault(Vector3::Ones);
}

void UIElement::setCenterPoint(const Vector3 & value)
{
    m_localStyle->centerPoint = value;
}

const Vector3 & UIElement::centerPoint() const
{
    return m_localStyle->centerPoint.getOrDefault(Vector3::Zero);
}

Size UIElement::measureOverride(const Size& constraint)
{
    // TODO: tmp
    return constraint;
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
    context->setBaseTransfrom(Matrix::makeAffineTransformation(
        scale(), Vector3::Zero, rotation(), position() - centerPoint()));

	onRender(context);

	// child elements
	int count = getVisualChildrenCount();
	for (int i = 0; i < count; i++) {
		getVisualChild(i)->render(context);
	}
}

} // namespace ln

