
#include "Internal.h"
#include <Lumino/GUI/RootFrame.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// RootFrame
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(RootFrame, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(RootFrame);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootFrame* RootFrame::Create()
{
	return internalCreateInstance(GetUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootFrame::RootFrame(GUIManagerImpl* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootFrame::~RootFrame()
{
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
