
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIGridLayout.hpp>

namespace ln {

//==============================================================================
// UIGridLayout

/*
GridLayout 側でサイズを指定する
- QtQuick
- Flutter https://flutter.ctrnost.com/basic/layout/gridview/
	- カラム数だけ指定
- kivy https://kivy.org/doc/stable/api-kivy.uix.gridlayout.html#kivy.uix.gridlayout.GridLayout
	- カラム数だけ指定
- ImGUI(提案) https://github.com/ocornut/imgui/issues/857
	- カラム数だけ指定
- Unity https://qiita.com/t_Kaku_7/items/588fada25cf2d519589d
	- row or col どちらかの数を指定


Item 側で配置セルを指定する
- CSS https://qiita.com/kura07/items/e633b35e33e43240d363


両方 (めんどい)
- WPF

その他
- Android http://ichitcltk.hustle.ne.jp/gudon2/index.php?pageType=file&id=Android061_TableLayout
	- TableRow というクラスで行を表す
*/
	
UIGridLayout::UIGridLayout()
{
}

void UIGridLayout::init()
{
    UIElement::init();
}

Size UIGridLayout::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	// Measure layout and grid size.
	int childCount = getVisualChildrenCount();
	int rowCount = 0;
	int colCount = 0;
	for (int i = 0; i < childCount; i++) {
		auto* child = getVisualChild(i);
		child->measureLayout(layoutContext, constraint);

		const auto* info = child->getGridLayoutInfo();
		rowCount = std::max(rowCount, info->actualLayoutRow + 1);
		colCount = std::max(colCount, info->actualLayoutColumn + 1);
	}

	// Allocate definitions
	while (m_rowDefinitions.size() < rowCount) {
		m_rowDefinitions.add(detail::GridDefinitionData());
	}
	while (m_columnDefinitions.size() < colCount) {
		m_columnDefinitions.add(detail::GridDefinitionData());
	}

	// Measure desired sizes per cells.
	for (int i = 0; i < childCount; i++) {
		auto* child = getVisualChild(i);

		const auto* info = child->getGridLayoutInfo();
		auto& row = m_rowDefinitions[info->actualLayoutRow];
		auto& col = m_rowDefinitions[info->actualLayoutColumn];

		const auto& childDesiredSize = child->desiredSize();
		if (row.type == UILayoutLengthType::Auto) {
			row.desiredSize = std::max(row.desiredSize, childDesiredSize.height);
		}
		if (col.type == UILayoutLengthType::Auto) {
			col.desiredSize = std::max(col.desiredSize, childDesiredSize.width);
		}
	}

	// Aggregate DesiredSize of each cell to find DesiredSize of entire Grid
	Size cellsDesiredSize;
	for (const auto& row : m_rowDefinitions) {
		cellsDesiredSize.height += row.getAvailableDesiredSize();
	}
	for (const auto& col : m_columnDefinitions) {
		cellsDesiredSize.width += col.getAvailableDesiredSize();
	}

	Size desiredSize = UILayoutPanel2::measureOverride(layoutContext, constraint);
	return Size::max(desiredSize, cellsDesiredSize);
}

Size UIGridLayout::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
	const Thickness& padding = finalStyle()->padding;
	Size childrenBoundSize(finalSize.width - (padding.left + padding.right), finalSize.height - (padding.top + padding.bottom));

	// Fix final size of 'Auto' and 'Direct', and count 'Ratio'
	Size totalActualSize;
	float ratioRowCount = 0.0f;
	float ratioColCount = 0.0f;
	for (auto& row : m_rowDefinitions) {
		if (row.type == UILayoutLengthType::Auto || row.type == UILayoutLengthType::Direct) {
			row.actualSize = row.getAvailableDesiredSize();
			totalActualSize.height += row.actualSize;
		}
		else {
			ratioRowCount += row.getRatioSize();
		}
	}
	for (auto& col : m_columnDefinitions) {
		if (col.type == UILayoutLengthType::Auto || col.type == UILayoutLengthType::Direct) {
			col.actualSize = col.getAvailableDesiredSize();
			totalActualSize.width += col.actualSize;
		}
		else {
			ratioColCount += col.getRatioSize();
		}
	}

	// "1*" 分のセルの領域を計算する
	Size ratioUnit(
		(ratioColCount != 0.0f) ? (childrenBoundSize.width - totalActualSize.width) / ratioColCount : 0.0f,
		(ratioRowCount != 0.0f) ? (childrenBoundSize.height - totalActualSize.height) / ratioRowCount : 0.0f);
	ratioUnit.width = std::max(0.0f, ratioUnit.width);
	ratioUnit.height = std::max(0.0f, ratioUnit.height);

	// "*" 指定である Row/Column の最終サイズを確定させ、
	// 全セルのオフセット (位置) も確定させる
	Point totalOffset;
	for (auto& row : m_rowDefinitions) {
		if (row.type == UILayoutLengthType::Ratio) {
			row.actualSize = ratioUnit.height * row.getRatioSize();
		}
		row.adjustActualSize();

		// Fix cell offset
		row.actualOffset = totalOffset.y;
		totalOffset.y += row.actualSize;
	}
	for (auto& col : m_columnDefinitions) {
		if (col.type == UILayoutLengthType::Ratio) {
			col.actualSize = ratioUnit.width * col.getRatioSize();
		}
		col.adjustActualSize();

		// Fix cell offset
		col.actualOffset = totalOffset.x;
		totalOffset.x += col.actualSize;
	}

	// 子要素の最終位置・サイズを確定させる
	int rowDefCount = m_rowDefinitions.size();
	int colDefCount = m_columnDefinitions.size();
	int childCount = getVisualChildrenCount();
	for (int iChild = 0; iChild < childCount; iChild++) {
		auto* child = getVisualChild(iChild);
		const auto* info = child->getGridLayoutInfo();
		int rowIdx = info->actualLayoutRow;
		int colIdx = info->actualLayoutColumn;
		int rowSpan = info->layoutRowSpan;
		int colSpan = info->layoutColumnSpan;
		rowSpan = std::max(1, rowSpan);	// 最低 1
		colSpan = std::max(1, colSpan);	// 最低 1
		rowSpan = std::min(rowSpan, rowIdx + rowDefCount);	// 最大値制限
		colSpan = std::min(colSpan, colIdx + colDefCount);	// 最大値制限

		// Span を考慮してサイズを確定
		Rect rect = Rect(padding.left, padding.top, 0, 0);
		{
			rect.y += m_rowDefinitions[rowIdx].actualOffset;
			for (int iRow = 0; iRow < rowSpan; iRow++) {
				rect.height += m_rowDefinitions[rowIdx + iRow].actualSize;
			}
		}
		{
			rect.x += m_columnDefinitions[colIdx].actualOffset;
			for (int iCol = 0; iCol < colSpan; iCol++) {
				rect.width += m_columnDefinitions[colIdx + iCol].actualSize;
			}
		}

		// Arrange
		child->arrangeLayout(layoutContext, rect);
	}

	return finalSize;
}

} // namespace ln

