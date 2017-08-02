
#pragma once
#include "Common.h"
#include <Lumino/Math/MathUtils.h>
#include "../Base/GeometryStructs.h"

LN_NAMESPACE_BEGIN
namespace detail { template<class TPanel> class LayoutImpl; }
namespace detail { class LayoutHelper; }

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
	virtual void updateLayout(const Size& viewSize);
	virtual void measureLayout(const Size& availableSize);
	virtual void arrangeLayout(const Rect& finalLocalRect);

protected:
	virtual const Point& getLayoutPosition() const = 0;
	virtual Size getLayoutSize() const = 0;
	virtual const Thickness& getLayoutMargin() const = 0;
	virtual const Thickness& getLayoutPadding() const = 0;
	virtual AlignmentAnchor getLayoutAnchor() const = 0;
	virtual HAlignment getLayoutHAlignment() const = 0;
	virtual VAlignment getLayoutVAlignment() const = 0;
	virtual void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const = 0;
	virtual ILayoutElement* getLayoutParent() const = 0;
	virtual const HAlignment* getLayoutContentHAlignment() = 0;
	virtual const VAlignment* getLayoutContentVAlignment() = 0;
	virtual const Size& getLayoutDesiredSize() const = 0;
	virtual void setLayoutDesiredSize(const Size& size) = 0;
	virtual void setLayoutFinalLocalRect(const Rect& renderRect) = 0;
	virtual void getLayoutFinalLocalRect(Rect* outRenderRect) const = 0;
	virtual void setLayoutFinalGlobalRect(const Rect& rect) = 0;

	virtual int getVisualChildrenCount() const = 0;
	virtual ILayoutElement* getVisualChild(int index) const = 0;

protected:
	// GridLayout properties
	virtual int getLayoutColumn() const = 0;
	virtual int getLayoutRow() const = 0;
	virtual int getLayoutColumnSpan() const = 0;
	virtual int getLayoutRowSpan() const = 0;

	virtual Size measureOverride(const Size& constraint);
	virtual Size arrangeOverride(const Size& finalSize);
	virtual void updateTransformHierarchy(const Rect& parentGlobalRect);

protected:
	ILayoutElement();
	virtual ~ILayoutElement();

	template<class TPanel> friend class detail::LayoutImpl;
	friend class detail::LayoutHelper;
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

	float getAvailableDesiredSize() const
	{
		if (type == GridLengthType::Auto) {
			return desiredSize;
		}
		else if (type == GridLengthType::Pixel) {
			return Math::clamp(size, minSize, maxSize);
		}
		else {
			return 0;
		}
	}

	float getRatioSize() const
	{
		return (size == 0.0f) ? 1.0f : size;
	}

	void adjustActualSize()
	{
		actualSize = Math::clamp(actualSize, minSize, maxSize);
	}
};

class ILayoutPanel
{
protected:
	virtual int getLayoutChildrenCount() = 0;
	virtual ILayoutElement* getLayoutChild(int index) = 0;

	// GridLayout properties
	virtual int getLayoutGridColumnDefinitionCount() = 0;
	virtual GridDefinitionData* getLayoutGridColumnDefinition(int index) = 0;
	virtual int getLayoutGridRowDefinitionCount() = 0;
	virtual GridDefinitionData* getLayoutGridRowDefinition(int index) = 0;

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

	// 単純に element のサイズによった measure を行う。measureOverride() の中で使用することを想定している。
	static Size measureElement(ILayoutElement* element, const Size& constraint);

	//static void forEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	//{
	//	int count = element->getVisualChildrenCount();
	//	for (int i = 0; i < count; ++i)
	//	{
	//		func(element->getVisualChild(i));
	//	}
	//}

	// widthNan : ユーザーが希望するサイズを指定しているか
	static void adjustHorizontalAlignment(const Size& areaSize, const Size& desiredSize, bool widthNan, HAlignment align, Rect* outRect)
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
			if (widthNan)
			{
				outRect->x = 0;
				outRect->width = areaSize.width;
			}
			else
			{
				outRect->x = (areaSize.width - desiredSize.width) / 2;
				outRect->width = desiredSize.width;
			}

			//outRect->x = 0;
			//outRect->width = areaSize.width;
			break;
		}
	}

	static void adjustVerticalAlignment(const Size& areaSize, const Size& desiredSize, bool heightNan, VAlignment align, Rect* outRect)
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
			if (heightNan)
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
