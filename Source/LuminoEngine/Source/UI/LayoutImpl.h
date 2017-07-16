
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

template<class TPanel>
class LayoutImpl
{
public:
	using BaseMeasureOverrideCallback = Size(*)(TPanel* panel, const Size& constraint);

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_measureOverride(TPanel* panel, const Size& constraint, BaseMeasureOverrideCallback baseCallback)
	{
		Size desiredSize = baseCallback(static_cast<TPanel*>(panel), constraint);
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);

			child->measureLayout(constraint);
			const Size& childDesiredSize = child->getLayoutDesiredSize();

			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_arrangeOverride(TPanel* panel, const Vector2& offset, const Size& finalSize)
	{
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);
			Size childDesiredSize = child->getLayoutDesiredSize();
			childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
			childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
			child->arrangeLayout(Rect(offset.x, offset.y, childDesiredSize));
		}
		return finalSize;
	}

	//------------------------------------------------------------------------------
	static Size UIStackPanel_measureOverride(ILayoutPanel* panel, const Size& constraint, Orientation orientation)
	{
		Size size = constraint;

		if (orientation == Orientation::Horizontal)
		{
			// 横に並べる場合、幅の制限を設けない
			size.width = std::numeric_limits<float>::infinity();
		}
		else
		{
			// 縦に並べる場合、高さの制限を設けない
			size.height = std::numeric_limits<float>::infinity();
		}

		Size desiredSize;
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);
			child->measureLayout(size);

			const Size& childDesiredSize = child->getLayoutDesiredSize();
			if (orientation == Orientation::Horizontal || orientation == Orientation::ReverseHorizontal)
			{
				desiredSize.width += childDesiredSize.width;
				desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
			}
			else
			{
				desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
				desiredSize.height += child->getLayoutDesiredSize().height;
			}
		}

		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UIStackPanel_arrangeOverride(TPanel* panel, const Size& finalSize, Orientation orientation)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		Size childrenBoundSize(finalSize.width, finalSize.height);

		float prevChildSize = 0;
		float rPos = 0;
		Rect childRect(0, 0, 0, 0);
		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);
			const Size& childDesiredSize = child->getLayoutDesiredSize();

			switch (orientation)
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
				assert(0);
				break;
			}

			child->arrangeLayout(childRect);
		}

		return finalSize;
	}

	//------------------------------------------------------------------------------
	static Size UIGridLayout_measureOverride(TPanel* panel, const Size& constraint, BaseMeasureOverrideCallback baseCallback)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		int colDefCount = basePanel->getLayoutGridColumnDefinitionCount();
		int rowDefCount = basePanel->getLayoutGridRowDefinitionCount();

		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);

			// まずは子を measure
			child->measureLayout(constraint);

			// child が配置されるべき column と row を探す
			int colIdx = child->getLayoutColumn();
			int rowIdx = child->getLayoutRow();

			colIdx = (0 <= colIdx && colIdx < colDefCount) ? colIdx : 0;
			rowIdx = (0 <= rowIdx && rowIdx < rowDefCount) ? rowIdx : 0;

			detail::GridDefinitionData* col = (colIdx < colDefCount) ? basePanel->getLayoutGridColumnDefinition(colIdx) : nullptr;
			detail::GridDefinitionData* row = (rowIdx < rowDefCount) ? basePanel->getLayoutGridRowDefinition(rowIdx) : nullptr;

			// 子要素の DesiredSize (最低サイズ) を測るのは、セルのサイズ指定が "Auto" の時だけでよい。
			const Size& childDesiredSize = child->getLayoutDesiredSize();
			if (col != nullptr && col->type == GridLengthType::Auto)
			{
				col->desiredSize = std::max(col->desiredSize, childDesiredSize.width);
			}
			if (row != nullptr && row->type == GridLengthType::Auto)
			{
				row->desiredSize = std::max(row->desiredSize, childDesiredSize.height);
			}
		}

		// 各セルの DesiredSize を集計して、Grid 全体の DesiredSize を求める
		Size desiredSize = baseCallback(panel, constraint);
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			desiredSize.width += basePanel->getLayoutGridColumnDefinition(iCol)->getAvailableDesiredSize();
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			desiredSize.height += basePanel->getLayoutGridRowDefinition(iRow)->getAvailableDesiredSize();
		}

		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UIGridLayout_arrangeOverride(TPanel* panel, const Size& finalSize)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		Size childrenBoundSize(finalSize.width, finalSize.height);

		// "Auto" と "Pixel" 指定である Column/Row の最終サイズを確定させる。
		// また、"*" である行列の数をカウントする。
		Size totalActual = Size::Zero;
		float starColCount = 0.0f;
		float starRowCount = 0.0f;
		int colDefCount = basePanel->getLayoutGridColumnDefinitionCount();
		int rowDefCount = basePanel->getLayoutGridRowDefinitionCount();
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			auto* col = basePanel->getLayoutGridColumnDefinition(iCol);
			if (col->type == GridLengthType::Auto || col->type == GridLengthType::Pixel)
			{
				col->actualSize = col->getAvailableDesiredSize();
				totalActual.width += col->actualSize;
			}
			else
			{
				starColCount += col->getRatioSize();
			}
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			auto* row = basePanel->getLayoutGridRowDefinition(iRow);
			if (row->type == GridLengthType::Auto || row->type == GridLengthType::Pixel)
			{
				row->actualSize = row->getAvailableDesiredSize();
				totalActual.height += row->actualSize;
			}
			else
			{
				starRowCount += row->getRatioSize();
			}
		}

		// "1*" 分のセルの領域を計算する
		Size starUnit(
			(starColCount != 0.0f) ? (childrenBoundSize.width - totalActual.width) / starColCount : 0.0f,
			(starRowCount != 0.0f) ? (childrenBoundSize.height - totalActual.height) / starRowCount : 0.0f);
		starUnit.width = std::max(0.0f, starUnit.width);	// 負値はダメ
		starUnit.height = std::max(0.0f, starUnit.height);	// 負値はダメ

		// "*" 指定である Column/Row の最終サイズを確定させ、
		// 全セルのオフセット (位置) も確定させる
		PointF totalOffset;
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			auto* col = basePanel->getLayoutGridColumnDefinition(iCol);
			if (col->type == GridLengthType::Ratio)
			{
				col->actualSize = starUnit.width * col->getRatioSize();
			}

			col->adjustActualSize();

			// セルX座標確定
			col->actualOffset = totalOffset.x;
			totalOffset.x += col->actualSize;
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			auto* row = basePanel->getLayoutGridRowDefinition(iRow);
			if (row->type == GridLengthType::Ratio)
			{
				row->actualSize = starUnit.height * row->getRatioSize();
			}

			row->adjustActualSize();

			// セルY座標確定
			row->actualOffset = totalOffset.y;
			totalOffset.y += row->actualSize;
		}

		// 子要素の最終位置・サイズを確定させる
		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);
			int colIdx = child->getLayoutColumn();
			int rowIdx = child->getLayoutRow();
			int colSpan = child->getLayoutColumnSpan();
			int rowSpan = child->getLayoutRowSpan();
			colSpan = std::max(1, colSpan);	// 最低 1
			rowSpan = std::max(1, rowSpan);	// 最低 1
			colSpan = std::min(colSpan, colIdx + colDefCount);	// 最大値制限
			rowSpan = std::min(rowSpan, rowIdx + rowDefCount);	// 最大値制限

			// Span を考慮してサイズを確定
			Rect rect = Rect(0, 0, 0, 0);
			if (colDefCount == 0)	// is empty
			{
				rect.width = childrenBoundSize.width;
			}
			else
			{
				rect.x += basePanel->getLayoutGridColumnDefinition(colIdx)->actualOffset;
				for (int iCol = 0; iCol < colSpan; ++iCol)
				{
					rect.width += basePanel->getLayoutGridColumnDefinition(colIdx + iCol)->actualSize;
				}
			}
			if (rowDefCount == 0)	// is empty
			{
				rect.height = childrenBoundSize.height;
			}
			else
			{
				rect.y += basePanel->getLayoutGridRowDefinition(rowIdx)->actualOffset;
				for (int iRow = 0; iRow < rowSpan; ++iRow)
				{
					rect.height += basePanel->getLayoutGridRowDefinition(rowIdx + iRow)->actualSize;
				}
			}

			// Arrange
			child->arrangeLayout(rect);
		}

		return finalSize;
	}
};

} // namespace detail
LN_NAMESPACE_END
