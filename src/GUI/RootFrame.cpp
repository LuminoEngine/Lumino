
#include "../Internal.h"
#include <Lumino/GUI/RootFrame.h>

namespace Lumino
{
namespace GUI
{

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

} // namespace GUI
} // namespace Lumino
