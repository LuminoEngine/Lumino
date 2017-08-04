
#pragma once
#include "Internal.h"
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
	
class UIHelper
{
public:

	static void forEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	{
		int count = element->getVisualChildrenCount();
		for (int i = 0; i < count; ++i)
		{
			func(static_cast<UIElement*>(element->getVisualChild(i)));
		}
	}

	template<typename TPred>
	static UIElement* findVisualAncestor(UIElement* element, bool self, TPred pred)
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
			element = element->getVisualParent();
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

	template<typename TPred>
	static UIElement* findLogicalAncestor(UIElement* element, bool self, TPred pred)
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
			element = element->getLogicalParent();
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

	static UIElement* getLayoutRoot(UIElement* element)
	{
		while (element != nullptr)
		{
			if (element->getSpcialUIElementType() == detail::SpcialUIElementType::LayoutRoot)
			{
				return element;
			}
			element = element->getVisualParent();
		}
		return nullptr;
	}

	static UIElement* findSpcialElementAncestor(UIElement* element, UISpecialElementType type)
	{
		while (element != nullptr)
		{
			if (element->getSpecialElementType2() == type)
			{
				return element;
			}
			element = element->getVisualParent();
		}
		return nullptr;
	}
};

LN_NAMESPACE_END
