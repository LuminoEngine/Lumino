
/*
    Layout は UIElement の派生とするべきか？
    一番困るのは、レイアウトをネストするとき、デザイナ上で他の UIElement と同じような処理で操作できなくなること。
    意外と Element と Layout が独立してるのって Qt Widgets くらいなんだよな…。
*/


#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutElement.hpp>

namespace ln {

//==============================================================================
// UILayoutElement
LN_OBJECT_IMPLEMENT(UILayoutElement, Object) {}

UILayoutElement::UILayoutElement()
	//: m_layoutSize(Math::NaN, Math::NaN)
{
}

UILayoutElement::~UILayoutElement()
{
}

// actualStyle : サブクラスの m_actualStyle へのポインタ。細かい値をとるのにいちいち仮想関数を呼び出すのがパフォーマンス的に心配なのでこの形にしている。
void UILayoutElement::init(const detail::UIStyleInstance* finalStyle)
{
	Object::init();
	m_finalStyle = finalStyle;
}

void UILayoutElement::updateLayout(const Rect& parentFinalGlobalRect)
{
    Size itemSize(m_finalStyle->width, m_finalStyle->height);// = //getLayoutSize();
	Size size(
		Math::isNaNOrInf(itemSize.width) ? parentFinalGlobalRect.width : itemSize.width,
		Math::isNaNOrInf(itemSize.height) ? parentFinalGlobalRect.height : itemSize.height);

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	measureLayout(size);
	arrangeLayout(parentFinalGlobalRect);
}

void UILayoutElement::measureLayout(const Size& availableSize)
{
	Size outerSpace = m_finalStyle->actualOuterSpace();
	Size localAvailableSize(std::max(availableSize.width - outerSpace.width, 0.0f), std::max(availableSize.height - outerSpace.height, 0.0f));
	
	Size desiredSize = measureOverride(localAvailableSize);

	desiredSize.width += outerSpace.width;
	desiredSize.height += outerSpace.height;

	setLayoutDesiredSize(desiredSize);
}

void UILayoutElement::arrangeLayout(const Rect& localSlotRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	const Size& areaSize = localSlotRect.getSize();

#if 1
	HAlignment		hAlign = getLayoutHAlignment();
	VAlignment		vAlign = getLayoutVAlignment();
#else
	UILayoutElement* parent = GetLayoutParent();
	HAlignment		hAlign = GetLayoutHAlignment();
	VAlignment		vAlign = GetLayoutVAlignment();
	const HAlignment* parentHAlign = (parent != nullptr) ? parent->GetLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (parent != nullptr) ? parent->GetLayoutContentVAlignment() : nullptr;
	if (parentHAlign != nullptr) hAlign = *parentHAlign;
	if (parentVAlign != nullptr) vAlign = *parentVAlign;
#endif

	//Size ds;// = getLayoutDesiredSize();
	//ds.width = Math::IsNaNOrInf(layoutSize.width) ? finalLocalRect.width : layoutSize.width;
	//ds.height = Math::IsNaNOrInf(layoutSize.height) ? finalLocalRect.height : layoutSize.height;
	Size ds = getLayoutDesiredSize();
	LN_DCHECK(!Math::isNaNOrInf(ds.width));
	LN_DCHECK(!Math::isNaNOrInf(ds.height));

	// DesiredSize は Margin 考慮済み

	// Alignment で調整する領域は、margin 領域も含む
	//float marginWidth = margin.left + margin.right + m_finalStyle->borderThickness.width();
	//float marginHeight = margin.top + margin.bottom + m_finalStyle->borderThickness.height();
	Size outerSpace = m_finalStyle->actualOuterSpace();
    Size layoutSize(m_finalStyle->width + outerSpace.width, m_finalStyle->height + outerSpace.height);
	if (!m_finalStyle->borderInset) {
		layoutSize.width += m_finalStyle->borderThickness.width();
		layoutSize.height += m_finalStyle->borderThickness.height();
	}

	Rect arrangeRect;
	detail::LayoutHelper::adjustHorizontalAlignment(areaSize, ds, layoutSize.width, hAlign, &arrangeRect);
	detail::LayoutHelper::adjustVerticalAlignment(areaSize, ds, layoutSize.height, vAlign, &arrangeRect);


	// Margin を考慮する (0 以下には出来ない)
	arrangeRect.width = std::max(arrangeRect.width - outerSpace.width, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - outerSpace.height, 0.0f);

    // apply border size
    //arrangeRect.width -= m_finalStyle->borderThickness.width();
    //arrangeRect.height -= m_finalStyle->borderThickness.height();

	// Padding を考慮する
	//const Thickness& padding = getLayoutPadding();

    Size contentAreaSize = arrangeRect.getSize();
	//Size contentAreaSize(
	//	std::max(arrangeRect.width - padding.getWidth(), 0.0f),
	//	std::max(arrangeRect.height - padding.getHeight(), 0.0f));
	Size finalContentAreaSize = arrangeOverride(contentAreaSize);

    

	//Rect finalLocalRect;
	//Rect finalContentRect;
	const Thickness& margin = getLayoutMargin();
	m_localPosition.x = localSlotRect.x + /*finalLocalRect.x + */margin.left + arrangeRect.x;// +m_finalStyle->borderThickness.left;
	m_localPosition.y = localSlotRect.y + /*finalLocalRect.y + */margin.top + arrangeRect.y;// +m_finalStyle->borderThickness.top;
    m_actualSize.width = finalContentAreaSize.width;// +padding.getWidth();
    m_actualSize.height = finalContentAreaSize.height;// + padding.getHeight();
	//finalContentRect.x = finalRenderRect.x + padding.left;
	//finalContentRect.y = finalRenderRect.y + padding.top;
	//finalContentRect.width = finalRenderRect.width - padding.getWidth();
	//finalContentRect.height = finalRenderRect.height - padding.getHeight();
	//setLayoutFinalLocalRect(finalLocalRect/*, finalContentRect*/);

	//updateFinalRects(localSlotRect);

}

void UILayoutElement::updateFinalRects(const Rect& parentFinalGlobalRect)
{
	//Rect localRenderRect = getLayoutFinalLocalRect();

	Rect finalGlobalRect;
	//if (m_parent != nullptr)
	//{
	finalGlobalRect.x = parentFinalGlobalRect.x + m_localPosition.x;
	finalGlobalRect.y = parentFinalGlobalRect.y + m_localPosition.y;
	//m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
//}
//else
//{
//	m_finalGlobalRect.x = m_finalLocalRect.x;
//	m_finalGlobalRect.y = m_finalLocalRect.y;
//	m_combinedOpacity = m_opacity;
//}
	finalGlobalRect.width = m_actualSize.width;
	finalGlobalRect.height = m_actualSize.height;

	setLayoutFinalGlobalRect(finalGlobalRect);

    onUpdateLayout(finalGlobalRect);

	// update children
	//Rect finalGlobalContentRect(
	//	parentGlobalRect.x + localContentRect.x,
	//	parentGlobalRect.y + localContentRect.y,
	//	localContentRect.width,
	//	localContentRect.height);
	//Rect finalGlobalContentRect(
	//	finalSlotGlobalRect.x + localRenderRect.x,
	//	finalSlotGlobalRect.y + localRenderRect.y,
	//	localRenderRect.width,
	//	localRenderRect.height);

	//int childCount = getVisualChildrenCount();
	//for (int i = 0; i < childCount; i++)
	//{
	//	ILayoutElement* child = getVisualChild(i);
	//	child->updateTransformHierarchy(finalGlobalContentRect);
	//}

	// 子要素
	//UIHelper::forEachVisualChildren(this, [](UIElement* child) { child->updateTransformHierarchy(); });

}

Size UILayoutElement::measureOverride(const Size& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	return detail::LayoutHelper::measureElement(this, constraint, Size::Zero);
}

Size UILayoutElement::arrangeOverride(const Size& finalSize)
{
	return finalSize;
}

void UILayoutElement::onUpdateLayout(const Rect& finalGlobalRect)
{
}

const Thickness& UILayoutElement::getLayoutMargin() const
{
	return m_finalStyle->margin;
}

const Thickness& UILayoutElement::getLayoutPadding() const
{
	return m_finalStyle->padding;
}

HAlignment UILayoutElement::getLayoutHAlignment() const
{
	return m_finalStyle->horizontalAlignment;
}

VAlignment UILayoutElement::getLayoutVAlignment() const
{
	return m_finalStyle->verticalAlignment;
}

void UILayoutElement::getLayoutMinMaxInfo(Size* outMin, Size* outMax) const
{
	outMin->width = m_finalStyle->minWidth;
	outMin->height = m_finalStyle->minHeight;
	outMax->width = m_finalStyle->maxWidth;
	outMax->height = m_finalStyle->maxHeight;
}

//==============================================================================
// ILayoutPanel

ILayoutPanel::ILayoutPanel()
{
}
//==============================================================================
// LayoutHelper

namespace detail {

Size LayoutHelper::measureElementSpacing(UILayoutElement* element)
{
    Size spacing;

    spacing.width = element->m_finalStyle->padding.width();
    spacing.height = element->m_finalStyle->padding.height();

    if (!element->m_finalStyle->borderInset) {
        spacing.width += element->m_finalStyle->borderThickness.width();
        spacing.height += element->m_finalStyle->borderThickness.height();
    }

    return spacing;
}

Size LayoutHelper::measureElementClientSize(UILayoutElement* element)
{
    Size size(element->m_finalStyle->width, element->m_finalStyle->height);
    Size bodySize;
    bodySize.width = Math::isNaNOrInf(size.width) ? 0.0f : size.width;
    bodySize.height = Math::isNaNOrInf(size.height) ? 0.0f : size.height;

    Size minSize, maxSize;
    element->getLayoutMinMaxInfo(&minSize, &maxSize);
    if (!Math::isNaNOrInf(minSize.width)) bodySize.width = std::max(bodySize.width, minSize.width);
    if (!Math::isNaNOrInf(minSize.height)) bodySize.height = std::max(bodySize.height, minSize.height);
    if (!Math::isNaNOrInf(maxSize.width)) bodySize.width = std::min(bodySize.width, maxSize.width);
    if (!Math::isNaNOrInf(maxSize.height)) bodySize.height = std::min(bodySize.height, maxSize.height);

    return bodySize;
}

Size LayoutHelper::measureElement(UILayoutElement* element, const Size& constraint, const Size& childrenDesiredSize)
{
    //Size size(element->m_finalStyle->width, element->m_finalStyle->height);
	Size desiredSize = Size::max(measureElementClientSize(element), childrenDesiredSize);

	//// NaN の場合、この要素として必要な最小サイズは 0 となる。
	//desiredSize.width = Math::isNaNOrInf(size.width) ? 0.0f : size.width;
	//desiredSize.height = Math::isNaNOrInf(size.height) ? 0.0f : size.height;
	//desiredSize.width = std::min(desiredSize.width, constraint.width);
	//desiredSize.height = std::min(desiredSize.height, constraint.height);

	//Size minSize, maxSize;
	//element->getLayoutMinMaxInfo(&minSize, &maxSize);
	//if (!Math::isNaNOrInf(minSize.width)) desiredSize.width = std::max(desiredSize.width, minSize.width);
	//if (!Math::isNaNOrInf(minSize.height)) desiredSize.height = std::max(desiredSize.height, minSize.height);
	//if (!Math::isNaNOrInf(maxSize.width)) desiredSize.width = std::min(desiredSize.width, maxSize.width);
	//if (!Math::isNaNOrInf(maxSize.height)) desiredSize.height = std::min(desiredSize.height, maxSize.height);

    desiredSize += measureElementSpacing(element);

	return Size::min(desiredSize, constraint);
}

Rect LayoutHelper::arrangeClientArea(UILayoutElement* element, const Size& finalSize)
{
	Rect area;

	area.x = element->m_finalStyle->padding.left;
	area.y = element->m_finalStyle->padding.top;
	area.width = finalSize.width - element->m_finalStyle->padding.width();
	area.height = finalSize.height - element->m_finalStyle->padding.height();

	// arrange では inset に関わらず border が影響する (inset=true のとき、最終サイズが width,height 直接指定と矛盾すると見切れたりするが、それでよい (WPF))
	area = area.makeDeflate(element->m_finalStyle->borderThickness);

	return area;
}

} // namespace detail
} // namespace ln

