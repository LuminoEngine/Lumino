
#pragma once
#include "Internal.h"
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
	
class UIHelper
{
public:

	static void ForEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	{
		int count = element->getVisualChildrenCount();
		for (int i = 0; i < count; ++i)
		{
			func(static_cast<UIElement*>(element->getVisualChild(i)));
		}
	}

	template<typename TPred>
	static UIElement* FindVisualAncestor(UIElement* element, bool self, TPred pred)
	{
		if (LN_CHECK_ARG(element != nullptr)) return nullptr;

		// 自分もチェックするか
		if (self)
		{
			if (pred(element))
			{
				return element;
			}
		}

		while (true)
		{
			element = element->GetVisualParent();
			if (element == nullptr)
			{
				break;
			}

			if (pred(element))
			{
				return element;
			}
		}
		return nullptr;
	}

	static UIElement* GetLayoutRoot(UIElement* element)
	{
		while (element != nullptr)
		{
			if (element->GetSpcialUIElementType() == detail::SpcialUIElementType::LayoutRoot)
			{
				return element;
			}
			element = element->GetVisualParent();
		}
		return nullptr;
	}

	static UIElement* FindSpcialElementAncestor(UIElement* element, UISpecialElementType type)
	{
		while (element != nullptr)
		{
			if (element->GetSpecialElementType2() == type)
			{
				return element;
			}
			element = element->GetVisualParent();
		}
		return nullptr;
	}
};

LN_NAMESPACE_END
