
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutElement.hpp>

namespace ln {

//==============================================================================
// UILayoutElement

UILayoutElement::UILayoutElement()
{
}

UILayoutElement::~UILayoutElement()
{
}

void UILayoutElement::init(UIStyle* actualStyle)
{
	Object::init();
	m_actualStyle = actualStyle;
}

void UILayoutElement::updateLayout(const Rect& parentFinalGlobalRect)
{
	Size itemSize = getLayoutSize();
	Size size(
		Math::isNaNOrInf(itemSize.width) ? parentFinalGlobalRect.width : itemSize.width,
		Math::isNaNOrInf(itemSize.height) ? parentFinalGlobalRect.height : itemSize.height);

	// サイズが定まっていない場合はレイアウトを決定できない
	// TODO: 例外の方が良いかも？
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	measureLayout(size);
	arrangeLayout(parentFinalGlobalRect);
	updateFinalRects(parentFinalGlobalRect);
}

void UILayoutElement::measureLayout(const Size& availableSize)
{
	// Margin と Padding を考慮する
	const Thickness& margin = getLayoutMargin();
	const Thickness& padding = getLayoutPadding();
	float spaceWidth = (margin.left + margin.right) + (padding.left + padding.right);
	float spaceHeight = (margin.top + margin.bottom) + (padding.top + padding.bottom);
	Size localAvailableSize(
		std::max(availableSize.width - spaceWidth, 0.0f),
		std::max(availableSize.height - spaceHeight, 0.0f));

	Size desiredSize = measureOverride(localAvailableSize);

	// Margin を考慮する
	desiredSize.width += spaceWidth;
	desiredSize.height += spaceHeight;

	setLayoutDesiredSize(desiredSize);
}

void UILayoutElement::arrangeLayout(const Rect& finalLocalRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	const Size& areaSize = finalLocalRect.getSize();

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
	const Thickness& margin = getLayoutMargin();
	float marginWidth = margin.left + margin.right;
	float marginHeight = margin.top + margin.bottom;

	Size layoutSize = getLayoutSize();
	Rect arrangeRect;
	detail::LayoutHelper::adjustHorizontalAlignment(areaSize, ds, Math::isNaN(layoutSize.width), hAlign, &arrangeRect);
	detail::LayoutHelper::adjustVerticalAlignment(areaSize, ds, Math::isNaN(layoutSize.height), vAlign, &arrangeRect);


	// Margin を考慮する (0 以下には出来ない)
	arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);

	// Padding を考慮する
	const Thickness& padding = getLayoutPadding();

	Size contentAreaSize(
		std::max(arrangeRect.width - padding.getWidth(), 0.0f),
		std::max(arrangeRect.height - padding.getHeight(), 0.0f));
	Size finalContentAreaSize = arrangeOverride(contentAreaSize);


	Rect finalRenderRect;
	Rect finalContentRect;
	finalRenderRect.x = finalLocalRect.x + margin.left + arrangeRect.x;
	finalRenderRect.y = finalLocalRect.y + margin.top + arrangeRect.y;
	finalRenderRect.width = finalContentAreaSize.width + padding.getWidth();
	finalRenderRect.height = finalContentAreaSize.height + padding.getHeight();
	//finalContentRect.x = finalRenderRect.x + padding.left;
	//finalContentRect.y = finalRenderRect.y + padding.top;
	//finalContentRect.width = finalRenderRect.width - padding.getWidth();
	//finalContentRect.height = finalRenderRect.height - padding.getHeight();
	setLayoutFinalLocalRect(finalRenderRect/*, finalContentRect*/);
}

void UILayoutElement::updateFinalRects(const Rect& parentGlobalRect)
{
	Rect localRenderRect = getLayoutFinalLocalRect();

	Rect finalGlobalRect;
	//if (m_parent != nullptr)
	//{
	finalGlobalRect.x = parentGlobalRect.x + localRenderRect.x;
	finalGlobalRect.y = parentGlobalRect.y + localRenderRect.y;
	//m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// 不透明度もココで混ぜてしまう
//}
//else
//{
//	m_finalGlobalRect.x = m_finalLocalRect.x;
//	m_finalGlobalRect.y = m_finalLocalRect.y;
//	m_combinedOpacity = m_opacity;
//}
	finalGlobalRect.width = localRenderRect.width;
	finalGlobalRect.height = localRenderRect.height;

	setLayoutFinalGlobalRect(finalGlobalRect);

	// update children
	//Rect finalGlobalContentRect(
	//	parentGlobalRect.x + localContentRect.x,
	//	parentGlobalRect.y + localContentRect.y,
	//	localContentRect.width,
	//	localContentRect.height);
	Rect finalGlobalContentRect(
		parentGlobalRect.x + localRenderRect.x,
		parentGlobalRect.y + localRenderRect.y,
		localRenderRect.width,
		localRenderRect.height);

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

	return detail::LayoutHelper::measureElement(this, constraint);
}

Size UILayoutElement::arrangeOverride(const Size& finalSize)
{
	return finalSize;
}

const Thickness& UILayoutElement::getLayoutMargin() const
{
	return m_actualStyle->margin.get();// OrDefault(Thickness::Zero);
}

const Thickness& UILayoutElement::getLayoutPadding() const
{
	return m_actualStyle->padding.get();//OrDefault(Thickness::Zero);
}

HAlignment UILayoutElement::getLayoutHAlignment() const
{
	return m_actualStyle->horizontalAlignment.get();//OrDefault(HAlignment::Center);
}

VAlignment UILayoutElement::getLayoutVAlignment() const
{
	return m_actualStyle->verticalAlignment.get();
}

void UILayoutElement::getLayoutMinMaxInfo(Size* outMin, Size* outMax) const
{
	outMin->width = m_actualStyle->minWidth.get();
	outMin->height = m_actualStyle->minHeight.get();
	outMax->width = m_actualStyle->maxWidth.get();
	outMax->height = m_actualStyle->maxHeight.get();
}

//==============================================================================
// ILayoutPanel

ILayoutPanel::ILayoutPanel()
{
}
//==============================================================================
// LayoutHelper

namespace detail {

Size LayoutHelper::measureElement(UILayoutElement* element, const Size& constraint)
{
	Size size = element->getLayoutSize();
	Size desiredSize;
	// NaN の場合、この要素として必要な最小サイズは 0 となる。
	desiredSize.width = Math::isNaNOrInf(size.width) ? 0.0f : size.width;
	desiredSize.height = Math::isNaNOrInf(size.height) ? 0.0f : size.height;
	desiredSize.width = std::min(desiredSize.width, constraint.width);
	desiredSize.height = std::min(desiredSize.height, constraint.height);

	Size minSize, maxSize;
	element->getLayoutMinMaxInfo(&minSize, &maxSize);
	if (!Math::isNaNOrInf(minSize.width)) desiredSize.width = std::max(desiredSize.width, minSize.width);
	if (!Math::isNaNOrInf(minSize.height)) desiredSize.height = std::max(desiredSize.height, minSize.height);
	if (!Math::isNaNOrInf(maxSize.width)) desiredSize.width = std::min(desiredSize.width, maxSize.width);
	if (!Math::isNaNOrInf(maxSize.height)) desiredSize.height = std::min(desiredSize.height, maxSize.height);

	return desiredSize;
}

} // namespace detail
} // namespace ln

