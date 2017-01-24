
#include "../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/Grid.h>
#include "../GUIHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// ColumnDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ColumnDefinition, ContentElement);
LN_GUI_CONTENT_ELEMENT_IMPLEMENT(ColumnDefinition);

const float ColumnDefinition::Auto = NAN;
const float ColumnDefinition::Star = std::numeric_limits<float>::infinity();

// Register property
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, WidthProperty, "Width", m_width, PropertyMetadata(ColumnDefinition::Star));
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, MinWidthProperty, "MinWidth", m_minWidth, PropertyMetadata(0.0f));
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, MaxWidthProperty, "MaxWidth", m_maxWidth, PropertyMetadata(FLT_MAX));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinitionPtr ColumnDefinition::Create()
{
	return ColumnDefinitionPtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::ColumnDefinition(GUIManagerImpl* manager)
	: ContentElement(manager)
	, m_width(std::numeric_limits<float>::infinity())
	, m_minWidth(0)
	, m_maxWidth(FLT_MAX)
	, m_actualWidth(0)
	, m_actualOffsetX(0)
	, m_elementGroup()
	, m_desiredWidth(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::~ColumnDefinition()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float ColumnDefinition::GetAvailableDesiredWidth() const
{
	if (IsAuto()) {
		return m_desiredWidth;
	}
	else if (IsPixel()) {
		return Math::Clamp(m_width, m_minWidth, m_maxWidth);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

//=============================================================================
// RowDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RowDefinition, ContentElement);
LN_GUI_CONTENT_ELEMENT_IMPLEMENT(RowDefinition);

const float RowDefinition::Auto = NAN;
const float RowDefinition::Star = std::numeric_limits<float>::infinity();

// Register property
LN_PROPERTY_IMPLEMENT(RowDefinition, float, HeightProperty, "Height", m_height, PropertyMetadata(ColumnDefinition::Star));
LN_PROPERTY_IMPLEMENT(RowDefinition, float, MinHeightProperty, "MinHeight", m_minHeight, PropertyMetadata(0.0f));
LN_PROPERTY_IMPLEMENT(RowDefinition, float, MaxHeightProperty, "MaxHeight", m_maxHeight, PropertyMetadata(FLT_MAX));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinitionPtr RowDefinition::Create()
{
	return RowDefinitionPtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::RowDefinition(GUIManagerImpl* manager)
	: ContentElement(manager)
	, m_height(std::numeric_limits<float>::infinity())
	, m_minHeight(0)
	, m_maxHeight(FLT_MAX)
	, m_actualHeight(0)
	, m_actualOffsetY(0)
	, m_elementGroup()
	, m_desiredHeight(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::~RowDefinition()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float RowDefinition::GetAvailableDesiredHeight() const
{
	if (IsAuto()) {
		return m_desiredHeight;
	}
	else if (IsPixel()) {
		return Math::Clamp(m_height, m_minHeight, m_maxHeight);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

//=============================================================================
// Grid
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Grid, Panel);
LN_UI_ELEMENT_SUBCLASS_IMPL(Grid);

// Register property
LN_PROPERTY_IMPLEMENT(Grid, ColumnDefinitionList*, ColumnDefinitionsProperty, "ColumnDefinitions", m_columnDefinitionList, PropertyMetadata(NULL));
LN_PROPERTY_IMPLEMENT(Grid, RowDefinitionList*, RowDefinitionsProperty, "RowDefinitions", m_rowDefinitionList, PropertyMetadata(NULL));

// Register attached property
LN_DEFINE_ATTACHED_PROPERTY(Grid, ColumnProperty, "Column", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, ColumnSpanProperty, "ColumnSpan", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, RowProperty, "Row", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, RowSpanProperty, "RowSpan", 0);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid* Grid::Create()
{
	auto* obj = internalCreateInstance(GetUIManager());
	AutoReleasePool::AddObjectToCurrent(obj);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::Grid(GUIManagerImpl* manager)
	: Panel(manager)
{
	m_columnDefinitionList.Attach(LN_NEW ColumnDefinitionList());
	m_rowDefinitionList.Attach(LN_NEW RowDefinitionList());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::~Grid()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::MeasureLayout(const SizeF& availableSize)
{
	//for (ColumnDefinition* col : *m_columnDefinitionList) { col->m_elementGroup.Clear(); col->m_desiredWidth = 0; }
	//for (RowDefinition* row : *m_rowDefinitionList)	 { row->m_elementGroup.Clear(); row->m_desiredHeight = 0; }

	// まず Measure は全部まわしておく
	GUIHelper::ForEachVisualChildren(this, [availableSize](UIElement* child) { child->MeasureLayout(availableSize); });

	for (auto child : *m_children)
	{
		// 子要素を行と列ごとにグループ化する
		//		- 範囲外のインデックスが指定されていたら 0 にする。
		int colIdx = Grid::GetColumn(child);	// TODO: このプロパティ検索がボトルネックになるかも
		int rowIdx = Grid::GetRow(child);
		colIdx = m_columnDefinitionList->IsOutOfRange(colIdx) ? 0 : colIdx;
		rowIdx = m_rowDefinitionList->IsOutOfRange(rowIdx) ? 0 : rowIdx;

		ColumnDefinition* col = m_columnDefinitionList->IsEmpty() ? NULL : m_columnDefinitionList->GetAt(colIdx);
		RowDefinition*    row = m_rowDefinitionList->IsEmpty() ? NULL : m_rowDefinitionList->GetAt(rowIdx);

		//col->m_elementGroup.Add(child);
		//row->m_elementGroup.Add(child);

		// 子要素の DesiredSize (最低サイズ) を測るのは、セルのサイズ指定が "Auto" の時だけでよい。
		if ((col == NULL || col->IsAuto()) || (row == NULL || row->IsAuto()))
		{
			//child->MeasureLayout(availableSize);	// TODO: Measuer は常にやったほうが良いのか確認しておいたほうが良いかも

			if (col != NULL && col->IsAuto() && Grid::GetColumnSpan(child) <= 1) {	// span が 2 以上の要素はサイズ集計に考慮しない
				col->m_desiredWidth = std::max(col->m_desiredWidth, child->GetDesiredSize().Width);
			}
			if (row != NULL && row->IsAuto() && Grid::GetRowSpan(child) <= 1) {		// span が 2 以上の要素はサイズ集計に考慮しない
				row->m_desiredHeight = std::max(row->m_desiredHeight, child->GetDesiredSize().Height);
			}
		}
	}

	// 各セルの DesiredSize を集計して、Grid 全体の DesiredSize を求める。
	SizeF desiredSize = SizeF::Zero;
	for (auto col : *m_columnDefinitionList) {
		desiredSize.Width += col->GetAvailableDesiredWidth();
	}
	for (auto row : *m_rowDefinitionList) {
		desiredSize.Height += row->GetAvailableDesiredHeight();
	}

	// this の m_desiredSize を設定する
	Panel::MeasureLayout(desiredSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::ArrangeLayout(const RectF& finalLocalRect)
{
	// "Auto" と "Pixel" 指定である Column/Row の最終サイズを確定させる
	SizeF totalActual = SizeF::Zero;
	int starColCount = 0;
	int starRowCount = 0;
	for (auto col : *m_columnDefinitionList)
	{
		if (col->IsAuto() || col->IsPixel())
		{
			col->m_actualWidth = col->GetAvailableDesiredWidth();
			totalActual.Width += col->m_actualWidth;
		}
		else {
			starColCount++;
		}
	}
	for (auto row : *m_rowDefinitionList)
	{
		if (row->IsAuto() || row->IsPixel())
		{
			row->m_actualHeight = row->GetAvailableDesiredHeight();
			totalActual.Height += row->m_actualHeight;
		}
		else {
			starRowCount++;
		}
	}

	// "*" ひとつ分のセルの領域
	SizeF starUnit(
		(finalLocalRect.Width - totalActual.Width) / starColCount,
		(finalLocalRect.Height - totalActual.Height) / starRowCount);
	starUnit.Width = std::max(0.0f, starUnit.Width);	// 負値はダメ
	starUnit.Height = std::max(0.0f, starUnit.Height);	// 負値はダメ

	// "*" 指定である Column/Row の最終サイズを確定させ、
	// 全セルのオフセット (位置) も確定させる
	PointF totalOffset = PointF::Zero;
	for (auto col : *m_columnDefinitionList)
	{
		if (col->IsStar()) {
			col->m_actualWidth = starUnit.Width;
		}

		// セルX座標確定
		col->m_actualOffsetX = totalOffset.X;
		totalOffset.X += col->m_actualWidth;
	}
	for (auto row : *m_rowDefinitionList)
	{
		if (row->IsStar()) {
			row->m_actualHeight = starUnit.Height;
		}

		// セルY座標確定
		row->m_actualOffsetY = totalOffset.Y;
		totalOffset.Y += row->m_actualHeight;
	}

	// 子要素の最終位置・サイズを確定させる
	for (auto child : *m_children)
	{
		int colIdx = Grid::GetColumn(child);
		int rowIdx = Grid::GetRow(child);
		int colSpan = Grid::GetColumnSpan(child);
		int rowSpan = Grid::GetRowSpan(child);
		colSpan = std::max(1, colSpan);	// 0 は 1 とする
		rowSpan = std::max(1, rowSpan);	// 0 は 1 とする
		colSpan = std::min(colSpan, colIdx + m_columnDefinitionList->GetCount());	// 最大値制限
		rowSpan = std::min(rowSpan, rowIdx + m_rowDefinitionList->GetCount());		// 最大値制限

		// 配置先のセルで座標を確定
		//RectF rect(
		//	m_columnDefinitionList->GetAt(colIdx)->m_actualOffsetX,
		//	m_rowDefinitionList->GetAt(rowIdx)->m_actualOffsetY,
		//	0, 0);
		RectF rect = RectF::Zero;

		// Span を考慮してサイズを確定
		if (m_columnDefinitionList->IsEmpty())
		{
			rect.Width = finalLocalRect.Width;
		}
		else
		{
			rect.X = m_columnDefinitionList->GetAt(colIdx)->m_actualOffsetX;
			for (int iCol = 0; iCol < colSpan; ++iCol) {
				rect.Width += m_columnDefinitionList->GetAt(colIdx + iCol)->m_actualWidth;
			}
		}
		if (m_rowDefinitionList->IsEmpty())
		{
			rect.Height = finalLocalRect.Height;
		}
		else
		{
			rect.Y = m_rowDefinitionList->GetAt(rowIdx)->m_actualOffsetY;
			for (int iRow = 0; iRow < rowSpan; ++iRow) {
				rect.Height += m_rowDefinitionList->GetAt(rowIdx + iRow)->m_actualHeight;
			}
		}

		// Arrange
		child->ArrangeLayout(rect);
	}

	Panel::ArrangeLayout(finalLocalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	Grid::SetColumn(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, ColumnProperty, value); }
int		Grid::GetColumn(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, ColumnProperty, int); }
void	Grid::SetColumnSpan(UIElement* element, int value)	{ LN_SET_ATTACHED_PROPERTY(element, ColumnSpanProperty, value); }
int		Grid::GetColumnSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, ColumnSpanProperty, int); }
void	Grid::SetRow(UIElement* element, int value)			{ LN_SET_ATTACHED_PROPERTY(element, RowProperty, value); }
int		Grid::GetRow(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, RowProperty, int); }
void	Grid::SetRowSpan(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, RowSpanProperty, value); }
int		Grid::GetRowSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, RowSpanProperty, int); }

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::AddColumnDefinition(float width, float minWidth, float maxWidth)
{
	RefPtr<ColumnDefinition> col(LN_NEW ColumnDefinition(m_manager), false);
	m_columnDefinitionList->Add(col);
}
void Grid::AddRowDefinition(float height, float minHeight, float maxHeight)
{
	RefPtr<RowDefinition> row(LN_NEW RowDefinition(m_manager), false);
	m_rowDefinitionList->Add(row);
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

