
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

	template<class TElement, class TElementBaseClass, class TChildrenList>
	static Size measureOverride_AbsoluteLayout(TElement* element, const Size& constraint, TChildrenList& children)
	{
		//return UILayoutPanel::measureOverride(constraint);

		// LayoutPanel ではなく、UIElement の measureOverride を実施 (this のサイズを測る)
		Size size = element->TElementBaseClass::measureOverride(constraint);

		Size childMaxSize(0, 0);
		for (UIElement* child : children)
		{
			child->measureLayout(constraint);
			const Size& desiredSize = child->getDesiredSize();
			const Point& pos = child->getPositionInternal();

			childMaxSize.width = std::max(childMaxSize.width, pos.x + desiredSize.width);
			childMaxSize.height = std::max(childMaxSize.height, pos.y + desiredSize.height);
		}

		return Size::min(constraint, Size::max(size, childMaxSize));

		//if (Math::IsNaN(size.width)) size.width = 

		//Size desiredSize = baseCallback(static_cast<TPanel*>(panel), constraint);
		//int childCount = panel->getLayoutChildrenCount();
		//for (int i = 0; i < childCount; i++)
		//{
		//	ILayoutElement* child = panel->getLayoutChild(i);
		//	Point pos = child->getPositionInternal();

		//	child->measureLayout(constraint);
		//	const Size& childDesiredSize = child->getLayoutDesiredSize();

		//	desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		//	desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		//}
		//return desiredSize;


		//Size size = UILayoutPanel::measureOverride(constraint);
		//
		//for (UIElement* child : *getChildren())
		//{

		//}

		//return size;
	}



	template<class TElement, class TElementBaseClass, class TChildrenList>
	static Size arrangeOverride_AbsoluteLayout(TElement* element, const Size& finalSize, TChildrenList& children)
	{
		//Thickness canvas;
		const Thickness& padding = element->getPadding();
		Point childrenOffset(padding.left, padding.top);
		Size childrenBoundSize(finalSize.width - padding.getWidth(), finalSize.height - padding.getHeight());


		for (UIElement* child : children)
		{
			//if (child->getInvalidateFlags().TestFlag(detail::InvalidateFlags::ParentChangedUpdating))
			//{
			//}

			const Size& desiredSize = child->getDesiredSize();
			//Size layoutSize = child->getSizeInternal();
			Size size = desiredSize;
			//size.width = Math::IsNaN(layoutSize.width) ? desiredSize.width : layoutSize.width;
			//size.height = Math::IsNaN(layoutSize.height) ? desiredSize.height : layoutSize.height;

			Rect childRect(child->getPositionInternal(), size/*child->getSizeInternal()*/);
			Flags<AlignmentAnchor> anchor = child->getAnchorInternal();

			if (anchor != AlignmentAnchor::None)
			{
				const Thickness& margin = element->getMargineInternal();
				//float l = childRect.getLeft(), t = childRect.GetTop(), r = childRect.getRight(), b = childRect.getBottom();

				//if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
				//	l = margin.Left;
				//else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
				//	l = finalSize.width * margin.Left;

				//if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
				//	t = margin.Top;
				//else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
				//	t = finalSize.height * margin.Top;

				//if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
				//	r = finalSize.width - margin.Right;
				//else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				//	r = finalSize.width - (finalSize.width * margin.Right);

				//if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
				//	b = finalSize.height - margin.Bottom;
				//else if (anchor.TestFlag(AlignmentAnchor::BottomRatios))
				//	b = finalSize.height - (finalSize.height * margin.Bottom);

				//if (anchor.TestFlag(AlignmentAnchor::HCenter))
				//	childRect.x = (finalSize.width - childRect.width) / 2;

				//if (anchor.TestFlag(AlignmentAnchor::VCenter))
				//	childRect.y = (finalSize.height - childRect.height) / 2;

				//childRect.Set(l, t, r - l, b - t);

				//child->arrangeLayout(childRect);
#if 1
				float l = NAN, t = NAN, r = NAN, b = NAN;
				if (anchor.isSet(AlignmentAnchor::LeftOffsets))
					l = margin.left;
				else if (anchor.isSet(AlignmentAnchor::LeftRatios))
					l = childrenBoundSize.width * margin.left;

				if (anchor.isSet(AlignmentAnchor::TopOffsets))
					t = margin.top;
				else if (anchor.isSet(AlignmentAnchor::TopRatios))
					t = childrenBoundSize.height * margin.top;

				if (anchor.isSet(AlignmentAnchor::RightOffsets))
					r = childrenBoundSize.width - margin.right;
				else if (anchor.isSet(AlignmentAnchor::RightRatios))
					r = childrenBoundSize.width - (childrenBoundSize.width * margin.right);

				if (anchor.isSet(AlignmentAnchor::BottomOffsets))
					b = childrenBoundSize.height - margin.bottom;
				else if (anchor.isSet(AlignmentAnchor::BottomRatios))
					b = childrenBoundSize.height - (childrenBoundSize.height * margin.bottom);

				if (anchor.isSet(AlignmentAnchor::HCenter))
					childRect.x = (childrenBoundSize.width - childRect.width) / 2;

				if (anchor.isSet(AlignmentAnchor::VCenter))
					childRect.y = (childrenBoundSize.height - childRect.height) / 2;

				if (!Math::isNaN(l) || !Math::isNaN(r))
				{
					if (!Math::isNaN(l) && Math::isNaN(r))
					{
						childRect.x = l;
					}
					else if (Math::isNaN(l) && !Math::isNaN(r))
					{
						childRect.x = r - childRect.width;
					}
					else
					{
						childRect.x = l;
						childRect.width = r - l;
					}
				}

				if (!Math::isNaN(t) || !Math::isNaN(b))
				{
					if (!Math::isNaN(t) && Math::isNaN(b))
					{
						childRect.y = t;
					}
					else if (Math::isNaN(t) && !Math::isNaN(b))
					{
						childRect.y = b - childRect.height;
					}
					else
					{
						childRect.y = t;
						childRect.height = b - t;
					}
				}

				childRect.x += childrenOffset.x;
				childRect.y += childrenOffset.y;
#endif
			}

			child->arrangeLayout(childRect);
		}

		return finalSize;
	}

};

} // namespace detail
LN_NAMESPACE_END
