
#include "Internal.h"
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutRoot.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UILayoutRoot
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UILayoutRoot, UIContentControl)

//------------------------------------------------------------------------------
UILayoutRoot::UILayoutRoot()
	: m_owner(nullptr)
{
}

//------------------------------------------------------------------------------
UILayoutRoot::~UILayoutRoot()
{
}

//------------------------------------------------------------------------------
void UILayoutRoot::Initialize(detail::UIManager* manager, UILayoutView* owner)
{
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(owner != nullptr);
	UIContentControl::Initialize(manager);
	m_owner = owner;
}

//------------------------------------------------------------------------------
void UILayoutRoot::ActivateInternal(UIElement* child)
{
	m_owner->GetOwnerContext()->SetFocusElement(child);
}

LN_NAMESPACE_END
