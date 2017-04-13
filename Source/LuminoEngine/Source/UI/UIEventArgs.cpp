
#include "Internal.h"
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/UI/UIElement.h>
#include "EventArgsPool.h"
#include "UIManager.h"


LN_NAMESPACE_BEGIN

//==============================================================================
// UIEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIEventArgs, tr::ReflectionEventArgs)

//------------------------------------------------------------------------------
UIEventArgsPtr UIEventArgs::Create(Object* sender, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		return UIEventArgsPtr(pool->Create<UIEventArgs>(), false);
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

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

//------------------------------------------------------------------------------
PointF UIMouseEventArgs::GetPosition(UIElement* relativeTo)
{
	const RectF& rc = relativeTo->GetFinalGlobalRect();
	return PointF(x - rc.x, y - rc.y);
}

//==============================================================================
// UIKeyEventArgs
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

//==============================================================================
// UICommandEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UICommandEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UICommandEventArgs::UICommandEventArgs()
	: command(nullptr)
{
}

//------------------------------------------------------------------------------
UICommandEventArgs::~UICommandEventArgs()
{
}

LN_NAMESPACE_END
