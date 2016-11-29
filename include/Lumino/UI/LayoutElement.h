
#pragma once
#include "Common.h"
#include <Lumino/Math/MathUtils.h>
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN
namespace detail { template<class TPanel> class LayoutImpl; }

/** グリッドレイアウトのセルサイズを指定する値の種類です。*/
enum class GridLengthType
{
	Auto,				/**< 子要素のサイズに合わせる */
	Pixel,				/**< サイズを直接指定する */
	Ratio,				/**< レイアウト後、残りの領域を使うか */
};

/**
	@brief		
*/
class ILayoutElement
{
public:
	virtual void UpdateLayout(const Size& viewSize);
	virtual void MeasureLayout(const Size& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

protected:
	virtual const PointF& GetLayoutPosition() const = 0;
	virtual const Size& GetLayoutSize() const = 0;
	virtual const ThicknessF& GetLayoutMargin() const = 0;
	virtual const ThicknessF& GetLayoutPadding() const = 0;
	virtual AlignmentAnchor GetLayoutAnchor() const = 0;
	virtual HAlignment GetLayoutHAlignment() const = 0;
	virtual VAlignment GetLayoutVAlignment() const = 0;
	virtual ILayoutElement* GetLayoutParent() const = 0;
	virtual const HAlignment* GetLayoutContentHAlignment() = 0;
	virtual const VAlignment* GetLayoutContentVAlignment() = 0;
	virtual const Size& GetLayoutDesiredSize() const = 0;
	virtual void SetLayoutDesiredSize(const Size& size) = 0;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) = 0;
	virtual const RectF& GetLayoutFinalLocalRect() = 0;
	virtual void SetLayoutFinalGlobalRect(const RectF& rect) = 0;

	virtual int GetVisualChildrenCount() const = 0;
	virtual ILayoutElement* GetVisualChild(int index) const = 0;

protected:
	// GridLayout properties
	virtual int GetLayoutColumn() const = 0;
	virtual int GetLayoutRow() const = 0;
	virtual int GetLayoutColumnSpan() const = 0;
	virtual int GetLayoutRowSpan() const = 0;

	virtual Size MeasureOverride(const Size& constraint);
	virtual Size ArrangeOverride(const Size& finalSize);
	virtual void UpdateTransformHierarchy(const RectF& parentGlobalRect);

protected:
	ILayoutElement();
	virtual ~ILayoutElement();

	template<class TPanel> friend class detail::LayoutImpl;
};


namespace detail {

struct GridDefinitionData
{
	// input data
	GridLengthType	type = GridLengthType::Ratio;
	float			size = 0.0f;
	float			minSize = 0.0f;
	float			maxSize = FLT_MAX;

	// working data
	float			desiredSize = 0.0f;
	float			actualOffset = 0.0f;	// 最終オフセット
	float			actualSize = 0.0f;		// 最終サイズ

	float GetAvailableDesiredSize() const
	{
		if (type == GridLengthType::Auto) {
			return desiredSize;
		}
		else if (type == GridLengthType::Pixel) {
			return Math::Clamp(size, minSize, maxSize);
		}
		else {
			return 0;
		}
	}

	float GetRatioSize() const
	{
		return (size == 0.0f) ? 1.0f : size;
	}

	void AdjustActualSize()
	{
		actualSize = Math::Clamp(actualSize, minSize, maxSize);
	}
};

class ILayoutPanel
{
protected:
	virtual int GetLayoutChildrenCount() = 0;
	virtual ILayoutElement* GetLayoutChild(int index) = 0;

	// GridLayout properties
	virtual int GetLayoutGridColumnDefinitionCount() = 0;
	virtual GridDefinitionData* GetLayoutGridColumnDefinition(int index) = 0;
	virtual int GetLayoutGridRowDefinitionCount() = 0;
	virtual GridDefinitionData* GetLayoutGridRowDefinition(int index) = 0;

protected:
	virtual ~ILayoutPanel() = default;

	template<class TPanel> friend class detail::LayoutImpl;
};

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
	//		func(element->GetVisualChild(i));
	//	}
	//}

	static void AdjustHorizontalAlignment(const Size& areaSize, const Size& desiredSize, HAlignment align, RectF* outRect)
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
			if (Math::IsNaN(desiredSize.width))
			{
				outRect->x = 0;
				outRect->width = areaSize.width;
			}
			else
			{
				outRect->x = (areaSize.width - desiredSize.width) / 2;
				outRect->width = desiredSize.width;
			}
			break;
		}
	}

	static void AdjustVerticalAlignment(const Size& areaSize, const Size& desiredSize, VAlignment align, RectF* outRect)
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
			if (Math::IsNaN(desiredSize.height))
			{
				outRect->y = 0;
				outRect->height = areaSize.height;
			}
			else
			{
				outRect->y = (areaSize.height - desiredSize.height) / 2;
				outRect->height = desiredSize.height;
			}
			break;
		}
	}
};

} // namespace detail
LN_NAMESPACE_END
