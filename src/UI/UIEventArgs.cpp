
#include "Internal.h"
#include <Lumino/UI/UIEventArgs.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIEventArgs, tr::ReflectionEventArgs)

//------------------------------------------------------------------------------
UIEventArgs::UIEventArgs()
	: sender(nullptr)
	//, handled(false)
{
}

//------------------------------------------------------------------------------
UIEventArgs::~UIEventArgs()
{
}

//==============================================================================
// UIMouseEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs()
	: button(MouseButtons::None)
	, x(0)
	, y(0)
	, clickCount(0)
{
}

//------------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs(MouseButtons button_, float x_, float y_, int clickCount_)
	: button(button_)
	, x(x_)
	, y(y_)
	, clickCount(clickCount_)
{
}

//------------------------------------------------------------------------------
UIMouseEventArgs::~UIMouseEventArgs()
{
}

//==============================================================================
// UIEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIKeyEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs()
	: keyCode(Keys::Unknown)
	, modifierKeys(ModifierKeys::None)
	, charCode(0x00)
{
}

//------------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs(Keys keyCode_, ModifierKeys modifierKeys_)
	: keyCode(keyCode_)
	, modifierKeys(modifierKeys_)
	, charCode(0x00)
{
}

//------------------------------------------------------------------------------
UIKeyEventArgs::~UIKeyEventArgs()
{
}

//==============================================================================
// UIMouseWheelEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::UIMouseWheelEventArgs()
	: wheel(0)
{
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::UIMouseWheelEventArgs(int wheel_)
	: wheel(wheel_)
{
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::~UIMouseWheelEventArgs()
{
}

LN_NAMESPACE_END
