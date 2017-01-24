
#pragma once
#include <functional>
#include "UIManagerImpl.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
	
// 様々なオブジェクトの private メンバにアクセスする内部クラス。
// このクラスは絶対にライブラリのユーザーに見えるところに公開してはならない。
// フレームワークの都合上、多くのメンバを public で公開しなければならないが、しかしそれはあまりにも危険な実装。
// やむを得ず friend class に逃がす。
// C# の internal が使えれば楽なのだが・・・。
class GUIHelper	// TODO: Internal
{
public:
	template<class T, typename... TArgs>
	static RefPtr<T> CreateUIElemenInstance(TArgs... args) { return RefPtr<T>(T::internalCreateInstance(args...), false); }

	static Internal::GUIPainter* GUIManager_GetGUIPainter(GUIManagerImpl* manager) { return manager->m_painter; }


	static void UIElement_SetKeyName(UIElement* element, const String& name) { element->m_keyName = name; }
	static void UIElement_SetTemplateParent(UIElement* element, UIElement* templateParent) { element->m_templateParent = templateParent; }
	static Array< RefPtr<AnimationClock> >* UIElement_GetAnimationClockList(UIElement* element) { return &element->m_animationClockList; }
	static InvalidateFlags UIElement_GetInvalidateFlags(UIElement* element) { return element->m_invalidateFlags; }


#ifdef LUMINO_GUI_GUICONTEXT_H
	static const Size& GetViewPixelSize(GUIContext* context) { return context->GetViewPixelSize(); }
	static void SetFocusElement(GUIContext* context, UIElement* element) { context->SetFocusElement(element); }
	static void CaptureMouse(GUIContext* context, UIElement* element) { context->CaptureMouse(element); }
	static void ReleaseMouseCapture(GUIContext* context, UIElement* element) { context->ReleaseMouseCapture(element); }
	static void AddAnimationClock(GUIContext* context, AnimationClock* clock) { context->AddAnimationClock(clock); }
	static void RemoveAnimationClock(GUIContext* context, AnimationClock* clock) { context->RemoveAnimationClock(clock); }
	static double GetTime(GUIContext* context) { return context->GetTime(); }

#endif


	static void ForEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	{
		int count = element->GetVisualChildrenCount();
		for (int i = 0; i < count; ++i)
		{
			func(element->GetVisualChild(i));
		}
	}

	static bool SimpleMeasureOverrideSingleVisual(UIElement* element, const SizeF& constraint, SizeF* desiredSize)
	{
		int count = element->GetVisualChildrenCount();
		if (count > 0)
		{
			UIElement* child = element->GetVisualChild(0);
			if (child != NULL)
			{
				child->MeasureLayout(constraint);
				*desiredSize = child->GetDesiredSize();
				return true;
			}
		}
		return false;
	}

	static bool SimpleArrangeOverrideSingleVisual(UIElement* element, const SizeF& finalSize)
	{
		int count = element->GetVisualChildrenCount();
		if (count > 0)
		{
			UIElement* child = element->GetVisualChild(0);
			if (child != NULL)
			{
				child->ArrangeLayout(RectF(0, 0, finalSize));
				return true;
			}
		}
		return false;
	}

	static void AdjustHorizontalAlignment(const SizeF& arrangeSize, const SizeF& desiredSize, HorizontalAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case HorizontalAlignment::Left:
			outRect->X = 0;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Center:
			outRect->X = (arrangeSize.Width - desiredSize.Width) / 2;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Right:
			outRect->X = arrangeSize.Width - desiredSize.Width;
			outRect->Width = desiredSize.Width;
			break;
		case HorizontalAlignment::Stretch:
			outRect->X = 0;
			outRect->Width = arrangeSize.Width;
			break;
		}
	}

	static void AdjustVerticalAlignment(const SizeF& arrangeSize, const SizeF& desiredSize, VerticalAlignment align, RectF* outRect)
	{
		switch (align)
		{
		case VerticalAlignment::Top:
			outRect->Y = 0;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Center:
			outRect->Y = (arrangeSize.Height - desiredSize.Height) / 2;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Bottom:
			outRect->Y = arrangeSize.Height - desiredSize.Height;
			outRect->Height = desiredSize.Height;
			break;
		case VerticalAlignment::Stretch:
			outRect->Y = 0;
			outRect->Height = arrangeSize.Height;
			break;
		}
	}
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
