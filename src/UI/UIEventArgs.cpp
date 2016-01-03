
#include "Internal.h"
#include <Lumino/UI/UIEventArgs.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// UIEventArgs
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIEventArgs, tr::ReflectionEventArgs)

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIEventArgs::UIEventArgs()
	: sender(nullptr)
	//, handled(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIEventArgs::~UIEventArgs()
{
}

//=============================================================================
// UIMouseEventArgs
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseEventArgs, UIEventArgs)

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs()
	: button(MouseButton::None)
	, wheel(0)
	, x(0)
	, y(0)
	, clickCount(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs(MouseButton button_, int wheel_, float x_, float y_, int clickCount_)
	: button(button_)
	, wheel(wheel_)
	, x(x_)
	, y(y_)
	, clickCount(clickCount_)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIMouseEventArgs::~UIMouseEventArgs()
{
}

//=============================================================================
// UIEventArgs
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIKeyEventArgs, UIEventArgs)

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs()
	: keyCode(Key::Unknown)
	, isAlt(false)
	, isShift(false)
	, isControl(false)
	, charCode(0x00)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs(Key keyCode_, bool isAlt_, bool isShift_, bool isControl_)
	: keyCode(keyCode_)
	, isAlt(isAlt_)
	, isShift(isShift_)
	, isControl(isControl_)
	, charCode(0x00)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIKeyEventArgs::~UIKeyEventArgs()
{
}

LN_NAMESPACE_END
