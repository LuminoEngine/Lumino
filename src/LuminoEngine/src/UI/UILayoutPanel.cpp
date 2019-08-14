
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>

namespace ln {

//==============================================================================
// UILayoutPanel2

UILayoutPanel2::UILayoutPanel2()
    : m_ownerItemsControl(nullptr)
{
    //m_isHitTestVisible = false;
}

void UILayoutPanel2::init()
{
    UIElement::init();
}

void UILayoutPanel2::addChild(UIElement* child)
{
    if (LN_REQUIRE(child)) return;
    m_logicalChildren.add(child);
    addVisualChild(child);
}

void UILayoutPanel2::removeChild(UIElement* child)
{
    if (LN_REQUIRE(child)) return;
    m_logicalChildren.remove(child);
    removeVisualChild(child);
}

void UILayoutPanel2::removeAllChildren()
{
    for (auto& child : m_logicalChildren) {
        removeVisualChild(child);
    }
    m_logicalChildren.clear();
}

void UILayoutPanel2::onDispose(bool explicitDisposing)
{
    removeAllChildren();
    UIElement::onDispose(explicitDisposing);
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

UIFrameLayout2::UIFrameLayout2()
{
}

void UIFrameLayout2::init()
{
    UILayoutPanel2::init();
}

Size UIFrameLayout2::measureOverride(const Size& constraint)
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
    //return UIElement::measureOverride(constraint);
	return UIFrameLayout2::staticMeasureOverride(this, constraint);
}

Size UIFrameLayout2::arrangeOverride(const Size& finalSize)
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

	return UIFrameLayout2::staticArrangeOverride(this, finalSize);
    //return finalSize;
}

Size UIFrameLayout2::staticMeasureChildrenAreaSize(UIElement* ownerElement, const Size& constraint)
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
    return childMaxSize;
}

Size UIFrameLayout2::staticMeasureOverride(UIElement* ownerElement, const Size& constraint)
{
    Size childMaxSize = staticMeasureChildrenAreaSize(ownerElement, constraint);

    return detail::LayoutHelper::measureElement(ownerElement, constraint, childMaxSize);

    //auto selfSize = ownerElement->UIElement::measureOverride(constraint);
    //return Size::max(selfSize, childMaxSize);

    //Size size = Size(ownerElement->width(), ownerElement->height());
    //size.width = Math::isNaN(size.width) ? 0.0 : size.width;
    //size.height = Math::isNaN(size.height) ? 0.0 : size.height;
    //return Size::min(constraint, Size::max(size, childMaxSize));
}

Size UIFrameLayout2::staticArrangeChildrenArea(UIElement* ownerElement, const Rect& finalArea)
{
    int childrenCount = ownerElement->getVisualChildrenCount();
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = ownerElement->getVisualChild(i);

        Rect slotRect;
        detail::LayoutHelper::adjustAlignment(finalArea, child->desiredSize(), ownerElement->m_finalStyle->horizontalContentAlignment, ownerElement->m_finalStyle->verticalContentAlignment, &slotRect);

        child->arrangeLayout(slotRect);
    }
    return finalArea.getSize();
}

Size UIFrameLayout2::staticArrangeOverride(UIElement* ownerElement, const Size& finalSize)
{
    //const Thickness& padding = ownerElement->finalStyle()->padding;
    //Point childrenOffset(padding.left, padding.top);
    ////Size childrenBoundSize = finalSize;//(finalSize.width - padding.right, finalSize.height - padding.bottom);
    //Size childrenBoundSize(finalSize.width - padding.width(), finalSize.height - padding.height());
    //Rect bounds(childrenOffset, childrenBoundSize);
	Rect contentArea = detail::LayoutHelper::arrangeClientArea(ownerElement, finalSize);

    staticArrangeChildrenArea(ownerElement, contentArea);

    return finalSize;
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

void UIStackLayout2::init()
{
    UILayoutPanel2::init();
}

void UIStackLayout2::addChild(UIElement* element, UILayoutLengthType type)
{
    UILayoutPanel2::addChild(element);
    CellDefinition cell;
    cell.type = type;
    m_cellDefinitions.add(cell);
}

Size UIStackLayout2::measureOverride(const Size& constraint)
{
    int childCount = getVisualChildrenCount();

    // Create missing cells.
    if (m_cellDefinitions.size() < childCount) {
        m_cellDefinitions.resize(childCount);
    }

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
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = getVisualChild(i);
        child->measureLayout(size);

        const Size& childDesiredSize = child->getLayoutDesiredSize();
        if (isHorizontal()) {
            desiredSize.width += childDesiredSize.width;
            desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
            m_cellDefinitions[i].desiredSize = childDesiredSize.width;
        }
        else {
            desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
            desiredSize.height += childDesiredSize.height;
            m_cellDefinitions[i].desiredSize = childDesiredSize.height;
        }
    }

    return desiredSize;
}

Size UIStackLayout2::arrangeOverride(const Size& finalSize)
{
    const Thickness& padding = finalStyle()->padding;
    Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));



    float boundSize = 0.0f;
    if (isHorizontal())
        boundSize = childrenBoundSize.width;
    else
        boundSize = childrenBoundSize.height;

    // Fix size of 'Auto' and 'Direct', and count 'Ratio'
    float totalActualSize = 0.0f;
    float ratioCellCount = 0.0f;
    for (auto& cell : m_cellDefinitions) {
        if (cell.type == UILayoutLengthType::Ratio) {
            ratioCellCount += cell.size;
        }
        else {
            if (cell.type == UILayoutLengthType::Auto) {
                // measure で計算した desiredSize をそのまま使う
            }
            else if (cell.type == UILayoutLengthType::Direct) {
                cell.desiredSize = cell.size;
            }
            totalActualSize += cell.desiredSize;
        }
    }

    // "1*" 分のセルの領域を計算する
    float ratioUnit = (ratioCellCount != 0.0f) ? (boundSize - totalActualSize) / ratioCellCount : 0.0f;
    ratioUnit = std::max(0.0f, ratioUnit);	// 負値はダメ

    // "*" 指定である Row/Column の最終サイズを確定させ、
    // 全セルのオフセット (位置) も確定させる
    float totalOffset = 0.0f;
    for (int i = 0; i < m_cellDefinitions.size(); i++) {
        auto& cell = m_cellDefinitions[i];
        if (cell.type == UILayoutLengthType::Ratio) {
            cell.desiredSize = ratioUnit * cell.size;
        }

        cell.actualOffset = totalOffset;

        //if (i == m_cellDefinitions.size() - 1 && cell.type == UILayoutLengthType::Auto) {
        //    cell.actualSize = Math::clamp(boundSize - cell.actualOffset, cell.minSize, cell.maxSize);
        //}
        //else {
            cell.actualSize = Math::clamp(cell.desiredSize, cell.minSize, cell.maxSize);
        //}

        totalOffset += cell.actualSize;
    }

    // 子要素の最終位置・サイズを確定させる
    int childCount = getVisualChildrenCount();
    if (isHorizontal()) {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(cell.actualOffset, 0, cell.actualSize, finalSize.height);
            child->arrangeLayout(childRect);
        }
    }
    else {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
            child->arrangeLayout(childRect);
        }
    }

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::arrangeOverride(finalSize);
    return selfSize;

#if 0
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
#endif
}

//==============================================================================
// UIVBoxLayout2

void UIVBoxLayout2::init()
{
    UIStackLayout2::init();
    setOrientation(Orientation::Vertical);
}

//==============================================================================
// UIHBoxLayout2

void UIHBoxLayout2::init()
{
	UIStackLayout2::init();
	setOrientation(Orientation::Horizontal);
}


//==============================================================================
// UIBoxLayout3

UIBoxLayout3::UIBoxLayout3()
    : m_orientation(Orientation::Vertical)
{
}

void UIBoxLayout3::init()
{
    UILayoutPanel2::init();
}

Size UIBoxLayout3::measureOverride(const Size& constraint)
{
    int childCount = getVisualChildrenCount();

    // Create missing cells.
    if (m_cellDefinitions.size() < childCount) {
        m_cellDefinitions.resize(childCount);
    }

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
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = getVisualChild(i);
        child->measureLayout(size);

        const Size& childDesiredSize = child->getLayoutDesiredSize();
        if (isHorizontal()) {
            desiredSize.width += childDesiredSize.width;
            desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
            m_cellDefinitions[i].desiredSize = childDesiredSize.width;
        }
        else {
            desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
            desiredSize.height += childDesiredSize.height;
            m_cellDefinitions[i].desiredSize = childDesiredSize.height;
        }
    }

    return desiredSize;
}

Size UIBoxLayout3::arrangeOverride(const Size& finalSize)
{
    const Thickness& padding = finalStyle()->padding;
    Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));
    int actualCellCount = m_logicalChildren.size();


    float boundSize = 0.0f;
    if (isHorizontal())
        boundSize = childrenBoundSize.width;
    else
        boundSize = childrenBoundSize.height;

    // Fix size of 'Auto' and 'Direct', and count 'Ratio'
    float totalActualSize = 0.0f;
    float ratioCellCount = 0.0f;
    for (int iCell = 0; iCell < actualCellCount; iCell++) {
        auto type = layoutType(iCell);
        if (type == UILayoutLengthType::Ratio) {
            ratioCellCount += layoutWeight(iCell);
        }
        else {
            if (type == UILayoutLengthType::Auto) {
                // measure で計算した desiredSize をそのまま使う
            }
            else if (type == UILayoutLengthType::Direct) {
                m_cellDefinitions[iCell].desiredSize = layoutDirectSize(iCell);
            }
            totalActualSize += m_cellDefinitions[iCell].desiredSize;
        }
    }

    // "1*" 分のセルの領域を計算する
    float ratioUnit = (ratioCellCount != 0.0f) ? (boundSize - totalActualSize) / ratioCellCount : 0.0f;
    ratioUnit = std::max(0.0f, ratioUnit);	// 負値はダメ

    // "*" 指定である Row/Column の最終サイズを確定させ、
    // 全セルのオフセット (位置) も確定させる
    float totalOffset = 0.0f;
    for (int iCell = 0; iCell < actualCellCount; iCell++) {
        auto& cell = m_cellDefinitions[iCell];
        if (layoutType(iCell) == UILayoutLengthType::Ratio) {
            cell.desiredSize = ratioUnit * layoutWeight(iCell);
        }

        float minSize, maxSize;
        getLayoutMinMaxSize(iCell, &minSize, &maxSize);

        cell.actualOffset = totalOffset;
        cell.actualSize = Math::clamp(cell.desiredSize, minSize, maxSize);
        totalOffset += cell.actualSize;
    }

    // 子要素の最終位置・サイズを確定させる
    int childCount = getVisualChildrenCount();
    if (isHorizontal()) {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(cell.actualOffset, 0, cell.actualSize, finalSize.height);
            child->arrangeLayout(childRect);
        }
    }
    else {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
            child->arrangeLayout(childRect);
        }
    }

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::arrangeOverride(finalSize);
    return selfSize;
}

UILayoutLengthType UIBoxLayout3::layoutType(int index) const
{
    auto& child = m_logicalChildren[index];

    if (isHorizontal()) {
        if (!Math::isNaN(child->m_finalStyle->width)) {
            return UILayoutLengthType::Direct;
        }
    }
    else {
        if (!Math::isNaN(child->m_finalStyle->height)) {
            return UILayoutLengthType::Direct;
        }
    }

    auto& info = child->m_gridLayoutInfo;
    if (!info)
        return UILayoutLengthType::Auto;
    if (info->layoutWeight <= 0.0)
        return UILayoutLengthType::Auto;
    else
        return UILayoutLengthType::Ratio;
}

float UIBoxLayout3::layoutWeight(int index) const
{
    auto& child = m_logicalChildren[index];
    if (child->m_gridLayoutInfo)
        return child->m_gridLayoutInfo->layoutWeight;
    else
        return 0.0f;
}

float UIBoxLayout3::layoutDirectSize(int index) const
{
    auto& child = m_logicalChildren[index];
    if (isHorizontal()) {
        if (!Math::isNaN(child->m_finalStyle->width)) {
            return child->m_finalStyle->width;
        }
    }
    else {
        if (!Math::isNaN(child->m_finalStyle->height)) {
            return child->m_finalStyle->height;
        }
    }
    return 0.0f;
}

void UIBoxLayout3::getLayoutMinMaxSize(int index, float* minSize, float* maxSize) const
{
    *minSize = 0.0f;
    *maxSize = FLT_MAX;

    auto& child = m_logicalChildren[index];
    if (isHorizontal()) {
        if (!Math::isNaN(child->m_finalStyle->minWidth))
            *minSize = child->m_finalStyle->minWidth;
        if (!Math::isNaN(child->m_finalStyle->width))
            *maxSize = child->m_finalStyle->maxWidth;
    }
    else {
        if (!Math::isNaN(child->m_finalStyle->minHeight))
            *minSize = child->m_finalStyle->minHeight;
        if (!Math::isNaN(child->m_finalStyle->maxHeight))
            *maxSize = child->m_finalStyle->maxHeight;
    }
}

//==============================================================================
// UISwitchLayout

UISwitchLayout::UISwitchLayout()
	: m_activeIndex(0)
{
}

void UISwitchLayout::setActiveIndex(int index)
{
	m_activeIndex = index;
	invalidateLayout();
}

void UISwitchLayout::setActive(UIElement* child)
{
    m_activeIndex = m_logicalChildren.indexOf(child);
    invalidateLayout();
}

void UISwitchLayout::init()
{
	UIFrameLayout2::init();
}

Size UISwitchLayout::measureOverride(const Size& constraint)
{
	for (int i = 0; i < m_logicalChildren.size(); i++) {
		if (i == m_activeIndex)
			m_logicalChildren[i]->m_internalVisibility = UIVisibility::Visible;
		else
			m_logicalChildren[i]->m_internalVisibility = UIVisibility::Collapsed;
	}

	return UIFrameLayout2::measureOverride(constraint);
}



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
// UIFrameLayout

Ref<UIFrameLayout> UIFrameLayout::create()
{
    return makeObject<UIFrameLayout>();
}

UIFrameLayout::UIFrameLayout()
{
}

UIFrameLayout::~UIFrameLayout()
{
}

void UIFrameLayout::init()
{
    UILayoutPanel::init();
}

Size UIFrameLayout::measureOverride(const IUIElementList* childElements, const Size& constraint)
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

Size UIFrameLayout::arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect)
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
// UIStackLayout

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

Size UIStackLayout::measureOverride(const IUIElementList* childElements, const Size& constraint)
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

Size UIStackLayout::arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect)
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

//==============================================================================
// UIVBoxLayout

void UIVBoxLayout::init()
{
    UIStackLayout::init();
    setOrientation(Orientation::Vertical);
}

} // namespace ln

