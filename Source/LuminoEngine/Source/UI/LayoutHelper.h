
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class LayoutHelper2
{
public:
	
	template<class TElement>
	static void measureLayout(TElement* element, const Size& availableSize)
	{
		if (element->isLayoutVisible())
		{
			// Margin と Padding を考慮する
			const Thickness& margin = element->getLayoutMargin();
			//const Thickness& padding = element->getLayoutPadding();
			float spaceWidth = (margin.left + margin.right);// +(padding.left + padding.right);
			float spaceHeight = (margin.top + margin.bottom);// +(padding.top + padding.bottom);
			Size localAvailableSize(
				std::max(availableSize.width - spaceWidth, 0.0f),
				std::max(availableSize.height - spaceHeight, 0.0f));

			Size desiredSize = element->measureOverride(localAvailableSize);

			// Margin を考慮する
			desiredSize.width += spaceWidth;
			desiredSize.height += spaceHeight;

			element->setLayoutDesiredSize(desiredSize);
		}
		else
		{
			element->setLayoutDesiredSize(Size::Zero);
		}
	}

	
	template<class TElement>
	static void arrangeLayout(TElement* element, const Rect& finalLocalRect)
	{
		if (element->isLayoutVisible())
		{
			// finalLocalRect はこの要素を配置できる領域サイズ。と、親要素内でのオフセット。
			// 要素に直接設定されているサイズよりも大きいこともある。
			// TODO: HorizontalAlignment 等を考慮して、最終的な座標とサイズを決定する。
			//		 この要素のサイズが省略されていれば、Stretch ならサイズは最大に、それ以外なら最小になる。

			const Size& areaSize = finalLocalRect.getSize();

#if 1
			HAlignment		hAlign = element->getLayoutHAlignment();
			VAlignment		vAlign = element->getLayoutVAlignment();
#else
			ILayoutElement* parent = GetLayoutParent();
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
			Size ds = element->getLayoutDesiredSize();
			LN_ASSERT(!Math::isNaNOrInf(ds.width));
			LN_ASSERT(!Math::isNaNOrInf(ds.height));

			// DesiredSize は Margin 考慮済み

			// Alignment で調整する領域は、margin 領域も含む
			const Thickness& margin = element->getLayoutMargin();
			float marginWidth = margin.left + margin.right;
			float marginHeight = margin.top + margin.bottom;

			Size layoutSize = element->getLayoutSize();
			Rect arrangeRect;
			detail::LayoutHelper::adjustHorizontalAlignment(areaSize, ds, Math::isNaN(layoutSize.width), hAlign, &arrangeRect);
			detail::LayoutHelper::adjustVerticalAlignment(areaSize, ds, Math::isNaN(layoutSize.height), vAlign, &arrangeRect);

			// Margin を考慮する (0 以下には出来ない)
			arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
			arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);

			// Padding を考慮する
			//const Thickness& padding = element->getLayoutPadding();

			//Size contentAreaSize(
			//	std::max(arrangeRect.width - padding.getWidth(), 0.0f),
			//	std::max(arrangeRect.height - padding.getHeight(), 0.0f));
			Size contentAreaSize(
				std::max(arrangeRect.width, 0.0f),
				std::max(arrangeRect.height, 0.0f));
			Size finalContentAreaSize = element->arrangeOverride(contentAreaSize);

			Rect finalRenderRect;
			finalRenderRect.x = finalLocalRect.x + margin.left + arrangeRect.x;
			finalRenderRect.y = finalLocalRect.y + margin.top + arrangeRect.y;
			finalRenderRect.width = finalContentAreaSize.width;// +padding.getWidth();
			finalRenderRect.height = finalContentAreaSize.height;// +padding.getHeight();
			//Rect finalContentRect;
			//finalContentRect.x = finalRenderRect.x + padding.left;
			//finalContentRect.y = finalRenderRect.y + padding.top;
			//finalContentRect.width = finalRenderRect.width - padding.getWidth();
			//finalContentRect.height = finalRenderRect.height - padding.getHeight();
			element->setLayoutFinalLocalRect(finalRenderRect/*, finalContentRect*/);
		}
		else
		{
			element->setLayoutFinalLocalRect(Rect::Zero/*, Rect::Zero*/);
		}
	}

	// 単一の子要素を、要素全体にレイアウトするための measureOverride の実装
	template<class TElement, class TElementBaseClass>
	static Size measureOverride_SimpleOneChild(TElement* element, const Size& constraint, UIElement* child)
	{
		//if (m_invalidateItemsHostPanel)
		//{

		//	m_invalidateItemsHostPanel = false;
		//}


#if 1
		Size desiredSize = element->TElementBaseClass::measureOverride(constraint);

		Size childDesiredSize(0, 0);
		//if (readCoreFlag(detail::UICoreFlags_LogicalChildrenPresenterAutoManagement))
		if (child != nullptr)
		{
			child->measureLayout(constraint);
			childDesiredSize = child->getDesiredSize();
		}

		desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);

		return desiredSize;
#else
		return detail::LayoutImpl<UIControl>::UILayoutPanel_MeasureOverride(
			this, constraint,
			[](UIControl* panel, const Size& constraint) { return panel->UIElement::MeasureOverride(constraint); });
		//Size desiredSize = UIElement::measureOverride(constraint);
		//if (m_visualTreeRoot != nullptr)
		//{
		//    m_visualTreeRoot->measureLayout(constraint);
		//    const Size& childDesiredSize = m_visualTreeRoot->getDesiredSize();

		//    desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		//    desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		//}
		//return desiredSize;
#endif

	}

	// 単一の子要素を、要素全体にレイアウトするための arrangeOverride の実装
	template<class TElement, class TElementBaseClass>
	static Size arrangeOverride_SimpleOneChild(TElement* element, const Size& finalSize, UIElement* child)
	{
#if 1
		if (child != nullptr)
		{
			Size childDesiredSize = child->getDesiredSize();
			childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
			childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
			child->arrangeLayout(Rect(0.0f, 0.0f, childDesiredSize));
		}

		return element->TElementBaseClass::arrangeOverride(finalSize);
#else
		return detail::LayoutImpl<UIControl>::UILayoutPanel_ArrangeOverride(this, Vector2::Zero, finalSize);
		//RectF childFinal(0, 0, finalSize);
		//if (m_visualTreeRoot != nullptr)
		//{
		//    Size childDesiredSize = m_visualTreeRoot->getDesiredSize();
		//    childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
		//    childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
		//    m_visualTreeRoot->arrangeLayout(RectF(0, 0, childDesiredSize));
		//}
		//return finalSize;
#endif
	}

};

} // namespace detail
LN_NAMESPACE_END
