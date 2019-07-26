
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
    Object::init();
}

void UILayoutPanel::measureLayout(const IUIElementList* childElements, const Size& availableSize)
{
    m_desiredSize = measureOverride(childElements, availableSize);
}

void UILayoutPanel::arrangeLayout(const IUIElementList* childElements, const Rect& finalSlotRect)
{
    Rect rect = finalSlotRect;
    rect.x -= m_scrollOffset.x;
    rect.y -= m_scrollOffset.y;
    m_actualSize = arrangeOverride(childElements, rect);
}

float UILayoutPanel::getExtentWidth() const { return m_desiredSize.width; }
float UILayoutPanel::getExtentHeight() const { return m_desiredSize.height; }
float UILayoutPanel::getViewportWidth() const { return m_actualSize.width; }
float UILayoutPanel::getViewportHeight() const { return m_actualSize.height; }
void UILayoutPanel::setHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel::getHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel::setVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel::getVerticalOffset() const { return m_scrollOffset.y; }

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
    UILayoutPanel::init();
}

Size UIFrameLayout2::measureOverride(const IUIElementList* childElements, const Size& constraint)
{
    int childrenCount = childElements->getElementCount();
    Size childMaxSize(0, 0);
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements->getElement(i);
        child->measureLayout(constraint);
        const Size& desiredSize = child->desiredSize();
        childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
        childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
    }
    return childMaxSize;
}

Size UIFrameLayout2::arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect)
{
    Rect bounds(finalSlotRect);

    int childrenCount = childElements->getElementCount();
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements->getElement(i);
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
    UILayoutPanel::init();
}

Size UIStackLayout2::measureOverride(const IUIElementList* childElements, const Size& constraint)
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
    int childCount = childElements->getElementCount();
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = childElements->getElement(i);
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

Size UIStackLayout2::arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect)
{
    Size childrenBoundSize(finalSlotRect.width, finalSlotRect.height);
    //Size selfBounds(FLT_MAX, FLT_MAX);
    //if (stretch) {
    //    selfBounds = finalSlotRect.getSize();
    //}

    float prevChildSize = 0;
    float rPos = 0;
    Rect childRect(0, 0, 0, 0);
    int childCount = childElements->getElementCount();
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = childElements->getElement(i);
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

        Rect actual(finalSlotRect.x + childRect.x, finalSlotRect.y + childRect.y, childRect.width, childRect.height);
        if (lastStretch && i == childCount - 1) {
            actual.width = finalSlotRect.width - actual.x;
            actual.height = finalSlotRect.height - actual.y;
        }

        child->arrangeLayout(actual);
    }

    return finalSlotRect.getSize();
}


} // namespace ln

