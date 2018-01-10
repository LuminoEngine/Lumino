
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
	, m_type(0)
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
void UIMouseEventArgs::initialize(UIEventType type, MouseButtons button, float x, float y, int clickCount)
{
	UIEventArgs::initialize(type);
	m_button = button;
	m_position.x = x;
	m_position.y = y;
	m_clickCount = clickCount;
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
Ref<UIKeyEventArgs> UIKeyEventArgs::create(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode, bool caching)
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
void UIKeyEventArgs::initialize(UIEventType type, Keys keyCode, ModifierKeys modifierKeys, Char charCode)
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
void UIMouseWheelEventArgs::initialize(UIEventType type, int delta)
{
	UIEventArgs::initialize(type);
	m_delta = delta;
}

//==============================================================================
// UIDragDropEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIDragDropEventArgs, UIEventArgs)

Ref<UIDragDropEventArgs> UIDragDropEventArgs::create(UIEventType type, DataObject* data, DragDropEffects effect, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		Ref<UIDragDropEventArgs> ptr(pool->create<UIDragDropEventArgs>(type, data, effect), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

UIDragDropEventArgs::UIDragDropEventArgs()
	: m_data(nullptr)
	, m_effect(DragDropEffects::None)
{
}

UIDragDropEventArgs::~UIDragDropEventArgs()
{
}

void UIDragDropEventArgs::initialize(UIEventType type, DataObject* data, DragDropEffects effect)
{
	UIEventArgs::initialize(type);
	if (LN_REQUIRE(data)) return;
	m_data = data;
	m_effect = effect;
}

LN_NAMESPACE_END
