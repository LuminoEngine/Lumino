
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
			outRect->x = 0;
			outRect->width = desiredSize.width;
			break;
		case HorizontalAlignment::Center:
			outRect->x = (arrangeSize.width - desiredSize.width) / 2;
			outRect->width = desiredSize.width;
			break;
		case HorizontalAlignment::Right:
			outRect->x = arrangeSize.width - desiredSize.width;
			outRect->width = desiredSize.width;
			break;
		case HorizontalAlignment::Stretch:
			outRect->x = 0;
			outRect->width = arrangeSize.width;
			break;
		}
	}

	static void AdjustVerticalAlignment(const SizeF& arrangeSize, const SizeF& desiredSize, VerticalAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case VerticalAlignment::Top:
			outRect->y = 0;
			outRect->height = desiredSize.height;
			break;
		case VerticalAlignment::Center:
			outRect->y = (arrangeSize.height - desiredSize.height) / 2;
			outRect->height = desiredSize.height;
			break;
		case VerticalAlignment::Bottom:
			outRect->y = arrangeSize.height - desiredSize.height;
			outRect->height = desiredSize.height;
			break;
		case VerticalAlignment::Stretch:
			outRect->y = 0;
			outRect->height = arrangeSize.height;
			break;
		}
	}
};

LN_NAMESPACE_END
