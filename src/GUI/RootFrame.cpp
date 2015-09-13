
#include "../Internal.h"
#include <Lumino/GUI/RootFrame.h>

namespace Lumino
{
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
	RefPtr<RootFrame> obj(ApplicationContext::CreateRefObject<RootFrame>(ApplicationContext::GetGUIManager()));
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootFrame::RootFrame(GUIManager* manager)
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
} // namespace Lumino
