
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
}

void UIContentsControl::removeChild(UIElement* element)
{
	if (element == nullptr) return;

	element->setLogicalParent(nullptr);
	m_logicalChildren->remove(element);
}

void UIContentsControl::clearChildren()
{
	for (auto* c : *m_logicalChildren)
	{
		c->setLogicalParent(nullptr);
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
