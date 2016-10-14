
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
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

protected:
	virtual const PointF& GetLayoutPosition() const = 0;
	virtual const SizeF& GetLayoutSize() const = 0;
	virtual AlignmentAnchor GetLayoutAnchor() const = 0;
	virtual VerticalAlignment GetLayoutVerticalAlignment() const = 0;
	virtual HorizontalAlignment GetLayoutHorizontalAlignment() const = 0;
	virtual ILayoutElement* GetLayoutParent() const = 0;
	virtual const SizeF& GetLayoutDesiredSize() const = 0;
	virtual int GetLayoutChildCount() const = 0;
	virtual UIElement* GetLayoutChild(int index) const = 0;

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

LN_NAMESPACE_END
