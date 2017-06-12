
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
UIEventArgsPtr UIEventArgs::Create(UIEventType type, Object* sender, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		return UIEventArgsPtr(pool->Create<UIEventArgs>(type), false);
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
	, m_type(nullptr)
{
}

//------------------------------------------------------------------------------
UIEventArgs::~UIEventArgs()
{
}

//------------------------------------------------------------------------------
void UIEventArgs::initialize(UIEventType type)
{
	m_type = type;
}

//==============================================================================
// UIMouseEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIMouseEventArgs> UIMouseEventArgs::Create(UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIMouseEventArgs> ptr(pool->Create<UIMouseEventArgs>(type, button, x, y, clickCount), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIMouseEventArgs::UIMouseEventArgs()
	: m_button(MouseButtons::None)
	, m_position(0, 0)
	, m_clickCount(0)
{
}

//------------------------------------------------------------------------------
UIMouseEventArgs::~UIMouseEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseEventArgs::initialize(UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
	UIEventArgs::initialize(type);
	m_button = button;
	m_position.x = x;
	m_position.y = y;
	m_clickCount = clickCount;
}

//------------------------------------------------------------------------------
PointF UIMouseEventArgs::GetPosition(UIElement* relativeTo) const
{
	const Rect& rc = relativeTo->GetFinalGlobalRect();
	return PointF(m_position.x - rc.x, m_position.y - rc.y);
}

//==============================================================================
// UIKeyEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIKeyEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIKeyEventArgs> UIKeyEventArgs::Create(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIKeyEventArgs> ptr(pool->Create<UIKeyEventArgs>(type, keyCode, modifierKeys, charCode), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIKeyEventArgs::UIKeyEventArgs()
	: m_keyCode(Keys::Unknown)
	, m_modifierKeys(ModifierKeys::None)
	, m_charCode(0)
{
}

//------------------------------------------------------------------------------
UIKeyEventArgs::~UIKeyEventArgs()
{
}

//------------------------------------------------------------------------------
void UIKeyEventArgs::initialize(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode)
{
	UIEventArgs::initialize(type);
	m_keyCode = keyCode;
	m_modifierKeys = modifierKeys;
	m_charCode = charCode;
}

//==============================================================================
// UIMouseWheelEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
RefPtr<UIMouseWheelEventArgs> UIMouseWheelEventArgs::Create(UIEventType type, int delta, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::GetInstance()->GetEventArgsPool();
		RefPtr<UIMouseWheelEventArgs> ptr(pool->Create<UIMouseWheelEventArgs>(type, delta), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::UIMouseWheelEventArgs()
	: m_delta(0)
{
}

//------------------------------------------------------------------------------
UIMouseWheelEventArgs::~UIMouseWheelEventArgs()
{
}

//------------------------------------------------------------------------------
void UIMouseWheelEventArgs::initialize(UIEventType type, int delta)
{
	UIEventArgs::initialize(type);
	m_delta = delta;
}

LN_NAMESPACE_END
