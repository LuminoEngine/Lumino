
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

template<class TPanel>
class LayoutImpl
{
public:
	using BaseMeasureOverrideCallback = Size(*)(TPanel* panel, const Size& constraint);

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_MeasureOverride(ILayoutPanel* panel, const Size& constraint, BaseMeasureOverrideCallback baseCallback)
	{
		Size desiredSize = baseCallback(static_cast<TPanel*>(panel), constraint);
		int childCount = panel->GetLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->GetLayoutChild(i);

			child->MeasureLayout(constraint);
			const Size& childDesiredSize = child->GetLayoutDesiredSize();

			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_ArrangeOverride(ILayoutPanel* panel, const Size& finalSize)
	{
		int childCount = panel->GetLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->GetLayoutChild(i);
			Size childDesiredSize = child->GetLayoutDesiredSize();
			childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
			childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
			child->ArrangeLayout(RectF(0, 0, childDesiredSize));
		}
		return finalSize;
	}


	//------------------------------------------------------------------------------
	static Size UIStackPanel_MeasureOverride(ILayoutPanel* panel, const Size& constraint, Orientation orientation)
	{
		Size size = constraint;

		if (orientation == Orientation::Horizontal)
		{
			// ‰¡‚É•À‚×‚éê‡A•‚Ì§ŒÀ‚ğİ‚¯‚È‚¢
			size.width = std::numeric_limits<float>::infinity();
		}
		else
		{
			// c‚É•À‚×‚éê‡A‚‚³‚Ì§ŒÀ‚ğİ‚¯‚È‚¢
			size.height = std::numeric_limits<float>::infinity();
		}

		Size desiredSize;
		int childCount = panel->GetLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->GetLayoutChild(i);
			child->MeasureLayout(size);

			const Size& childDesiredSize = child->GetLayoutDesiredSize();
			if (orientation == Orientation::Horizontal || orientation == Orientation::ReverseHorizontal)
			{
				desiredSize.width += childDesiredSize.width;
				desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
			}
			else
			{
				desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
				desiredSize.height += child->GetLayoutDesiredSize().height;
			}
		}

		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UIStackPanel_ArrangeOverride(TPanel* panel, const Size& finalSize, Orientation orientation)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		const ThicknessF& padding = panel->GetLayoutPadding();

		Size childrenBoundSize(finalSize.width - (padding.Left + padding.Right), finalSize.height - (padding.Top + padding.Bottom));

		float prevChildSize = 0;
		float rPos = 0;
		RectF childRect(padding.Left, padding.Top, 0, 0);
		int childCount = basePanel->GetLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->GetLayoutChild(i);
			const Size& childDesiredSize = child->GetLayoutDesiredSize();

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

			child->ArrangeLayout(childRect);
		}

		return finalSize;
	}
};

} // namespace detail
LN_NAMESPACE_END
