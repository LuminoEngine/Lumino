
#include "Internal.h"
#include <Lumino/UI/LayoutElement.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ILayoutElement
//==============================================================================

//------------------------------------------------------------------------------
ILayoutElement::ILayoutElement()
{
}

//------------------------------------------------------------------------------
ILayoutElement::~ILayoutElement()
{
}

//------------------------------------------------------------------------------
void ILayoutElement::MeasureLayout(const SizeF& availableSize)
{
	//// 無効情報フラグをこの要素に伝播させる
	//if (m_parent != nullptr)
	//{
	//	// フォントは MeasureOverride() の中で更新する
	//	m_invalidateFlags |= (m_parent->m_invalidateFlags & detail::InvalidateFlags::Font);
	//}

	// 親要素から子要素を配置できる範囲(availableSize)を受け取り、DesiredSize を更新する。
	// ① Pane ―[measure()   … この範囲内なら配置できるよ]→ Button
	// ② Pane ←[DesiredSize … じゃあこのサイズでお願いします]― Button		※この時点で inf を返すこともあり得る。
	// ③ Pane ―[arrange()   … 他の子要素との兼ね合いで最終サイズはコレで]→ Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin を考慮する
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	SizeF localAvailableSize(
		std::max(availableSize.width - marginWidth, 0.0f),
		std::max(availableSize.height - marginHeight, 0.0f));

	SizeF desiredSize = MeasureOverride(localAvailableSize);

	// Margin を考慮する
	desiredSize.width += marginWidth;
	desiredSize.height += marginHeight;

	SetLayoutDesiredSize(desiredSize);
}

//------------------------------------------------------------------------------
void ILayoutElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
	// 要素に直接設定されているサイズよりも大きいこともある。
	// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
	//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

	const SizeF& areaSize = finalLocalRect.GetSize();

	// この要素のサイズが明示的に指定されている場合はそちらを優先する
	//const SizeF& size = GetLayoutSize();
	//arrangeSize.width = Math::IsNaNOrInf(size.width) ? finalLocalRect.width : size.width;
	//arrangeSize.height = Math::IsNaNOrInf(size.height) ? finalLocalRect.height : size.height;

	ILayoutElement* parent = GetLayoutParent();
	HAlignment  hAlign = GetLayoutHAlignment();
	VAlignment    vAlign = GetLayoutVAlignment();
	const HAlignment* parentHAlign = (parent != nullptr) ? parent->GetLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (parent != nullptr) ? parent->GetLayoutContentVAlignment() : nullptr;
	if (parentHAlign != nullptr) hAlign = *parentHAlign;
	if (parentVAlign != nullptr) vAlign = *parentVAlign;

	const SizeF& ds = GetLayoutDesiredSize();
	RectF arrangeRect;
	detail::LayoutHelper::AdjustHorizontalAlignment(areaSize, ds, hAlign, &arrangeRect);
	detail::LayoutHelper::AdjustVerticalAlignment(areaSize, ds, vAlign, &arrangeRect);

	// Margin を考慮する (0 以下には出来ない)
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);


	SizeF renderSize = ArrangeOverride(arrangeRect.GetSize());
	RectF thisFinalLocalRect;
	thisFinalLocalRect.x = finalLocalRect.x + margin.Left + arrangeRect.x;
	thisFinalLocalRect.y = finalLocalRect.y + margin.Top + arrangeRect.y;
	thisFinalLocalRect.width = renderSize.width;
	thisFinalLocalRect.height = renderSize.height;
	SetLayoutFinalLocalRect(thisFinalLocalRect);
}

//------------------------------------------------------------------------------
SizeF ILayoutElement::MeasureOverride(const SizeF& constraint)
{
	// 戻り値は、constraint の制限の中で、子要素をレイアウトするために必要な最小サイズ。
	// ユーザー指定のサイズがある場合はそれを返す。
	// ただし、constraint を超えることはできない。

	const SizeF& size = GetLayoutSize();
	SizeF desiredSize;
	// NaN の場合、この要素として必要な最小サイズは 0 となる。
	desiredSize.width = Math::IsNaNOrInf(size.width) ? 0.0f : size.width;
	desiredSize.height = Math::IsNaNOrInf(size.height) ? 0.0f : size.height;
	desiredSize.width = std::min(desiredSize.width, constraint.width);
	desiredSize.height = std::min(desiredSize.height, constraint.height);

	return desiredSize;
}

//------------------------------------------------------------------------------
SizeF ILayoutElement::ArrangeOverride(const SizeF& finalSize)
{
	return finalSize;
}

LN_NAMESPACE_END
