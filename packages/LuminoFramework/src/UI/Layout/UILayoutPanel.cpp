
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include "../UIStyleInstance.hpp"

namespace ln {

//==============================================================================
// UILayoutPanel::Builder

    /*
LN_BUILDER_IMPLEMENT(UILayoutPanel, UIElement);

UILayoutPanel::Builder& UILayoutPanel::Builder::children(std::initializer_list<UIElement::Builder> list)
{
	for (auto& p : list)
		detailsAs<Details>()->children.add(p);
	return *this;
}

Ref<Object> UILayoutPanel::Builder::Details::build()
{
	auto ptr = makeObject_deprecated<UILayoutPanel>();
	for (auto& b : children)
		ptr->addChild(b.build());
	return ptr;
}
    */

//==============================================================================
// UILayoutPanel
/*
    [2020/3/29] UILayoutPanel2_Deprecated は UIElement の派生とするべきか？
    ----------
    派生としないパターンは QtWidgets. ネストしたい場合は Layout::addLayout で重ねる。
    ただしこの場合、通常の論理ツリーとほぼ同義のツリーが、それとは別のルーティングで存在することになる。
    特にメモリ効率よくなるわけでもないし、一方でイベントルーティングをカスタマイズする必要があったり、
    ユーザープログラムとしては addElement と addLayout を分けないとならない (addChild でまとめるの不自然) し、
    メリットがほとんどない。


*/

LN_OBJECT_IMPLEMENT(UILayoutPanel, UIElement) {}

UILayoutPanel::UILayoutPanel()
    : m_logicalChildren(nullptr)    // Deferred construction
    , m_ownerItemsControl(nullptr)
{
	m_hitTestMode = detail::UIHitTestMode::InvisiblePanel;
}

bool UILayoutPanel::init()
{
    if (!UIElement::init()) return false;
    style()->setMargin(0);  // Fill parent rect.
    return true;
}

void UILayoutPanel::removeChild(UIElement* child)
{
    if (LN_REQUIRE(child)) return;
    if (m_logicalChildren) {
        m_logicalChildren->remove(child);
        removeVisualChild(child);
    }
}

void UILayoutPanel::removeAllChildren()
{
    if (m_logicalChildren) {
        for (auto& child : *m_logicalChildren) {
            removeVisualChild(child);
        }
        m_logicalChildren->clear();
    }
}

void UILayoutPanel::onDispose(bool explicitDisposing)
{
    removeAllChildren();
    UIElement::onDispose(explicitDisposing);
}

void UILayoutPanel::onAddChild(UIElement* child)
{
    if (LN_REQUIRE(child)) return;

    if (!m_logicalChildren) {
        m_logicalChildren = makeCollection<Ref<UIElement>>();
    }

    if (child->logicalParent()) {
        child->removeFromLogicalParent();
    }

    m_logicalChildren->add(child);
    addVisualChild(child);
}

float UILayoutPanel::getExtentWidth() const { return m_desiredSize.width; }
float UILayoutPanel::getExtentHeight() const { return m_desiredSize.height; }
float UILayoutPanel::getViewportWidth() const { return actualSize().width; }
float UILayoutPanel::getViewportHeight() const { return actualSize().height; }
void UILayoutPanel::setHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel::getHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel::setVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel::getVerticalOffset() const { return m_scrollOffset.y; }


//==============================================================================
// UIFrameLayout2

UIFrameLayout2::UIFrameLayout2()
{
}

void UIFrameLayout2::init()
{
    UILayoutPanel::init();
}

Size UIFrameLayout2::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
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
	return UIFrameLayout2::staticMeasureLogicalChildren(layoutContext, this, constraint);
}

Size UIFrameLayout2::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
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

	return UIFrameLayout2::staticArrangeLogicalChildren(layoutContext, this, finalArea);
    //return finalSize;
}

//Size UIFrameLayout2::staticMeasureChildrenAreaSize(UILayoutContext* layoutContext, UIElement* ownerElement, const Size& constraint)
//{
//    int childrenCount = ownerElement->getVisualChildrenCount();
//    Size childMaxSize(0, 0);
//    for (int i = 0; i < childrenCount; i++)
//    {
//        UIElement* child = ownerElement->getVisualChild(i);
//		if (layoutContext->testLayoutEnabled(child)) {
//			child->measureLayout(layoutContext, constraint);
//			const Size& desiredSize = child->desiredSize();
//			childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
//			childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
//		}
//    }
//    return childMaxSize;
//}

//Size UIFrameLayout2::staticMeasureOverride(UILayoutContext* layoutContext, UIElement* ownerElement, const Size& constraint)
//{
//    Size childMaxSize = staticMeasureChildrenAreaSize(layoutContext, ownerElement, constraint);
//
//    return layoutContext->makeDesiredSize(ownerElement, childMaxSize);
//    //return detail::LayoutHelper::measureElement(ownerElement, constraint, childMaxSize);
//
//    //auto selfSize = ownerElement->UIElement::measureOverride(constraint);
//    //return Size::max(selfSize, childMaxSize);
//
//    //Size size = Size(ownerElement->width(), ownerElement->height());
//    //size.width = Math::isNaN(size.width) ? 0.0 : size.width;
//    //size.height = Math::isNaN(size.height) ? 0.0 : size.height;
//    //return Size::min(constraint, Size::max(size, childMaxSize));
//}
//
//Size UIFrameLayout2::staticArrangeLogicalChildrenArea(UILayoutContext* layoutContext, UIElement* ownerElement, const Rect& finalArea)
//{
//    int childrenCount = ownerElement->m_logicalChildren.;////getVisualChildrenCount();
//    for (int i = 0; i < childrenCount; i++)
//    {
//        UIElement* child = ownerElement->getVisualChild(i);
//		if (layoutContext->testLayoutEnabled(child)) {
//
//			Rect slotRect;
//			detail::LayoutHelper::adjustAlignment(finalArea, child->desiredSize(), ownerElement->m_finalStyle->horizontalContentAlignment, ownerElement->m_finalStyle->verticalContentAlignment, &slotRect);
//
//			child->arrangeLayout(layoutContext, slotRect);
//		}
//    }
//    return finalArea.getSize();
//}

//Size UIFrameLayout2::staticArrangeLogicalChildren(UILayoutContext* layoutContext, UIElement* ownerElement, const Rect& finalArea)
//{
//	Rect contentArea = detail::LayoutHelper::arrangeClientArea(ownerElement, finalArea);
//    staticArrangeLogicalChildrenArea(layoutContext, ownerElement, contentArea);
//    return contentArea.getSize();
//}

//==============================================================================
// UIStackLayout2_Obsolete

Ref<UIStackLayout2_Obsolete> UIStackLayout2_Obsolete::create()
{
    return makeObject_deprecated<UIStackLayout2_Obsolete>();
}

UIStackLayout2_Obsolete::UIStackLayout2_Obsolete()
    : m_orientation(UILayoutOrientation::Vertical)
{
}

void UIStackLayout2_Obsolete::init()
{
    UILayoutPanel::init();
}

void UIStackLayout2_Obsolete::addChild(UIElement* element, UILayoutLengthType type)
{
    UILayoutPanel::addChild(element);
    CellDefinition cell;
    cell.type = type;
    m_cellDefinitions.add(cell);
}

Size UIStackLayout2_Obsolete::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    int childCount = getVisualChildrenCount();

    // Create missing cells.
    if (m_cellDefinitions.size() < childCount) {
        m_cellDefinitions.resize(childCount);
    }

    Size size = constraint;

    if (m_orientation == UILayoutOrientation::Horizontal) {
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
        if (child->specialElementFlags().hasFlag(detail::UISpecialElementFlags::Popup)) {   // TODO: layoutcontext のメソッドで判定
            m_cellDefinitions[i].desiredSize = 0;
            continue;
        }
        child->measureLayout(layoutContext, size);

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

Size UIStackLayout2_Obsolete::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
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
            if (child->specialElementFlags().hasFlag(detail::UISpecialElementFlags::Popup)) continue; // TODO: layoutcontext のメソッドで判定
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(cell.actualOffset, 0, cell.actualSize, finalSize.height);
            child->arrangeLayout(layoutContext, childRect);
        }
    }
    else {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            if (child->specialElementFlags().hasFlag(detail::UISpecialElementFlags::Popup)) continue; // TODO: layoutcontext のメソッドで判定
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
            child->arrangeLayout(layoutContext, childRect);
        }
    }

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::arrangeOverride(layoutContext, finalArea);
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
        case UILayoutOrientation::Horizontal:
            childRect.x += prevChildSize;
            prevChildSize = childDesiredSize.width;
            childRect.width = prevChildSize;
            childRect.height = childrenBoundSize.height;
            break;
        case UILayoutOrientation::Vertical:
            childRect.y += prevChildSize;
            prevChildSize = childDesiredSize.height;
            childRect.width = childrenBoundSize.width;
            childRect.height = prevChildSize;
            break;
        case UILayoutOrientation::ReverseHorizontal:
            prevChildSize = childDesiredSize.width;
            rPos -= prevChildSize;
            childRect.x = childrenBoundSize.width + rPos;
            childRect.width = prevChildSize;
            childRect.height = childrenBoundSize.height;
            break;
        case UILayoutOrientation::ReverseVertical:
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
// UIHBoxLayout2

void UIHBoxLayout2::init()
{
    UIStackLayout2_Obsolete::init();
    setOrientation(UILayoutOrientation::Horizontal);
}

//==============================================================================
// UIVBoxLayout2

void UIVBoxLayout2::init()
{
    UIStackLayout2_Obsolete::init();
    setOrientation(UILayoutOrientation::Vertical);
}

//==============================================================================
// UIBoxLayout

LN_OBJECT_IMPLEMENT(UIBoxLayout, UILayoutPanel) {}

UIBoxLayout::UIBoxLayout()
    : m_orientation(UILayoutOrientation::Vertical)
{
}

void UIBoxLayout::init()
{
    UILayoutPanel::init();
}

Size UIBoxLayout::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    int childCount = getVisualChildrenCount();

    // Create missing cells.
    if (m_cellDefinitions.size() < childCount) {
        m_cellDefinitions.resize(childCount);
    }

    Size size = constraint;

    //if (m_orientation == UILayoutOrientation::Horizontal) {
    //    // 横に並べる場合、幅の制限を設けない
    //    size.width = std::numeric_limits<float>::infinity();
    //}
    //else {
    //    // 縦に並べる場合、高さの制限を設けない
    //    size.height = std::numeric_limits<float>::infinity();
    //}

    Size desiredSize;
    for (int i = 0; i < childCount; i++)
    {
        UIElement* child = getVisualChild(i);
        child->measureLayout(layoutContext, size);

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

Size UIBoxLayout::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
    const Thickness& padding = finalStyle()->padding;
    Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));
    int actualCellCount = (m_logicalChildren) ? m_logicalChildren->size() : 0;

    float boundSize = 0.0f;
    if (isHorizontal())
        boundSize = childrenBoundSize.width;
    else
        boundSize = childrenBoundSize.height;

    // Fix final size of 'Auto' and 'Direct', and count 'Ratio'
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
            if (iChild == childCount - 1)  // 最終 Cell は余りサイズをすべて使う
                childRect.width = std::max(childRect.width, finalSize.width - cell.actualOffset);
            child->arrangeLayout(layoutContext, childRect);
        }
    }
    else {
        for (int iChild = 0; iChild < childCount; iChild++) {
            auto* child = getVisualChild(iChild);
            auto& cell = m_cellDefinitions[iChild];
            Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
            if (iChild == childCount - 1) 
                childRect.height = std::max(childRect.height, finalSize.height - cell.actualOffset);
            child->arrangeLayout(layoutContext, childRect);
        }
    }

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::arrangeOverride(layoutContext, finalArea);
    return selfSize;
}

UILayoutLengthType UIBoxLayout::layoutType(int index) const
{
    if (LN_REQUIRE(m_logicalChildren)) return UILayoutLengthType::Ratio;

    auto& child = m_logicalChildren->at(index);

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
    if (!info) {
        if (isHorizontal()) {
            if (child->getLayoutHAlignment() == UIHAlignment::Stretch)
                return UILayoutLengthType::Ratio;
            else
                return UILayoutLengthType::Auto;
        }
        else {
            if (child->getLayoutVAlignment() == UIVAlignment::Stretch)
                return UILayoutLengthType::Ratio;
            else
                return UILayoutLengthType::Auto;
        }
    }
    else {
        if (info->layoutWeight < 0.0f)
            return UILayoutLengthType::Ratio;	// default
        if (info->layoutWeight == 0.0)
            return UILayoutLengthType::Auto;
        else
            return UILayoutLengthType::Ratio;
    }
}

float UIBoxLayout::layoutWeight(int index) const
{
    if (LN_REQUIRE(m_logicalChildren)) return 1.0f;

    auto& child = m_logicalChildren->at(index);
    if (child->m_gridLayoutInfo)
        return child->m_gridLayoutInfo->layoutWeight;
    else
        return 1.0f;
}

float UIBoxLayout::layoutDirectSize(int index) const
{
    if (LN_REQUIRE(m_logicalChildren)) return 1.0f;

    auto& child = m_logicalChildren->at(index);
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

void UIBoxLayout::getLayoutMinMaxSize(int index, float* minSize, float* maxSize) const
{
    *minSize = 0.0f;
    *maxSize = std::numeric_limits<float>::max();

    if (LN_REQUIRE(m_logicalChildren)) return;

    auto& child = m_logicalChildren->at(index);
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
// UIHBoxLayout3

void UIHBoxLayout3::init()
{
	UIBoxLayout::init();
	setOrientation(UILayoutOrientation::Horizontal);
}

//==============================================================================
// UIVBoxLayout3

void UIVBoxLayout3::init()
{
    UIBoxLayout::init();
    setOrientation(UILayoutOrientation::Vertical);
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
    if (LN_REQUIRE(m_logicalChildren)) return;
    m_activeIndex = m_logicalChildren->indexOf(child);
    invalidateLayout();
}

void UISwitchLayout::init()
{
	UIFrameLayout2::init();
}

Size UISwitchLayout::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    int count = (m_logicalChildren) ? m_logicalChildren->size() : 0;
	for (int i = 0; i < count; i++) {
		if (i == m_activeIndex)
			m_logicalChildren->at(i)->m_internalVisibility = UIVisibility::Visible;
		else
			m_logicalChildren->at(i)->m_internalVisibility = UIVisibility::Collapsed;
	}

	return UIFrameLayout2::measureOverride(layoutContext, constraint);
}



//==============================================================================
// UILayoutPanel2_Deprecated

UILayoutPanel2_Deprecated::UILayoutPanel2_Deprecated()
{
}

void UILayoutPanel2_Deprecated::init()
{
    Object::init();
}

void UILayoutPanel2_Deprecated::measureLayout(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& availableSize)
{
    m_desiredSize = measureOverride(layoutContext, childElements, availableSize);
}

void UILayoutPanel2_Deprecated::arrangeLayout(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect)
{
    Rect rect = finalSlotRect;
    rect.x -= m_scrollOffset.x;
    rect.y -= m_scrollOffset.y;
    m_actualSize = arrangeOverride(layoutContext, childElements, rect);
}

float UILayoutPanel2_Deprecated::getExtentWidth() const { return m_desiredSize.width; }
float UILayoutPanel2_Deprecated::getExtentHeight() const { return m_desiredSize.height; }
float UILayoutPanel2_Deprecated::getViewportWidth() const { return m_actualSize.width; }
float UILayoutPanel2_Deprecated::getViewportHeight() const { return m_actualSize.height; }
void UILayoutPanel2_Deprecated::setHorizontalOffset(float offset) { m_scrollOffset.x = offset; }
float UILayoutPanel2_Deprecated::getHorizontalOffset() const { return m_scrollOffset.x; }
void UILayoutPanel2_Deprecated::setVerticalOffset(float offset) { m_scrollOffset.y = offset; }
float UILayoutPanel2_Deprecated::getVerticalOffset() const { return m_scrollOffset.y; }

//==============================================================================
// UIFrameLayout

Ref<UIFrameLayout> UIFrameLayout::create()
{
    return makeObject_deprecated<UIFrameLayout>();
}

UIFrameLayout::UIFrameLayout()
{
}

UIFrameLayout::~UIFrameLayout()
{
}

void UIFrameLayout::init()
{
    UILayoutPanel2_Deprecated::init();
}

Size UIFrameLayout::measureOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& constraint)
{
    int childrenCount = childElements->getElementCount();
    Size childMaxSize(0, 0);
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements->getElement(i);
        child->measureLayout(layoutContext, constraint);
        const Size& desiredSize = child->desiredSize();
        childMaxSize.width = std::max(childMaxSize.width, desiredSize.width);
        childMaxSize.height = std::max(childMaxSize.height, desiredSize.height);
    }
    return childMaxSize;
}

Size UIFrameLayout::arrangeOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect)
{
    Rect bounds(finalSlotRect);

    int childrenCount = childElements->getElementCount();
    for (int i = 0; i < childrenCount; i++)
    {
        UIElement* child = childElements->getElement(i);
        child->arrangeLayout(layoutContext, bounds);
    }

    return finalSlotRect.getSize();
}


//==============================================================================
// UIStackLayout_Obsolete

Ref<UIStackLayout_Obsolete> UIStackLayout_Obsolete::create()
{
    return makeObject_deprecated<UIStackLayout_Obsolete>();
}

UIStackLayout_Obsolete::UIStackLayout_Obsolete()
    : m_orientation(UILayoutOrientation::Vertical)
{
}

UIStackLayout_Obsolete::~UIStackLayout_Obsolete()
{
}

void UIStackLayout_Obsolete::init()
{
    UILayoutPanel2_Deprecated::init();
}

Size UIStackLayout_Obsolete::measureOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Size& constraint)
{
    Size size = constraint;

    if (m_orientation == UILayoutOrientation::Horizontal) {
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
        child->measureLayout(layoutContext, size);

        const Size& childDesiredSize = child->getLayoutDesiredSize();
        if (m_orientation == UILayoutOrientation::Horizontal || m_orientation == UILayoutOrientation::ReverseHorizontal) {
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

Size UIStackLayout_Obsolete::arrangeOverride(UILayoutContext* layoutContext, const IUIElementList* childElements, const Rect& finalSlotRect)
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
        case UILayoutOrientation::Horizontal:
            childRect.x += prevChildSize;
            prevChildSize = childDesiredSize.width;
            childRect.width = prevChildSize;
            childRect.height = childrenBoundSize.height;
            break;
        case UILayoutOrientation::Vertical:
            childRect.y += prevChildSize;
            prevChildSize = childDesiredSize.height;
            childRect.width = childrenBoundSize.width;
            childRect.height = prevChildSize;
            break;
        case UILayoutOrientation::ReverseHorizontal:
            prevChildSize = childDesiredSize.width;
            rPos -= prevChildSize;
            childRect.x = childrenBoundSize.width + rPos;
            childRect.width = prevChildSize;
            childRect.height = childrenBoundSize.height;
            break;
        case UILayoutOrientation::ReverseVertical:
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

        child->arrangeLayout(layoutContext, actual);
    }

    return finalSlotRect.getSize();
}

//==============================================================================
// UIStackLayout

Ref<UIStackLayout> UIStackLayout::create()
{
    return makeObject_deprecated<UIStackLayout>();
}

Ref<UIStackLayout> UIStackLayout::create(UILayoutOrientation orientation)
{
    return makeObject_deprecated<UIStackLayout>(orientation);
}

UIStackLayout::UIStackLayout()
    : m_orientation(UILayoutOrientation::Vertical)
{
}

bool UIStackLayout::init()
{
    UILayoutPanel::init();
    return true;
}

bool UIStackLayout::init(UILayoutOrientation orientation)
{
    if (!init()) return false;
    setOrientation(orientation);
    return true;
}


Size UIStackLayout::measureOverrideImpl(UILayoutContext* layoutContext, const Size& constraint, int itemCount, const std::function<UIElement*(int i)>& getItem, UILayoutOrientation orientation)
{
    Size size = constraint;

    if (orientation == UILayoutOrientation::Horizontal) {
        // 横に並べる場合、幅の制限を設けない
        size.width = std::numeric_limits<float>::infinity();
    }
    else {
        // 縦に並べる場合、高さの制限を設けない
        size.height = std::numeric_limits<float>::infinity();
    }

    Size desiredSize;
    for (int i = 0; i < itemCount; i++)
    {
        UIElement* child = getItem(i);
        if (layoutContext->testLayoutEnabled(child)) {
            child->measureLayout(layoutContext, size);

            const Size& childDesiredSize = child->getLayoutDesiredSize();
            if (orientation == UILayoutOrientation::Horizontal || orientation == UILayoutOrientation::ReverseHorizontal) {
                desiredSize.width += childDesiredSize.width;
                desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
            }
            else {
                desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
                desiredSize.height += child->getLayoutDesiredSize().height;
            }
        }
    }

    return desiredSize;
}

Size UIStackLayout::arrangeOverrideImpl(
    UILayoutContext* layoutContext,
    const Rect& finalArea,
    int itemCount,
    const std::function<UIElement*(int i)>& getItem,
    UILayoutOrientation orientation,
    detail::UIStyleInstance* style,
    bool lastStretch,
    const Vector2& scrollOffset)
{
    const auto finalSize = finalArea.getSize();
    const Thickness& padding = style->padding;
    Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));
    //int actualCellCount = m_logicalChildren.size();
    Rect finalSlotRect(padding.left, padding.top, childrenBoundSize.width, childrenBoundSize.height);

    //Size childrenBoundSize(finalSlotRect.width, finalSlotRect.height);
    //Size selfBounds(FLT_MAX, FLT_MAX);
    //if (stretch) {
    //    selfBounds = finalSlotRect.getSize();
    //}

    float prevChildSize = 0;
    float rPos = 0;
    Rect childRect(0, 0, 0, 0);
    for (int i = 0; i < itemCount; i++)
    {
        UIElement* child = getItem(i);
        if (layoutContext->testLayoutEnabled(child)) {
            const Size& childDesiredSize = child->getLayoutDesiredSize();

            switch (orientation)
            {
            case UILayoutOrientation::Horizontal:
                childRect.x += prevChildSize;
                prevChildSize = childDesiredSize.width;
                childRect.width = prevChildSize;
                childRect.height = childrenBoundSize.height;
                break;
            case UILayoutOrientation::Vertical:
                childRect.y += prevChildSize;
                prevChildSize = childDesiredSize.height;
                childRect.width = childrenBoundSize.width;
                childRect.height = prevChildSize;
                break;
            case UILayoutOrientation::ReverseHorizontal:
                prevChildSize = childDesiredSize.width;
                rPos -= prevChildSize;
                childRect.x = childrenBoundSize.width + rPos;
                childRect.width = prevChildSize;
                childRect.height = childrenBoundSize.height;
                break;
            case UILayoutOrientation::ReverseVertical:
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
            if (lastStretch && i == itemCount - 1) {
                actual.width = finalSlotRect.width - actual.x;
                actual.height = finalSlotRect.height - actual.y;
            }

            actual.x -= scrollOffset.x;
            actual.y -= scrollOffset.y;
            child->arrangeLayout(layoutContext, actual);
        }
    }

    return finalSlotRect.getSize();
}

Size UIStackLayout::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    return measureOverrideImpl(
        layoutContext,
        constraint,
        getVisualChildrenCount(),
        [this](int i) { return getVisualChild(i); },
        m_orientation);
}

Size UIStackLayout::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    return arrangeOverrideImpl(
        layoutContext,
        finalArea,
        getVisualChildrenCount(),
        [this](int i) { return getVisualChild(i); },
        m_orientation,
        finalStyle(),
        lastStretch,
        m_scrollOffset);
}

} // namespace ln

