
#include "Internal.hpp"
#include <LuminoEngine/UI/UIScrollView.hpp> // for Thumb
#include <LuminoEngine/UI/UISplitter.hpp>

namespace ln {

//==============================================================================
// UISplitter

UISplitter::UISplitter()
{
}

void UISplitter::init()
{
    UIControl::init();
}

void UISplitter::addCellDefinition(UILayoutLengthType type, float size, float minSize, float maxSize)
{
    detail::GridDefinitionData data;
    data.type = type;
    data.size = size;
    data.minSize = minSize;
    data.maxSize = maxSize;
    m_cellDefinitions.add(data);
}

void UISplitter::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    int thumbCount = logicalChildren().size() - 1;
    int diff = thumbCount - m_thumbs.size();
    for (int i = 0; i < diff; i++) {
        auto thumb = makeObject<UIThumb>();
        thumb->setBackgroundColor(Color::Blue);
        addVisualChild(thumb);
        m_thumbs.add(thumb);
    }

    return UIControl::onUpdateStyle(styleContext, finalStyle);
}

Size UISplitter::measureOverride(const Size& constraint)
{
    // まずは子を measure
    Size childrenSize;
    auto& children = logicalChildren();
    for (int iChild = 0; iChild < children.size(); iChild++) {
        auto& child = children[iChild];
        child->measureLayout(constraint);
        const Size& childDesiredSize = child->desiredSize();

        int cellIndex = getGridLayoutInfo()->layoutRow;
        if (cellIndex < 0) cellIndex = iChild;

        detail::GridDefinitionData* cell = (0 <= cellIndex && cellIndex < m_cellDefinitions.size()) ? &m_cellDefinitions[cellIndex] : nullptr;

        // セルサイズを子要素のサイズに合わせる場合
        if (cell != nullptr && cell->type == UILayoutLengthType::Auto) {
            if (m_orientation == Orientation::Horizontal)
                cell->desiredSize = std::max(cell->desiredSize, childDesiredSize.width);
            else
                cell->desiredSize = std::max(cell->desiredSize, childDesiredSize.height);
        }

        // total size
        if (m_orientation == Orientation::Horizontal)
            childrenSize.width += childDesiredSize.width;
        else
            childrenSize.height += childDesiredSize.height;
    }

    // bar area
    if (m_orientation == Orientation::Horizontal)
        childrenSize.width += m_thumbs.size() * ThumbWidth;
    else
        childrenSize.height += m_thumbs.size() * ThumbWidth;

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::measureOverride(constraint);
    return Size::max(selfSize, childrenSize);
}

Size UISplitter::arrangeOverride(const Size& finalSize)
{
    float boundSize = 0.0f;
    if (m_orientation == Orientation::Horizontal)
        boundSize = finalSize.width;
    else
        boundSize = finalSize.height;


    float totalActualSize = 0.0f;
    float ratioCellCount = 0.0f;
    for (auto& cell : m_cellDefinitions) {
        if (cell.type == UILayoutLengthType::Auto || cell.type == UILayoutLengthType::Direct) {
            cell.actualSize = cell.getAvailableDesiredSize();
            totalActualSize += cell.actualSize;
        }
        else {
            ratioCellCount += cell.getRatioSize();
        }
    }


    // "1*" 分のセルの領域を計算する
    float ratioUnit = (ratioCellCount != 0.0f) ? (boundSize - totalActualSize) / ratioCellCount : 0.0f;
    ratioUnit = std::max(0.0f, ratioUnit);	// 負値はダメ

    // "*" 指定である Row/Column の最終サイズを確定させ、
    // 全セルのオフセット (位置) も確定させる
    float totalOffset = 0.0f;
    //for (auto& cell : m_cellDefinitions) {
    for (int i = 0; i < m_cellDefinitions.size(); i++) {
        auto& cell = m_cellDefinitions[i];
        if (cell.type == UILayoutLengthType::Ratio) {
            cell.actualSize = ratioUnit * cell.getRatioSize();
        }

        cell.adjustActualSize();

        // 座標確定
        cell.actualOffset = totalOffset;
        totalOffset += cell.actualSize;

        // bar size
        if (i > 0) {
            totalOffset += ThumbWidth;
        }
    }

    // 子要素の最終位置・サイズを確定させる
    auto& children = logicalChildren();
    if (m_orientation == Orientation::Horizontal) {
        for (int iChild = 0; iChild < children.size(); iChild++) {
            auto& child = children[iChild];
            int cellIndex = child->getGridLayoutInfo()->layoutRow;
            if (cellIndex < 0) cellIndex = iChild;
            auto& cell = m_cellDefinitions[cellIndex];
            Rect childRect(cell.actualOffset, 0, cell.actualSize, finalSize.height);
            child->arrangeLayout(childRect);

            // bar
            if (iChild < m_thumbs.size()) {
                childRect.x = childRect.getRight();
                childRect.width = ThumbWidth;
                m_thumbs[iChild]->arrangeLayout(childRect);
            }
        }
    }
    else {
        for (int iChild = 0; iChild < children.size(); iChild++) {
            auto& child = children[iChild];
            int cellIndex = child->getGridLayoutInfo()->layoutRow;
            if (cellIndex < 0) cellIndex = iChild;
            auto& cell = m_cellDefinitions[cellIndex];
            Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
            child->arrangeLayout(childRect);

            // bar
            if (iChild < m_thumbs.size()) {
                childRect.y = childRect.getBottom();
                childRect.height = ThumbWidth;
                m_thumbs[iChild]->arrangeLayout(childRect);
            }
        }
    }

    // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
    Size selfSize = UIElement::arrangeOverride(finalSize);
    return selfSize;
}

void UISplitter::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::ScrollDragStartedEvent)
    {
        e->handled = true;
    }
    else if (e->type() == UIEvents::ScrollDragDeltaEvent)
    {
        e->handled = true;
    }
    else if (e->type() == UIEvents::ScrollDragCompletedEvent)
    {
        e->handled = true;
    }
    UIElement::onRoutedEvent(e);
}

} // namespace ln

