
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UIGridLayout.hpp>
#include "../UIStyleInstance.hpp"

namespace ln {
	
//==============================================================================
// UIGridLayout::Builder
	
LN_OBJECT_IMPLEMENT(UIGridLayout, UILayoutPanel) {}
//LN_BUILDER_IMPLEMENT(UIGridLayout, UILayoutPanel);

//UIGridLayout::Builder::Builder() : Builder(makeRef<Details>()) {}
//	UIGridLayout::Builder::Builder(Details* d) : base::Builder(d) {}
//
	//type::Builder() : Builder(makeRef<type::Builder::Details>()) {} \
	//	type::Builder(type::Builder::Details* d) : base::Builder(d) {}

//UIGridLayout::Builder::Builder()
//	: Builder(makeRef<Details>())
//{
//}
//
//UIGridLayout::Builder::Builder(Details* d)
//	: UIElement::Builder(d)
//{
//}

//UIGridLayout::Builder& UIGridLayout::Builder::columnCount(int value)
//{
//	detailsAs<Details>()->columnCount = value;
//	return *this;
//}

//UIGridLayout::Builder& UIGridLayout::Builder::add(const UIElement::Builder& value)
//{
//	detailsAs<Details>()->children.push_back(value);
//	return *this;
//}

//Ref<UIGridLayout> UIGridLayout::Builder::build()
//{
//	return buildAs<UIGridLayout>();
//}

//Ref<Object> UIGridLayout::Builder::Details::build()
//{
//	auto ptr = makeObject<UIGridLayout>();
//	ptr->setColumnCount(columnCount);
//	for (auto& b : children)
//		ptr->addChild(b.build());
//	return ptr;
//}
//
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


Flow layout と Box layout
----------
↓こんな感じはどうだろう？

### Flow
WPF の StackPanel.
指定方向にはサイズを固定しない。Cell 常に Auto で、子要素の総和に合わせる。＝子要素に依存する。
Height 0 である子要素はすべて潰れる。
主にコレクションの配置に使用する。
なお Cell に 指定方向 Ratio は使用不可。

### Box
LayoutPanel の大きさは常に親要素のサイズに制限される。
（大量の子要素を追加した結果はみ出しても、LayoutPanel の大きさは調整しない。＝子要素に依存しない。）
Cell の Default は Ratio.
主にコレクション以外の、一般的な要素の配置に使用する。

*/
	
UIGridLayout::UIGridLayout()
	: m_rule(UILayoutRule::Box)
	, m_rowDefinitions()
	, m_columnDefinitions()
	, m_columnCount(1)
{
}

bool UIGridLayout::init()
{
	if (!UILayoutPanel::init()) return false;
	m_nextCellIndex = 0;
	return true;
}

void UIGridLayout::setRule(UILayoutRule value)
{
	m_rule = value;
}

void UIGridLayout::setColumnCount(int value)
{
	m_columnCount = value;
}

void UIGridLayout::setRow(UIElement* element, int row)
{
	auto* info = element->getGridLayoutInfo();
	info->actualLayoutRow = row;
}

void UIGridLayout::setColumn(UIElement* element, int column)
{
	auto* info = element->getGridLayoutInfo();
	info->actualLayoutColumn = column;
}

void UIGridLayout::setPlacement(UIElement* element, int row, int column, int rowSpan, int columnSpan)
{
	auto* info = element->getGridLayoutInfo();
	info->actualLayoutRow = row;
	info->actualLayoutColumn = column;
	info->layoutRowSpan = rowSpan;
	info->layoutColumnSpan = columnSpan;
}

void UIGridLayout::onAddChild(UIElement* child)
{
	//int childCount = getVisualChildrenCount();
	//auto* info = child->getGridLayoutInfo();
	//info->actualLayoutRow = childCount / actualColumnCount();
	//info->actualLayoutColumn = childCount % actualColumnCount();

	UILayoutPanel::onAddChild(child);

	auto* info = child->getGridLayoutInfo();
	info->actualLayoutRow = m_nextCellIndex / actualColumnCount();
	info->actualLayoutColumn = m_nextCellIndex % actualColumnCount();

	int rowSpan = std::max(1, info->layoutRowSpan) - 1;
	int colSpan = std::max(1, info->layoutColumnSpan);
	m_nextCellIndex += colSpan + (rowSpan * actualColumnCount());
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
		m_rowDefinitions.add(GridDefinitionData({ defaultRowLengthType() }));
	}
	while (m_columnDefinitions.size() < colCount) {
		m_columnDefinitions.add(GridDefinitionData({ defaultColumnLengthType() }));
	}

	// Measure desired sizes per cells.
	for (int i = 0; i < childCount; i++) {
		auto* child = getVisualChild(i);

		const auto* info = child->getGridLayoutInfo();
		auto& row = m_rowDefinitions[info->actualLayoutRow];
		auto& col = m_columnDefinitions[info->actualLayoutColumn];

		const auto& childDesiredSize = child->desiredSize();
		if (row.type == UILayoutLengthType::Auto || row.type == UILayoutLengthType::Ratio) {
			row.desiredSize = std::max(row.desiredSize, childDesiredSize.height);
		}
		if (col.type == UILayoutLengthType::Auto || row.type == UILayoutLengthType::Ratio) {
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

	Size desiredSize = UILayoutPanel::measureOverride(layoutContext, constraint);
	return Size::max(desiredSize, cellsDesiredSize);
}

Size UIGridLayout::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
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

int UIGridLayout::actualColumnCount() const
{
	return std::max(1, m_columnCount);
}

UILayoutLengthType UIGridLayout::defaultRowLengthType() const
{
	if (m_rule == UILayoutRule::VerticalFlow)
		return UILayoutLengthType::Auto;
	else
		return UILayoutLengthType::Ratio;
}

UILayoutLengthType UIGridLayout::defaultColumnLengthType() const
{
	if (m_rule == UILayoutRule::HorizontalFlow)
		return UILayoutLengthType::Auto;
	else
		return UILayoutLengthType::Ratio;
}

} // namespace ln

