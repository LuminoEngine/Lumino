
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>

namespace ln {

//==============================================================================
// UILayoutPanel

UILayoutPanel::UILayoutPanel()
{
}

void UILayoutPanel::initialize()
{
    UIElement::initialize();
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

} // namespace ln

