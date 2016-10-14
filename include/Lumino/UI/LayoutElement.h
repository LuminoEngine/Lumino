
#pragma once
#include "Common.h"

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
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	virtual const PointF& GetLayoutPosition() const = 0;
	virtual const SizeF& GetLayoutSize() const = 0;
	virtual const ThicknessF& GetLayoutMargin() const = 0;
	virtual const ThicknessF& GetLayoutPadding() const = 0;
	virtual AlignmentAnchor GetLayoutAnchor() const = 0;
	virtual HorizontalAlignment GetLayoutHorizontalAlignment() const = 0;
	virtual VerticalAlignment GetLayoutVerticalAlignment() const = 0;
	virtual ILayoutElement* GetLayoutParent() const = 0;
	//virtual int GetLayoutChildCount() const = 0;
	//virtual ILayoutElement* GetLayoutChild(int index) const = 0;
	virtual HorizontalAlignment* GetLayoutContentHorizontalAlignment() = 0;
	virtual VerticalAlignment* GetLayoutContentVerticalAlignment() = 0;
	virtual const SizeF& GetLayoutDesiredSize() const = 0;
	virtual void SetLayoutDesiredSize(const SizeF& size) = 0;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) = 0;
	

	//const SizeF& GetRenderSize() const { return m_finalLocalRect.GetSize(); }
	//float GetActualWidth() const { return m_finalLocalRect.width; }
	//float GetActualHeight() const { return m_finalLocalRect.height; }

protected:
	ILayoutElement();
	virtual ~ILayoutElement();
	
private:
	//SizeF					m_desiredSize;			// MeasureLayout() で決定されるこのコントロールの要求サイズ
	//RectF					m_finalLocalRect;		// 描画に使用する最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	//RectF					m_finalGlobalRect;
	//PointF					m_position;
	//SizeF					m_size;
	//ThicknessF				m_margin;
	//ThicknessF				m_padding;
	//AlignmentAnchor			m_anchor;
	//HorizontalAlignment		m_horizontalAlignment;
	//VerticalAlignment		m_verticalAlignment;
};


namespace detail {

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

} // namespace detail
LN_NAMESPACE_END
