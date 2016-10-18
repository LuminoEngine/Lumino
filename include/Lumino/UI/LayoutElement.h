
#pragma once
#include "Common.h"
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class ILayoutElement
{
public:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);


protected:
	virtual const PointF& GetLayoutPosition() const = 0;
	virtual const SizeF& GetLayoutSize() const = 0;
	virtual const ThicknessF& GetLayoutMargin() const = 0;
	virtual const ThicknessF& GetLayoutPadding() const = 0;
	virtual AlignmentAnchor GetLayoutAnchor() const = 0;
	virtual HAlignment GetLayoutHAlignment() const = 0;
	virtual VAlignment GetLayoutVAlignment() const = 0;
	virtual ILayoutElement* GetLayoutParent() const = 0;
	virtual HAlignment* GetLayoutContentHAlignment() = 0;
	virtual VAlignment* GetLayoutContentVAlignment() = 0;
	virtual const SizeF& GetLayoutDesiredSize() const = 0;
	virtual void SetLayoutDesiredSize(const SizeF& size) = 0;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) = 0;


LN_PROTECTED_INTERNAL_ACCESS:
	// GridLayout properties
	virtual int GetLayoutColumn() const = 0;
	virtual int GetLayoutRow() const = 0;
	virtual int GetLayoutColumnSpan() const = 0;
	virtual int GetLayoutRowSpan() const = 0;

	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

protected:
	ILayoutElement();
	virtual ~ILayoutElement();
};


namespace detail {

struct GridLayoutInfo
{
	int		layoutColumn = 0;
	int		layoutRow = 0;
	int		layoutColumnSpan = 0;
	int		layoutRowSpan = 0;
};

class LayoutHelper
{
public:

	//static void ForEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	//{
	//	int count = element->GetVisualChildrenCount();
	//	for (int i = 0; i < count; ++i)
	//	{
	//		func(element->GetVisualChildOrderd(i));
	//	}
	//}

	static void AdjustHorizontalAlignment(const SizeF& areaSize, const SizeF& desiredSize, HAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case HAlignment::Left:
			outRect->x = 0;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Center:
			outRect->x = (areaSize.width - desiredSize.width) / 2;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Right:
			outRect->x = areaSize.width - desiredSize.width;
			outRect->width = desiredSize.width;
			break;
		case HAlignment::Stretch:
			outRect->x = 0;
			outRect->width = areaSize.width;
			break;
		}
	}

	static void AdjustVerticalAlignment(const SizeF& areaSize, const SizeF& desiredSize, VAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case VAlignment::Top:
			outRect->y = 0;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Center:
			outRect->y = (areaSize.height - desiredSize.height) / 2;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Bottom:
			outRect->y = areaSize.height - desiredSize.height;
			outRect->height = desiredSize.height;
			break;
		case VAlignment::Stretch:
			outRect->y = 0;
			outRect->height = areaSize.height;
			break;
		}
	}
};

} // namespace detail
LN_NAMESPACE_END
