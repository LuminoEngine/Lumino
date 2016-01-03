
#pragma once
#include "Internal.h"
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
	
class UIHelper
{
public:

	static void ForEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	{
		int count = element->GetVisualChildrenCount();
		for (int i = 0; i < count; ++i)
		{
			func(element->GetVisualChild(i));
		}
	}

	static void AdjustHorizontalAlignment(const SizeF& arrangeSize, const SizeF& desiredSize, HorizontalAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case HorizontalAlignment::Left:
			outRect->X = 0;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Center:
			outRect->X = (arrangeSize.Width - desiredSize.Width) / 2;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Right:
			outRect->X = arrangeSize.Width - desiredSize.Width;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Stretch:
			outRect->X = 0;
			outRect->Width = arrangeSize.Width;
			break;
		}
	}

	static void AdjustVerticalAlignment(const SizeF& arrangeSize, const SizeF& desiredSize, VerticalAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case VerticalAlignment::Top:
			outRect->Y = 0;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Center:
			outRect->Y = (arrangeSize.Height - desiredSize.Height) / 2;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Bottom:
			outRect->Y = arrangeSize.Height - desiredSize.Height;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Stretch:
			outRect->Y = 0;
			outRect->Height = arrangeSize.Height;
			break;
		}
	}
};

LN_NAMESPACE_END
