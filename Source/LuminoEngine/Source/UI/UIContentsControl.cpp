
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIContentsControl.h>
#include <Lumino/UI/UIElementCollection.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIContentsControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIContentsControl, UIElement);

UIContentsControl::UIContentsControl()
	: m_logicalChildren(Ref<UIElementCollection>::makeRef(this))
{
}

UIContentsControl::~UIContentsControl()
{
}

void UIContentsControl::initialize()
{
	UIControl::initialize();

}

void UIContentsControl::addChild(UIElement* element)
{
	if (LN_REQUIRE(element != nullptr)) return;

	m_logicalChildren->add(element);
	element->setLogicalParent(this);

	// Presenter が無ければ自身の Visual としても管理する
	if (!getLogicalChildrenPresenter())
	{
		addVisualChild(element);
	}
}

void UIContentsControl::removeChild(UIElement* element)
{
	if (element == nullptr) return;

	element->setLogicalParent(nullptr);
	m_logicalChildren->remove(element);

	// Presenter が無ければ自身の Visual としても管理する
	if (!getLogicalChildrenPresenter())
	{
		removeVisualChild(element);
	}
}

void UIContentsControl::clearChildren()
{
	for (auto* c : *m_logicalChildren)
	{
		c->setLogicalParent(nullptr);

		// Presenter が無ければ自身の Visual としても管理する
		if (!getLogicalChildrenPresenter())
		{
			removeVisualChild(c);
		}
	}
	m_logicalChildren->clear();
}

int UIContentsControl::getLogicalChildrenCount() const
{
	return m_logicalChildren->getCount();
}

UIElement* UIContentsControl::getLogicalChild(int index)
{
	return m_logicalChildren->getAt(index);
}

Size UIContentsControl::measureOverride(const Size& constraint)
{
	if (getLogicalChildrenPresenter())
	{
		return UIControl::measureOverride(constraint);
	}
	else
	{
		// 主に省メモリを目的として、単一子要素しか持たないコントロールは Presenter を省略することがある。
		// この場合、自分で子要素をレイアウトする。
		Size desiredSize = UIElement::measureOverride(constraint);
		for (auto& child : (*m_logicalChildren))
		{
			child->measureLayout(constraint);
			const Size& childDesiredSize = child->getDesiredSize();
			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		return desiredSize;
	}

}

Size UIContentsControl::arrangeOverride(const Size& finalSize)
{
	if (getLogicalChildrenPresenter())
	{
		return UIControl::arrangeOverride(finalSize);
	}
	else
	{
		// 主に省メモリを目的として、単一子要素しか持たないコントロールは Presenter を省略することがある。
		// この場合、自分で子要素をレイアウトする。

		// レイアウトしない＝単に重ねていく、なので、まずは全子要素を包める領域サイズを求める
		Size totalDesiredSize;
		for (auto& child : (*m_logicalChildren))
		{
			totalDesiredSize = Size::max(totalDesiredSize, child->getDesiredSize());
		}

		// その領域をどこに配置する？
		Rect childArea;
		detail::LayoutHelper::adjustHorizontalAlignment(finalSize, totalDesiredSize, Math::isNaN(finalSize.width), HContentAlignment, &childArea);
		detail::LayoutHelper::adjustVerticalAlignment(finalSize, totalDesiredSize, Math::isNaN(finalSize.height), VContentAlignment, &childArea);

		// 決定した領域で、子要素をすべて同じ場所に配置
		for (auto& child : (*m_logicalChildren))
		{
			child->arrangeLayout(childArea);
		}
		return finalSize;
	}
}

//==============================================================================
// UIUserControl
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIUserControl, UIContentsControl);

Ref<UIUserControl> UIUserControl::create()
{
	return newObject<UIUserControl>();
}

UIUserControl::UIUserControl()
{
}

UIUserControl::~UIUserControl()
{
}

void UIUserControl::initialize()
{
	UIContentsControl::initialize();
}

LN_NAMESPACE_END
