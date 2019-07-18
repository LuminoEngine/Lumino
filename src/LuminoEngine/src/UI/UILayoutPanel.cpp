
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>

namespace ln {

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
    Rect rect = finalSlotRect;
    rect.x -= m_scrollOffset.x;
    rect.y -= m_scrollOffset.y;
    m_actualSize = arrangeOverride(childElements, rect);
}

float UILayoutPanel2::getExtentWidth() const { return m_desiredSize.width; }
float UILayoutPanel2::getExtentHeight() const { return m_desiredSize.height; }
float UILayoutPanel2::getViewportWidth() const { return m_actualSize.width; }
float UILayoutPanel2::getViewportHeight() const { return m_actualSize.height; }
void UILayoutPanel2::setHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel2::getHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel2::setVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel2::getVerticalOffset() const { return m_scrollOffset.y; }

//==============================================================================
// UIFrameLayout2

Ref<UIFrameLayout2> UIFrameLayout2::create()
{
    return makeObject<UIFrameLayout2>();
}

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

Size UIFrameLayout2::arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect)
{
    Rect bounds(finalSlotRect);

    int childrenCount = childElements.size();
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements[i];
        child->arrangeLayout(bounds);
    }

    return finalSlotRect.getSize();
}


//==============================================================================
// UIStackLayout2

Ref<UIStackLayout2> UIStackLayout2::create()
{
    return makeObject<UIStackLayout2>();
}

UIStackLayout2::UIStackLayout2()
    : m_orientation(Orientation::Vertical)
{
}

UIStackLayout2::~UIStackLayout2()
{
}

void UIStackLayout2::init()
{
    UILayoutPanel2::init();
}

Size UIStackLayout2::measureOverride(const List<Ref<UIElement>>& childElements, const Size& constraint)
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
    int childCount = childElements.size();
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = childElements[i];
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

Size UIStackLayout2::arrangeOverride(const List<Ref<UIElement>>& childElements, const Rect& finalSlotRect)
{
    Size childrenBoundSize(finalSlotRect.width, finalSlotRect.height);

    float prevChildSize = 0;
    float rPos = 0;
    Rect childRect(0, 0, 0, 0);
    int childCount = childElements.size();
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = childElements[i];
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

    return finalSlotRect.getSize();
}


} // namespace ln

