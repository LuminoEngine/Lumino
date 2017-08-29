
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
UIEventArgsPtr UIEventArgs::create(UIEventType type, Object* sender, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		return UIEventArgsPtr(pool->create<UIEventArgs>(type), false);
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
bool UIEventArgs::initialize(UIEventType type)
{
	if (!ReflectionEventArgs::initialize()) return false;
	m_type = type;
	return true;
}

//==============================================================================
// UIMouseEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
Ref<UIMouseEventArgs> UIMouseEventArgs::create(UIEventType type, MouseButtons button, float x, float y, int clickCount, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		Ref<UIMouseEventArgs> ptr(pool->create<UIMouseEventArgs>(type, button, x, y, clickCount), false);
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
bool UIMouseEventArgs::initialize(UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
	LN_BASE_INITIALIZE(UIEventArgs, type);
	m_button = button;
	m_position.x = x;
	m_position.y = y;
	m_clickCount = clickCount;
	return true;
}

//------------------------------------------------------------------------------
Point UIMouseEventArgs::getPosition(UIElement* relativeTo) const
{
	const Rect& rc = relativeTo->getFinalGlobalRect();
	return Point(m_position.x - rc.x, m_position.y - rc.y);
}

//==============================================================================
// UIKeyEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIKeyEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
Ref<UIKeyEventArgs> UIKeyEventArgs::create(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		Ref<UIKeyEventArgs> ptr(pool->create<UIKeyEventArgs>(type, keyCode, modifierKeys, charCode), false);
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
bool UIKeyEventArgs::initialize(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, TCHAR charCode)
{
	LN_BASE_INITIALIZE(UIEventArgs, type);
	m_keyCode = keyCode;
	m_modifierKeys = modifierKeys;
	m_charCode = charCode;
	return true;
}

//==============================================================================
// UIMouseWheelEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMouseWheelEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
Ref<UIMouseWheelEventArgs> UIMouseWheelEventArgs::create(UIEventType type, int delta, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		Ref<UIMouseWheelEventArgs> ptr(pool->create<UIMouseWheelEventArgs>(type, delta), false);
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
bool UIMouseWheelEventArgs::initialize(UIEventType type, int delta)
{
	LN_BASE_INITIALIZE(UIEventArgs, type);
	m_delta = delta;
	return true;
}

LN_NAMESPACE_END
