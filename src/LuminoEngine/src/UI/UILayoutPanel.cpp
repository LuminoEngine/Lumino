
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>

namespace ln {

//==============================================================================
// UILayoutPanel

UILayoutPanel::UILayoutPanel()
{
}

void UILayoutPanel::init()
{
	objectManagementFlags().set(detail::ObjectManagementFlags::AutoAddToActiveScene, false);
    UIElement::init();
	setHorizontalAlignment(HAlignment::Stretch);
	setVerticalAlignment(VAlignment::Stretch);
}

int UILayoutPanel::getVisualChildrenCount() const
{
	return m_layoutOwnerLogicalChildren.size();
}

UIElement* UILayoutPanel::getVisualChild(int index) const
{
	return m_layoutOwnerLogicalChildren[index];
}

void UILayoutPanel::addLayoutOwnerLogicalChild(UIElement* element)
{
	m_layoutOwnerLogicalChildren.add(element);
}

void UILayoutPanel::removeLayoutOwnerLogicalChild(UIElement* element)
{
    m_layoutOwnerLogicalChildren.remove(element);
}

void UILayoutPanel::clearLayoutOwnerLogicalChildren()
{
	m_layoutOwnerLogicalChildren.clear();
}

//==============================================================================
// UIFrameLayout

Size UIFrameLayout::measureOverride(const Size& constraint)
{
    //Size size = element->TElementBaseClass::measureOverride(constraint);

    //Size childMaxSize(0, 0);
    //for (UIElement* child : children)
    //{
    //    child->measureLayout(constraint);
    //    const Size& desiredSize = child->getDesiredSize();
    //    const Point& pos = child->getPositionInternal();

    //    childMaxSize.width = std::max(childMaxSize.width, pos.x + desiredSize.width);
    //    childMaxSize.height = std::max(childMaxSize.height, pos.y + desiredSize.height);
    //}

    //return Size::min(constraint, Size::max(size, childMaxSize));
    return UIElement::measureOverride(constraint);
}

Size UIFrameLayout::arrangeOverride(const Size& finalSize)
{
#if 0
    //Thickness canvas;
    const Thickness& padding = element->getPadding();
    Point childrenOffset(padding.left, padding.top);
    Size childrenBoundSize(finalSize.width - padding.getWidth(), finalSize.height - padding.getHeight());

    for (UIElement* child : children)
    {
        const Size& desiredSize = child->getDesiredSize();
        Size size = desiredSize;

        Rect childRect(child->getPositionInternal(), size/*child->getSizeInternal()*/);
        Flags<AlignmentAnchor> anchor = child->getAnchorInternal();

        if (anchor != AlignmentAnchor::None)
        {
            const Thickness& margin = element->getMargineInternal();
            //float l = childRect.getLeft(), t = childRect.GetTop(), r = childRect.getRight(), b = childRect.getBottom();

            //if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
            //	l = margin.Left;
            //else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
            //	l = finalSize.width * margin.Left;

            //if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
            //	t = margin.Top;
            //else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
            //	t = finalSize.height * margin.Top;

            //if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
            //	r = finalSize.width - margin.Right;
            //else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
            //	r = finalSize.width - (finalSize.width * margin.Right);

            //if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
            //	b = finalSize.height - margin.Bottom;
            //else if (anchor.TestFlag(AlignmentAnchor::BottomRatios))
            //	b = finalSize.height - (finalSize.height * margin.Bottom);

            //if (anchor.TestFlag(AlignmentAnchor::HCenter))
            //	childRect.x = (finalSize.width - childRect.width) / 2;

            //if (anchor.TestFlag(AlignmentAnchor::VCenter))
            //	childRect.y = (finalSize.height - childRect.height) / 2;

            //childRect.Set(l, t, r - l, b - t);

            //child->arrangeLayout(childRect);
#if 1
            float l = NAN, t = NAN, r = NAN, b = NAN;
            if (anchor.isSet(AlignmentAnchor::LeftOffsets))
                l = margin.left;
            else if (anchor.isSet(AlignmentAnchor::LeftRatios))
                l = childrenBoundSize.width * margin.left;

            if (anchor.isSet(AlignmentAnchor::TopOffsets))
                t = margin.top;
            else if (anchor.isSet(AlignmentAnchor::TopRatios))
                t = childrenBoundSize.height * margin.top;

            if (anchor.isSet(AlignmentAnchor::RightOffsets))
                r = childrenBoundSize.width - margin.right;
            else if (anchor.isSet(AlignmentAnchor::RightRatios))
                r = childrenBoundSize.width - (childrenBoundSize.width * margin.right);

            if (anchor.isSet(AlignmentAnchor::BottomOffsets))
                b = childrenBoundSize.height - margin.bottom;
            else if (anchor.isSet(AlignmentAnchor::BottomRatios))
                b = childrenBoundSize.height - (childrenBoundSize.height * margin.bottom);

            if (anchor.isSet(AlignmentAnchor::HCenter))
                childRect.x = (childrenBoundSize.width - childRect.width) / 2;

            if (anchor.isSet(AlignmentAnchor::VCenter))
                childRect.y = (childrenBoundSize.height - childRect.height) / 2;

            if (!Math::isNaN(l) || !Math::isNaN(r))
            {
                if (!Math::isNaN(l) && Math::isNaN(r))
                {
                    childRect.x = l;
                }
                else if (Math::isNaN(l) && !Math::isNaN(r))
                {
                    childRect.x = r - childRect.width;
                }
                else
                {
                    childRect.x = l;
                    childRect.width = r - l;
                }
            }

            if (!Math::isNaN(t) || !Math::isNaN(b))
            {
                if (!Math::isNaN(t) && Math::isNaN(b))
                {
                    childRect.y = t;
                }
                else if (Math::isNaN(t) && !Math::isNaN(b))
                {
                    childRect.y = b - childRect.height;
                }
                else
                {
                    childRect.y = t;
                    childRect.height = b - t;
                }
            }

            childRect.x += childrenOffset.x;
            childRect.y += childrenOffset.y;
#endif
        }

        child->arrangeLayout(childRect);
    }
#endif

    return finalSize;
}

Size UIFrameLayout::staticMeasureOverride(UIElement* ownerElement, const Size& constraint)
{
	int childrenCount = ownerElement->getVisualChildrenCount();
	Size childMaxSize(0, 0);
	for (int i = 0; i < childrenCount; i++)
	{
		UIElement* child = ownerElement->getVisualChild(i);
		child->measureLayout(constraint);
		const Size& desiredSize = child->desiredSize();
		childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
		childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
	}

	Size size = ownerElement->getLayoutSize();
	size.width = Math::isNaN(size.width) ? 0.0 : size.width;
	size.height = Math::isNaN(size.height) ? 0.0 : size.height;
	return Size::min(constraint, Size::max(size, childMaxSize));
}

Size UIFrameLayout::staticArrangeOverride(UIElement* ownerElement, const Rect& finalSlotRect)
{
	const Thickness& padding = ownerElement->finalStyle()->padding;
	Point childrenOffset(finalSlotRect.x + padding.left, finalSlotRect.y + padding.top);
    //Size childrenBoundSize = finalSize;//(finalSize.width - padding.right, finalSize.height - padding.bottom);
    Size childrenBoundSize(finalSlotRect.width - padding.width(), finalSlotRect.height - padding.height());
	Rect bounds(childrenOffset, childrenBoundSize);

	int childrenCount = ownerElement->getVisualChildrenCount();
	for (int i = 0; i < childrenCount; i++)
	{
		UIElement* child = ownerElement->getVisualChild(i);
		child->arrangeLayout(bounds);
	}

	return finalSlotRect.getSize();
}

//==============================================================================
// UIFrameLayout

Ref<UIStackLayout> UIStackLayout::create()
{
	return makeObject<UIStackLayout>();
}

UIStackLayout::UIStackLayout()
	: m_orientation(Orientation::Vertical)
{
}

UIStackLayout::~UIStackLayout()
{
}

void UIStackLayout::init()
{
	UILayoutPanel::init();
}

Size UIStackLayout::measureOverride(const Size& constraint)
{
	Size size = constraint;

	if (m_orientation == Orientation::Horizontal) {
		// 横に並べる場合、幅の制限を設けない
		size.width = std::numeric_limits<float>::infinity();
	}
	else {
		// 縦に並べる場合、高さの制限を設けない
		size.height = std::numeric_limits<float>::infinity();
	}

	Size desiredSize;
	int childCount = getVisualChildrenCount();
	for (int i = 0; i < childCount; i++)
	{
		UIElement* child = getVisualChild(i);
		child->measureLayout(size);

		const Size& childDesiredSize = child->getLayoutDesiredSize();
		if (m_orientation == Orientation::Horizontal || m_orientation == Orientation::ReverseHorizontal) {
			desiredSize.width += childDesiredSize.width;
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		else {
			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height += child->getLayoutDesiredSize().height;
		}
	}

	return desiredSize;
}

Size UIStackLayout::arrangeOverride(const Size& finalSize)
{
	const Thickness& padding = finalStyle()->padding; //  static_cast<ILayoutElement*>(panel)->getLayoutPadding();


	//ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
	//Size childrenBoundSize(finalSize.width, finalSize.height);	
	Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));

	float prevChildSize = 0;
	float rPos = 0;
	Rect childRect(padding.left, padding.top, 0, 0);
	int childCount = getVisualChildrenCount();
	for (int i = 0; i < childCount; i++)
	{
		UIElement* child = getVisualChild(i);
		const Size& childDesiredSize = child->getLayoutDesiredSize();

		switch (m_orientation)
		{
		case Orientation::Horizontal:
			childRect.x += prevChildSize;
			prevChildSize = childDesiredSize.width;
			childRect.width = prevChildSize;
			childRect.height = childrenBoundSize.height;
			break;
		case Orientation::Vertical:
			childRect.y += prevChildSize;
			prevChildSize = childDesiredSize.height;
			childRect.width = childrenBoundSize.width;
			childRect.height = prevChildSize;
			break;
		case Orientation::ReverseHorizontal:
			prevChildSize = childDesiredSize.width;
			rPos -= prevChildSize;
			childRect.x = childrenBoundSize.width + rPos;
			childRect.width = prevChildSize;
			childRect.height = childrenBoundSize.height;
			break;
		case Orientation::ReverseVertical:
			prevChildSize = childDesiredSize.height;
			rPos -= prevChildSize;
			childRect.y = childrenBoundSize.height + rPos;
			childRect.width = childrenBoundSize.width;
			childRect.height = prevChildSize;
			break;
		default:
			LN_UNREACHABLE();
			break;
		}

		child->arrangeLayout(childRect);
	}

	return finalSize;
}





//==============================================================================
// UILayoutPanel2

UILayoutPanel2::UILayoutPanel2()
{
}

void UILayoutPanel2::init()
{
    Object::init();
}

void UILayoutPanel2::measureLayout(const List<Ref<UIElement>>& childElements, const Size& availableSize)
{
    m_desiredSize = measureOverride(childElements, availableSize);
}

void UILayoutPanel2::arrangeLayout(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect)
{
    arrangeOverride(childElements, finalSlotRect);
}

//==============================================================================
// UIFrameLayout2

UIFrameLayout2::UIFrameLayout2()
{
}

UIFrameLayout2::~UIFrameLayout2()
{
}

void UIFrameLayout2::init()
{
    UILayoutPanel2::init();
}

Size UIFrameLayout2::measureOverride(const List<Ref<UIElement>>& childElements, const Size& constraint)
{
    int childrenCount = childElements.size();
    Size childMaxSize(0, 0);
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements[i];
        child->measureLayout(constraint);
        const Size& desiredSize = child->desiredSize();
        childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
        childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
    }
    return childMaxSize;
}

void UIFrameLayout2::arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect)
{
    Rect bounds(finalSlotRect);

    int childrenCount = childElements.size();
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements[i];
        child->arrangeLayout(bounds);
    }
}


} // namespace ln

