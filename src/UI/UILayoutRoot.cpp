
#include "Internal.h"
#include <Lumino/UI/UILayoutRoot.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UILayoutRoot
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UILayoutRoot, UIContentControl)

//------------------------------------------------------------------------------
UILayoutRoot::UILayoutRoot()
{
}

//------------------------------------------------------------------------------
UILayoutRoot::~UILayoutRoot()
{
}

//------------------------------------------------------------------------------
void UILayoutRoot::Initialize(detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
}

LN_NAMESPACE_END
